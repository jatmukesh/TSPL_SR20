/* File Name: TsplNative.c; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: TSPL ports using the existing PDL and native print-list engine. */
#include "TsplNative.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include "TsplFirmware.h"
#include "TsplReceive.h"
#include "PrinterParType.h"
#include "PDL.h"
#include "flash.h"
#include "ReceiveBufferPro.h"
#include "HP12832_01_SSD1306_I2C.h"
#include <string.h>
#include <stdio.h>

extern _PDL PRINTER_PDL;
extern PrintControl s_PrinterMOTOPar;
extern PrinterStruct s_PrintParameters;
extern PrintrConfig s_PrintrConfig;
extern PrintListPar s_ListParmeter_A, s_ListParmeter_B;
extern unsigned short g_BufferSwitch, g_PrintStatusReg, g_PrinterDip, g_SpeedChoice;
extern unsigned short g_LPSStatus, g_PE_Standard, g_PaperWide, g_HorShift, g_RunFlag;
extern unsigned int Receive_count, g_Order_count, g_Receive_count, g_PowerOffCount;
extern unsigned char receive_bufer[BUFFERSIZE];
extern unsigned short GetPaper(void);
extern void RunFeedLabPaper(void), OLED_RestoreHomeScreen(void);
extern uint16_t label_length_mm;
extern uint8_t g_label_size_final;
extern unsigned short g_ReceiveBusyFlag, g_ReturnBusy;
extern unsigned short g_PaperType, g_ReturnEp;
extern unsigned short g_PrintLineCount;
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
extern volatile unsigned char g_ThermalRasterLabelActive, g_ThermalRasterFinalChunkQueued;
extern volatile unsigned char g_ThermalRasterFinalBuffer;
#endif

volatile uint32_t g_tsplMilliseconds;
static uint32_t s_mediaGeneration;
static uint16_t s_nextSwitch, s_previousFault;
static uint16_t s_finalRows;
static uint8_t s_calibrated, s_published, s_finalPublished, s_copyComplete, s_copyActive;
static uint8_t s_engineFailed;

/* Function name: TsplNative_Init; Description: Initialize native ownership.
 * Parameters: None; Returns: None; Global vars: native state above.
 * Remarks: Called before startup calibration; does not reset system time. */
void TsplNative_Init(void)
{
    s_mediaGeneration = 0U; s_previousFault = 0U; s_calibrated = 0U;
    s_published = s_finalPublished = s_copyComplete = s_copyActive = s_engineFailed = 0U;
}

/* Function name: TsplNative_Calibrated; Description: Mark a new sensor measurement.
 * Parameters: None; Returns: None; Global vars: s_mediaGeneration, s_calibrated.
 * Remarks: Called only after the existing label-size calculation. */
void TsplNative_Calibrated(void)
{
    ++s_mediaGeneration;
    s_calibrated = (uint8_t)((label_length_mm != 0U) && (g_label_size_final >= 12U) && (g_label_size_final <= 75U));
}

/* Function name: TsplNative_Read; Description: Snapshot actual media and engine state.
 * Parameters: inputs destination; Returns: None; Global vars: native media state.
 * Remarks: Width is configured, not sensed. A lid/media fault invalidates calibration. */
void TsplNative_Read(TsplInputs_t *inputs)
{
    uint16_t fault, width, shift, left = 0U, right = 0U;
    memset(inputs, 0, sizeof(*inputs));
    g_LPSStatus = GetPaper();
    fault = (uint16_t)((g_LPSStatus < g_PE_Standard) || PRINTER_PDL.SYS._GetHSStatus());
    if (fault && !s_previousFault) { ++s_mediaGeneration; s_calibrated = 0U; }
    s_previousFault = fault;
    inputs->now = g_tsplMilliseconds; inputs->mediaGeneration = s_mediaGeneration;
    inputs->detectedHeight = (uint16_t)(label_length_mm * TSPL_DOTS_PER_MM);
    inputs->nominalHeight = g_label_size_final;
    inputs->calibrated = (uint8_t)(s_calibrated && !fault && (g_PaperType == 0U));
    width = (g_PaperWide > 48U) ? 384U : (uint16_t)(g_PaperWide * 8U);
    right = (uint16_t)(384U - width); shift = (uint16_t)((g_HorShift & 0xffU) * 4U);
    if ((g_HorShift & 0xf00U) == 0U) { left = shift; right = (right > shift) ? (uint16_t)(right - shift) : 0U; }
    else if (g_PaperWide > 48U) left = shift;
    inputs->left = left; inputs->right = (uint16_t)(384U - right);
    /* Add the established print-origin margins without expanding media bounds. */
    if (s_PrintParameters.m_Liftpos > inputs->left) inputs->left = s_PrintParameters.m_Liftpos;
    if (s_PrintParameters.m_Righttpos < inputs->right) inputs->right = s_PrintParameters.m_Righttpos;
    inputs->feedDown = (uint8_t)(PRINTER_PDL.SYS._GetKey_LF() == 0U);
    inputs->copyComplete = s_copyComplete; inputs->engineFailed = s_engineFailed;
    inputs->readiness = fault ? TSPL_MEDIA : TsplCore_Readiness();
}

/* Function name: TsplNative_CheckSettings; Description: Reject unrepresentable feed settings.
 * Parameters: label; Returns: OK or unsupported; Global vars: s_PrintrConfig.
 * Remarks: The existing feed controller exposes calibrated gap but no job gap-offset port. */
TsplResult_t TsplNative_CheckSettings(const TsplLabel_t *label)
{
    if (label->gapOffset || (label->gap != (uint16_t)(s_PrintrConfig.m_BarPaGap * 8U))) return TSPL_UNSUPPORTED;
    return TSPL_OK;
}

/* Function name: TsplNative_Start; Description: Reserve the existing engine for one copy.
 * Parameters: label; Returns: readiness; Global vars: copy flags.
 * Remarks: First raster handoff is separate. Never clears an engine-owned buffer. */
TsplResult_t TsplNative_Start(const TsplLabel_t *label)
{
    TsplResult_t result = TsplNative_CheckSettings(label);
    if (result != TSPL_OK) return result;
    if (s_copyActive || s_published || (g_PrintStatusReg == FINISH_PROCESS) ||
        (s_PrinterMOTOPar.m_StopPrint != ENABLE_VAR)) return TSPL_SUBMISSION;
    s_copyActive = 1U; s_copyComplete = s_engineFailed = s_finalPublished = 0U;
    TsplCore_LabelStarted();
    return TSPL_OK;
}

/* Function name: TsplNative_Glyph; Description: Read the existing 12x24 resident font.
 * Parameters: printable character, columns; Returns: None; Global vars: native font data.
 * Remarks: Preflight only permits ASCII 32..126. */
static void TsplNative_Glyph(uint8_t character, uint32_t columns[32])
{
    ReadInFlashAndDeg((unsigned int)(character - 32U) * 36U, 12U, 24U, (unsigned int *)columns);
}

/* Function name: TsplNative_Kick; Description: Start a published list through existing IRQs.
 * Parameters: None; Returns: None; Global vars: native print-control flags.
 * Remarks: Same first-buffer sequence as Handler_main; does not run a second engine. */
static void TsplNative_Kick(void)
{
    if ((g_PrintStatusReg == FINISH_PROCESS) && (s_PrinterMOTOPar.m_StopPrint == ENABLE_VAR)) {
        PRINTER_PDL.SYS._POWER_EN(); PRINTER_PDL.SYS._Drive_Power_EN();
        s_PrinterMOTOPar.m_In_printing = ENABLE_VAR;
        s_PrinterMOTOPar.m_InFirstPrint = ENABLE_VAR;
        s_PrinterMOTOPar.m_StopPrint = DISABLE_VAR;
        g_RunFlag = 0U;
        PRINTER_PDL.SYS._SystemPrintIRQ_EN(10U);
    }
}

/* Function name: TsplNative_Publish; Description: Build and publish one producer-owned list.
 * Parameters: immutable label, first row, row count; Returns: OK, busy or failure.
 * Global vars: existing A/B engine state; Remarks: Native ISR retains consumer ownership. */
TsplResult_t TsplNative_Publish(const TsplLabel_t *label, uint16_t row, uint16_t rows)
{
    PrintListPar *list;
    TsplInputs_t inputs;
    TsplResult_t result;
    uint16_t y, x, shift;
    uint16_t *pixels;
    if (!s_copyActive || s_finalPublished) return TSPL_SUBMISSION;
    if (s_published || (g_PrintStatusReg == FINISH_PROCESS)) return TSPL_NOT_READY;
    TsplNative_Read(&inputs);
    result = TsplJob_Validate(label, &inputs);
    if (result != TSPL_OK) return result;
    list = g_BufferSwitch ? &s_ListParmeter_B : &s_ListParmeter_A;
    memset(list, 0, sizeof(*list));
    list->m_PrintDir = PRINT_DIRECT_NOR; list->m_MotoRunDir = MOTO_CLOCKWISE;
    list->m_PrinterDip = g_PrinterDip; list->m_PrintLineHigh = rows;
    list->m_MotoRunTime = g_SpeedChoice; list->m_HotTime = s_PrintParameters.m_PrHeadHottime;
    list->m_TMIndex = PRINTER_PDL.SYS._GetCurTemperatureLevel();
    /* Preserve the existing voltage/temperature strobe compensation. */
    if (list->m_TMIndex > 9U) list->m_HotTime = (uint16_t)(list->m_HotTime +
        (list->m_TMIndex - 9U) * ((s_PrintParameters.m_PrHeadHottime * 9U) / 100U) * 2U);
    else {
        list->m_FeedTime = (uint16_t)((9U - list->m_TMIndex) * ((s_PrintParameters.m_PrHeadHottime * 6U) / 100U));
        list->m_HotTime = (list->m_HotTime > list->m_FeedTime) ? (uint16_t)(list->m_HotTime - list->m_FeedTime) : 0U;
    }
    list->m_ListIndex[0] = 0U; list->m_ListIndex[1] = LIST_END;
    list->m_ListBuffer[0] = PRINT_MAP;
    list->m_ListBuffer[1] = (384U << 4) | 1U;
    list->m_ListBuffer[2] = (uint16_t)((rows << 4) | 1U);
    list->m_ListBuffer[3] = 0U;
    pixels = &list->m_ListBuffer[4];
    result = TsplRaster_Band(label, row, rows, pixels, TsplNative_Glyph);
    if (result != TSPL_OK) return result;
    shift = inputs.left;
    if (shift != 0U) {
        for (y = 0U; y < rows; ++y) {
            for (x = 384U; x > 0U; --x) {
                uint16_t position = (uint16_t)(x - 1U);
                uint16_t bit = (uint16_t)(0x8000U >> (position & 15U));
                uint8_t set = (uint8_t)((position >= shift) &&
                    ((pixels[y * 24U + (position - shift) / 16U] & (0x8000U >> ((position - shift) & 15U))) != 0U));
                if (set) pixels[y * 24U + position / 16U] |= bit;
                else pixels[y * 24U + position / 16U] &= (uint16_t)~bit;
            }
        }
    }
    s_nextSwitch = g_BufferSwitch;
    s_published = 1U;
    if ((uint32_t)row + rows == label->height) {
        s_finalPublished = 1U;
        s_finalRows = rows;
        TsplCore_LabelFinal((uint8_t)((list == &s_ListParmeter_A) ? 1U : 2U));
    }
    __SETPRIMASK();
    s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;
    g_PrintStatusReg = FINISH_PROCESS;
    __RESETPRIMASK();
    TsplNative_Kick();
    return TSPL_OK;
}

/* Function name: TsplNative_Service; Description: Observe engine claim/completion.
 * Parameters: None; Returns: None; Global vars: copy/publication flags.
 * Remarks: Existing sensor-based feed aligns the next label after physical completion.
 * The feed routine is synchronous; overflow confirmation never calls it. */
void TsplNative_Service(void)
{
    if (s_published && (g_BufferSwitch != s_nextSwitch)) s_published = 0U;
    if (s_copyActive && s_finalPublished && !s_published &&
        (s_PrinterMOTOPar.m_StopPrint == ENABLE_VAR)) {
        s_copyActive = 0U;
        if (g_PrintLineCount < s_finalRows) {
            s_engineFailed = 1U;
            TsplCore_LabelAborted();
            return;
        }
        TsplCore_LabelFinished();
        if ((GetPaper() >= g_PE_Standard) && !PRINTER_PDL.SYS._GetHSStatus()) RunFeedLabPaper();
        if ((GetPaper() < g_PE_Standard) || PRINTER_PDL.SYS._GetHSStatus()) s_engineFailed = 1U;
        s_copyComplete = 1U;
    }
    if (s_copyActive) TsplNative_Kick();
}

/* Function name: TsplNative_Abort; Description: Stop native publication and existing IRQs.
 * Parameters: None; Returns: None; Global vars: native engine/copy state.
 * Remarks: Clear strobes/motor without using the board's power-off drive switch. */
void TsplNative_Abort(void)
{
    PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
    PRINTER_PDL.SYS._SystemPrintLineDat_DIS();
    PRINTER_PDL.SYS._STB_ALL_CLEAR(); PRINTER_PDL.MOTO._MotoDisable();
    s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
    s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;
    s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;
    g_PrintStatusReg = 0U;
    s_copyActive = s_published = s_finalPublished = s_copyComplete = 0U;
    TsplCore_LabelAborted();
}

/* Function name: TsplNative_Prompt; Description: Draw the TSPL-owned overflow prompt.
 * Parameters: first/second line; Returns: None; Global vars: OLED buffer.
 * Remarks: No delay or input polling. */
void TsplNative_Prompt(const char *first, const char *second)
{
    uint8_t width, end;
    ClearDisplayBuf();
    width = GetStr8x16Wide((char *)first, 0U);
    end = DisplayStr8x16char((char *)first, (uint8_t)((128U - width) / 2U), 0U, 0U);
    if (end > 128U) { ClearDisplayBuf(); return; }
    width = GetStr8x16Wide((char *)second, 0U);
    end = DisplayStr8x16char((char *)second, (uint8_t)((128U - width) / 2U), 16U, 0U);
    if (end > 128U) { ClearDisplayBuf(); return; }
    DisplayBufEn();
}

/* Function name: TsplNative_Report; Description: Report error or both overflow heights.
 * Parameters: reason, requested label, detected dots; Returns: None; Global vars: PDL.
 * Remarks: Existing app telemetry uses integer mm; requested fractional mm rounds up. */
void TsplNative_Report(TsplResult_t reason, const TsplLabel_t *label, uint16_t detectedHeight)
{
    char message[40];
    int length;
    if (reason == TSPL_OVERFLOW) {
#ifdef BT_CMD_REQ
        Get_Oversize_Detection_BT_Response((uint16_t)((label->height + 7U) / 8U), (uint16_t)(detectedHeight / 8U));
#endif
        return;
    }
    length = snprintf(message, sizeof(message), "TSPL_ERROR:%u\r\n", (unsigned int)reason);
    if ((length > 0) && ((size_t)length < sizeof(message))) {
        PRINTER_PDL.BT._ReturnNBytes((unsigned char *)message, (unsigned short)length);
        if (g_ReturnEp == 0xA003U) {
            unsigned short sent = PRINTER_PDL.USB._ReturnNBytes((unsigned char *)message, (unsigned short)length);
            if (sent != (unsigned short)length) g_ReturnBusy |= 4U;
        }
    }
}

/* Function name: TsplNative_Restore; Description: Release prompt display ownership.
 * Parameters: None; Returns: None; Global vars: OLED; Remarks: Existing home screen. */
void TsplNative_Restore(void)
{
    OLED_RestoreHomeScreen();
}

/* Function name: TsplNative_Receive; Description: Consume at most one existing ring byte.
 * Parameters: output byte; Returns: availability; Global vars: receive-ring counters.
 * Remarks: Foreground only; never waits for input. */
uint8_t TsplNative_Receive(uint8_t *value)
{
    uint8_t available = 0U;
    __SETPRIMASK();
    if (Receive_count != 0U) {
        *value = receive_bufer[g_Order_count & (MASK_DATA)];
        g_Order_count = (g_Order_count + 1U) & (MASK_DATA);
        --Receive_count; available = 1U;
    }
    __RESETPRIMASK();
    if (Receive_count < PRINTER_RECEIVE_L) {
        g_ReturnBusy = 0U;
        if (g_ReceiveBusyFlag == 0xAAAAU) {
            g_ReceiveBusyFlag = 0U; PRINTER_PDL.USB._ReceiveEPValid();
        }
    }
    return available;
}

/* Function name: TsplNative_ClearReceive; Description: Drop bytes after transport loss.
 * Parameters: None; Returns: None; Global vars: existing receive counters.
 * Remarks: Caller invalidates pending label and requires a fresh CLS. */
void TsplNative_ClearReceive(void)
{
    __SETPRIMASK();
    Receive_count = 0U; g_Order_count = g_Receive_count & (MASK_DATA);
    TsplReceive_Reset();
    __RESETPRIMASK();
}

/* Function name: TsplNative_SelfTest; Description: Build a native capability diagnostic.
 * Parameters: pending label storage; Returns: None; Global vars: media/config values.
 * Remarks: TSPL capabilities replace the legacy ESC/POS barcode advertisement. */
void TsplNative_SelfTest(TsplLabel_t *label)
{
    static const char * const lines[] = { "POSIFLOW PSF20 V4.1.0", "TSPL 384 dots 8 dots/mm", "FONT PSF12X24.FNT" };
    uint8_t i;
    size_t length;
    memset(label, 0, sizeof(*label));
    label->width = 384U; label->height = (uint16_t)(g_label_size_final * 8U);
    label->gap = (uint16_t)(s_PrintrConfig.m_BarPaGap * 8U); label->copies = 1U;
    for (i = 0U; i < 3U; ++i) {
        TsplText_t *op = &label->operations[i];
        length = strlen(lines[i]);
        op->y = (int16_t)(i * 24U); op->length = (uint16_t)length;
        op->scaleX = op->scaleY = 1U; op->textOffset = label->textUsed;
        memcpy(label->text + label->textUsed, lines[i], length + 1U);
        label->textUsed = (uint16_t)(label->textUsed + length + 1U);
    }
    label->operationCount = 3U;
}
#endif
