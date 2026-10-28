/** \file

$Id: oled.c 40486 2018-08-12 13:50:21Z tangmingfei2013@126.com $

Copyright (c)tangmingfei2013@126.com Holding B.V.
All Rights Reserved.

This source code and any compilation or derivative thereof is the proprietary
information of mingfei.tang Holding B.V. and is confidential in nature.
Under no circumstances is this software to be combined with any
Open Source Software in any way or placed under an Open Source License
of any type without the express written permission of mingfei.tang Holding B.V.
*/
/*******************************************************************************
* EXPORT INCLUDE FILES
*******************************************************************************/
//#include "I2C.h"

/*******************************************************************************
* LOCAL INCLUDE FILES
*******************************************************************************/
#include "RD_V2_DeviceBSP.h"
#include "HP12832_01_SSD1306_I2C.h"
#include "PDL.h"
#include "PrinterParType.h"
#include "Audio_Indication.h"

/*******************************New Firmware Modification**********************************/
#ifdef FIVE_DARKNESS_LEVEL_CONFIG
	#define VAL_DARKNESS_LIMIT		5 	//Simplified 1-5 Scale Index Options
#else
	#define VAL_DARKNESS_LIMIT		10	// Original 10 Levels
#endif

#ifdef SYSTEM_SOUND_SETTING	
extern unsigned char g_System_Sound_Flag;
extern void BuzzerManager_UpdateUI(void); 
#endif

#ifdef POWER_ON_CAL
extern unsigned char g_boundary_detection_flag;
extern uint8_t g_label_size_final;
#endif

#ifdef AUTO_CAL_DISCARD_OPTION
extern char *mediaType;
extern uint16_t g_boundary_gap_count_val;
extern uint16_t label_length_mm;
extern void  Delay1ms(unsigned short mdelay);
extern void OLED_RestoreHomeScreen(void);

#endif

#ifdef TEST_SPEED_DARKNESS_ACK_COMMAND_ADDITION
void Get_Darkness_Speed_cmd(uint16_t Set_Value, char* string);
#endif	

#ifdef MEDIA_UNKNOWN_DETECTION_IN_SETTING_MENU
extern unsigned int User_calibration_discarded_flag;
#endif
//--------------------------------------------------------------------
extern _PDL		PRINTER_PDL ;
extern unsigned short g_PaperType;
unsigned short g_Item,g_ItemSave;
unsigned short g_ChiEng;
extern unsigned int g_PowerOffTimeOut;
extern void readconfigcon(void);
extern void DisplayBTPro(void);
extern void SetSelftestData(void);
extern void SavePrinterConfig(void);
extern void PrinterConfigInt(void);
extern void delaynms(unsigned int mdelay);
void DisSetBPS(unsigned char mdens);
void DisplayOption(unsigned short Item);
extern unsigned int	g_THDeep;//增加加热值
extern unsigned int	g_Speed;//速度
/*---------------------------正在打印---------------------------------------------------
;  若数据乱码，请检查字模格式设置，注意选择正确的取模方向和字节位顺序。
;  源文件 / 文字 : ?? ?
;  宽×高（像素）: 67×24
;  字模格式/大小 : 单色点阵液晶字模，纵向取模，字节倒序/201字节
;  数据转换日期  : 2019/12/16 13:43:24
------------------------------------------------------------------------------*/
/******************************************************************************
* LOCAL MACROS AND DEFINITIONS
******************************************************************************/
#define OLED_DEBUG      1
#define OLED_ADDRESS    0x78
unsigned short gHomeMenuRefreshFlag;

extern unsigned int g_PowerOffCount;
extern PrintrConfig	s_PrintrConfig;//打印机配置参数

extern void ReadPrinterConfig(void);
static unsigned char charSize;


extern const unsigned char NoPaperNULL[];
extern const unsigned char NoPaper[];


extern void RunFeedLabPaper(void);












const unsigned char  PrintStatus5[]=
{
0x40,0x10,
0x10,0x10,0x10,0xFF,0x10,0x90,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,
0x00,0xFC,0x44,0x44,0x44,0x42,0x42,0x00,0xFC,0x04,0x04,0x04,0x04,0xFC,0x00,0x00,
0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,
0x00,0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,
0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,
0x00,0x1F,0x10,0x10,0x08,0x08,0x08,0x00,0xFF,0x00,0x08,0x10,0x08,0x07,0x00,0x00,
0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00,
0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,
};

const unsigned char PaperEnd[]=
{
0x40,0x10,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x48,0x47,0xFC,
0x44,0x44,0x40,0x08,0x08,0x08,0xFF,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x30,0xAC,0x63,0x30,0x00,0xFC,0x84,0x84,0x84,0xFE,0x82,
0x83,0x82,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x20,0x1F,
0x10,0x7E,0x81,0x41,0x31,0x0D,0x03,0x0D,0x31,0x41,0x81,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x22,0x67,0x22,0x12,0x12,0x00,0xFF,0x40,0x20,0x00,0x01,0x0E,
0x30,0x40,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const unsigned char SET_BMP[]=
{
0x0D,0x0C,
0x7F,0xE0,0xF9,0xF0,0xC9,0x30,0xC0,0x30,0xE6,0x70,0x89,0x10,0x89,
0x10,0xE6,0x70,0xC0,0x30,0xC9,0x30,0xF9,0xF0,0x7F,0xE0,0x00,0x00,
};
const unsigned char SET_BMPNULL[]=
{
0x0D,0x0C,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};









void oled_PrintfString(unsigned char x,unsigned char y,char *str  )
{
   DisplayStr6x8(str,x,y*8);
}

/* End of this file */
//104,0



void SetPtinterOption(void)
{


}


unsigned char DisplayStrchi(char * pstr,unsigned char x,unsigned char y)
{
	
	return  0;
}

unsigned char DisplayStrchar8x16(char * pstr,unsigned char x,unsigned char y)
{

return  0;

}


#define 	REFRESH_BT				0x0001//刷新蓝牙

#define 	REFRESH_OPTION		0x0002//刷新設置


#define 	REFRESH_PE				0x0004//刷新缺纸图标


unsigned short g_StatusBT;
extern unsigned short g_OptionStatus;
unsigned short g_paperStatus;
unsigned short g_PrinterStatus;
unsigned short g_UsbConnect;

extern unsigned short g_Battystatus;
//显示和按键调用

//void oled_Clear_All(void)
//{
//  OLED_Clear();

//	g_StatusBT = 0xFFF;
//	g_paperStatus = 0xFFF;
//	g_PrinterStatus = 0xFFF;
//	g_UsbConnect = 0xFFF;
//	g_Battystatus = 0xFFF;
//}


//=====显示设置图标=================================
extern void GetLabPaperSta(void);
extern 	unsigned int  g_PowerOffCount;
#ifdef NEW_UI_MENU_SETTINGS
const unsigned char  MenuItemdeail[]={1,2,2,4,6,1,1,5,5,1,1}; // selectable options in menu
#else
const unsigned char  MenuItemdeail[]={1,2,6,2,1,10,5,1,1,1,15,1,1};
#endif
extern unsigned int g_optionCount;

#ifdef NEW_UI_MENU_SETTINGS

/**
* @FuncName  ReadKeyMenu
* @param     void
* @retval    void
* @LocalVar  mItem, mToatol, mCount, mstatus, mtep, max_items, cal_pos, self_pos, alert_pos
* @GlobalVar g_OptionStatus, g_Item, PRINTER_PDL, g_PowerOffCount, g_optionCount,
*            s_PrintrConfig, MenuItemdeail, g_KeySoundEnable, g_System_Sound_Flag,
*            g_ItemSave, gHomeMenuRefreshFlag
* @Description Polling handler for user key button navigation within the OLED settings menu.
*
*/

void ReadKeyMenu(void)
{
    unsigned char mItem, mToatol, mCount, mstatus;
    unsigned short mtep;
	  unsigned char max_items = 8; // Base: MAC, Sound, Shutdown, Paper, Cal, Dark, Speed, Self
    mstatus = 0;

    if(g_OptionStatus)
    {
        mItem = g_Item >> 8;
        
        // --- 1. Power Button: Main Menu Navigation ---
        if(PRINTER_PDL.SYS._Get_Plus_POW())                
        {        
            g_PowerOffCount = 0;
            g_optionCount = 0;
            mstatus = 1;
            mItem++;
            
            // Total items based on your sequence (MAC, Sound, Key, Alerts, Shutdown, Paper, Cal, Dark, Speed, Self)
            
#ifdef PWR_USER_BTN_KEY_AUDIO_EN  //Calculate dynamic total item count based on compiled feature macros
            max_items++; 
#endif
#ifdef AUDIO_BEEP_SEQUENCE
            max_items++; 
#endif

            if(mItem > max_items) { mItem = 0x01; }

            // Re-acquire saved sub-option for the new main menu item
            ReadPrinterConfig();
            g_Item = s_PrintrConfig.m_SaveConPar[mItem - 1] & 0xFF;
            g_Item |= (mItem << 8);
        }

        // --- 2. LF Button: Sub-menu Toggle & Actions ---
        if(PRINTER_PDL.SYS._Get_Plus_LF() == 1)
        {
#ifdef PWR_USER_BTN_KEY_AUDIO_EN
            if(g_KeySoundEnable) 
            { 
							Audio_Beep_Custom(1, 30, 0);  //Play 30ms key press feedback
						}
#endif            
            mstatus = 1;
            g_PowerOffCount = 0;
						g_optionCount = 0;
            mToatol = MenuItemdeail[mItem - 1];
            mCount = g_Item & 0xFF;

            if(mToatol == 1) // Direct Actions (Calibration / SelfTest)
            {
                // Sequence check: MAC(1), Sound(2), Key(3), Alert(4), Shut(5), Paper(6), CAL(7)
               // unsigned char cal_pos = 5; 
#ifndef PWR_USER_BTN_KEY_AUDIO_EN 
                cal_pos++; 
#endif
#ifndef AUDIO_BEEP_SEQUENCE 
                cal_pos++; 
#endif
								
							// Calibration Alignment
          if(mItem == 7) 
					{
#ifdef AUTO_CAL_DISCARD_OPTION
						unsigned int m_w;
						g_CAL_Mode_statusFlag = 1; // 1. Enable boundary/gap detection mode
						
						Power_ON_CAL_Status = PowerON_Calibration();
						
						PRINTER_PDL.SYS._WDG_ReloadCounter();
						
						if (Power_ON_CAL_Status == 1)
						{
							ClearDisplayBuf();
									if(g_boundary_detection_flag == 1)
										{
											Label_Size_Calculation(g_boundary_gap_count_val);	// calculate lab size
											mediaType = "L";
											// Update Persistent Settings
											s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN; 
											s_PrintrConfig.m_BarPaGap = 3; // Standard Gap 3mm
											s_PrintrConfig.m_LabelLength = g_label_size_final;

											m_w = GetStr8x16Wide((char*)"Media: Label", 1);
											DisplayStr8x16char("Media: Label", (128-m_w)/2, 0, 1);

											char size_buf[32]; // Temporary buffer for formatting
											sprintf(size_buf, "Size: @%d mm", label_length_mm); // label_length_mm

											m_w = GetStr8x16Wide(size_buf, 1);
											DisplayStr8x16char(size_buf, (128-m_w)/2, 16, 1);
											DisplayBufEn();	
											PRINTER_PDL.SYS._WDG_ReloadCounter();			
										}
										else
										{
											mediaType = "P";
#ifndef DISCARDING_CAL_SET_PREV_SR20_FUNCTIONALITY		   //[Modify by: Kajal ---> 10 Oct 2026] ---> see macro definition											
											s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS; 
#endif											
											m_w = GetStr8x16Wide((char*)"Media: Paper", 1);
											DisplayStr8x16char("Media: Paper", (128-m_w)/2, 14, 1);
										}
										
										DisplayBufEn();
										Delay1ms(2000);
							
										ClearDisplayBuf();
										m_w = GetStr8x16Wide((char*)"Calibration Done", 0);
										DisplayStr8x16char("Calibration Done", (128-m_w)/2, 14, 0);
										
										DisplayBufEn();
										Delay1ms(2000);   //wait 2secs
												
										//g_CAL_Mode_statusFlag = 0; // Reset calibration mode
														
						}
							else if (Power_ON_CAL_Status == 3 || Power_ON_CAL_Status == 4) // Failure codes
						{
					#ifdef AUDIO_BEEP_SEQUENCE
							BuzzerManager_Init((Power_ON_CAL_Status == 3), (Power_ON_CAL_Status == 4));
					#endif
							ClearDisplayBuf();
							m_w = GetStr8x16Wide((char*)"CAL Failed", 1);
							DisplayStr8x16char("CAL Failed", (128-m_w)/2, 14, 1);
							DisplayBufEn();
							Delay1ms(2000);

							ClearDisplayBuf();
							if (Power_ON_CAL_Status == 3)
							{
								mediaType = "A";
								m_w = GetStr8x16Wide((char*)"Media Absent", 1);
								DisplayStr8x16char("Media Absent", (128-m_w)/2, 14, 1);
							}
							else if (Power_ON_CAL_Status == 4)
							{
								mediaType = "LO";
								m_w = GetStr8x16Wide((char*)"Lid Open", 1);
								DisplayStr8x16char("Lid Open", (128-m_w)/2, 14, 1);
							}
							DisplayBufEn();
							Delay1ms(2000);
						}
						
						g_CAL_Mode_statusFlag = 0; // 2. Always reset calibration mode flag
						OLED_RestoreHomeScreen();
						g_OptionStatus = 0;
						mstatus = 0;
						return;									
#else									
                    RunFeedLabPaper();//GetLabPaperSta(); //delaynms(100); // 500
									g_OptionStatus = 0;          
#endif								
								}
                
                // SelfTest is always the last item
               // unsigned char self_pos = cal_pos + 3; 
                if(mItem == 10) { // Self Test
                    SetSelftestData(); 
									  g_OptionStatus = 0; 
                }
            }
            else // Parameter Toggles
            {
                mCount++;
                if(mCount > (mToatol - 1)) { mCount = 0; }
                g_Item = (g_Item & 0xFF00) | mCount;
                
                // NEW SEQUENCE LOGIC TRIGGERS:
#ifdef SYSTEM_SOUND_SETTING										
                if(mItem == 2) { // Sound Master							
                    g_System_Sound_Flag = (mCount == 0) ? 1 : 0; 
                }
#endif								
#ifdef PWR_USER_BTN_KEY_AUDIO_EN
                if(mItem == 3) { // Key Sound
                    extern unsigned char g_KeySoundEnable;
                    g_KeySoundEnable = (mCount == 0) ? 1 : 0;
                }
#endif
#ifdef AUDIO_BEEP_SEQUENCE
                // If Key Audio is present, Alerts is #4, else it's #3
                unsigned char alert_pos = 3;
#ifdef PWR_USER_BTN_KEY_AUDIO_EN 
                alert_pos++; 
#endif
                if(mItem == alert_pos) {
                    BuzzerManager_UpdateUI(); 
                }
#endif
                // Save parameters to NVRAM slot corresponding to visual index
                s_PrintrConfig.m_SaveConPar[mItem - 1] = g_Item;            
                SavePrinterConfig();
            }
        }

        // --- 3. Refresh Display ---
        if((g_ItemSave != g_Item) || (mstatus == 1))
        {
            __SETPRIMASK();
           // g_PowerOffCount = 0;
					//Below code block is added to recover home screen after calibration done via setting menu					
							if(g_OptionStatus == 0 )
							{
								g_PowerOffCount = 1005; // Bypass 1s home screen delay on exit
							}
							else
							{
								g_PowerOffCount = 0;
							}
            __RESETPRIMASK();
            g_ItemSave = g_Item;
            DisplayOption(g_Item);
            gHomeMenuRefreshFlag = 0xFF;
        }
    }
}
#else
void ReadKeyMenu(void)
{
	unsigned char mItem,i,n;
	unsigned char mToatol;
	unsigned char mCount;
	unsigned char mstatus;
	unsigned short mtep;
	char mBuf[32];
	mstatus = 0;
	if(g_OptionStatus)
	{

		mItem = g_Item >> 8;
		if(PRINTER_PDL.SYS._Get_Plus_POW())
		{
			g_PowerOffCount = 0;
			g_optionCount = 0;
			g_optionCount = 0;
			//主菜单切换
			mstatus = 1;
	
			mItem++;
			if(mItem > 8)
			{
				mItem = 0x01;
			}

			g_Item = mItem - 1;
			//增加读取flash中的数据
			//重新得到g_Item
			ReadPrinterConfig();//得到配置参数
		
			g_Item = s_PrintrConfig.m_SaveConPar[g_Item] & 0xFF;
			g_Item |= (mItem << 8);
		
		}

		if(PRINTER_PDL.SYS._Get_Plus_LF() == 1)
		{
#ifdef PWR_USER_BTN_KEY_AUDIO_EN			
			Audio_Beep_Custom(1, 30, 0);
#endif			
			g_optionCount = 0;
			g_optionCount = 0;
			
			
			mstatus = 1;
			//子菜单切换
			//得到子项个数
			g_PowerOffCount = 0;
			mToatol = MenuItemdeail[mItem - 1];
			mCount = g_Item & 0xFF;//得到子项
			if(mToatol == 1)
			{
				//直接执行
				//校准
				if(mItem == 5)
				{
					GetLabPaperSta();
					delaynms(500);
					g_OptionStatus = 0;
				}
				//打印自检
				if(mItem == 8)
				{
					SetSelftestData();
					g_OptionStatus = 0;
				}

			}
			else
			{
				mCount++;
				if(mCount > (mToatol - 1))
				{
					mCount = 0;
				}
				g_Item &= 0xFF00;
				g_Item |= mCount;
				//保存参数
				mtep = mItem - 1;

				s_PrintrConfig.m_SaveConPar[mtep] =g_Item;
			
				SavePrinterConfig();

			}
		}
		if((g_ItemSave != g_Item) || (mstatus == 1))
		{
					
			__SETPRIMASK();
			 g_PowerOffCount = 0;
			__RESETPRIMASK();
			g_ItemSave = g_Item;
			DisplayOption(g_Item);
			gHomeMenuRefreshFlag = 0xFF;
		}
	}
}
#endif

void DisSelftest(void)
{
	oled_ClearMap(0,2,128);
	oled_ClearMap(0,5,128);
	g_ChiEng = 0;
	if(g_ChiEng == 0)
	{
		DisplayStrchi("打印自检",16,3);//打印自檢
	}
	else if(g_ChiEng == 1)
	{
		DisplayStrchar("Selftest",16,3);
	}

}
void DisDefault(void)
{
	unsigned char n;
	g_ChiEng = 0;
	if(g_ChiEng == 0)
	{
		oled_ClearMap(0,2,128);
		oled_ClearMap(0,5,128);
		DisplayStrchi("恢复默认值",4,3);//恢復默認值
	}
	else if(g_ChiEng == 1)
	{
		oled_ClearMap(0,2,4);
		n = DisplayStrchar("Restore to",4,2);
		oled_ClearMap(n,2,128-n);

		oled_ClearMap(0,5,128);
		oled_PrintfString(0,6,"default settings");
	}


}

/*

no paper
Cover is opened

MAC:   Version:
Sound:
Shutdown:1Min 30Min 1Hour Never
Paper:Interval Continuity
calibration
Darkness:
Speed:





*/
extern unsigned char bt_addr[6];

void SetDisplay(unsigned  short mdis)
{
	if(mdis)
	{
		oled_PrintfMap(DIS_SET_X,DIS_SET_Y,(unsigned char*)SET_BMP);
	}
	else
	{
		oled_PrintfMap(DIS_SET_X,DIS_SET_Y,(unsigned char*)SET_BMPNULL);

	}	
	
}


//=====显示设置图标=================================
#ifdef NEW_UI_MENU_SETTINGS
/**
	* @FuncName  DisplayOption
	* @param     unsigned short Item - Menu item index selected by user
	* @retval    void
	* @LocalVar  m_buf[64], m_Item, i, n, m_w, current_pos, opt, shut_setting, audio_offset
	* @GlobalVar s_PrintrConfig, g_PowerOffTimeOut, g_PaperType, g_THDeep, g_Speed
	* 
	*	@Description Renders the interactive settings menu items on the OLED graphic display
	*              and updates system runtime variables with newly selected parameters. 
	*							-Updates OLED menu display strings and applies printer configuration settings.
	*
	*           
*/
void DisplayOption(unsigned short Item)
{
	char m_buf[64];
	unsigned short m_Item, i, m_w;
	unsigned char id, opt;
	m_Item = Item;
	id = (unsigned char)(m_Item >> 8);
	opt = (unsigned char)(m_Item & 0xFF);

	// Display settings icon
	if(g_OptionStatus)
	{
		oled_PrintfMap(DIS_SET_X, DIS_SET_Y, (unsigned char*)SET_BMP);
	}
	else
	{
		oled_PrintfMap(DIS_SET_X, DIS_SET_Y, (unsigned char*)SET_BMPNULL);
		g_ItemSave = 0xFF;
	}
	
	oled_ClearMap(0, 16, 128);
	oled_ClearMap(0, 24, 128);	

	if(g_OptionStatus)
	{
		unsigned char current_pos = 1;

		// 1. MAC Address / Version (ID: 1)
		if(id == current_pos)
		{
			memset(m_buf, 0, 64);
			sprintf(m_buf, "MAC:%X-%X-%X-%X-%X-%X", bt_addr[5], bt_addr[4], bt_addr[3], bt_addr[2], bt_addr[1], bt_addr[0]);
			DisplayStr6x8(m_buf, 0, 20);
		}
		
		// 2. Sound Master (ID: 2)
		else if(id == ++current_pos)
		{
			if(opt == 0) {
				m_w = GetStr8x16Wide((char*)"Sound: On", 1);
				DisplayStr8x16char("Sound: On", (128-m_w)/2, 16, 1);
			} else {
				m_w = GetStr8x16Wide((char*)"Sound: Off", 1);
				DisplayStr8x16char("Sound: Off", (128-m_w)/2, 16, 1);
			}
		}

#ifdef PWR_USER_BTN_KEY_AUDIO_EN
		// 3. Key Sound (Conditional ID)
		else if(id == ++current_pos)
		{
			if(opt == 0) {
				m_w = GetStr8x16Wide((char*)"KeySound: On", 1);
				DisplayStr8x16char("KeySound: On", (128-m_w)/2, 16, 1);
			} else {
				m_w = GetStr8x16Wide((char*)"KeySound: Off", 1);
				DisplayStr8x16char("KeySound: Off", (128-m_w)/2, 16, 1);
			}
		}
#endif

#ifdef AUDIO_BEEP_SEQUENCE
		// 4. Alerts (Conditional ID)
		else if(id == ++current_pos)
		{
			if (opt == 0) DisplayStr8x16char("Alert: Continual", (128-GetStr8x16Wide("Alert: Continual",1))/2, 16, 0);
			else if (opt == 1) DisplayStr8x16char("Alert: 3 Times", (128-GetStr8x16Wide("Alert: 3 Times",1))/2, 16, 1);
			else if (opt == 2) DisplayStr8x16char("Alert: 5 Times", (128-GetStr8x16Wide("Alert: 5 Times",1))/2, 16, 1);
			else if (opt == 3) DisplayStr8x16char("Alert: 9 Times", (128-GetStr8x16Wide("Alert: 9 Times",1))/2, 16, 1);
		}
#endif

		// 5. Shutdown (Conditional ID)
		else if(id == ++current_pos)
		{
			char* shut_strs[] = {"Shutdown: Never", "Shutdown: 10Min", "Shutdown: 20Min", "Shutdown: 30Min", "Shutdown: 1Hour", "Shutdown: 6Hour"};
			if(opt < 6) {
				m_w = GetStr8x16Wide(shut_strs[opt], 0);
				DisplayStr8x16char(shut_strs[opt], (128-m_w)/2, 16, 0);
			}
		}

		// 6. Paper (Conditional ID)
		else if(id == ++current_pos)
		{
#ifdef POWER_ON_CAL
#ifdef NEW_UI_MENU_SETTINGS
			if(g_boundary_detection_flag == 1)
			{
        memset(m_buf, 0, 64);
				sprintf(m_buf, "Media:%dmm Label",label_length_mm);
				DisplayStr8x16char(m_buf, 0, 16,0);
			}
			else
			{
#ifdef MEDIA_UNKNOWN_DETECTION_IN_SETTING_MENU				
				if(Power_ON_CAL_Status == 2 || User_calibration_discarded_flag == 1)    //Power on calibration & Lid open close calibration flag 
				{
					  m_w = GetStr8x16Wide((char*)"Media: Unknown", 1);
						DisplayStr8x16char("Media: Unknown", (128-m_w)/2, 16, 1);
				}
				else
				{
				    m_w = GetStr8x16Wide((char*)"Media: Paper", 1);
						DisplayStr8x16char("Media: Paper", (128-m_w)/2, 16, 1);
				}
#else				
				m_w = GetStr8x16Wide((char*)"Media: Paper", 1);
				DisplayStr8x16char("Media: Paper", (128-m_w)/2, 16, 1);
#endif			
		  }
#else			
			if(g_boundary_detection_flag == 1) {
				m_w = GetStr8x16Wide((char*)"Media: Label", 1);
				DisplayStr8x16char("Media: Label", (128-m_w)/2, 16, 1);
			}			
			else 
			{
				m_w = GetStr8x16Wide((char*)"Media: Paper", 1);
				DisplayStr8x16char("Media: Paper", (128-m_w)/2, 16, 1);
			}
#endif			
#else			
			if(opt == 0) {
				m_w = GetStr8x16Wide((char*)"Paper: Interval", 1);
				DisplayStr8x16char("Paper: Interval", (128-m_w)/2, 16, 1);
			} else {
				m_w = GetStr8x16Wide((char*)"Paper: Continuity", 1);
				DisplayStr8x16char("Paper: Continuity", (128-m_w)/2, 16, 1);
			}
#endif			
		}

		// 7. Calibration (Conditional ID)
		else if(id == ++current_pos)
		{
			m_w = GetStr8x16Wide((char*)"Calibration", 1);
			DisplayStr8x16char("Calibration", (128-m_w)/2, 16, 1);
		}

		// 8. Darkness (Conditional ID)
		else if(id == ++current_pos)
		{
#ifdef FIVE_DARKNESS_LEVEL_CONFIG
			sprintf(m_buf, "Darkness: %d", opt + 1);
#else
			sprintf(m_buf, "Darkness: %d", opt + 5);
#endif
			m_w = GetStr8x16Wide(m_buf, 1);
			DisplayStr8x16char(m_buf, (128-m_w)/2, 16, 1);
		}

		// 9. Speed (Conditional ID)
		else if(id == ++current_pos)
		{
			sprintf(m_buf, "Speed: %d", opt + 1);
			if(opt == 0) strcat(m_buf, "(slow)");
			if(opt == 4) strcat(m_buf, "(fast)");
			m_w = GetStr8x16Wide(m_buf, 1);
			DisplayStr8x16char(m_buf, (128-m_w)/2, 16, 1);
		}

		// 10. Self Test (Conditional ID)
		else if(id == ++current_pos)
		{
			m_w = GetStr8x16Wide((char*)"Selftest", 1);
			DisplayStr8x16char("Selftest", (128-m_w)/2, 16, 1);
		}
	}

	DisplayBufEn();

	   
	  // Calculate audio_offset to find parameters in m_SaveConPar
	   
	unsigned char audio_offset = 0;
#ifdef PWR_USER_BTN_KEY_AUDIO_EN
	audio_offset++;
#endif
#ifdef AUDIO_BEEP_SEQUENCE
	audio_offset++;
#endif

	// Apply Shutdown Timeout (Index: 2 + audio_offset)
	unsigned short shut_setting = s_PrintrConfig.m_SaveConPar[2 + audio_offset];
	opt = shut_setting & 0xFF;
	if(opt == 0)      g_PowerOffTimeOut = 0;
	else if(opt == 1) g_PowerOffTimeOut = 10*60*1000;
	else if(opt == 2) g_PowerOffTimeOut = 20*60*1000;
	else if(opt == 3) g_PowerOffTimeOut = 30*60*1000;
	else if(opt == 4) g_PowerOffTimeOut = 60*60*1000;
	else if(opt == 5) g_PowerOffTimeOut = 6*60*60*1000;
	else              g_PowerOffTimeOut = 10*60*1000;

	// Apply Paper Type (Index: 3 + audio_offset)
	//MyPrintf("CNf:%d\r\n",s_PrintrConfig.m_SaveConPar[5]);
	if((s_PrintrConfig.m_SaveConPar[3 + audio_offset] & 0xF) == 0) {
		g_PaperType = 0; s_PrintrConfig.m_InterFace = 0;
	} else {
		g_PaperType = 1; s_PrintrConfig.m_InterFace = 1;
	}	
	
	// Apply Darkness (Index: 5 + audio_offset)
	i = s_PrintrConfig.m_SaveConPar[5 + audio_offset] & 0x1F;
#ifdef TEST_SPEED_DARKNESS_ACK_COMMAND_ADDITION		
	if(id == 8)   //BT Nack
	{
	  //MyPrintf("D:%d\r\n",i);
		Get_Darkness_Speed_cmd(i,"Darkness");
	}
#endif	
	
	
#ifdef FIVE_DARKNESS_LEVEL_CONFIG
	i = (i * 2) + 1; // Simplified 1-5 mapping
#endif	
	if(i < 5) g_THDeep = 0x8000 | ((5 - i)*50);
	else      g_THDeep = (i - 5)*50;
	s_PrintrConfig.m_Density = i;	

	// Apply Speed (Index: 6 + audio_offset)
	i = s_PrintrConfig.m_SaveConPar[6 + audio_offset] & 0x7;
	if(i > 4) i = 4;
	g_Speed = (4 - i) * 120;
#ifdef TEST_SPEED_DARKNESS_ACK_COMMAND_ADDITION		
 if(id == 9)    //BT Nack
 {
  //MyPrintf("Spd:%d\r\n",i);	
	 Get_Darkness_Speed_cmd(i,"Speed");
 }
#endif

	
}

#else
void DisplayOption(unsigned short Item)
{
	char m_buf[64];
	unsigned short m_Item,i,n,m_w;
	m_Item = Item;

	//显示设置图标
	if(g_OptionStatus)
	{
		oled_PrintfMap(DIS_SET_X,DIS_SET_Y,(unsigned char*)SET_BMP);
	}
	else
	{
		oled_PrintfMap(DIS_SET_X,DIS_SET_Y,(unsigned char*)SET_BMPNULL);
		g_ItemSave = 0xFF;
	}
	oled_ClearMap(0,16,128);
	oled_ClearMap(0,24,128);	
	if(g_OptionStatus)
	{
		//--mac 地址  0
		if((m_Item &0xFF00) == 0x0100)
		{

			memset(m_buf,0,64);
			sprintf(m_buf,"MAC:%X-%X-%X-%X-%X-%X",bt_addr[5],bt_addr[4],bt_addr[3],bt_addr[2],bt_addr[1],bt_addr[0]);
			DisplayStr6x8(m_buf,0,20);
		}
		//--sound on/off 1
		else if(m_Item == 0x0200)
		{
			m_w = GetStr8x16Wide((char*)"Sound:on",1);
			DisplayStr8x16char("Sound:on",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0201)
		{
			m_w = GetStr8x16Wide((char*)"Sound:off",1);
			DisplayStr8x16char("Sound:off",(128-m_w)/2,16,1);
		}
		//Shutdown 2
		else if(m_Item == 0x0300)
		{
			m_w = GetStr8x16Wide((char*)"Shutdown:Never",1);
			DisplayStr8x16char("Shutdown:Never",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0301)
		{
			m_w = GetStr8x16Wide((char*)"Shutdown:10Min",1);
			DisplayStr8x16char("Shutdown:10Min",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0302)
		{
			m_w = GetStr8x16Wide((char*)"Shutdown:20Min",1);
			DisplayStr8x16char("Shutdown:20Min",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0303)
		{
			m_w = GetStr8x16Wide((char*)"Shutdown:30Min",1);
			DisplayStr8x16char("Shutdown:30Min",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0304)
		{
			m_w = GetStr8x16Wide((char*)"Shutdown:1Hour",1);
			DisplayStr8x16char("Shutdown:1Hour",(128-m_w)/2,16,1);
		}		
		else if(m_Item == 0x0305)
		{
			m_w = GetStr8x16Wide((char*)"Shutdown:6Hour",1);
			DisplayStr8x16char("Shutdown:6Hour",(128-m_w)/2,16,1);
		}				
		//Paper 3
		else if(m_Item == 0x0400)
		{
			m_w = GetStr8x16Wide((char*)"Paper:Interval",1);
			DisplayStr8x16char("Paper:Interval",(128-m_w)/2,16,1);
		}		
		else if(m_Item == 0x0401)
		{
			m_w = GetStr8x16Wide((char*)"Paper:Continuity",1);
			DisplayStr8x16char("Paper:Continuity",(128-m_w)/2,16,1);
		}				
		//Calibration 4
		if((m_Item &0xFF00) == 0x0500)
		{
			m_w = GetStr8x16Wide((char*)"Calibration",1);
			DisplayStr8x16char("Calibration",(128-m_w)/2,16,1);
		}
		//Darkness 5
		else if(m_Item == 0x0600)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:5(L)",1);
			DisplayStr8x16char("Darkness:1(L)",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0601)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:6",1);
			DisplayStr8x16char("Darkness:2",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0602)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:7",1);
			DisplayStr8x16char("Darkness:3",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0603)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:8",1);
			DisplayStr8x16char("Darkness:4",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0604)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:9",1);
			DisplayStr8x16char("Darkness:5",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0605)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:10",1);
			DisplayStr8x16char("Darkness:6",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0606)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:11",1);
			DisplayStr8x16char("Darkness:7",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0607)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:12",1);
			DisplayStr8x16char("Darkness:8",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0608)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:13",1);
			DisplayStr8x16char("Darkness:9",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0609)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:14",1);
			DisplayStr8x16char("Darkness:10",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x060A)
		{
			m_w = GetStr8x16Wide((char*)"Darkness:15",1);
			DisplayStr8x16char("Darkness:11(H)",(128-m_w)/2,16,1);
		}
		//===Speed 6
		else if(m_Item == 0x0700)
		{
			m_w = GetStr8x16Wide((char*)"Speed:1(slow)",1);
			DisplayStr8x16char("Speed:1(slow)",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0701)
		{
			m_w = GetStr8x16Wide((char*)"Speed:2",1);
			DisplayStr8x16char("Speed:2",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0702)
		{
			m_w = GetStr8x16Wide((char*)"Speed:3",1);
			DisplayStr8x16char("Speed:3",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0703)
		{
			m_w = GetStr8x16Wide((char*)"Speed:4",1);
			DisplayStr8x16char("Speed:4",(128-m_w)/2,16,1);
		}
		else if(m_Item == 0x0704)
		{
			m_w = GetStr8x16Wide((char*)"Speed:5(fast)",1);
			DisplayStr8x16char("Speed:5(fast)",(128-m_w)/2,16,1);
		}
		//Selftest 7
		if((m_Item &0xFF00) == 0x0800)
		{
			m_w = GetStr8x16Wide((char*)"Selftest",1);
			DisplayStr8x16char("Selftest",(128-m_w)/2,16,1);
		}	
	}


	//OLED_Refresh();
DisplayBufEn();
	//休眠时间
	if(s_PrintrConfig.m_SaveConPar[2]  == 0x0300)
	{
		g_PowerOffTimeOut = 0;
	}	
	else if(s_PrintrConfig.m_SaveConPar[2]  == 0x0301)
	{
		g_PowerOffTimeOut = 10*60*1000;
	}
	else if(s_PrintrConfig.m_SaveConPar[2]  == 0x0302)
	{
	g_PowerOffTimeOut = 20*60*1000;
	}
	else if(s_PrintrConfig.m_SaveConPar[2]  == 0x0303)
	{
		g_PowerOffTimeOut = 30*60*1000;
	}
	else if(s_PrintrConfig.m_SaveConPar[2]  == 0x0304)
	{
		g_PowerOffTimeOut = 60*60*1000;
	}
	else if(s_PrintrConfig.m_SaveConPar[2]  == 0x0305)
	{
		g_PowerOffTimeOut = 6*60*60*1000;
	}
	else
	{
		s_PrintrConfig.m_SaveConPar[2]  = 0x0301;
		g_PowerOffTimeOut = 10*60*1000;
	}

	if((s_PrintrConfig.m_SaveConPar[3] & 0xF) == 0)
	{
		g_PaperType = 0;
		s_PrintrConfig.m_InterFace = 0;
	}
	else
	{
		g_PaperType = 1;
		s_PrintrConfig.m_InterFace = 1;
	}	
	
	//浓度
	i = s_PrintrConfig.m_SaveConPar[5] & 0x1F;
	if(i < 5)
	{
		g_THDeep = 0x8000 | ((5 - i)*50);
	}
	else
	{
		g_THDeep = (i - 5)*50;
	}	
		
	
	
	
	s_PrintrConfig.m_Density = i;	
	//speed
	i = s_PrintrConfig.m_SaveConPar[6] & 0x7;
	if(i > 4)
	{
		i = 4;
		s_PrintrConfig.m_SaveConPar[6] &= 0xFF00;
		s_PrintrConfig.m_SaveConPar[6] |= i;
	}
	g_Speed = (4- i) * 120; 
	
	

}
#endif

extern unsigned short g_BTStatus ;
extern unsigned short g_StaBat;
/*
 * The home-menu module references DisplayTemp only in the protected build,
 * where PrintCore provides the thermistor-backed implementation.
 */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
extern void DisplayTemp(unsigned char x,unsigned char y);
#endif
void DisplayHomeMenu(unsigned short mRefreshFlag)
{
	char tempVUf[32];
		unsigned char i,n;

	//======================================
	if(mRefreshFlag == 0)
	{
		return ;
	}

	//======================================
	if(gHomeMenuRefreshFlag == mRefreshFlag)
	{
		return ;
	}
	
	

	g_BTStatus = 0xFFFF;
	g_Battystatus = 0xFF;
	g_StaBat = 0xA100;
ClearDisplayBuf();
	gHomeMenuRefreshFlag = mRefreshFlag;
	//home 界面
				DisplayBatty(96,0);
/*
 * Append the live print-head temperature to the home menu only when the
 * temperature-safe firmware image includes the DisplayTemp implementation.
 */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
				DisplayTemp(DIS_TEMP_X,0);
#endif
	
//	
//		memset(tempVUf,0,32);
//		sprintf(tempVUf,"欢迎使用");
//		OLED_ShowSting(tempVUf,16,20);
					DisplayBTPro();//显示蓝牙
	//	OLED_Refresh();			
	
	return;
	
	if(g_ChiEng == 0)
	{
		if(mRefreshFlag == 1)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaperNULL);
			DisplayStrchi("欢迎使用",16,2);//歡迎使用

			DisplayStrchar("RD-V300",22,5);

		}
		else if(mRefreshFlag == 2)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaperNULL);
			DisplayStrchi("正在打印",16,2);
			DisplayStrchar("RD-V300",22,5);

		}
		else if(mRefreshFlag == 3)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaper);
			DisplayStrchi("未检测到",16,2);
			DisplayStrchi("纸张",40,5);
		}
		else if(mRefreshFlag == 4)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaperNULL);
			DisplayStrchi("电池",40,2);
			DisplayStrchi("电量低",28,5);
		}
	}
	else if(g_ChiEng == 1)
	{
		if(mRefreshFlag == 1)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaperNULL);
			DisplayStrchar("Welcome",22,2);
			DisplayStrchar("RD-V300",22,5);

		}
		else if(mRefreshFlag == 2)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaperNULL);
			DisplayStrchar("Printing",16,2);
			DisplayStrchar("RD-V300",22,5);

		}
		else if(mRefreshFlag == 3)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaper);
			DisplayStrchar("Out of",28,2);
			DisplayStrchar("paper",34,5);
		}
		else if(mRefreshFlag == 4)
		{
			oled_PrintfMap(48,0,(unsigned char*)NoPaperNULL);
			DisplayStrchar("Battery",22,2);
			DisplayStrchar("  Low",22,5);
		}
	}
	

}


//printing
void displaypaper(unsigned short num)
{
	
}
void displaypaper_All(unsigned short num)
{
	
}

/*
void DisplayLogo(void)
{

}
*/

//处理光电值
extern void GetLabPaperSta(void);
void DisplayCalibrate(void)
{

	GetLabPaperSta();//处理光电




}

#ifdef TEST_SPEED_DARKNESS_ACK_COMMAND_ADDITION
void Get_Darkness_Speed_cmd(uint16_t Set_Value, char* string)//void Get_Label_Size_BT_Response(char *Media_Type,uint16_t Media_Height)
{
	char TxBuffer_NACK[64];
	
	sprintf(TxBuffer_NACK,"SRS20 %s:%d\r\n",string,Set_Value);
//	sprintf(TxBuffer_NACK,"SRS20_SPEED:%d\r\n",Set_Value);
#ifndef MY_DEBUG_ENABLE_O
	MyPrintf("%s\n", TxBuffer_NACK);
#endif
	PRINTER_PDL.BT._ReturnNBytes((unsigned char *)TxBuffer_NACK, strlen(TxBuffer_NACK));

}
#endif










































