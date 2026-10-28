/* File Name: TsplFirmware.c; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Single-slot integration, with bounded work on each foreground pass. */
#include "TsplFirmware.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include "TsplNative.h"
#include <stddef.h>
static TsplParser_t s_parser;
static TsplJob_t s_job;
static uint16_t s_nextRow;
static volatile uint8_t s_receiveError;
static uint8_t s_selfTestRequested;

/* Function name: TsplFirmware_Submit; Description: Begin one copy without blocking.
 * Parameters: label; Returns: native preflight result; Global vars: s_nextRow.
 * Remarks: Copy progress resets only after native acceptance. */
static TsplResult_t TsplFirmware_Submit(const TsplLabel_t *label)
{
    TsplResult_t result = TsplNative_Start(label);
    if (result == TSPL_OK) s_nextRow = 0U;
    return result;
}
static const TsplJobPorts_t s_ports = {
    TsplNative_Read, TsplFirmware_Submit, TsplNative_Prompt,
    TsplNative_Report, TsplNative_Restore, TsplNative_Abort
};

/* Function name: TsplFirmware_Init; Description: Initialize selected protocol.
 * Parameters: None; Returns: None; Global vars: s_parser, s_job, s_receiveError.
 * Remarks: Call before startup self-test requests and after system peripherals exist. */
void TsplFirmware_Init(void)
{
    TsplParser_Init(&s_parser); TsplJob_Init(&s_job, &s_ports);
    s_receiveError = 0U; s_nextRow = 0U; s_selfTestRequested = 0U;
}

/* Function name: TsplFirmware_Busy; Description: Query immutable-label ownership.
 * Parameters: None; Returns: true while a job is retained; Global vars: s_job.
 * Remarks: Receive bytes stay in the existing bounded transport storage. */
uint8_t TsplFirmware_Busy(void)
{
    return (uint8_t)(s_job.state != TSPL_JOB_IDLE);
}

/* Function name: TsplFirmware_OwnsFeed; Description: Claim the prompt's Feed event.
 * Parameters: None; Returns: ownership; Global vars: s_job; Remarks: Includes release. */
uint8_t TsplFirmware_OwnsFeed(void)
{
    return TsplJob_OwnsFeed(&s_job);
}

/* Function name: TsplFirmware_ReceiveError; Description: Latch ISR transport loss.
 * Parameters: None; Returns: None; Global vars: s_receiveError.
 * Remarks: Actual cancellation/reporting happens only in the foreground. */
void TsplFirmware_ReceiveError(void)
{
    s_receiveError = 1U;
}

/* Function name: TsplFirmware_RequestSelfTest; Description: Defer a local diagnostic.
 * Parameters: None; Returns: None; Global vars: s_selfTestRequested.
 * Remarks: Never injects local bytes into the selected incoming language. */
void TsplFirmware_RequestSelfTest(void)
{
    s_selfTestRequested = 1U;
}

/* Function name: TsplFirmware_Consume; Description: Parse a byte without hardware work.
 * Parameters: value; Returns: parser/preflight status; Global vars: s_parser, s_job.
 * Remarks: PRINT validates native capabilities before showing an overflow prompt. */
TsplResult_t TsplFirmware_Consume(uint8_t value)
{
    TsplResult_t result;
    TsplInputs_t inputs;
    if (TsplFirmware_Busy()) return TSPL_NOT_READY;
    result = TsplParser_PushByte(&s_parser, value);
    if (result == TSPL_PRINT) {
        result = TsplNative_CheckSettings(&s_parser.label);
        if (result == TSPL_OK) result = TsplJob_Request(&s_job, &s_parser.label);
        else {
            TsplNative_Read(&inputs);
            TsplNative_Report(result, &s_parser.label, inputs.detectedHeight);
        }
        if (result != TSPL_OK) TsplParser_Cancel(&s_parser);
    } else if (result >= TSPL_SYNTAX) {
        TsplNative_Read(&inputs);
        TsplNative_Report(result, &s_parser.label, inputs.detectedHeight);
    }
    return result;
}

/* Function name: TsplFirmware_Service; Description: Advance one bounded work unit.
 * Parameters: None; Returns: None; Global vars: protocol state above.
 * Remarks: Advances confirmation even with no receive bytes; no wait loop or delay. */
void TsplFirmware_Service(void)
{
    TsplInputs_t inputs;
    TsplResult_t result;
    uint8_t value;
    uint16_t rows;
    if (s_receiveError) {
        s_receiveError = 0U;
        TsplNative_ClearReceive();
        TsplJob_Cancel(&s_job, TSPL_RECEIVE_ERROR);
        TsplParser_Cancel(&s_parser);
    }
    TsplNative_Read(&inputs);
    TsplJob_Service(&s_job, &inputs);
    if ((s_job.result >= TSPL_SYNTAX) && (s_job.state == TSPL_JOB_IDLE)) {
        TsplParser_Cancel(&s_parser); s_job.result = TSPL_OK;
    }
    if (s_job.state == TSPL_JOB_PRINTING) {
        if (s_nextRow < s_parser.label.height) {
            rows = (uint16_t)(s_parser.label.height - s_nextRow);
            if (rows > TSPL_BAND_ROWS) rows = TSPL_BAND_ROWS;
            result = TsplNative_Publish(&s_parser.label, s_nextRow, rows);
            if (result == TSPL_OK) s_nextRow = (uint16_t)(s_nextRow + rows);
            else if (result != TSPL_NOT_READY) {
                TsplJob_Cancel(&s_job, result); TsplParser_Cancel(&s_parser);
            }
        }
        return;
    }
    if (s_job.state == TSPL_JOB_CONFIRM) return;
    if (s_selfTestRequested) {
        s_selfTestRequested = 0U;
        /* An explicit local diagnostic replaces the idle pending drawing set. */
        TsplNative_SelfTest(&s_parser.label);
        result = TsplJob_Request(&s_job, &s_parser.label);
        if (result != TSPL_OK) TsplParser_Cancel(&s_parser);
        return;
    }
    if (TsplNative_Receive(&value)) {
        result = TsplFirmware_Consume(value);
        if (result >= TSPL_SYNTAX) s_parser.lastResult = result;
    }
}
#endif
