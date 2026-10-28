/* File Name: TsplJob.h; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Portable label validation and asynchronous job ownership. */
#ifndef TSPL_JOB_H
#define TSPL_JOB_H
#include "TsplParser.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
typedef struct {
    uint32_t now, mediaGeneration;
    uint16_t detectedHeight, nominalHeight, left, right;
    uint8_t calibrated, feedDown, copyComplete, engineFailed;
    TsplResult_t readiness;
} TsplInputs_t;
typedef struct {
    void (*read)(TsplInputs_t *inputs);
    TsplResult_t (*submit)(const TsplLabel_t *label);
    void (*prompt)(const char *first, const char *second);
    void (*report)(TsplResult_t reason, const TsplLabel_t *label, uint16_t detectedHeight);
    void (*restore)(void);
    void (*abort)(void);
} TsplJobPorts_t;
typedef enum { TSPL_JOB_IDLE, TSPL_JOB_CONFIRM, TSPL_JOB_PRINTING } TsplJobState_t;
typedef struct {
    const TsplLabel_t *label;
    const TsplJobPorts_t *ports;
    uint32_t started, mediaGeneration;
    uint16_t detectedHeight;
    uint8_t remaining, feedOwned, feedArmed;
    TsplJobState_t state;
    TsplResult_t result;
} TsplJob_t;
typedef struct { int32_t x, y; } TsplPoint_t;
void TsplJob_Init(TsplJob_t *job, const TsplJobPorts_t *ports);
TsplResult_t TsplJob_Request(TsplJob_t *job, const TsplLabel_t *label);
void TsplJob_Service(TsplJob_t *job, const TsplInputs_t *inputs);
void TsplJob_Cancel(TsplJob_t *job, TsplResult_t reason);
uint8_t TsplJob_OwnsFeed(const TsplJob_t *job);
void TsplJob_Transform(const TsplLabel_t *label, const TsplText_t *operation, TsplPoint_t *point);
TsplResult_t TsplJob_Validate(const TsplLabel_t *label, const TsplInputs_t *inputs);
#endif
#endif
