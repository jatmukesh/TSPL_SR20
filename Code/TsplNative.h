/* File Name: TsplNative.h; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Minimal ports onto SR20's existing hardware and print engine. */
#ifndef TSPL_NATIVE_H
#define TSPL_NATIVE_H
#include "TsplRaster.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
void TsplNative_Read(TsplInputs_t *inputs);
TsplResult_t TsplNative_CheckSettings(const TsplLabel_t *label);
TsplResult_t TsplNative_Start(const TsplLabel_t *label);
TsplResult_t TsplNative_Publish(const TsplLabel_t *label, uint16_t row, uint16_t rows);
void TsplNative_Abort(void);
void TsplNative_Prompt(const char *first, const char *second);
void TsplNative_Report(TsplResult_t reason, const TsplLabel_t *label, uint16_t detectedHeight);
void TsplNative_Restore(void);
uint8_t TsplNative_Receive(uint8_t *value);
void TsplNative_ClearReceive(void);
void TsplNative_SelfTest(TsplLabel_t *label);
void TsplNative_Calibrated(void);
void TsplNative_Init(void);
void TsplNative_Service(void);
extern volatile uint32_t g_tsplMilliseconds;
void TsplCore_Service(void);
TsplResult_t TsplCore_Readiness(void);
void TsplCore_LabelStarted(void);
void TsplCore_LabelFinal(uint8_t buffer);
void TsplCore_LabelFinished(void);
void TsplCore_LabelAborted(void);
/* Called from existing ISR receive callbacks. Never parses or drives hardware. */
uint16_t TsplReceive_Bytes(const uint8_t *bytes, uint16_t length);
void TsplReceive_Byte(uint8_t value);
#endif
#endif
