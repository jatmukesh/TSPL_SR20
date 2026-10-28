#ifndef __BSP_HP12832__H__
#define __BSP_HP12832__H__

#include "macros.h"


#define DIS_BAT_X 112
#define DIS_BAT_Y 0

#define DIS_PE_X 50
#define DIS_PE_Y 0

#define DIS_COV_X 62
#define DIS_COV_Y 0



#define DIS_SET_X 76
#define DIS_SET_Y 0

#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
#define DIS_TEMP_X 88
#define DIS_TEMP_Y 0
#endif

unsigned char GetStr8x16Wide(char * pstr,unsigned char msp);

unsigned char  oled_PrintfMap(unsigned char x,unsigned char y,unsigned char * pMapDat );

unsigned char  oled_ClearMap(unsigned char x,unsigned char y,unsigned char mClearwide );


void delflash(void);
void iicdelay(unsigned int t);
void contrast();
void write_byte(unsigned char data1);
void writed(unsigned char data1);
void WriteAddress(unsigned char c);
void DisplayBatty(unsigned char x,unsigned char y);
 void test_main(void);
void DisplayBufEn(void)  ;
void SetBattyStatus(unsigned char x,unsigned char y,unsigned short mstsa);
void DisplayStr6x8(char * pstr,unsigned char x,unsigned char y);
void ClearDisplayBuf(void) ; 
void DisplayBufEn(void) ; 
void oled_Clear(void)  ;
void SetMapToDisplayBuffer(unsigned int * ProDisplayBuffer,unsigned short x,unsigned short y,unsigned short w,unsigned short h,unsigned char  * MapBuffer);
void SetDatScreen(unsigned int * Displaybuffer)	;
void oled_init(void);

void DisplayBufEn(void)  ;
void DisplayLogo(void);
void SetCoverStatus(unsigned char x,unsigned char y,unsigned short mstsa);
unsigned char DisplayStrchar(char * pstr,unsigned char x,unsigned char y);
void SetDatScreen(unsigned int * Displaybuffer);	// DisplayData should be 164x64/8 = 1312byte;
unsigned char DisplayStr8x16char(char * pstr,unsigned char x,unsigned char y,unsigned char msp);
void DisplayBT(unsigned char x,unsigned char y,unsigned char mbt);
void SetPrinterStatus(unsigned char x,unsigned char y,unsigned short mstsa);
void DisplayBatteryL(unsigned char x,unsigned char y,unsigned char mbarry);
#endif
