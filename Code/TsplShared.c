/* File Name: TsplShared.c; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Preserve existing shared-state ABI without linking an ESC/POS parser. */
#include "TsplShared.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include "PrinterParType.h"
/* These existing public names are retained for firmware ABI compatibility. */
uint16_t g_CurveCountFlag;
uint16_t g_PaperType;
uint16_t g_cruve_arry[20];
uint16_t g_cruvefalg;
uint16_t g_PrinterDip;
uint16_t g_PrintListOrNorm;
uint16_t g_PaperRunToOut;
uint16_t g_CRCValue;
uint16_t g_CodePage;
uint16_t g_InternationChar;
uint16_t g_unicodeType;
uint16_t g_Lab_head_Feed = 0xA537U;
uint16_t g_PaperWide;
uint16_t g_PaperLength;
uint16_t g_PaperGapL;
uint16_t g_ListTable[BUFFER_SHORT_LENGTH];
uint8_t g_HTCharSpace;
uint8_t g_0D0AHigh;
uint16_t g_LabPrintEndF;
uint16_t g_ListControlFor;
uint16_t g_ListPrintFinish;
uint8_t g_ListSaveToFlashCount;
uint16_t g_ThType;
uint16_t g_pinrtStb = 600U;

/* Function name: TsplShared_UpdateCRC; Description: Existing configuration CRC step.
 * Parameters: prior CRC, byte; Returns: next CRC; Global vars: None.
 * Remarks: Preserves the exact legacy polynomial and augmented-zero algorithm. */
static uint16_t TsplShared_UpdateCRC(uint16_t previous, uint8_t value)
{
    uint32_t crc = previous, input = (uint32_t)value | 0x100U;
    do {
        crc <<= 1; input <<= 1;
        if (input & 0x100U) ++crc;
        if (crc & 0x10000U) crc ^= 0x1021U;
    } while (!(input & 0x10000U));
    return (uint16_t)(crc & 0xffffU);
}

/* Function name: Cal_CRC16; Description: Preserve persisted configuration validation.
 * Parameters: bytes, length; Returns: legacy CRC; Global vars: None.
 * Remarks: ABI name retained; ESC/POS builds retain their original definition. */
uint16_t Cal_CRC16(uint8_t *bytes, uint16_t length)
{
    uint16_t i, crc = 0U;
    for (i = 0U; i < length; ++i) crc = TsplShared_UpdateCRC(crc, bytes[i]);
    crc = TsplShared_UpdateCRC(crc, 0U);
    return TsplShared_UpdateCRC(crc, 0U);
}
#endif
