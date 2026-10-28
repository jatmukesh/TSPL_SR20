#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TsplParser.h"
static unsigned checks;
#define CHECK(x) do { ++checks; if (!(x)) { fprintf(stderr,"limits:%d: %s\n",__LINE__,#x); exit(1); } } while(0)
static TsplResult_t Feed(TsplParser_t *p,const char *s) { TsplResult_t r=TSPL_OK; while(*s)r=TsplParser_PushByte(p,(uint8_t)*s++); return r; }
int main(void)
{
    struct { uint32_t before; TsplParser_t parser; uint32_t after; } memory;
    TsplParser_t *p=&memory.parser;
    char text[257], command[400];
    uint32_t random=1234567U;
    unsigned i;
    memory.before=0x12345678U; memory.after=0xabcdef12U;
    TsplParser_Init(p);
    memset(text,'X',255);text[255]='\0';
    CHECK(snprintf(command,sizeof command,"TEXT 0,0,\"PSF12X24.FNT\",0,1,1,\"%s\"\n",text)>0);
    for(i=0;i<8;++i)CHECK(Feed(p,command)==TSPL_LINE);
    CHECK(p->label.textUsed==2048 && p->label.operationCount==8);
    CHECK(Feed(p,command)==TSPL_CAPACITY);
    CHECK(Feed(p,"PRINT 1\n")==TSPL_INVALID_JOB);
    CHECK(Feed(p,"CLS\n")==TSPL_LINE && p->label.textUsed==0);
    text[255]='X';text[256]='\0';
    CHECK(snprintf(command,sizeof command,"TEXT 0,0,\"PSF12X24.FNT\",0,1,1,\"%s\"\n",text)>0);
    CHECK(Feed(p,command)==TSPL_CAPACITY);
    CHECK(Feed(p,"SIZE 999999999999999999 mm,12 mm\n")==TSPL_RANGE);
    CHECK(Feed(p,"SIZE 1e2 mm,12 mm\n")==TSPL_SYNTAX);
    CHECK(Feed(p,"SIZE 1.26 mm,12 mm\n")==TSPL_LINE && p->label.width==10);
    CHECK(Feed(p,"SIZE 48 mm,30 mm\r")==TSPL_LINE);
    CHECK(TsplParser_PushByte(p,'\n')==TSPL_OK);
    CHECK(Feed(p,"GAP 3 mm,0\nDIRECTION 1\nREFERENCE 0,0\nCLS\n")==TSPL_LINE);
    for(i=0;i<100000;++i) {
        TsplResult_t result;
        random=random*1664525U+1013904223U;
        result=TsplParser_PushByte(p,(uint8_t)(random>>24));
        CHECK(memory.before==0x12345678U && memory.after==0xabcdef12U);
        CHECK(p->commandUsed<TSPL_COMMAND_SIZE && p->label.operationCount<=24 && p->label.textUsed<=2048);
        if(result==TSPL_PRINT)CHECK(!p->label.invalid && !p->label.settingsErrors);
    }
    printf("Limits/chunk/framing fuzz: %u checks passed\n",checks);
    return 0;
}
