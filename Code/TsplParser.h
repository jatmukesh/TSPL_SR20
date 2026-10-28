/* File Name: TsplParser.h
 * Author: Not recorded for this addition; Version: PSF20 R002 V4.1.0
 * Date: 2026-10-19; Description: Bounded incremental TSPL command interface.
 */
#ifndef TSPL_PARSER_H
#define TSPL_PARSER_H
#include <stdint.h>
#include "macros.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#define TSPL_MAX_OPERATIONS 24U
#define TSPL_TEXT_POOL_SIZE 2048U
#define TSPL_COMMAND_SIZE 512U
#define TSPL_DOTS_PER_MM 8U
#define TSPL_HEAD_DOTS 384U
#define TSPL_BAND_ROWS 24U

typedef enum {
    TSPL_OK = 0, TSPL_LINE, TSPL_PRINT,
    TSPL_SYNTAX = 10, TSPL_UNSUPPORTED, TSPL_RANGE, TSPL_CAPACITY,
    TSPL_INVALID_JOB, TSPL_MEDIA, TSPL_NOT_READY, TSPL_SUBMISSION,
    TSPL_TIMEOUT, TSPL_RECEIVE_ERROR, TSPL_HARDWARE, TSPL_OVERFLOW
} TsplResult_t;

typedef struct {
    int16_t x, y;
    uint16_t rotation, textOffset, length;
    uint8_t scaleX, scaleY;
} TsplText_t;

typedef struct {
    uint16_t width, height, gap;
    int16_t gapOffset, referenceX, referenceY;
    uint16_t textUsed;
    uint8_t direction, copies, operationCount, invalid, settingsErrors;
    TsplText_t operations[TSPL_MAX_OPERATIONS];
    char text[TSPL_TEXT_POOL_SIZE];
} TsplLabel_t;

typedef struct {
    TsplLabel_t label;
    uint16_t commandUsed;
    uint8_t skipLf;
    TsplResult_t dropping, lastResult;
    char command[TSPL_COMMAND_SIZE];
} TsplParser_t;

void TsplParser_Init(TsplParser_t *parser);
TsplResult_t TsplParser_PushByte(TsplParser_t *parser, uint8_t value);
void TsplParser_Cancel(TsplParser_t *parser);
const TsplLabel_t *TsplParser_GetLabel(const TsplParser_t *parser);
#endif
#endif
