/* File Name: TsplFirmware.h; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Cooperative TSPL parser/job integration. */
#ifndef TSPL_FIRMWARE_H
#define TSPL_FIRMWARE_H
#include "TsplJob.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
void TsplFirmware_Init(void);
void TsplFirmware_Service(void);
TsplResult_t TsplFirmware_Consume(uint8_t value);
void TsplFirmware_ReceiveError(void);
uint8_t TsplFirmware_OwnsFeed(void);
uint8_t TsplFirmware_Busy(void);
void TsplFirmware_RequestSelfTest(void);
#endif
#endif
