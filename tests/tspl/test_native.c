#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TsplNative.h"
#include "TsplReceive.h"
#include "PrinterParType.h"
#include "PDL.h"
#include "HP12832_01_SSD1306_I2C.h"
_PDL PRINTER_PDL;
PrintControl s_PrinterMOTOPar;
PrinterStruct s_PrintParameters;
PrintrConfig s_PrintrConfig;
PrintListPar s_ListParmeter_A, s_ListParmeter_B;
unsigned short g_BufferSwitch,g_PrintStatusReg,g_PrinterDip,g_SpeedChoice,g_LPSStatus,g_PE_Standard,g_PaperWide,g_HorShift,g_RunFlag,g_PaperOutAD;
unsigned int Receive_count,g_Order_count,g_Receive_count,g_PowerOffCount;
unsigned short g_ReceiveBusyFlag,g_ReturnBusy,g_ReturnEp,g_PaperType,g_PrintLineCount;
unsigned char receive_bufer[BUFFERSIZE];
uint16_t label_length_mm;
uint8_t g_label_size_final;
volatile unsigned char g_ThermalRasterLabelActive,g_ThermalRasterFinalChunkQueued,g_ThermalRasterFinalBuffer;
static unsigned checks, feeds, starts, finals, finishes, stops, rxErrors, oversizeRequested, oversizeDetected;
static uint8_t lid, down;
static unsigned short paperSensor;
static char report[128];
#define CHECK(x) do { ++checks; if (!(x)) { fprintf(stderr,"native:%d: %s\n",__LINE__,#x); exit(1); } } while(0)
static void Noop(void) { }
static void Stop(void) { ++stops; }
static unsigned int Lid(void) { return lid; }
static unsigned int FeedKey(void) { return !down; }
static unsigned int Temperature(void) { return 9; }
static unsigned int PaperOut(void) { return paperSensor; }
static void StartIrq(unsigned short time) { CHECK(time==10); ++starts; }
static void Send(unsigned char *bytes,unsigned short length) { CHECK(length<sizeof report); memcpy(report,bytes,length); report[length]='\0'; }
static unsigned short UsbSend(unsigned char *bytes,unsigned short length) { Send(bytes,length); return length; }
unsigned short GetPaper(void) { return g_PaperOutAD; }
void RunFeedLabPaper(void) { ++feeds; }
void OLED_RestoreHomeScreen(void) { }
void TsplCore_LabelStarted(void) { }
void TsplCore_LabelFinal(uint8_t buffer) { CHECK(buffer==1 || buffer==2); ++finals; }
void TsplCore_LabelFinished(void) { ++finishes; }
void TsplCore_LabelAborted(void) { }
TsplResult_t TsplCore_Readiness(void) { return TSPL_OK; }
void TsplFirmware_ReceiveError(void) { ++rxErrors; }
void Get_Oversize_Detection_BT_Response(uint16_t requested,uint16_t detected) { oversizeRequested=requested; oversizeDetected=detected; }
void ReadInFlashAndDeg(unsigned int offset,unsigned short width,unsigned short height,unsigned int columns[32])
{ CHECK(width==12 && height==24 && offset==1188); memset(columns,0,32U*sizeof columns[0]); columns[0]=0x80000000U; }
void ClearDisplayBuf(void) { }
unsigned char GetStr8x16Wide(char *text,unsigned char type) { CHECK(type==0); return (unsigned char)(strlen(text)*8); }
unsigned char DisplayStr8x16char(char *text,unsigned char x,unsigned char y,unsigned char type)
{ CHECK(text!=NULL && x<128 && y<32 && type==0); return (unsigned char)(x+strlen(text)*8); }
void DisplayBufEn(void) { }
static void Setup(TsplLabel_t *label)
{
    memset(&PRINTER_PDL,0,sizeof PRINTER_PDL); memset(&s_PrinterMOTOPar,0,sizeof s_PrinterMOTOPar);
    memset(&s_PrintParameters,0,sizeof s_PrintParameters); memset(label,0,sizeof *label);
    PRINTER_PDL.SYS._GetHSStatus=Lid; PRINTER_PDL.SYS._GetKey_LF=FeedKey;
    PRINTER_PDL.SYS._GetCurTemperatureLevel=Temperature; PRINTER_PDL.SYS._PaperOutDet=PaperOut; PRINTER_PDL.SYS._POWER_EN=Noop;
    PRINTER_PDL.SYS._Drive_Power_EN=Noop; PRINTER_PDL.SYS._SystemPrintIRQ_EN=StartIrq;
    PRINTER_PDL.SYS._SystemPrintIRQ_DIS=Stop; PRINTER_PDL.SYS._SystemPrintLineDat_DIS=Stop;
    PRINTER_PDL.SYS._STB_ALL_CLEAR=Stop; PRINTER_PDL.MOTO._MotoDisable=Stop;
    PRINTER_PDL.USB._ReceiveEPValid=Noop; PRINTER_PDL.BT._ReturnNBytes=Send; PRINTER_PDL.USB._ReturnNBytes=UsbSend;
    s_PrinterMOTOPar.m_StopPrint=ENABLE_VAR;
    s_PrintParameters.m_Righttpos=384; s_PrintParameters.m_PrHeadHottime=1000;
    s_PrintrConfig.m_BarPaGap=3; g_PrinterDip=3; g_LPSStatus=1000; g_PaperOutAD=1000; paperSensor=1000; g_PE_Standard=300;
    g_PaperWide=48; g_PaperType=g_HorShift=g_BufferSwitch=g_PrintStatusReg=0;
    g_PrintLineCount=0; lid=down=0;
    Receive_count=g_Order_count=g_Receive_count=0;
    feeds=starts=finals=finishes=stops=rxErrors=0;
    label_length_mm=30; g_label_size_final=30;
    label->width=384; label->height=240; label->gap=24; label->copies=1; label->direction=1;
    label->operationCount=1; label->textUsed=2; strcpy(label->text,"A");
    label->operations[0].length=1; label->operations[0].scaleX=label->operations[0].scaleY=1;
    TsplNative_Init(); TsplNative_Calibrated();
}
int main(void)
{
    TsplLabel_t label;
    TsplInputs_t input;
    unsigned i;
    Setup(&label);
    g_PaperOutAD=0; paperSensor=1000; TsplNative_Read(&input);
    CHECK(input.readiness==TSPL_OK && input.calibrated); /* Live sensor must override stale cached paper-out state. */
    CHECK(TsplNative_Start(&label)==TSPL_OK);
    CHECK(TsplNative_Publish(&label,0,24)==TSPL_OK);
    CHECK(s_ListParmeter_A.m_ListBuffer[0]==PRINT_MAP);
    CHECK(s_ListParmeter_A.m_ListBuffer[1]==0x1801 && s_ListParmeter_A.m_ListBuffer[2]==0x181);
    CHECK(s_ListParmeter_A.m_ListBuffer[4]==0x8000 && s_ListParmeter_A.m_ListIndex[1]==0xffff);
    CHECK(TsplNative_Publish(&label,24,24)==TSPL_NOT_READY);
    for(i=1;i<10;++i) {
        g_BufferSwitch=(unsigned short)~g_BufferSwitch; g_PrintStatusReg=0;
        TsplNative_Service();
        CHECK(TsplNative_Publish(&label,(uint16_t)(i*24),24)==TSPL_OK);
    }
    CHECK(finals==1 && finishes==0 && feeds==0);
    g_BufferSwitch=(unsigned short)~g_BufferSwitch; g_PrintStatusReg=0; g_PrintLineCount=1;
    s_PrinterMOTOPar.m_StopPrint=ENABLE_VAR; TsplNative_Service();
    CHECK(feeds==0); /* Stopping early is not successful physical completion. */
    Setup(&label); CHECK(TsplNative_Start(&label)==TSPL_OK);
    CHECK(TsplNative_Publish(&label,216,24)==TSPL_OK);
    g_BufferSwitch=(unsigned short)~g_BufferSwitch; g_PrintStatusReg=0; g_PrintLineCount=24;
    s_PrinterMOTOPar.m_StopPrint=ENABLE_VAR; TsplNative_Service();
    CHECK(feeds==0 && finishes==1); /* TSPL PRINT ends at its final raster row; do not advance a blank label. */
    TsplNative_Read(&input); CHECK(input.copyComplete && !input.engineFailed);
    lid=1; TsplNative_Read(&input); CHECK(!input.calibrated);
    lid=0; TsplNative_Read(&input); CHECK(!input.calibrated); /* Stale height is insufficient. */
    TsplNative_Calibrated(); TsplNative_Read(&input); CHECK(input.calibrated);
    label.gapOffset=1; CHECK(TsplNative_CheckSettings(&label)==TSPL_UNSUPPORTED);
    TsplNative_Report(TSPL_OVERFLOW,&label,96); CHECK(oversizeRequested==30 && oversizeDetected==12);
    TsplNative_Report(TSPL_INVALID_JOB,&label,96); CHECK(!strcmp(report,"TSPL_ERROR:14\r\n"));
    for(i=0;i<BUFFERSIZE;++i) TsplReceive_Byte('X');
    CHECK(Receive_count==BUFFERSIZE); TsplReceive_Byte('Y');
    CHECK(Receive_count==BUFFERSIZE && rxErrors==1);
    TsplNative_ClearReceive(); CHECK(Receive_count==0);
    TsplReceive_Byte('Z'); CHECK(Receive_count==0);
    TsplReceive_Byte('\n'); TsplReceive_Byte('C');
    { uint8_t byte=0; CHECK(TsplNative_Receive(&byte) && byte=='C'); }
    printf("Native engine/transport ports: %u checks passed\n",checks);
    return 0;
}
