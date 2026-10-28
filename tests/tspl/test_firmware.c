#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TsplFirmware.h"
#include "TsplNative.h"
static TsplInputs_t input;
static unsigned published, prompts, reports, errors;
static uint8_t bytes[2048];
static size_t readAt, writeAt;
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"firmware:%d: %s\n",__LINE__,#x); exit(1); } } while(0)
void TsplNative_Read(TsplInputs_t *value) { *value=input; }
TsplResult_t TsplNative_CheckSettings(const TsplLabel_t *label) { return label->gapOffset ? TSPL_UNSUPPORTED : TSPL_OK; }
TsplResult_t TsplNative_Start(const TsplLabel_t *label) { CHECK(label->height==240); return TSPL_OK; }
TsplResult_t TsplNative_Publish(const TsplLabel_t *label, uint16_t row, uint16_t rows)
{ CHECK(label->height==240); CHECK(row==published*24); CHECK(rows==24); ++published; return TSPL_OK; }
void TsplNative_Abort(void) { ++errors; }
void TsplNative_Prompt(const char *first,const char *second)
{ CHECK(!strcmp(first,"Size Overflow")); CHECK(!strcmp(second,"Print? Feed = Y")); ++prompts; }
void TsplNative_Report(TsplResult_t reason,const TsplLabel_t *label,uint16_t detected)
{ CHECK(label!=NULL); CHECK(detected==input.detectedHeight); if(reason>=TSPL_SYNTAX) ++reports; }
void TsplNative_Restore(void) { }
uint8_t TsplNative_Receive(uint8_t *value) { if(readAt==writeAt)return 0; *value=bytes[readAt++]; return 1; }
void TsplNative_ClearReceive(void) { readAt=writeAt; }
void TsplNative_SelfTest(TsplLabel_t *label) { (void)label; }
static void Queue(const char *text) { size_t length=strlen(text); CHECK(writeAt+length<=sizeof bytes); memcpy(bytes+writeAt,text,length); writeAt+=length; }
static void Setup(void)
{
    memset(&input,0,sizeof input); input.calibrated=1; input.nominalHeight=12;
    input.detectedHeight=96; input.right=384; input.readiness=TSPL_OK;
    published=prompts=reports=errors=0; readAt=writeAt=0; TsplFirmware_Init();
}
int main(void)
{
    unsigned i;
    Setup(); Queue("SIZE 48 mm,30 mm\nGAP 3 mm,0 mm\nCLS\nTEXT 0,0,\"PSF12X24.FNT\",0,1,1,\"A\"\nPRINT 1\nCLS\n");
    for(i=0;i<300;++i)TsplFirmware_Service();
    CHECK(prompts==1 && published==0 && TsplFirmware_OwnsFeed());
    CHECK(readAt<writeAt); /* Later CLS cannot mutate retained operations. */
    input.now=5000; TsplFirmware_Service(); CHECK(published==0);
    for(i=0;i<30;++i)TsplFirmware_Service();
    CHECK(!TsplFirmware_Busy());
    Setup(); Queue("SIZE 48 mm,30 mm\nGAP 3 mm,0 mm\nCLS\nPRINT 1\n");
    for(i=0;i<100;++i)TsplFirmware_Service();
    input.feedDown=1; TsplFirmware_Service();
    for(i=0;i<20;++i) { TsplFirmware_Service(); }
    CHECK(published==10);
    input.copyComplete=1; input.feedDown=0; TsplFirmware_Service(); CHECK(!TsplFirmware_Busy());
    Setup(); Queue("SIZE 48 mm,30 mm\nGAP 3 mm,0 mm\nCLS\nPRINT 1\n");
    for(i=0;i<100;++i)TsplFirmware_Service();
    TsplFirmware_ReceiveError(); TsplFirmware_Service(); CHECK(!TsplFirmware_Busy());
    input.feedDown=1; TsplFirmware_Service(); CHECK(published==0 && reports!=0);
    puts("Firmware integration passed (real parser/controller; mocked hardware only)");
    return 0;
}
