/* File Name: TsplJob.c; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Preflight and nonblocking overflow/copy state machine. */
#include "TsplJob.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include <string.h>

/* Function name: TsplJob_Init; Description: Initialize controller ports.
 * Parameters: job, ports; Returns: None; Global vars: None.
 * Remarks: Ports outlive controller; label storage is borrowed, never copied. */
void TsplJob_Init(TsplJob_t *job, const TsplJobPorts_t *ports)
{
    memset(job, 0, sizeof(*job)); job->ports = ports;
}

/* Function name: TsplJob_Transform; Description: Transform a glyph pixel.
 * Parameters: label, operation, pixel; Returns: None; Global vars: None.
 * Remarks: Rotation is clockwise from the text anchor; direction reverses both axes. */
void TsplJob_Transform(const TsplLabel_t *label, const TsplText_t *operation, TsplPoint_t *point)
{
    int32_t x = point->x, y = point->y;
    if (operation->rotation == 90U) { point->x = -y; point->y = x; }
    else if (operation->rotation == 180U) { point->x = -x; point->y = -y; }
    else if (operation->rotation == 270U) { point->x = y; point->y = -x; }
    point->x += (int32_t)operation->x + label->referenceX;
    point->y += (int32_t)operation->y + label->referenceY;
    if (label->direction == 0U) {
        point->x = (int32_t)label->width - 1 - point->x;
        point->y = (int32_t)label->height - 1 - point->y;
    }
}

/* Function name: TsplJob_Validate; Description: Preflight media and every extent.
 * Parameters: label, current inputs; Returns: precise failure or OK.
 * Global vars: None; Remarks: Height overflow is a separate overridable check. */
TsplResult_t TsplJob_Validate(const TsplLabel_t *label, const TsplInputs_t *inputs)
{
    uint8_t i, corner;
    uint16_t width, height;
    const TsplText_t *operation;
    TsplPoint_t point;
    if (label->invalid || label->settingsErrors || !label->width ||
        (label->width > TSPL_HEAD_DOTS) || (label->height < 96U) ||
        (label->height > 600U) || !label->copies ||
        (label->operationCount > TSPL_MAX_OPERATIONS) ||
        (label->textUsed > TSPL_TEXT_POOL_SIZE)) return TSPL_INVALID_JOB;
    if (!inputs->calibrated || !inputs->detectedHeight ||
        (inputs->nominalHeight < 12U) || (inputs->nominalHeight > 75U) ||
        (inputs->right > TSPL_HEAD_DOTS) || (inputs->left >= inputs->right)) return TSPL_MEDIA;
    if (inputs->readiness != TSPL_OK) return inputs->readiness;
    if (((uint32_t)inputs->left + label->width) > inputs->right) return TSPL_INVALID_JOB;
    for (i = 0U; i < label->operationCount; ++i) {
        operation = &label->operations[i];
        if (!operation->scaleX || !operation->scaleY ||
            (operation->scaleX > 10U) || (operation->scaleY > 10U) ||
            ((uint32_t)operation->textOffset + operation->length >= label->textUsed) ||
            label->text[operation->textOffset + operation->length] != '\0') return TSPL_INVALID_JOB;
        if (operation->length == 0U) continue;
        width = (uint16_t)(operation->length * 12U * operation->scaleX);
        height = (uint16_t)(24U * operation->scaleY);
        for (corner = 0U; corner < 4U; ++corner) {
            point.x = (corner & 1U) ? (int32_t)width - 1 : 0;
            point.y = (corner & 2U) ? (int32_t)height - 1 : 0;
            TsplJob_Transform(label, operation, &point);
            if ((point.x < 0) || (point.y < 0) ||
                (point.x >= label->width) || (point.y >= label->height)) return TSPL_INVALID_JOB;
        }
    }
    return TSPL_OK;
}

/* Function name: TsplJob_OwnsFeed; Description: Query consumed Feed ownership.
 * Parameters: job; Returns: true until safe release; Global vars: None; Remarks: None. */
uint8_t TsplJob_OwnsFeed(const TsplJob_t *job)
{
    return job->feedOwned;
}

/* Function name: TsplJob_Cancel; Description: Drop executable job and report reason.
 * Parameters: job, reason; Returns: None; Global vars: None.
 * Remarks: Abort port stops further publication; engine-owned buffers stay owned. */
void TsplJob_Cancel(TsplJob_t *job, TsplResult_t reason)
{
    if (job->state == TSPL_JOB_PRINTING) job->ports->abort();
    if (job->label != NULL) job->ports->report(reason, job->label, job->detectedHeight);
    if (job->state == TSPL_JOB_CONFIRM) job->ports->restore();
    job->state = TSPL_JOB_IDLE; job->label = NULL; job->remaining = 0U; job->result = reason;
}

/* Function name: TsplJob_Start; Description: Submit a validated copy.
 * Parameters: job; Returns: submission result; Global vars: None.
 * Remarks: Failed submission never leaves a retained executable job. */
static TsplResult_t TsplJob_Start(TsplJob_t *job)
{
    TsplResult_t result = job->ports->submit(job->label);
    if (result != TSPL_OK) { TsplJob_Cancel(job, result); return result; }
    job->state = TSPL_JOB_PRINTING; job->result = TSPL_OK;
    return TSPL_OK;
}

/* Function name: TsplJob_Request; Description: Validate and retain one PRINT.
 * Parameters: job, immutable label; Returns: preflight/submission result.
 * Global vars: None; Remarks: Oversize waits asynchronously; no engine handoff yet. */
TsplResult_t TsplJob_Request(TsplJob_t *job, const TsplLabel_t *label)
{
    TsplInputs_t inputs;
    TsplResult_t result;
    if (job->state != TSPL_JOB_IDLE) return TSPL_NOT_READY;
    job->ports->read(&inputs);
    job->label = label; job->detectedHeight = inputs.detectedHeight;
    job->mediaGeneration = inputs.mediaGeneration; job->remaining = label->copies;
    result = TsplJob_Validate(label, &inputs);
    if (result != TSPL_OK) { TsplJob_Cancel(job, result); return result; }
    if (label->height > inputs.detectedHeight) {
        job->started = inputs.now; job->feedOwned = 1U;
        job->feedArmed = (uint8_t)!inputs.feedDown; job->state = TSPL_JOB_CONFIRM;
        job->ports->report(TSPL_OVERFLOW, label, inputs.detectedHeight);
        job->ports->prompt("Size Overflow", "Print? Feed = Y");
        job->result = TSPL_OK;
        return TSPL_OK;
    }
    return TsplJob_Start(job);
}

/* Function name: TsplJob_Service; Description: Advance prompt/copy lifecycle once.
 * Parameters: job, current inputs; Returns: None; Global vars: None.
 * Remarks: Timeout wins ties; held keys cannot confirm; wrap-safe unsigned elapsed. */
void TsplJob_Service(TsplJob_t *job, const TsplInputs_t *inputs)
{
    TsplResult_t result;
    if (job->state == TSPL_JOB_IDLE) {
        if (!inputs->feedDown) job->feedOwned = 0U;
        return;
    }
    job->detectedHeight = inputs->detectedHeight;
    if (inputs->mediaGeneration != job->mediaGeneration) { TsplJob_Cancel(job, TSPL_MEDIA); return; }
    result = TsplJob_Validate(job->label, inputs);
    if (result != TSPL_OK) { TsplJob_Cancel(job, result); return; }
    if (job->state == TSPL_JOB_CONFIRM) {
        if ((uint32_t)(inputs->now - job->started) >= 5000U) { TsplJob_Cancel(job, TSPL_TIMEOUT); return; }
        if (!inputs->feedDown) job->feedArmed = 1U;
        else if (job->feedArmed) {
            job->ports->restore();
            job->state = TSPL_JOB_IDLE;
            result = TsplJob_Start(job);
            if (result != TSPL_OK) return;
        }
    } else {
        if (!inputs->feedDown) job->feedOwned = 0U;
        if (inputs->engineFailed) { TsplJob_Cancel(job, TSPL_HARDWARE); return; }
        if (inputs->copyComplete) {
            --job->remaining;
            if (job->remaining == 0U) { job->label = NULL; job->state = TSPL_JOB_IDLE; }
            else {
                result = TsplJob_Start(job);
                if (result != TSPL_OK) return;
            }
        }
    }
}
#endif
