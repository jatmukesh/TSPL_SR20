/** \file

$Id: oled.h 40486 2018-08-12 13:50:21Z tangmingfei2013@126.com $

Copyright (c)tangmingfei2013@126.com Holding B.V.
All Rights Reserved.

This source code and any compilation or derivative thereof is the proprietary
information of mingfei.tang Holding B.V. and is confidential in nature.
Under no circumstances is this software to be combined with any
Open Source Software in any way or placed under an Open Source License
of any type without the express written permission of mingfei.tang Holding B.V.
*/
#ifndef __OLED_MENU_H
#define __OLED_MENU_H

/******************************************************************************
* EXPORTED MACROS AND DEFINITIONS
******************************************************************************/
typedef enum
{
  FONT_6 = 0,
 FONT_16 = 1,

}OLED_CHARSIZE;

typedef enum
{
   PARA = 0,
   DATA = 0x40,
}OLED_PARE_REG;

typedef enum
{
   X_WIDTH = 128,
   Y_WIDTH = 64,
}OLED_SCREEN_SIZE;

/******************************************************************************
* EXPORTED FUNCTIONS
******************************************************************************/
void oled_Clear(void) ;
unsigned char  oled_ClearMap(unsigned char x,unsigned char y,unsigned char mClearwide );
void oled_Init(void);
void oled_UpScreenOn(void);
void oled_WriteRegister(unsigned char val,  unsigned char reg);
void oled_PrintfString(unsigned char x,unsigned char y, char *str);
void oled_PrintfChar(unsigned char x,unsigned char y,unsigned char val);
void oled_SetPostion( unsigned char x, unsigned char y) ;
void oled_SetCharSize( unsigned char val );
unsigned char  oled_PrintfMap(unsigned char x,unsigned char y,unsigned char * pMapDat );
void oled_SPI_WriteRegister(unsigned char  reg, unsigned char * mbuffer,unsigned short len,unsigned char swit);

void SetBattyStatus(unsigned char x,unsigned char y,unsigned short mstsa);
void SetPrinterStatus(unsigned short mstsa);
void DisplayStr6x8(char * pstr,unsigned char x,unsigned char y);
void SetBTStatus(unsigned short mstsa);
void SetPtinterOption(void);
unsigned char DisplayStrchi(char * pstr,unsigned char x,unsigned char y);
void HomeScreen(void);
void DisplayBT(void);
void DisplayOption(unsigned short Item);
void DisplayHomeMenu(unsigned short mRefreshFlag);
void DisplayUSB(void);
void DisplayBatty(unsigned char x,unsigned char y);
void ReadKeyMenu(void);
void DisplayLogo(void);
void DisplayCalibrate(void);
void displaypaper(unsigned short num);
void OLED_OFF(void);
unsigned char DisplayStrchar(char * pstr,unsigned char x,unsigned char y);
void displaypaper_All(unsigned short num);
unsigned char  oled_PrintfMapFlash(unsigned char x,unsigned char y,unsigned char *  AddrMapDat );
/******************************************************************************
* END OF C++ DECLARATION WRAPPER
******************************************************************************/



#endif /* __OLED_H */
