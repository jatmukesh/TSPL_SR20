#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TsplJob.h"

static unsigned checks, submissions, prompts, restores, reports, aborts;
static TsplInputs_t input;
static TsplResult_t submitResult, lastReport;
#define CHECK(x) do { ++checks; if (!(x)) { fprintf(stderr,"job:%d: %s\n",__LINE__,#x); exit(1); } } while(0)
static void Read(TsplInputs_t *value) { *value = input; }
static TsplResult_t Submit(const TsplLabel_t *label) { CHECK(label->height == 240); ++submissions; return submitResult; }
static void Prompt(const char *first, const char *second) { CHECK(strcmp(first,"Size Overflow")==0); CHECK(strcmp(second,"Print? Feed = Y")==0); ++prompts; }
static void Report(TsplResult_t reason, const TsplLabel_t *label, uint16_t height) { CHECK(label != NULL); CHECK(height == input.detectedHeight); lastReport=reason; ++reports; }
static void Restore(void) { ++restores; }
static void Abort(void) { ++aborts; }
static const TsplJobPorts_t ports = {Read,Submit,Prompt,Report,Restore,Abort};
static void Setup(TsplJob_t *job, TsplLabel_t *label)
{
    memset(&input,0,sizeof input); memset(label,0,sizeof *label);
    input.detectedHeight=240; input.nominalHeight=30; input.right=384;
    input.calibrated=1; input.readiness=TSPL_OK;
    label->width=384; label->height=240; label->gap=24; label->copies=1;
    submissions=prompts=restores=reports=aborts=0; submitResult=TSPL_OK;
    TsplJob_Init(job,&ports);
}
int main(void)
{
    TsplJob_t job;
    TsplLabel_t label;
    TsplPoint_t point = {0,0};
    TsplText_t origin;
    memset(&origin,0,sizeof origin);
    Setup(&job,&label);
    label.direction=0;
    TsplJob_Transform(&label,&origin,&point);
    CHECK(point.x==383 && point.y==239); /* TSC Direction 0 reference diagram. */
    Setup(&job,&label);
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK); CHECK(submissions==1);
    CHECK(job.state==TSPL_JOB_PRINTING);
    input.copyComplete=1; TsplJob_Service(&job,&input); CHECK(job.state==TSPL_JOB_IDLE);
    Setup(&job,&label); input.detectedHeight=96; input.nominalHeight=12;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK); CHECK(prompts==1 && submissions==0);
    CHECK(TsplJob_OwnsFeed(&job)); CHECK(lastReport==TSPL_OVERFLOW);
    input.now=4999; TsplJob_Service(&job,&input); CHECK(submissions==0);
    input.feedDown=1; TsplJob_Service(&job,&input); CHECK(submissions==1 && restores==1);
    CHECK(TsplJob_OwnsFeed(&job));
    input.feedDown=0; TsplJob_Service(&job,&input); CHECK(!TsplJob_OwnsFeed(&job));
    Setup(&job,&label); input.detectedHeight=96; input.nominalHeight=12; input.feedDown=1;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK);
    input.now=1; TsplJob_Service(&job,&input); CHECK(submissions==0);
    input.feedDown=0; TsplJob_Service(&job,&input);
    input.feedDown=1; TsplJob_Service(&job,&input); CHECK(submissions==1);
    Setup(&job,&label); input.detectedHeight=96; input.nominalHeight=12;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK);
    input.now=5000; input.feedDown=1; TsplJob_Service(&job,&input);
    CHECK(submissions==0 && job.result==TSPL_TIMEOUT && job.label==NULL);
    Setup(&job,&label); input.detectedHeight=96; input.nominalHeight=12; input.now=0xfffffff0U;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK);
    input.now=4984; TsplJob_Service(&job,&input); CHECK(job.result==TSPL_TIMEOUT);
    Setup(&job,&label); input.calibrated=0;
    CHECK(TsplJob_Request(&job,&label)==TSPL_MEDIA && submissions==0);
    Setup(&job,&label); input.readiness=TSPL_NOT_READY;
    CHECK(TsplJob_Request(&job,&label)==TSPL_NOT_READY && submissions==0);
    Setup(&job,&label); submitResult=TSPL_SUBMISSION;
    CHECK(TsplJob_Request(&job,&label)==TSPL_SUBMISSION && submissions==1);
    Setup(&job,&label); input.detectedHeight=96; input.nominalHeight=12;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK);
    input.mediaGeneration=1; input.feedDown=1; TsplJob_Service(&job,&input);
    CHECK(job.result==TSPL_MEDIA && submissions==0);
    Setup(&job,&label); label.copies=2;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK);
    input.copyComplete=1; TsplJob_Service(&job,&input); CHECK(submissions==2);
    input.engineFailed=1; TsplJob_Service(&job,&input); CHECK(job.result==TSPL_HARDWARE && aborts==1);
    Setup(&job,&label);
    label.operationCount=1; label.textUsed=2; strcpy(label.text,"A");
    label.operations[0].length=1; label.operations[0].scaleX=1; label.operations[0].scaleY=1;
    label.direction=1; label.operations[0].x=373;
    CHECK(TsplJob_Request(&job,&label)==TSPL_INVALID_JOB && submissions==0);
    label.operations[0].x=372;
    CHECK(TsplJob_Request(&job,&label)==TSPL_OK);
    TsplJob_Cancel(&job,TSPL_RECEIVE_ERROR); CHECK(job.label==NULL);
    puts("TSPL job controller passed");
    printf("Job: %u checks passed\n",checks);
    return 0;
}
