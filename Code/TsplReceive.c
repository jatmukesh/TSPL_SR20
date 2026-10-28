/* File Name: TsplReceive.c; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Preserve shared receive storage without allowing overwrite. */
#include "TsplReceive.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include "TsplFirmware.h"
#include "PrinterParType.h"
extern unsigned int Receive_count, g_Receive_count;
extern unsigned char receive_bufer[BUFFERSIZE];
extern unsigned short g_ReturnBusy, g_ReceiveBusyFlag;
static volatile uint8_t s_dropBytes;

/* Function name: TsplReceive_Reset; Description: Recover producer after a lost stream.
 * Parameters: None; Returns: None; Global vars: s_dropBytes.
 * Remarks: Foreground calls under interrupt protection after discarding queued data. */
void TsplReceive_Reset(void)
{
    s_dropBytes = 1U;
}

/* Function name: TsplReceive_Byte; Description: Store one incoming byte without overwrite.
 * Parameters: value; Returns: None; Global vars: existing receive ring, s_dropBytes.
 * Remarks: Existing interrupt dispatch serializes producers; no parsing or allocation.
 * After loss discard through a fresh newline; foreground clears queued fragments. */
void TsplReceive_Byte(uint8_t value)
{
    if (s_dropBytes) {
        if ((value == '\n') || (value == '\r')) s_dropBytes = 0U;
        return;
    }
    if (Receive_count >= BUFFERSIZE) {
        s_dropBytes = 1U; TsplFirmware_ReceiveError(); return;
    }
    receive_bufer[g_Receive_count & (MASK_DATA)] = value;
    g_Receive_count = (g_Receive_count + 1U) & (MASK_DATA);
    ++Receive_count;
    if (Receive_count >= PRINTER_RECEIVE_L) {
        g_ReturnBusy |= 4U; g_ReceiveBusyFlag = 0xAAAAU;
    }
}

/* Function name: TsplReceive_Bytes; Description: Store an arbitrary receive chunk.
 * Parameters: bytes, length; Returns: existing busy convention; Global vars: receive ring.
 * Remarks: Transport backpressure remains bounded by the existing storage. */
uint16_t TsplReceive_Bytes(const uint8_t *bytes, uint16_t length)
{
    uint16_t i;
    for (i = 0U; i < length; ++i) TsplReceive_Byte(bytes[i]);
    return (Receive_count >= PRINTER_RECEIVE_L) ? 0xAAAAU : 0U;
}
#endif
