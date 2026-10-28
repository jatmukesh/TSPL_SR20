/* File Name: TsplParser.c
 * Author: Not recorded for this addition; Version: PSF20 R002 V4.1.0
 * Date: 2026-10-19; Description: Incremental TSPL subset, without hardware access.
 */
#include "TsplParser.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include <string.h>
#include <limits.h>

typedef struct { char *value; uint8_t quoted; } TsplField_t;

/* Function name: TsplParser_Init; Description: Reset framing and settings.
 * Parameters: parser storage; Returns: None; Global vars: None.
 * Remarks: SIZE and GAP must be established before printing. */
void TsplParser_Init(TsplParser_t *parser)
{
    memset(parser, 0, sizeof(*parser));
    parser->label.settingsErrors = 3U;
    parser->label.copies = 1U;
}

/* Function name: TsplParser_GetLabel; Description: Borrow pending label.
 * Parameters: parser; Returns: read-only label; Global vars: None.
 * Remarks: Consumer must suspend input while the label is owned by the engine. */
const TsplLabel_t *TsplParser_GetLabel(const TsplParser_t *parser)
{
    return &parser->label;
}

/* Function name: TsplParser_Cancel; Description: Prevent stale-job execution.
 * Parameters: parser; Returns: None; Global vars: None.
 * Remarks: Keep settings, require CLS before a new executable design. */
void TsplParser_Cancel(TsplParser_t *parser)
{
    parser->label.operationCount = 0U;
    parser->label.textUsed = 0U;
    parser->label.invalid = 1U;
    parser->commandUsed = 0U;
}

/* Function name: Tspl_Space; Description: Test grammar whitespace.
 * Parameters: value; Returns: true for space/tab; Global vars: None; Remarks: None. */
static uint8_t Tspl_Space(char value)
{
    return (uint8_t)((value == ' ') || (value == '\t'));
}

/* Function name: Tspl_Fields; Description: Split bounded CSV in place.
 * Parameters: input, fields, count; Returns: parser result; Global vars: None.
 * Remarks: Quotes are required around text; commas within quotes are data.
 * TSPL uses backslash, bracket, quote, bracket for an embedded quote. */
static TsplResult_t Tspl_Fields(char *input, TsplField_t fields[8], uint8_t *count)
{
    char *end, *write;
    *count = 0U;
    while (Tspl_Space(*input)) ++input;
    if (*input == '\0') return TSPL_OK;
    for (;;) {
        if (*count == 8U) return TSPL_SYNTAX;
        while (Tspl_Space(*input)) ++input;
        fields[*count].quoted = (uint8_t)(*input == '"');
        if (*input == '"') {
            ++input;
            fields[*count].value = input;
            write = input;
            while ((*input != '\0') && (*input != '"')) {
                if ((input[0] == '\\') && (input[1] == '[') &&
                    (input[2] == '"') && (input[3] == ']')) {
                    *write++ = '"'; input += 4;
                    continue;
                }
                *write++ = *input++;
            }
            if (*input != '"') return TSPL_SYNTAX;
            ++input;
            *write = '\0';
            while (Tspl_Space(*input)) ++input;
            if ((*input != ',') && (*input != '\0')) return TSPL_SYNTAX;
        } else {
            fields[*count].value = input;
            while ((*input != '\0') && (*input != ',')) {
                if (*input == '"') return TSPL_SYNTAX;
                ++input;
            }
            end = input;
            while ((end > fields[*count].value) && Tspl_Space(end[-1])) --end;
            if (end == fields[*count].value) return TSPL_SYNTAX;
            /* Save delimiter before terminating an unquoted field. */
            if (*input == ',') {
                ++input; *end = '\0'; ++*count;
                if (*input == '\0') return TSPL_SYNTAX;
                continue;
            }
            *end = '\0';
        }
        ++*count;
        if (*input == '\0') return TSPL_OK;
        ++input;
        if (*input == '\0') return TSPL_SYNTAX;
    }
}

/* Function name: Tspl_Number; Description: Checked integer/fixed-point conversion.
 * Parameters: field, dimension flag, result; Returns: validation result.
 * Global vars: None; Remarks: Dimensions default to inches; explicit mm/dot accepted.
 * At most four fractional decimal places; conversion rounds to nearest device dot. */
static TsplResult_t Tspl_Number(const TsplField_t *field, uint8_t dimension, int32_t *result)
{
    const char *input = field->value;
    uint32_t whole = 0U, fraction = 0U, divisor = 1U, numerator, denominator;
    uint8_t negative = 0U, digits = 0U, unitSpace = 0U;
    if (field->quoted) return TSPL_SYNTAX;
    if (*input == '-') { negative = 1U; ++input; }
    while ((*input >= '0') && (*input <= '9')) {
        if (whole > 100000U) return TSPL_RANGE;
        whole = whole * 10U + (uint32_t)(*input++ - '0'); digits = 1U;
    }
    if (!digits) return TSPL_SYNTAX;
    if (*input == '.') {
        if (!dimension) return TSPL_SYNTAX;
        ++input; digits = 0U;
        while ((*input >= '0') && (*input <= '9')) {
            if (divisor == 10000U) return TSPL_RANGE;
            divisor *= 10U; fraction = fraction * 10U + (uint32_t)(*input++ - '0'); digits = 1U;
        }
        if (!digits) return TSPL_SYNTAX;
    }
    while (Tspl_Space(*input)) { unitSpace = 1U; ++input; }
    numerator = dimension ? 1016U : 1U;
    denominator = dimension ? 5U : 1U;
    if (dimension && unitSpace && (strcmp(input, "mm") == 0)) { numerator = 8U; denominator = 1U; }
    else if (dimension && unitSpace && (strcmp(input, "dot") == 0)) { numerator = 1U; denominator = 1U; }
    else if (*input != '\0') return TSPL_SYNTAX;
    /* Bound before scaling so no intermediate can wrap. */
    if (whole > 32767U) return TSPL_RANGE;
    if (whole > (UINT32_MAX - fraction) / divisor) return TSPL_RANGE;
    whole = whole * divisor + fraction;
    denominator *= divisor;
    if (whole > (UINT32_MAX - denominator / 2U) / numerator) return TSPL_RANGE;
    whole = (whole * numerator + denominator / 2U) / denominator;
    if (whole > 32767U) return TSPL_RANGE;
    *result = negative ? -(int32_t)whole : (int32_t)whole;
    return TSPL_OK;
}

/* Function name: Tspl_Command; Description: Validate then commit one command.
 * Parameters: parser, mutable line; Returns: command result; Global vars: None.
 * Remarks: Failed settings remain invalid across CLS; drawing failures latch. */
static TsplResult_t Tspl_Command(TsplParser_t *parser, char *line)
{
    TsplField_t fields[8];
    TsplLabel_t *label = &parser->label;
    TsplText_t operation;
    TsplResult_t result;
    char *command = line;
    uint8_t count, i, setting = 0U;
    int32_t values[8];
    size_t length;
    while (Tspl_Space(*command)) ++command;
    if (*command == '\0') return TSPL_OK;
    line = command;
    while ((*line != '\0') && !Tspl_Space(*line)) ++line;
    if (*line != '\0') *line++ = '\0';
    if (strcmp(command, "SIZE") == 0) setting = 1U;
    else if (strcmp(command, "GAP") == 0) setting = 2U;
    else if (strcmp(command, "DIRECTION") == 0) setting = 4U;
    else if (strcmp(command, "REFERENCE") == 0) setting = 8U;
    if (setting != 0U) label->settingsErrors |= setting;
    result = Tspl_Fields(line, fields, &count);
    if (result != TSPL_OK) return result;
    if (strcmp(command, "CLS") == 0) {
        if (count != 0U) return TSPL_SYNTAX;
        label->operationCount = 0U; label->textUsed = 0U; label->invalid = 0U;
        return TSPL_LINE;
    }
    if (setting != 0U) {
        if ((setting == 4U) && ((count == 1U) || (count == 2U))) {
            result = Tspl_Number(&fields[0], 0U, &values[0]);
            if (result != TSPL_OK) return result;
            values[1] = 0;
            if (count == 2U) {
                result = Tspl_Number(&fields[1], 0U, &values[1]);
                if (result != TSPL_OK) return result;
            }
            if ((values[0] < 0) || (values[0] > 1) || (values[1] != 0)) return TSPL_UNSUPPORTED;
            label->direction = (uint8_t)values[0];
        } else {
            if (count != 2U) return TSPL_SYNTAX;
            for (i = 0U; i < 2U; ++i) {
                result = Tspl_Number(&fields[i], (uint8_t)(setting < 4U), &values[i]);
                if (result != TSPL_OK) return result;
            }
            if (setting == 1U) {
                if ((values[0] < 1) || (values[0] > 384) || (values[1] < 96) || (values[1] > 600)) return TSPL_RANGE;
                label->width = (uint16_t)values[0]; label->height = (uint16_t)values[1];
            } else if (setting == 2U) {
                if ((values[0] < 0) || (values[0] > 600) || (values[1] < -600) || (values[1] > 600)) return TSPL_RANGE;
                if (values[0] == 0) return TSPL_UNSUPPORTED;
                label->gap = (uint16_t)values[0]; label->gapOffset = (int16_t)values[1];
            } else if (setting == 8U) {
                label->referenceX = (int16_t)values[0]; label->referenceY = (int16_t)values[1];
            } else return TSPL_SYNTAX;
        }
        label->settingsErrors &= (uint8_t)~setting;
        return TSPL_LINE;
    }
    if (strcmp(command, "PRINT") == 0) {
        if ((count != 1U) && (count != 2U)) return TSPL_SYNTAX;
        result = Tspl_Number(&fields[0], 0U, &values[0]);
        if (result != TSPL_OK) return result;
        if (values[0] != 1) return TSPL_UNSUPPORTED;
        values[1] = 1;
        if (count == 2U) {
            result = Tspl_Number(&fields[1], 0U, &values[1]);
            if (result != TSPL_OK) return result;
        }
        if ((values[1] < 1) || (values[1] > 255)) return TSPL_RANGE;
        if (label->invalid || label->settingsErrors) return TSPL_INVALID_JOB;
        label->copies = (uint8_t)values[1];
        return TSPL_PRINT;
    }
    if (strcmp(command, "TEXT") != 0) return TSPL_UNSUPPORTED;
    if (count == 8U) return TSPL_UNSUPPORTED;
    if (count != 7U) return TSPL_SYNTAX;
    if (!fields[2].quoted || !fields[6].quoted) return TSPL_SYNTAX;
    if (strcmp(fields[2].value, "PSF12X24.FNT") != 0) return TSPL_UNSUPPORTED;
    for (i = 0U; i < 6U; ++i) {
        if (i == 2U) continue;
        result = Tspl_Number(&fields[i], 0U, &values[i]);
        if (result != TSPL_OK) return result;
    }
    if ((values[3] != 0) && (values[3] != 90) && (values[3] != 180) && (values[3] != 270)) return TSPL_UNSUPPORTED;
    if ((values[4] < 1) || (values[4] > 10) || (values[5] < 1) || (values[5] > 10)) return TSPL_RANGE;
    length = strlen(fields[6].value);
    if ((length > 255U) || (label->operationCount == TSPL_MAX_OPERATIONS) ||
        ((size_t)label->textUsed + length + 1U > TSPL_TEXT_POOL_SIZE)) return TSPL_CAPACITY;
    for (i = 0U; (size_t)i < length; ++i) {
        if (((uint8_t)fields[6].value[i] < 32U) || ((uint8_t)fields[6].value[i] > 126U)) return TSPL_UNSUPPORTED;
        /* length <= 255; stop before the uint8_t counter can wrap. */
        if ((size_t)i + 1U == length) break;
    }
    operation.x = (int16_t)values[0]; operation.y = (int16_t)values[1];
    operation.rotation = (uint16_t)values[3];
    operation.scaleX = (uint8_t)values[4]; operation.scaleY = (uint8_t)values[5];
    operation.textOffset = label->textUsed; operation.length = (uint16_t)length;
    memcpy(label->text + label->textUsed, fields[6].value, length + 1U);
    label->textUsed = (uint16_t)(label->textUsed + length + 1U);
    label->operations[label->operationCount++] = operation;
    return TSPL_LINE;
}

/* Function name: TsplParser_PushByte; Description: Preserve arbitrary input chunks.
 * Parameters: parser, byte; Returns: command/status result; Global vars: None.
 * Remarks: Oversized/malformed lines drain to a boundary; no hardware side effects. */
TsplResult_t TsplParser_PushByte(TsplParser_t *parser, uint8_t value)
{
    TsplResult_t result;
    if (parser->skipLf && (value == '\n')) { parser->skipLf = 0U; return TSPL_OK; }
    parser->skipLf = 0U;
    if ((value == '\n') || (value == '\r')) {
        parser->skipLf = (uint8_t)(value == '\r');
        parser->command[parser->commandUsed] = '\0';
        result = parser->dropping ? parser->dropping : Tspl_Command(parser, parser->command);
        parser->commandUsed = 0U; parser->dropping = TSPL_OK;
        if (result >= TSPL_SYNTAX) parser->label.invalid = 1U;
        parser->lastResult = result;
        return result;
    }
    if (parser->dropping) return TSPL_OK;
    if (((value < 32U) && (value != '\t')) || (value > 126U)) {
        parser->dropping = TSPL_SYNTAX; parser->label.invalid = 1U; return TSPL_SYNTAX;
    }
    if (parser->commandUsed == TSPL_COMMAND_SIZE - 1U) {
        parser->dropping = TSPL_CAPACITY; parser->label.invalid = 1U; return TSPL_CAPACITY;
    }
    parser->command[parser->commandUsed++] = (char)value;
    return TSPL_OK;
}
#endif
