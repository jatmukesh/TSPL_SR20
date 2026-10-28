#include "PDL.h"
#include "LabelPro.h"
#include "flash.h"
#include "ParseESC.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include "TsplFirmware.h"
#include "TsplNative.h"
extern void ReadKeyMenu(void);
#endif
#include <stdio.h>
#include <string.h>

#include "HP12832_01_SSD1306_I2C.h"
#include "Audio_Indication.h"

void Before_PrinterCOnfig(void);
void Set_default_printer_setting_at_power_on(void);

/*******************************New Firmware Modification**********************************/
#ifdef EMS_TEST_CODE
extern void Display_ALLSEG_ON(void);
#endif

#ifdef BT_NACK
char *mediaType;
void BT_NACK_Response_Media_And_Size(char *Media_Type,uint16_t Get_Media_Size);
#endif

#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
typedef enum {
    THERMAL_STATE_PRINT = 0,
    THERMAL_STATE_WARNING,
    THERMAL_STATE_STOP
} ThermalState_t;

#define THERMAL_WARNING_TEMP   550   // 43.0 deg C (in 0.1 deg C units)
#define THERMAL_STOP_TEMP      600  // 47.0 deg C (in 0.1 deg C units)	
#define THERMAL_RESUME_TEMP    400   // 38.0 deg C (in 0.1 deg C units)
#define THERMAL_DEBOUNCE_COUNT 2     // 2 consecutive 100ms periodic samples (~200ms stable debounce)

typedef struct {
    volatile unsigned short current_dot_line;      // Dot line completed
    volatile unsigned short next_dot_line;         // Dot line to print next
    volatile unsigned char  unit_in_transmission;  // 1 = active transmission/heating
    volatile unsigned char  thermal_pause_active;  // 1 = paused for cooling
} ThermalPrintTrack_t;

static volatile ThermalPrintTrack_t g_ThermalPrintTrack = {0, 0, 0, 0};
static volatile ThermalState_t g_ThermalState = THERMAL_STATE_PRINT;
static volatile unsigned short g_CurrentThermalTemp = 250; // default initial temperature 25.0 C

/* GS v 0 labels arrive as a continuous Bluetooth byte stream.  These flags
 * identify the last 24-line raster block of one label so a hot head can stop
 * after that block has physically completed, before the next label is parsed.
 * The buffer id is 1 for list A and 2 for list B. */
volatile unsigned char g_ThermalStopPending = 0;
volatile unsigned char g_ThermalRasterLabelActive = 0;
volatile unsigned char g_ThermalRasterFinalChunkQueued = 0;
volatile unsigned char g_ThermalRasterFinalBuffer = 0;
volatile unsigned char g_ThermalBoundaryPause = 0;

extern unsigned short Read_Thermistor_Temperature(unsigned short adc_value);
#endif

#ifdef BATTERY_INDICATION_SOLUTION
unsigned char g_BarLevelAtPrintStart;
unsigned char g_BatteryBarCandidateLevel;
unsigned char g_BatteryBarPrintState;
unsigned int g_BatteryBarRecoveryTime;
unsigned int g_BatteryBarSampleTime;
#define BATTERY_BAR_IDLE                 0
#define BATTERY_BAR_PRINTING             1
#define BATTERY_BAR_RECOVERING           2
#define BATTERY_BAR_CONFIRMING           3
#define BATTERY_BAR_POST_PRINT_DELAY_MS  3000

void BatteryBar_PrintStarted(void);

#endif

unsigned int LID_Detection_Flag;
extern unsigned short	g_Lab_head_Feed;

#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
extern void DisplayTemp(unsigned char x,unsigned char y);
#endif

#ifdef FIXED_LATE_BATTERY_DISPLAY
extern unsigned char g_DisplayHoldOff;
#endif

#ifdef MEDIA_UNKNOWN_DETECTION_IN_SETTING_MENU
unsigned int User_calibration_discarded_flag = 0;
#endif

#ifdef DISPLAY_FREEZE_ISSUE_SOLUTION	
unsigned int g_Display_freeze_issue_flag = 0;
#endif

#ifdef POWER_ON_CAL
//-----------------------Variables----------------------------
Cal_Mode_t Calibration_Mode;   //Enum Declaration

unsigned char Power_ON_CAL_Status; 
unsigned int g_LID_Status,g_Media_Status;

unsigned int g_CAL_Mode_statusFlag = 0;
unsigned int g_boundary_gap_counter = 0;
unsigned int g_label_size_counter = 0;

uint8_t g_label_size_final = 0;
uint8_t g_CAL_started_in_gap = 0;
uint16_t g_step_counter = 0;
unsigned char g_boundary_detection_flag = 0;

uint16_t g_step_counter_value = 0;
uint16_t g_boundary_gap_count_val = 0;

uint16_t label_length_mm;

extern unsigned short g_waitflag ;
extern unsigned short gHomeMenuRefreshFlag;
extern unsigned char g_DiscardPrinting;     //BT_NACK
//-----------------------Functions---------------------------
extern unsigned char PowerON_Calibration(void);
extern void OLED_RestoreHomeScreen(void);
extern void lid_mediacal(void);
void GetMediaCAl(void);

unsigned char g_SelfTestStartFlag = 0;

#endif

#ifdef PWR_USER_BTN_KEY_AUDIO_EN
static unsigned int POW_Key_Beep_EN_Time = 0;
unsigned char g_KeySoundEnable = 1 ;	
#endif

#ifdef AUDIO_BEEP_SEQUENCE
extern void Process_Buzzer_State_Machine(void);
extern void BuzzerManager_Init(uint8_t is_paper_out, uint8_t is_lid_open);
#endif

#ifdef SYSTEM_SOUND_SETTING
extern unsigned char g_System_Sound_Flag;
#endif

#ifdef CHECKSUM
extern uint32_t g_Flash_Calculated_Checksum;	
uint32_t g_Stored_Flash_Checksum;	
extern uint32_t g_Boot_Flash_Checksum;	
extern const uint32_t golden_checksum;
//uint8_t checksumPage[1] = {0xFF}; 
#endif
/*----------------------------------------------------*/
//Èí¼þÑÓÊ±³ÌÐò£¬²»ÊÇ¾«È·µÄÑÓÊ±
#define U8	unsigned  char
#define U16	unsigned  short
#define U32	unsigned  int
#define CR_OPTER_Set             ((unsigned int)0x00000020)
#define CR_STRT_Set              ((unsigned int)0x00000040)
#define CR_OPTER_Reset           ((unsigned int)0x00001FDF)
#define CR_OPTPG_Set             ((unsigned int)0x00000010)
#define CR_OPTPG_Reset           ((unsigned int)0x00001FEF)
#define RDP_Key                  ((unsigned short)0x00A5)
#define FLASH_KEY1               ((unsigned int)0x45670123)
#define FLASH_KEY2               ((unsigned int)0xCDEF89AB)

//*****************************************************************************
extern _PDL		PRINTER_PDL;
//extern void MyPrintf(char *format, ...);
//***************¼ÆËã²ÎÊý±í**************************************************************

const char SelfTestSerial[]={"SN:32F06430081605\r"};
#ifdef PRINTER_PROTOCOL
const char SelfTestD_1[]={"Interface:serial\rProtocol1\r"};
const char SelfTestD_P[]={"Interface:Parallel\rProtocol1\r"};
#endif
#ifndef PRINTER_PROTOCOL
const char SelfTestD_1[]={"Interface:serial333\r"};
const char SelfTestD_P[]={"Interface:Parallel\r"};
#endif


#ifdef PRINTER_MODEL
const char Model_Arry[]={"Model:serial\r"};
#endif


const char CompileDate[] = __DATE__;
const char CompileTime[] = __TIME__;


const char SelfTestD_150[]={"serial Baud:150"};
const char SelfTestD_300[]={"serial Baud:300"};
const char SelfTestD_600[]={"serial Baud:600"};
const char SelfTestD_1200[]={"serial Baud:1200"};
const char SelfTestD_2400[]={"serial Baud:2400"};
const char SelfTestD_4800[]={"serial Baud:4800"};
const char SelfTestD_9600[]={"serial Baud:9600"};
const char SelfTestD_19200[]={"serial Baud:19200"};
const char SelfTestD_38400[]={"serial Baud:38400"};
const char SelfTestD_57600[]={"serial Baud:57600"};
const char SelfTestD_115200[]={"serial Baud:115200"};
const char SelfTestD_230400[]={"serial Baud:230400"};
const char Setenergy[]={"energy consumption:"};
const char Setresort[]={"Restore default Settings\r"};

//8 ÎÞÐ£Ñé
const char SelfTestD_8N1[]={",N,8,1\r"};
//8 ÆæÐ£Ñé
const char SelfTestD_8O1[]={",O,8,1\r"};
//8 Å¼Ð£Ñé
const char SelfTestD_8E1[]={",E,8,1\r"};
//7 Å¼Ð£Ñé
const char SelfTestD_7E1[]={",E,7,1\r"};

const char Self_Reverse[]={"Reverse printing\r"};
const char Self_Forward[]={"Forward printing\r"};

const char SelfTestD_Ctrl[]={"flow ctrl:hardware\r"};
const char SelfTestD_XFCtrl[]={"flow ctrl:Xon-Xoff\r"};

const char SelfTestD_2[]={"Installed Fonts:\r\
0:ANK 6X8\r\
1:SYSTEM 9X17\r\
2:SYSTEM 12X24\r\
"};
const char SelfTestD_2_1[]={"Installed Fonts:\r\
0:ANK 6X8\r\
1:SYSTEM 8X16\r\
"};



const char GB2312_24x24[]={"GB2312_24x24"};
const char GB18030_24x24[]={"GB18030_24x24"};
const char GB2312_16x16[]={"GB2312_16x16"};



#ifdef INTELLIGENT_SELFTEST_RECEIPT
const U8 SelfTest_S[]={'F','u','l','l',' ','B','a','r','c','o','d','e',' ','S','u','p','p','o','r','t','.',0x0D,
0x1D,0x6B,0x49,14,0x7B,0x41,'A','B','0','1','2','3','4','5','6','7','8','9',0xff};
//0x43,0x68,0x61,0x72,0x61,0x63,0x74,0x65,0x72,0x20,0x53,0x65,0x74,0x3A,0x28,0x6D,0x6F,0x64,0x69,0x66,0x69,0x65,0x64,0x29,0x1C,0x38,0x0D,0xff};

const unsigned char SelfTest_COM[] = {                          
    "Posiflow Selftest Finished!"  
    "\r\n"                      
};
#else
const U8 SelfTest_S[]={'F','u','l','l',' ','B','a','r','c','o','d','e',' ','S','u','p','p','o','r','t','.',0x0D,
0x1D,0x6B,0x49,14,0x7B,0x41,'A','B','0','1','2','3','4','5','6','7','8','9',0x0D,
0x43,0x68,0x61,0x72,0x61,0x63,0x74,0x65,0x72,0x20,0x53,0x65,0x74,0x3A,0x28,0x6D,0x6F,0x64,0x69,0x66,0x69,0x65,0x64,0x29,0x1C,0x38,0x0D,0xff};


const U8 SelfTest_COM[]={"\
\r\
\r\
\r\
Selftest Finished!\r\r\r"};
#endif




const U32 CHAR_6X8_1[]={
		0x00000000,0x00000000,0x0000FA00,0x00000000,0x00E000E0,0x00000000,0x28FE28FE,0x28000000,0x2454FE54,0x48000000,0xC4C81026,0x46000000,0x6C926A04,0x0A000000,0x0020C080,0x00000000,
		0x38448200,0x00000000,0x00008244,0x38000000,0x4428FE28,0x44000000,0x1010FE10,0x10000000,0x00020C08,0x00000000,0x10101010,0x10000000,0x00000606,0x00000000,0x04081020,0x40000000,
		0x7C828282,0x7C000000,0x0042FE02,0x00000000,0x468A9292,0x62000000,0x848292B2,0xCC000000,0x182848FE,0x08000000,0xE4A2A2A2,0x9C000000,0x3C529292,0x8C000000,0x808E90A0,0xC0000000,
		0x6C929292,0x6C000000,0x62929294,0x78000000,0x00006C6C,0x00000000,0x00026C6C,0x00000000,0x10284482,0x00000000,0x28282828,0x28000000,0x00824428,0x10000000,0x40809AA0,0x40000000,
		0x7C82BAAA,0x7A000000,0x3E488848,0x3E000000,0xFE929292,0x6C000000,0x7C828282,0x44000000,0xFE828282,0x7C000000,0xFE929292,0x82000000,0xFE909090,0x80000000,0x7C828A8A,0x4E000000,
		0xFE101010,0xFE000000,0x0082FE82,0x00000000,0x040282FC,0x80000000,0xFE102844,0x82000000,0xFE020202,0x02000000,0xFE403040,0xFE000000,0xFE201008,0xFE000000,0x7C828282,0x7C000000,
		0xFE909090,0x60000000,0x7C828A84,0x7A000000,0xFE909894,0x62000000,0x64929292,0x4C000000,0x8080FE80,0x80000000,0xFC020202,0xFC000000,0xF8040204,0xF8000000,0xFE041804,0xFE000000,
		0xC6281028,0xC6000000,0xC0201E20,0xC0000000,0x868A92A2,0xC2000000,0xFEFE8282,0x00000000,0x40201008,0x04000000,0x008282FE,0xFE000000,0x2040FE40,0x20000000,0x10385410,0x10000000,
		0x00804020,0x00000000,0x242A2A1C,0x02000000,0xFE142222,0x1C000000,0x1C222222,0x10000000,0x1C222214,0xFE000000,0x1C2A2A2A,0x10000000,0x107E9090,0x40000000,0x19252525,0x1E000000,
		0xFE102020,0x1E000000,0x00009E00,0x00000000,0x01119E00,0x00000000,0xFE081422,0x02000000,0x0082FE02,0x00000000,0x1E201E20,0x1E000000,0x201E2020,0x1E000000,0x1C222222,0x1C000000,
		0x3F242424,0x18000000,0x18242424,0x3F000000,0x201E2020,0x10000000,0x122A2A2A,0x24000000,0x20FC2222,0x04000000,0x3C02023C,0x02000000,0x38040204,0x38000000,0x3C023C02,0x3C000000,
		0x22140814,0x22000000,0x39050509,0x3E000000,0x22262A32,0x22000000,0x106C8200,0x00000000,0x0000EE00,0x00000000,0x0000826C,0x10000000,0x10201008,0x10000000,0xAA54AA54,0xAA000000,
		0x7C828282,0x7C000000,0x10101010,0x10000000,0x04444444,0x04000000,0x82929292,0x82000000,0xFEFA82FA,0xFE000000,0xA2FEA2BE,0x82000000,0x222CE02C,0x22000000,0x20FE2222,0x24000000,
		0x02FC00FC,0x02000000,0x22FC203E,0x02000000,0x2020FE20,0x20000000,0x22BCA0BE,0x22000000,0x245CD47E,0x54000000,0x02FCA8AA,0xFE000000,0x007C5454,0x7C000000,0x94543E54,0x94000000,
		0x167E9292,0x44000000,0x0059A59A,0x00000000,0x0804FE04,0x08000000,0x10105438,0x10000000,0x20408040,0x20000000,0x2222FA22,0x22000000,0x10105410,0x10000000,0x30483048,0x30000000,
		0x24442414,0x24000000,0x08000800,0x08000000,0x0E11110E,0x00000000,0x70888870,0x00000000,0x004898A8,0x48000000,0x009088A8,0xD0000000,0x00091315,0x09000000,0x00121115,0x1A000000,
		0x0C120A04,0x3A000000,0x1F245252,0x2C000000,0x3040201E,0x60000000,0x4CB29292,0x4C000000,0x2C525242,0x24000000,0x008472CA,0x84000000,0x403C4040,0x3F000000,0x7C929292,0x7C000000,
		0x427C403C,0x02000000,0x7E100870,0x08000000,0x403E0408,0x70000000,0x3A464046,0x3A000000,0xA9555545,0x42000000,0x427C407C,0x82000000,0x023C4848,0x30000000,0x1C222222,0x5C000000,
		0x10203C22,0x24000000,0x30493E48,0x30000000,0x30087F08,0x30000000,0x3C421C42,0x3C000000,0xFE808080,0xC0000000,0x040C1424,0x7C000000,0x80FE80FE,0x80000000,0xC6AA9282,0xC6000000,
		0xF00AFE0A,0xF0000000,0x7A8E808E,0x7A000000,0xC6BA92BA,0xC6000000,0x7CBA92BA,0x7C000000,0x3E408040,0x3E000000,0x3844FE44,0x38000000,0x40827E82,0x40000000,0x040C1424,0x44000000,
		0xFF818181,0x81810000,0x81818181,0x81810000,0xFF818181,0x81FF0000,0x81818181,0x81FF0000,0x80808080,0x80800000,0x01010101,0x01010000,0xFF000000,0x00000000,0x00000000,0x00FF0000,
		0x01060810,0x60800000,0x80601008,0x06010000,0xFF808080,0x80800000,0xFF010101,0x01010000,0x01010101,0x01FF0000,0x80808080,0x80FF0000,0x81661818,0x66810000,0xC3E77E7E,0xE7C30000,
		0xFFFFC3C3,0xC3C30000,0xC3C3C3C3,0xC3C30000,0xFFFFC3C3,0xFFFF0000,0xC3C3C3C3,0xFFFF0000,0xC0C0C0C0,0xC0C00000,0x03030303,0x03030000,0xFFFF0000,0x00000000,0x00000000,0xFFFF0000,
		0x03071E78,0xE0C00000,0xC0E0781E,0x07030000,0xFFFFC0C0,0xC0C00000,0xFFFF0303,0x03030000,0x03030303,0xFFFF0000,0xC0C0C0C0,0xFFFF0000,0x18181818,0x18180000,0x0000FFFF,0x00000000,
		0x1818F8F8,0x00000000,0x18181F1F,0x00000000,0x00001F1F,0x18180000,0x0000F8F8,0x18180000,0x1818F8F8,0x18180000,0x18181F1F,0x18180000,0x0000FFFF,0x18180000,0x1818FFFF,0x00000000,
		0x1F1F1818,0xF8F80000,0xF8F81818,0x1F1F0000,0x18181C0E,0x07030000,0x03070E1C,0x18180000,0x18183870,0xE0C00000,0xC0E07038,0x18180000,0x18183C7E,0xE7C30000,0xC3E77E3C,0x18180000,
		0xF0F0F000,0x00000000,0x0F0F0F00,0x00000000,0x0000000F,0x0F0F0000,0x000000F0,0xF0F00000,0xF0F0F0F0,0xF0F00000,0x0F0F0F0F,0x0F0F0000,0xFFFFFF00,0x00000000,0x000000FF,0xFFFF0000,
		0x0F0F0FF0,0xF0F00000,0xF0F0F00F,0x0F0F0000,0xFFFFFFF0,0xF0F00000,0xFFFFFF0F,0x0F0F0000,0x0F0F0FFF,0xFFFF0000,0xF0F0F0FF,0xFFFF0000,0xFFFFFFFF,0xFFFF0000,0x1818FFFF,0x18180000,
		};
const U32 CHAR_6X8_2[]={
		0xBEAAEAAA,0xBE000000,0x50507E90,0x10000000,0x82FCA0A2,0xBE000000,0x82FE82FE,0x82000000,0xE0BC4242,0x24000000,0xE0A07E50,0x50000000,0x202000F8,0x00000000,0x3050F810,0x00000000,
		0x060A121F,0x02000000,0xE8103256,0x8A000000,0xE810226A,0xBE000000,0xE81C347E,0x84000000,0x80E08000,0x00000000,0x44281028,0x44000000,0x04080402,0xFC000000,0x0202FE02,0x02000000,
		0x0638C638,0xC0000000,0x00FE00FE,0x00000000,0xFC020202,0xFC000000,0x7E808080,0x7E000000,0x7C92FE92,0x7C000000,0x38444444,0x44000000,0x44444444,0x38000000,0x38549292,0x00000000,
		0x3854FE54,0x00000000,0xE0382438,0xE0000000,0x30282428,0x30000000,0x58545454,0x38000000,0x00027C80,0x00000000,0x003A6CB8,0x00000000,0x24247E24,0x18000000,0xC0000C00,0xC0000000,
		0x0C00C000,0x0C000000,0x54545454,0x54000000,0xD4345494,0xD4000000,0x30501014,0x18000000,0x282C3868,0x28000000,0x18241824,0x24000000,0x08142A44,0x82000000,0x82442A14,0x08000000,
		0x1028FE44,0x00000000,0x0044FE28,0x10000000,0x2046FA46,0x20000000,0x08C8BEC8,0x08000000,0x4444FE44,0x44000000,0x4040FE40,0x40000000,0xC2CC3646,0x86000000,0xC6000000,0xC6000000,
		0x54285428,0x54000000,0xB4484848,0xB4000000,0x7C820000,0x00000000,0x00000082,0x7C000000,0x102854AA,0x44000000,0x44AA5428,0x10000000,0xFE82BEA0,0xE0000000,0x0E0AFA82,0xFE000000,
		0xFE82BAC6,0x00000000,0x00C6BA82,0xFE000000,0x80402040,0x80000000,0x00400040,0x00000000,0x38448244,0x38000000,0x387C1E7C,0x38000000,0x387CFE7C,0x38000000,0x3852FE52,0x38000000,
		0x387AE67A,0x38000000,0x8082BC90,0xE0000000,0x08103E40,0x80000000,0x7040C244,0x78000000,0x42427C42,0x42000000,0x444850FE,0x40000000,0x42FC4042,0x7C000000,0x50F05854,0x52000000,
		0x30C0427C,0x00000000,0x30C0427C,0x40000000,0x42424242,0x7E000000,0x40F244F8,0x40000000,0xA2520408,0x10000000,0x42444854,0x62000000,0x40FC4252,0x62000000,0x60120204,0x78000000,
		0x10E2524C,0x78000000,0x50527C90,0x10000000,0x70007204,0x78000000,0x20A2BCA0,0x20000000,0x00FE1008,0x00000000,0x4244F840,0x40000000,0x02424242,0x02000000,0x42544854,0x60000000,
		0x4448DE68,0x44000000,0x00020CF0,0x00000000,0x0438007C,0x02000000,0x04FC1262,0x0E000000,0x40424244,0x78000000,0x20402010,0x08000000,0x4C40FE40,0x4C000000,0x4048444A,0x70000000,
		0xA8542A00,0x00000000,0x1C244404,0x0E000000,0x02140814,0x60000000,0x507C5252,0x52000000,0x20FE2028,0x30000000,0x0242427E,0x02000000,0x52525252,0x7E000000,0x20A0A2A4,0x38000000,
		0x00F00204,0xF8000000,0x3E007E02,0x1C000000,0x7E020408,0x10000000,0x7E424242,0x7E000000,0x70404244,0x78000000,0x087848FE,0x48000000,0x02425E52,0x62000000,0xA2A2A4A8,0xF0000000,
		0x42220408,0x10000000,0x20222C28,0x30000000,0x18103212,0x1C000000,0x12121E12,0x12000000,0x1214183E,0x10000000,0x103E1014,0x18000000,0x0212121E,0x02000000,0x2A2A2A3E,0x00000000,
		0x18021204,0x18000000,0x40209040,0x20000000,0x40A0A040,0x00000000,0xFE92928C,0xC0000000,0x8EF48484,0xFE000000,0x7ED252D2,0x44000000,0xC638FE38,0xC6000000,0xCC829292,0x6C000000,
		0x7E041820,0x7E000000,0x3E844890,0x3E000000,0x82FC80FE,0x82000000,0x84FC04FC,0x86000000,0xF0080810,0xFE000000,0xFC04FC04,0xFC000000,0xFC04FC04,0xFE000000,0x80FE1212,0x0C000000,
		0xFE12120C,0xFE000000,0xC48292D6,0x7C000000,0xFE107C82,0x7C000000,0x62949890,0xFE000000,0x3C525252,0x8C000000,0x50AAAAAA,0x14000000,0x1CAA6A2A,0x18000000,0x38467CC4,0x38000000,
		0x18263C64,0x18000000,0x708A8A8C,0x50000000,0xBC0202BC,0x02000000,0x1C2A6AAA,0x10000000,0x44AAAA9C,0x42000000,0x04AA2A9C,0x02000000,0x04AA6A1C,0x02000000,0x042AAA1C,0x02000000,
		0x304A4A4C,0x20000000,0x5CAAAAAA,0x58000000,0x1CAA2AAA,0x18000000,0x1C2AAA2A,0x18000000,0xA23E8200,0x00000000,0x40929E82,0x40000000,0x925E0200,0x00000000,0x0E942494,0x0E000000,
		0x0E54A454,0x0E000000,0x3E2A6AAA,0x22000000,0x2C2A3C2A,0x1A000000,0x3E50FE92,0x92000000,0x4C92924C,0x00000000,0x9C22229C,0x00000000,0x1CA2621C,0x00000000,0x5C82825C,0x02000000,
		0x3C82423C,0x02000000,0x20920C90,0x20000000,0x1CA222A2,0x1C000000,0x3C820282,0x3C000000,0x18247E24,0x24000000,0xFEA0E81E,0x0A000000,0x127E9080,0x40000000,0x042A6A9C,0x02000000,
		0x00125E82,0x00000000,0x1C22629C,0x00000000,0x3C42823C,0x02000000,0x409E905E,0x80000000,0x5E888442,0x9E000000,0x12AAAA72,0x0A000000,0x728A8A72,0x00000000,0x0C12A202,0x04000000,
		0x3A4A8C88,0x50000000,0x0C32820C,0xB2000000,0x0C1A6AAA,0x14000000,0x044AAAAC,0x52000000,0x040AAA2C,0x92000000,0x040AAA6C,0x12000000,0x040A2A2C,0x92000000,0x1A2A4C48,0x20000000,
		0x0C5AAAAA,0x50000000,0x0C1AAA2A,0x90000000,0x0C1AAA6A,0x10000000,0x0222AE32,0x80000000,0x02569A80,0x40000000,0x02965A00,0x00000000,0x06089828,0x9E000000,0x060858A8,0x5E000000,
		0x061A2A6A,0xA0000000,0x042A3E2A,0x10000000,0x0E305EF2,0x82000000,0x044A9294,0x48000000,0x0C12A224,0x98000000,0x0C12A264,0x18000000,0x0C52828E,0x52000000,0x0C32824E,0x32000000,
		0x328C0810,0xA0000000,0x0C12A224,0x98000000,0x0C32820C,0xB0000000,0x0C162C74,0x20000000,0x1E68AEBA,0xE8000000,0x121E7090,0xC0000000,0x042A6AAE,0x1A000000,0x02165A80,0x00000000,
		0x0C122264,0x98000000,0x0C32024E,0xB2000000,0x528C9052,0x8C000000,0x46988846,0x9C000000,0x122AAABA,0x6A000000,0x324A8A92,0x62000000,0x0C122282,0x04000000,0x1C202020,0x1C000000,
		};



const unsigned char chInvTable16[] = { 0x00, 0x08, 0x04, 0x0C,
    0x02, 0x0A, 0x06, 0x0E,
    0x01, 0x09, 0x05, 0x0D,
    0x03, 0x0B, 0x07, 0x0F };
const U32 DiV_config[9]={0,65536,32768,21846,16384,13108,10923,9363,8192};
const U32 Config_zoom[128]={
							0x00000000,0x00000001,0x00000002,0x00000003,0x00000004,0x00000005,0x00000006,0x00000007,0x00000008,0x00000009,0x0000000A,0x0000000B,0x0000000C,0x0000000D,0x0000000E,0x0000000F,
							0x00000000,0x00000003,0x0000000C,0x0000000F,0x00000030,0x00000033,0x0000003C,0x0000003F,0x000000C0,0x000000C3,0x000000CC,0x000000CF,0x000000F0,0x000000F3,0x000000FC,0x000000FF,
							0x00000000,0x00000007,0x00000038,0x0000003F,0x000001C0,0x000001C7,0x000001F8,0x000001FF,0x00000E00,0x00000E07,0x00000E38,0x00000E3F,0x00000FC0,0x00000FC7,0x00000FF8,0x00000FFF,
							0x00000000,0x0000000F,0x000000F0,0x000000FF,0x00000F00,0x00000F0F,0x00000FF0,0x00000FFF,0x0000F000,0x0000F00F,0x0000F0F0,0x0000F0FF,0x0000FF00,0x0000FF0F,0x0000FFF0,0x0000FFFF,
							0x00000000,0x0000001F,0x000003E0,0x000003FF,0x00007C00,0x00007C1F,0x00007FE0,0x00007FFF,0x000F8000,0x000F801F,0x000F83E0,0x000F83FF,0x000FFC00,0x000FFC1F,0x000FFFE0,0x000FFFFF,
							0x00000000,0x0000003F,0x00000FC0,0x00000FFF,0x0003F000,0x0003F03F,0x0003FFC0,0x0003FFFF,0x00FC0000,0x00FC003F,0x00FC0FC0,0x00FC0FFF,0x00FFF000,0x00FFF03F,0x00FFFFC0,0x00FFFFFF,
							0x00000000,0x0000007F,0x00003F80,0x00003FFF,0x001FC000,0x001FC07F,0x001FFF80,0x001FFFFF,0x0FE00000,0x0FE0007F,0x0FE03F80,0x0FE03FFF,0x0FFFC000,0x0FFFC07F,0x0FFFFF80,0x0FFFFFFF,
							0x00000000,0x000000FF,0x0000FF00,0x0000FFFF,0x00FF0000,0x00FF00FF,0x00FFFF00,0x00FFFFFF,0xFF000000,0xFF0000FF,0xFF00FF00,0xFF00FFFF,0xFFFF0000,0xFFFF00FF,0xFFFFFF00,0xFFFFFFFF,
							};

//***************µç»ú²ÎÊý¼ÓÈÈ²ÎÊý**************************************************************

extern const U32 PrintMotoHalfStep[8];

const unsigned short VolRUNTime[]=  {4500,4000,3500,3000,2500,2000,1500,1431,1362,1293,1224,1155,1086,917,848,779,710,641,572,572,572,572};

#ifdef NEW_JX
const unsigned short VolTHSTB[]=    {4500,4000,3500,3000,2500,2000,1400,1300,1200,1150,1090,1000, 832, 834,746,650,570,530,512,512,512,512};
#endif

#ifndef NEW_JX
const unsigned short VolTHSTB[]=    {4500,4000,3500,3000,2300,1900,1800,1700,1600,1450,1290,1000, 900, 834, 646, 550, 500, 430, 400, 400,400,400};
#endif
//     unsigned short VolDetStand[]= {3500,3750,4000,4250,4500,4750,5000,5250,5500,5750,6000,6250,6500,6750,7000,7250,7500,7750,8000,8250};



const unsigned short VolRUNPWM[]=   { 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304,304,304,304,304,304,304,304,304};
const unsigned short VolFreeTime[]= {1000,1000,1000,1000,1000,1000,1000, 950, 900, 850, 800, 750, 700, 650,600,550,500,450,400,400,400,400};

const unsigned short VolTemAdd[]= 	{ 300, 300, 300, 300, 300, 250, 200, 192, 184, 176, 166, 158, 150, 142,134,126,118,108,100,100,100,100};
const unsigned short VolTemSubb[]= 	{ 200, 200, 200, 200, 200, 150, 100,  96,  92,  88,  84,  80,  76,  72, 68, 64, 60, 56, 50, 50, 50, 50};

	
	
	//const unsigned short VolTHSTB_2[]=    {4500,4000,3500,3000,2500,2000,1500,1400,1300,1250,1190,1100, 932, 932,846, 800,750,700,650,650,650,650};

const unsigned short VolTHSTB_2[]=    {4500,4000,3500,3000,2500,2000,1500,1400,1300,1250,1190,1100, 550, 550,550, 550,550,550,550,550,550,550,550,550,550,400};

//************È«¾Ö±äÁ¿*****************************************************************
extern unsigned int   Getdata_ok;//ÅÐ¶ÏISPÊý¾ÝÊÇ·ñ½ÓÊÕÍê³É

extern	unsigned short g_printChar;

//------------Êý¾Ý»º³åÇø------------------------------------------------------------------------
unsigned char  receive_bufer[BUFFERSIZE]={0};

extern unsigned int Receive_count;
extern unsigned int	g_Receive_count;//Êý¾Ý½ÓÊÕ
extern unsigned int	g_Order_count;//È¡ÃüÁî¼ÆÊýÆ÷
unsigned short g_ProFlagVelue;
extern unsigned int *p_OcAndGDAdr;//µÃµ½Êý¾ÝµÄÖ¸Õë

extern  unsigned int *p_REAndSAcount;//±£´æÊý¾ÝµÄÖ¸Õë
extern unsigned int *p_SaveDrREcount;//´æ´¢Êý¾Ýµ½»º³åÇøµÄÆ«ÒÆÁ¿
//------------------------------------------------------------------------------------
unsigned short	g_CalcPrintPos;//´òÓ¡Êý¾Ýµ±Ç°µÄÎ»ÖÃ
unsigned short	g_PrintLineCount;//´òÓ¡ÐÐÊý¼ÆÊýÆ÷
unsigned short	g_MotoAll;//µç»ú×ß²½Ë÷Òý
//unsigned short	g_BufferLength;//½ÓÊÜÊý¾Ý³¤¶È
//unsigned short	g_MotoSTBStatus;//¼ÓÈÈÊ¹ÄÜ±êÖ¾
unsigned short	g_BufferSwitch;//Á´±í»º³åÇøÇÐ»»±êÖ¾

#define DATA_IN_SEND				0x5A67
#define DATA_SEND_OK				0x57A9
unsigned short	g_SendFlag;//Á´±í»º³åÇøÇÐ»»±êÖ¾

unsigned short	g_mapCount;//Í¼ÐÎÊý¾Ý¼ÆÊýÆ÷
unsigned char 	CodeBarStr[40];
unsigned char	HTValue[20];//±£´æË®Æ½Ôì±íÖµ
unsigned char	HTCount;//Ë®Æ½Ôì±í¼ÆÊýÆ÷
unsigned short	g_0D0AFlag;//0D0A´òÓ¡±êÖ¾
extern unsigned char	g_0D0AHigh;//»Ø³µ×ßÖ½¸ß¶È
//-----------------------------------------------------------------------------
unsigned short	g_PrintStatusReg;//ÃüÁîÁ´±í½âÎöÍê³É×´Ì¬
//****************************************************************************
GetOrderPoint	s_PrintOrderstr;//È¡ÃüÁî½á¹¹
PrinterStruct	s_PrintParameters;//´òÓ¡»ú²ÎÊý
PrintControl	s_PrinterMOTOPar;//´òÓ¡»ú²½½øµç»ú²ÎÊý
CodeBarCon		s_CodeBarCon;//ÌõÂë¿ØÖÆ²ÎÊý

PrintrConfig	s_PrintrConfig;//´òÓ¡»úÅäÖÃ²ÎÊý

unsigned short g_DetPaperGapSpace;
unsigned short g_DetPaperNoprint;

unsigned short g_NotStbFlag;

extern unsigned short g_PaperType;

unsigned int	m_OrderSave;//±£ÁôÈ¡ÊýÖ¸Õë
extern unsigned short g_Item,g_ItemSave;
//	unsigned short	m_flag;//
extern unsigned int   Getdata_ok;//ÅÐ¶ÏISPÊý¾ÝÊÇ·ñ½ÓÊÕÍê³É

PrintListPar		s_ListParmeter_A;//»º³åÇøA
PrintListPar		s_ListParmeter_B;//»º³åÇøB

unsigned short	Buffer_Dataflash[36];
unsigned short	Buffer_CodeBar[BUFFER_SHORT_LENGTH];//ÌõÂë»º³åÇø

unsigned short	g_alignType;//×óÓÒÖÐ ¶ÔÆë·½Ê½
extern unsigned short g_ReturnEp;
unsigned short	g_SpeedChoice;//ÉèÖÃ´òÓ¡»úÔËÐÐËÙ¶È
unsigned short	g_DriverAFlag;//Çý¶¯´òÓ¡±êÖ¾
//	unsigned short	g_CutterErr;//
//	unsigned int	g_BUSYFlag;//Busy ±êÖ¾
extern unsigned short*	Buffer_writer_I;//[10];//ÌõÂë»º³åÇø ¿ÉÒÔ¹«ÓÃ
p_PrintListPar			p_ListParmeters	;//Á´±í½á¹¹Ö¸Õë
//int dd;
unsigned short*	p_ListDataDot;//Á´±íÖÐ×Ö·ûÍ¼ÐÎµÄ´æ·ÅÎ»ÖÃ
unsigned int FlowFlag;//²¨ÌØÂÊ

unsigned short	g_dotSwitch;//Ô¤ÁôµãÊýÇÐ»»
unsigned int 	print_head_length;//			384

unsigned short g_StandbyPar;

unsigned short g_CutterErr;

unsigned short g_Char24_17;
unsigned short	m_charType;//×Ö·ûÀàÐÍ
unsigned short*	p_MapCurListDataDot;//Í¼ÐÎÇúÏßÖ¸ÕëÎ»ÖÃ
extern unsigned short g_CurveCountFlag;

extern	unsigned short g_pinrtStb;
unsigned short	m_high;//×Ö·û¸ß
unsigned short	m_Wide;//×Ö·û¿í

unsigned short	m_CharWide;//×Ö·û¿í
unsigned short	m_SaverightPos;//ÓÒ¿í¶È
unsigned short	m_UPDown;//ÉÏÏÂ¶ÔÆë

unsigned short	m_len;//µãÕóÊý¾Ý³¤¶È
unsigned short	g_SavePrintPos;//±£´æ×óÏÞ
unsigned short*	p_ListSave;//Á´±í±£´æÖ¸Õë
unsigned short	m_GetData;
p_PrintListPar	p_ListPrease;//Á´±í½á¹¹Ö¸Õë
unsigned short*	p_IndexListData;//Á´±íµ¥ÔªË÷ÒýµØÖ·
unsigned short	m_ProcessFlag;//ÃüÁî´¦Àí±êÖ¾
unsigned int	m_ProcessStatus;
unsigned short g_OptionStatus = 0;
unsigned int	g_THDeep;//Ôö¼Ó¼ÓÈÈÖµ
unsigned int	g_Speed;//ËÙ¶È
unsigned short	g_PaperEnd;//È±Ö½
unsigned short g_PaperOutAD,g_PaperGapAD;
//****************************************************************************
extern unsigned short AD_Value[3];
/****************************************************************************/
unsigned short g_TimeOutFlag;
unsigned int			g_lineBitCount,g_S_lineBitCount;//¼ÆËãµãÐÐÖÐ¼ÓÈÈµãÊý
unsigned short			g_RunFlag;//¼ÓÈÈÔËÐÐ±êÖ¾
unsigned short			SendBufferToSPI[BUFFER_SHORT_LENGTH];
unsigned short			SaveToBuffer[BUFFER_SHORT_LENGTH];
unsigned int			SendBuffer_counter;
unsigned int			SendToSPI_counter;
unsigned short			g_LPSStatus,g_PE_Standard;//È±Ö½¼ì²âÆ÷×´Ì¬

unsigned short			g_RunLTime;

extern  unsigned short g_CodePage;//¹ú¼Ê×Ö·û´úÂëÒ³
extern unsigned short g_InternationChar;//¹ú¼Ê×Ö·û¼¯

 //****************************************************************************
unsigned short g_ConfigRCC;
extern unsigned short	g_unicodeType;//unicode¼ÆÊýÆ÷
extern  unsigned int	Erase_Sector;
extern  unsigned int	Order_Erase_Sector;

unsigned short g_BarCharHigh;
extern unsigned int		g_SaveDataToFlashAddr;//´æ´¢Êý¾ÝµÄµ±Ç°µØÖ·
extern unsigned int		g_GetDataFromFlashAddr;//´ÓflashÖÐÈ¡Êý¾ÝµÄµØÖ·

extern unsigned int		g_SaveDataOffset;//´æ´¢Êý¾ÝµÄµ±Ç°Æ«ÒÆÁ¿
extern unsigned int		g_GetDataOffset;//µÃµ½´¢Êý¾ÝµÄµ±Ç°Æ«ÒÆÁ¿

extern unsigned short	g_InterFaceBufferCount;//¶Ë¿Ú½ÓÊÕÊý¾Ý»º³åÇø½ÓÊÕµ½µÄÊý¾Ý¸öÊý
extern unsigned short	g_ThType;//¼ÓÈÈÀàÐÍ
extern unsigned short	g_cruve_arry[20];
extern unsigned short	g_cruvefalg;
extern unsigned short	g_PrinterDip;//´òÓ¡DIP
extern unsigned char	g_HTCharSpace;//Ë®Æ½Ôì±í¼ÆËãÖµ

unsigned short			g_ParTypeValue;//·ÖÇøÀàÐÍÖµ
unsigned short* p_PrCon;//´òÓ¡»úÅäÖÃÖ¸Õë
unsigned int g_PowerOffTimeOut;
extern unsigned short g_ListControlFor;//×Ö·û¿ØÖÆ×Ö¸ñÊ½
extern unsigned short 	g_ReturnStatusFlag;
extern unsigned short g_ListPrintFinish;//ÊÇ·ñ´òÓ¡Íê³É
extern unsigned short g_PaperRunToOut;//³öÖ½²½Êý
extern unsigned char g_ListSaveToFlashCount;//Êý¾Ý´æ´¢µ½FLashÎ»ÖÃÆ«ÒÆÁ¿¼ÆÊý
extern unsigned short	g_PrintListOrNorm;//ÊÇ·ñÊÇ´òÓ¡±í¸ñ»òÕßÕý³£Êý¾Ý
void SendParData(unsigned short u16_anddata);
void SendDICLK(unsigned short u16_data);
void  PrinterParameterInt(void);
void SetSelftestData(void);
void PrinterConfigInt(void);
void SavePrinterConfig(void);
void ConfigToPrinter(void);//
void SetPrtintDataBaud(unsigned char baudvalue);
void SetPrtintDataParity(unsigned char baudvalue);
void SetPrtintDir(unsigned char baudvalue);
void ProKeySetModel(void);
void SetDataBaud(unsigned char baudvalue);
void SetSerialContrl(unsigned char baudvalue);
void DefaultSet(void);
void DisplayBatty(unsigned char x,unsigned char y);
void ProcessStbSingle(void);
void SetSoftwarePart(unsigned char baudvalue);
void SetPrintFont(unsigned char baudvalue);
void RunFeedLabPaper(void);
 void PrinterLPS(void);
extern void NpReturnStatus(unsigned char mdatOr);
void DisplayBTPro(void);
#ifdef PRINTER_AND_USB
extern void returnstatus(unsigned char flag);
#endif
extern unsigned short g_ListTable[BUFFER_SHORT_LENGTH];//
extern unsigned char(*PrinterCore)(void);//Íâ²¿¶¨ÒåµÄº¯ÊýÖ¸Õë
extern unsigned short	g_receiveFlage;
unsigned short	g_DumpReceiveFlage;
unsigned short g_ProFuncValue;

unsigned short g_PowerVol;
void  Delay1ms(unsigned short mdelay);

	unsigned short 	g_HotLineTOPD;//15 F ¼ÓÈÈÏßµ½È±Ö½¼ì²âµÄ¾àÀë ÒÔmmÎªµ¥Î» 8mm
	unsigned short 	g_HotLineTOOUT;//16 10 ¼ÓÈÈÏßµ½³öÖ½¿ÚµÄ¾àÀë ÒÔmmÎªµ¥Î» 5mm

unsigned char g_SelfRunFlag;//×Ô¼ìÔËÐÐ±êÖ¾
//unsigned char Trg,Cont;

unsigned short GetPaper(void);
unsigned short GetDETPaper(void);


unsigned short g_DetPaperValueFlag;

unsigned short g_DetPV_Count;

unsigned short  g_RunToGapFlag;

unsigned short g_CurrentVol,g_TMIndex;
extern unsigned short g_LabPrintEndF;

void RunFeedLabPaperLPS(void);
unsigned short g_HorShift;//Ë®Æ½Æ«ÒÆ
unsigned short g_VerShift;//ÉÏÏÂÆ«ÒÆ
unsigned short g_PaperLenCount;
unsigned short g_PaperLenStand;

extern unsigned short g_PaperWide;//Ö½ÕÅ¿í¶È
unsigned short g_detPaperEn;


void PrintLineTest(void)
{
		unsigned char m_bufer[48];
		memset(m_bufer,0x33,48);
	
		PRINTER_PDL.SDTH._DMA_SendToHead_u8(m_bufer,48);
		while(PRINTER_PDL.SDTH._IS_Send_Empty() == 0)
		{
			
		}
		PRINTER_PDL.SYS._LAT_H();
		//*********************************************
		PRINTER_PDL.SYS._LAT_L();
		PRINTER_PDL.SYS._LAT_H();				
		PRINTER_PDL.SYS._STB_A_EN();
		PRINTER_PDL.SYS._STB_B_EN();	
}






unsigned short g_ChineseEnco = 0;

/****************************************************************************
* º¯ÊýÃû£ºProcessCode128()
* ¹¦  ÄÜ£ºÖ÷³ÌÐò
*
* Ëµ  Ã÷£º

****************************************************************************/
#define BARBUF_MARK			0x7F//±íÊ¾»º³å²»³¬¹ý48 ²»³¬¹ýÈÈÃôÆ¬×î´ó¿í¶È
unsigned short g_DotProlen,g_DotBuf,g_Dotcount;
unsigned short g_QRCodeBuffer[128];

unsigned short g_QR_enable;
unsigned short g_QR_x,g_QR_y;
unsigned short g_QR_SpaceCount;




void QRCodeZoom(unsigned short m_InData,unsigned short m_len)
{
	unsigned short m_TemData;
	if((g_DotProlen + m_len) < 16)
	{
		g_DotBuf = g_DotBuf << m_len;
		g_DotBuf |= m_InData;
		g_DotProlen += m_len;
	}
	else
	{
		g_DotBuf = g_DotBuf << (16 - g_DotProlen);
		g_DotProlen = m_len + g_DotProlen - 16;
		m_TemData = m_InData >> g_DotProlen;
		g_DotBuf |= m_TemData;
		g_Dotcount &= BARBUF_MARK;
		g_QRCodeBuffer[g_Dotcount++]= g_DotBuf;
		g_DotBuf = m_InData;
	}
}

#ifdef PSF20_R002_ESC_POS_PROTOCOL
extern QRcode *qrcode;
void SetQRCodeBuffLine(unsigned short mLine)
{
	unsigned short k;
	unsigned short mDat;//,m_line;
	
	mDat = 0xFFFF;
	mDat = mDat>>(16 - s_PrintParameters.m_QRZoom);
	


	if(s_PrintParameters.m_PrintDir == PRINT_DIRECT_NOR)
	{
		//Ì¨Ðò
		
		g_Dotcount = g_QR_x/16;
		g_DotProlen =  g_QR_x%16;
		
		g_DotBuf = 0;		
		
		for (k = 0; k < qrcode->width; ++k)
		{

			if(qrcode->data[mLine*qrcode->width+k]&0x01)
			{
				QRCodeZoom(mDat,s_PrintParameters.m_QRZoom);
			}
			else
			{
				QRCodeZoom(0,s_PrintParameters.m_QRZoom);
			}
		}

		g_DotBuf = g_DotBuf << (16 - g_DotProlen);
		g_Dotcount &= BARBUF_MARK;
		g_QRCodeBuffer[g_Dotcount++]= g_DotBuf;	
	}
	else
	{
		g_Dotcount = (383 - g_QR_x - qrcode->width*s_PrintParameters.m_QRZoom)/16;
		g_DotProlen = (383 - g_QR_x - qrcode->width*s_PrintParameters.m_QRZoom)%16;
		
		g_DotBuf = 0;			
		
		//m_line = qrcode->width - 1 - mLine;
		for (k = qrcode->width; k > 0; k--)
		{
			
			if(qrcode->data[mLine*qrcode->width+(k-1)]&0x01)
			{
				QRCodeZoom(mDat,s_PrintParameters.m_QRZoom);
			}
			else
			{
				QRCodeZoom(0,s_PrintParameters.m_QRZoom);
			}
		}

		g_DotBuf = g_DotBuf << (16 - g_DotProlen);
		g_Dotcount &= BARBUF_MARK;
		g_QRCodeBuffer[g_Dotcount++]= g_DotBuf;			
		
		
		
	}
}


#endif
unsigned short g_BarProlen,g_BarBuf,g_Barcount;
unsigned short g_BarCodeBuffer[128];
unsigned short g_BarCode_enable;
unsigned short g_BarCode_x,g_BarCode_y;
unsigned short g_BarCode_SpaceCount;

unsigned short g_BarCodeDegree = 0;

void BarCodeZoom(unsigned short m_InData,unsigned short m_len)
{
	unsigned short m_TemData;
	if((g_BarProlen + m_len) < 16)
	{
		g_BarBuf = g_BarBuf << m_len;
		g_BarBuf |= m_InData;
		g_BarProlen += m_len;
	}
	else
	{
		g_BarBuf = g_BarBuf << (16 - g_BarProlen);
		g_BarProlen = m_len + g_BarProlen - 16;
		m_TemData = m_InData >> g_BarProlen;
		g_BarBuf |= m_TemData;
		g_Barcount &= BARBUF_MARK;
		g_BarCodeBuffer[g_Barcount++]= g_BarBuf;
		g_BarBuf = m_InData;
	}
}


extern unsigned short g_BarLen;
unsigned short g_BarCodeLen,g_BarCodeHigh,g_BarCodeHigh_Pro;
//Ò»Î¬ÂëÉèÖÃÊý¾Ý
#ifdef PSF20_R002_ESC_POS_PROTOCOL
void SetBarCodeBuffLine(unsigned short mLine)
{
	unsigned short i;
	unsigned short mDat,mlen;
	unsigned short m_barx;

	if(s_PrintParameters.m_PrintDir == PRINT_DIRECT_NOR)
	{
		//Ì¨Ðò
		if(g_BarCodeDegree == 0)
		{	
			g_Barcount = g_BarCode_x/16;
			g_BarProlen =  g_BarCode_x%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;
			for(i=0;i<g_BarCodeLen;i++)
			{
				mDat = mDat <<1;
				mlen++;
				if(GetBarcodedot(i,mLine,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
				{
					mDat |= 1;
				}
				if(mlen > 15)
				{
					BarCodeZoom(mDat,16);
					mDat = 0;
					mlen = 0;
				}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;	
		}
		else if(g_BarCodeDegree == 1)	
		{//90
			
			g_Barcount = g_BarCode_x/16;
			g_BarProlen =  g_BarCode_x%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;			
			for(i=0;i<g_BarCodeHigh;i++)
			{
					mDat = mDat <<1;
					mlen++;
					if(GetBarcodedot(mLine,g_BarCodeHigh - i - 1,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
					{
						mDat |= 1;
					}
					if(mlen > 15)
					{
						BarCodeZoom(mDat,16);
						mDat = 0;
						mlen = 0;
					}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}
		else if(g_BarCodeDegree == 2)	
		{//180
			g_Barcount = g_BarCode_x/16;
			g_BarProlen =  g_BarCode_x%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;
			for(i=0;i<g_BarCodeLen;i++)
			{
				mDat = mDat <<1;
				mlen++;
				if(GetBarcodedot(g_BarCodeLen - i - 1,g_BarCodeHigh - mLine - 1,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
				{
					mDat |= 1;
				}
				if(mlen > 15)
				{
					BarCodeZoom(mDat,16);
					mDat = 0;
					mlen = 0;
				}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}			
		else if(g_BarCodeDegree == 3)	
		{//270
			
			g_Barcount = g_BarCode_x/16;
			g_BarProlen =  g_BarCode_x%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;			
			for(i=0;i<g_BarCodeHigh;i++)
			{
					mDat = mDat <<1;
					mlen++;
					if(GetBarcodedot(g_BarCodeLen - mLine - 1, i,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
					{
						mDat |= 1;
					}
					if(mlen > 15)
					{
						BarCodeZoom(mDat,16);
						mDat = 0;
						mlen = 0;
					}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}			
						
			
			
	}
	else
	{
		//Ãæ°å
		if((g_BarCode_x + g_BarCodeLen) > 382)
		{
			m_barx = 0;
		}
		else
		{
				m_barx = 383 - (g_BarCode_x + g_BarCodeLen);
		}
		if(g_BarCodeDegree == 2)
		{	
			g_Barcount = m_barx/16;
			g_BarProlen =  m_barx%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;
			for(i=0;i<g_BarCodeLen;i++)
			{
				mDat = mDat <<1;
				mlen++;
				if(GetBarcodedot(i,mLine,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
				{
					mDat |= 1;
				}
				if(mlen > 15)
				{
					BarCodeZoom(mDat,16);
					mDat = 0;
					mlen = 0;
				}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;	
		}
		else if(g_BarCodeDegree == 3)	
		{//90
			
			g_Barcount = m_barx/16;
			g_BarProlen =  m_barx%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;			
			for(i=0;i<g_BarCodeHigh;i++)
			{
					mDat = mDat <<1;
					mlen++;
					if(GetBarcodedot(mLine,g_BarCodeHigh - i - 1,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
					{
						mDat |= 1;
					}
					if(mlen > 15)
					{
						BarCodeZoom(mDat,16);
						mDat = 0;
						mlen = 0;
					}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}
		else if(g_BarCodeDegree == 0)	
		{//180
			g_Barcount = m_barx/16;
			g_BarProlen =  m_barx%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;
			for(i=0;i<g_BarCodeLen;i++)
			{
				mDat = mDat <<1;
				mlen++;
				if(GetBarcodedot(g_BarCodeLen - i - 1,g_BarCodeHigh - mLine - 1,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
				{
					mDat |= 1;
				}
				if(mlen > 15)
				{
					BarCodeZoom(mDat,16);
					mDat = 0;
					mlen = 0;
				}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}			
		else if(g_BarCodeDegree == 1)	
		{//270
			
			g_Barcount = m_barx/16;
			g_BarProlen =  m_barx%16;		
			g_BarBuf = 0;
			mDat = 0;
			mlen = 0;			
			for(i=0;i<g_BarCodeHigh;i++)
			{
					mDat = mDat <<1;
					mlen++;
					if(GetBarcodedot(g_BarCodeLen - mLine - 1, i,s_CodeBarCon.m_BarHigh,g_BarCodeLen))
					{
						mDat |= 1;
					}
					if(mlen > 15)
					{
						BarCodeZoom(mDat,16);
						mDat = 0;
						mlen = 0;
					}
			}
			if(mlen)
			{
				BarCodeZoom(mDat,mlen);
			}	
			g_BarBuf = g_BarBuf << (16 - g_BarProlen);
			g_Barcount &= BARBUF_MARK;
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}			
						
			
			
	}
}

















void (*StandardSetPro[]) (unsigned char dat) =
{
    SetPrtintDataBaud,
    SetPrtintDataBaud,
    SetPrtintDataParity,
    SetSerialContrl,
    SetPrtintDir,
    SetSoftwarePart,
    SetPrintFont,
}
#endif
;

void ProTraehdat(unsigned short rule)
{
	if(rule == 0)
	{
		//ÅäÖÃÊý¾Ýµ½Êý×é
		//ÅäÖÃ²¨ÌØÂÊ
		g_cruve_arry[1] = s_PrintrConfig.m_BaudRate &  0x0F;

		if((s_PrintrConfig.m_BaudRate&0x30) == 0x00)
		{
			//ÎÞÐ£Ñé
			g_cruve_arry[2] = 0;
		}
		if((s_PrintrConfig.m_BaudRate&0x30) == 0x20)
		{	//Odd ÆæÐ§Ñé
			g_cruve_arry[2] = 1;
		}
		if((s_PrintrConfig.m_BaudRate&0x30) == 0x10)
		{	//Even Å¼Ð§Ñé
			g_cruve_arry[2] = 2;
		}
		if((s_PrintrConfig.m_BaudRate&0x30) == 0x30)
		{//7Î»Å¼Ð§Ñé
			g_cruve_arry[2] = 3;
		}
		if((s_PrintrConfig.m_BaudRate & 0x40) == 0)
		{
			g_cruve_arry[3] = 0;//Ó²¼þÁ÷¿Ø
		}
		else
		{
			g_cruve_arry[3] = 1;//Èí¼þÁ÷¿Ø
		}
		if(s_PrintrConfig.m_PrintDir == PRINT_DIRECT_NOR)
		{
			g_cruve_arry[4] = 0;//ÕýÏòÎª0

		}
		else
		{
			g_cruve_arry[4] = 1;//·´ÏòÎª1
		}
		if(s_PrintrConfig.m_SoftwarePart < 13)
		{
			s_PrintrConfig.m_SoftwarePart &= 0xFE;
			g_cruve_arry[5] = (s_PrintrConfig.m_SoftwarePart/2)-1;
		}
		else
		{
			g_cruve_arry[5] = 6;

		}
/*
GB2312_24x24 -- 0
GB18030_24x24 --1
GB2312_16x16 -- 2
GB18030_16x16 --3


*/
		if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_24)
		{

			if(s_PrintrConfig.m_ChineseLib == TYPE_GBK)
			{
				//
				g_cruve_arry[6] = 1;
			}
			else
			{
				g_cruve_arry[6] = 0;
			}
		}
		else
		{
			g_cruve_arry[6] = 2;
		}



	}
	else
	{
		//Êý×éµ½ÅäÖÃÊý¾Ý
		s_PrintrConfig.m_BaudRate = g_cruve_arry[1];

		if(g_cruve_arry[2] == 0x00)
		{
			//ÎÞÐ£Ñé
			s_PrintrConfig.m_BaudRate &= 0xCF;
		}
		if(g_cruve_arry[2] == 0x01)
		{	//Odd ÆæÐ§Ñé
			s_PrintrConfig.m_BaudRate &= 0xCF;
			s_PrintrConfig.m_BaudRate |= 0x20;
		}
		if(g_cruve_arry[2] == 0x2)
		{	//Even Å¼Ð§Ñé
			s_PrintrConfig.m_BaudRate &= 0xCF;
			s_PrintrConfig.m_BaudRate |= 0x10;
		}
		if(g_cruve_arry[2] == 3)
		{//7Î»Å¼Ð§Ñé
			s_PrintrConfig.m_BaudRate |= 0x30;
		}
		if(g_cruve_arry[3] == 0)
		{
			//Ó²¼þÁ÷¿Ø
			s_PrintrConfig.m_BaudRate &= 0x3F;
		}
		else
		{
			s_PrintrConfig.m_BaudRate |= 0x40;//Èí¼þÁ÷¿Ø
		}
		if(g_cruve_arry[4] == 0)
		{
			//ÕýÏòÎª0
			s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;

		}
		else
		{
			//·´ÏòÎª1
			s_PrintrConfig.m_PrintDir = PRINT_DIRECT_FX;
		}
		if(g_cruve_arry[5] < 6)
		{
			s_PrintrConfig.m_SoftwarePart = (g_cruve_arry[5]+ 1)*2;
		}
		else
		{
			s_PrintrConfig.m_SoftwarePart = 16;
		}
		if(g_cruve_arry[6] < 2)
		{
			if(g_cruve_arry[6] == 0)
			{
				s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//Ä¬ÈÏ¹ú±êÒ»¶þ¼¶
			}
			else
			{
				s_PrintrConfig.m_ChineseLib = TYPE_GBK;//Ä¬ÈÏ¹ú±êÒ»¶þ¼¶
			}
			s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//Ä¬ÈÏ24µãÕó»úÐÍ
			s_PrintrConfig.m_DefaultChar = TYPE_ASC12X24;//Ä¬ÈÏ´òÓ¡×Ö·û
			s_PrintrConfig.m_DefaultChinese = TYPE_CHIN24X24;//Ä¬ÈÏ´òÓ¡ºº×Ö
			s_PrintrConfig.m_HoriZoomOut = 1;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
			s_PrintrConfig.m_VertZoomOut = 1;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý
		}
		if(g_cruve_arry[6] == 2)
		{
			s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//Ä¬ÈÏ¹ú±êÒ»¶þ¼¶
			s_PrintrConfig.m_ChineseFont = PRINTTYPE_16;//Ä¬ÈÏ24µãÕó»úÐÍ
			s_PrintrConfig.m_DefaultChar = TYPE_ASC8X16;//Ä¬ÈÏ´òÓ¡×Ö·û
			s_PrintrConfig.m_DefaultChinese = TYPE_CHIN16X16;//Ä¬ÈÏ´òÓ¡ºº×Ö
			s_PrintrConfig.m_HoriZoomOut = 2;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
			s_PrintrConfig.m_VertZoomOut = 2;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý
		}

	}

}
unsigned int g_sysTime;
unsigned short gTimer_1ms,SysTickFlag,gTimer_cutter1ms;
unsigned short g_PowerFlag;
unsigned short gDelay1ms;
unsigned short g_PowerTimerCount;


unsigned short g_Tim3ProFlag;

void ServiceIRQ_A_Handler(void)
{
	g_Tim3ProFlag = 0xAA55;
	ProcessStbSingle();
}


 unsigned short  g_KeyTimeOutFlag;
extern unsigned int gTimer_Print;

unsigned short g_LedPFlag = 0;
unsigned short g_LedCount;
unsigned short g_PowerOnFlag;
unsigned short g_SetModelFlag;
unsigned int g_SetModel1ms;
unsigned short g_battL,g_CharCount;
unsigned int g_PowerOffCount;
unsigned int g_BeepCount;
unsigned int g_BatteryCount;
unsigned short g_BT1msCount;
unsigned short g_DlinkCount;



unsigned int g_BatDisCount;
unsigned int g_PowerOn1msCount;
unsigned int g_optionCount;

#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING
#define BATTERY_LOW_CHARGE_LATCH_MAGIC          0x4C43U
#define BATTERY_LOW_CHARGE_RESTART_MARGIN_MV    200U
#define BATTERY_LOW_CHARGE_RECOVERY_MV          7200U
#define BATTERY_LOW_CHARGE_CONFIRM_SAMPLES      3U
#define BATTERY_LOW_CHARGE_CONFIRM_INTERVAL_MS  3000U
#define BATTERY_LOW_CHARGE_POLL_MS              10U
#define BATTERY_LOW_CHARGE_CHECK_DISPLAY_MS     1000U
#define BATTERY_LOW_CHARGE_PROMPT_TIMEOUT_MS    10000U
#define BATTERY_LOW_CHARGE_POWER_HOLD_MS        400U
#define BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS      50U

static volatile unsigned char g_BatteryLowChargeShutdownRequest;
/* Set by the real 1 ms timer; ADC conversion remains foreground-only. */
static volatile unsigned char g_BatteryLowChargeVoltageSampleDue;
static unsigned short g_BatteryLowChargeVoltageSampleTicks;
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
static volatile unsigned char g_BatteryLabelActive;					// Indicates that a label is currently being printed.
static volatile unsigned char g_BatteryLabelFinalQueued;		// flag to Indicates that the final print buffer for the current label has been queued.
static volatile unsigned char g_BatteryLabelFinalBuffer;		// Stores which print buffer contains the final part of the label. 
static volatile unsigned char g_BatteryLabelComplete;				// Indicates that the final buffer has reached a completion state.(0/1/2)
static volatile unsigned char g_BatteryLabelChargerResetDue;// Indicates that a charger connection was detected and battery label state must be reset.
static unsigned short g_BatteryLabelLowCount;								// Counts how many completed labels were found to have low battery voltage after printing.
static unsigned char g_BatteryLabelWarning;									// Indicates that the next label should show the low-charge warning.
#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
static unsigned int g_BatteryDebugLabelId;
static unsigned int g_BatteryDebugCompletedLabels;
#endif

#if BATTERY_LOW_CHARGE_LABEL_LIMIT < 1 || BATTERY_LOW_CHARGE_LABEL_LIMIT > 65535U
#error BATTERY_LOW_CHARGE_LABEL_LIMIT must be between 1 and 65535
#endif
/* One accepted GS v 0 command is one application label. ISR calls only
 * record physical completion; ADC, OLED, flash and shutdown stay foreground. */
void BatteryLowCharge_LabelStarted(void);
void BatteryLowCharge_LabelFinalQueued(unsigned char buffer);
unsigned char BatteryLowCharge_LabelFinalBufferMatches(unsigned char buffer);
void BatteryLowCharge_LabelStopped(unsigned char buffer);
void BatteryLowCharge_LabelFinished(void);
void BatteryLowCharge_LabelAborted(void);
unsigned char BatteryLowCharge_PrintWarning(void);
void BatteryLowCharge_Service(void);

#endif


#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
/* Called only from foreground. Never take a debug-only voltage reading. */
static void BatteryLowCharge_DebugLog(char *prefix, char *event, unsigned char decision_sample)
{
    BatteryVoltageDebugSample sample;
    unsigned int label = 0, completed = 0, count = 0, limit = 0;
    unsigned int armed = 0, warning = 0, below = 0;
    char *policy = "IDLE_TIMER";
    BatteryVoltageDebug_GetSnapshot(&sample);
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    label = g_BatteryDebugLabelId;
    completed = g_BatteryDebugCompletedLabels;
    count = g_BatteryLabelLowCount;
    limit = BATTERY_LOW_CHARGE_LABEL_LIMIT;
    armed = (count >= limit);
    warning = g_BatteryLabelWarning;
    policy = "POST_LABEL";
#endif
    if (sample.valid)
        below = (prefix[4] == 'P') ? (sample.corrected_pack_mV < BATTERY_POWER_00) : (sample.corrected_pack_mV <= BATTERY_POWER_00);
//    MyPrintf("%s: event=%s time_ms=%u label=%u completed=%u sample_valid=%u sample_age_ms=%u decision_sample=%u adc_pin_mV=%u raw_pack_mV=%u corrected_pack_mV=%u threshold_mV=%u below=%u count=%u/%u armed=%u warning=%u shutdown_pending=%u usb=%u idle_low_ms=%u/10000 policy=%s\r\n",
//        prefix, event, g_sysTime, label, completed, (unsigned int)sample.valid,
//        sample.valid ? (g_sysTime - sample.sample_time_ms) : 0U,
//        (unsigned int)decision_sample, (unsigned int)sample.adc_pin_mV,
//        (unsigned int)sample.raw_pack_mV, (unsigned int)sample.corrected_pack_mV,
//        (unsigned int)BATTERY_POWER_00, below, count, limit, armed, warning,
//        (unsigned int)g_BatteryLowChargeShutdownRequest,
//        (unsigned int)PRINTER_PDL.USB._GetUSBDet(), g_BatteryCount, policy);
}
#endif
#ifdef mukesh_DEBUG_BATTERY_PRINT
#define BATTERY_PRINT_LOG(event, decision) BatteryLowCharge_DebugLog("BAT_PRINT", event, decision)
#else
#define BATTERY_PRINT_LOG(event, decision) ((void)0)
#endif
#ifdef mukesh_DEBUG_BATTERY_IDLE
#define BATTERY_IDLE_LOG(event) BatteryLowCharge_DebugLog("BAT_IDLE", event, 0)
static void BatteryLowCharge_DebugIdleSample(void)
{
    static unsigned int last_log_ms;
    static unsigned char first_log = 1;
    if (first_log || (g_sysTime - last_log_ms) >= 500U)
    {
        first_log = 0;
        last_log_ms = g_sysTime;
        BATTERY_IDLE_LOG("SAMPLE");
    }
}
#else
#define BATTERY_IDLE_LOG(event) ((void)0)
#define BatteryLowCharge_DebugIdleSample() ((void)0)
#endif

static void BatteryLowCharge_ShowText(char *line1, char *line2)
{
	unsigned short m_w;

	ClearDisplayBuf();
	m_w = GetStr8x16Wide(line1, 0);
	DisplayStr8x16char(line1, (128-m_w)/2, 0, 0);
	if (line2 != 0)
	{
		m_w = GetStr8x16Wide(line2, 0);
		DisplayStr8x16char(line2, (128-m_w)/2, 16, 0);
	}
	DisplayBufEn();
}

/* Single centered warning line. Callers clear the whole screen or just
 * the warning row as appropriate before drawing it. */
static void BatteryLowCharge_ShowLowCharge(void)
{
    unsigned short m_w;
    m_w = GetStr8x16Wide((char*)"Low charge", 1);
    DisplayStr8x16char("Low charge", (128-m_w)/2, 14, 1);
    DisplayBufEn();
}

static void BatteryLowCharge_DelayWithWatchdog(unsigned short delay_ms)
{
	while (delay_ms >= 100U)
	{
		Delay1ms(100);
		PRINTER_PDL.SYS._WDG_ReloadCounter();
		delay_ms -= 100U;
	}
	if (delay_ms != 0U)
	{
		Delay1ms(delay_ms);
		PRINTER_PDL.SYS._WDG_ReloadCounter();
	}
}

/* Called only by the real TIMER0 1 ms callback. No ADC, display, flash,
 * delay, beep, or power action is allowed here. */
void BatteryLowCharge_1msTick(void)
{
	g_BatteryLowChargeVoltageSampleTicks++;
	if (g_BatteryLowChargeVoltageSampleTicks >= 250U)
	{
		g_BatteryLowChargeVoltageSampleTicks = 0;
		g_BatteryLowChargeVoltageSampleDue = 1;
	}

	if (PRINTER_PDL.USB._GetUSBDet() != 0)
	{
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
        g_BatteryLabelChargerResetDue = 1;
#endif
		g_BatteryCount = 0;
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***1***\r\n");
#endif		
		g_BatteryLowChargeShutdownRequest = 0;
	}
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    /* Voltage under print load must never schedule a mid-label shutdown.
     * Reset elapsed idle confirmation whenever a label/buffer is active. */
    else if (g_BatteryLabelActive || s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)
    {
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***2***\r\n");
#endif			
        g_BatteryCount = 0;
        g_BatteryLowChargeShutdownRequest = 0;
    }
#endif
	else if (g_PowerVol <= BATTERY_POWER_00)
	{
		g_BatteryCount++;
		if (g_BatteryCount > 10000U)
		{
			g_BatteryLowChargeShutdownRequest = 1;
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***Set3***\r\n");
#endif	
		}			
	}
	else
	{
		g_BatteryCount = 0;
	}
}

/* This runs in normal foreground code, never in TIMER0_IRQHandler(). */
static void BatteryLowCharge_ServiceVoltageSample(void)
{
	if (g_BatteryLowChargeVoltageSampleDue == 0)
		return;

#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    if (g_BatteryLabelActive || s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)
        return;
#endif
	g_BatteryLowChargeVoltageSampleDue = 0;
	g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
    BatteryLowCharge_DebugIdleSample();
}

static unsigned char BatteryLowCharge_ConfirmBelow(unsigned short threshold_mV)
{
	unsigned char sample;
	for (sample = 0; sample < BATTERY_LOW_CHARGE_CONFIRM_SAMPLES; sample++)
	{
		g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
		if (g_PowerVol >= threshold_mV)
		{
   		return 0;
		}
		if ((sample + 1U) < BATTERY_LOW_CHARGE_CONFIRM_SAMPLES)
		{
			BatteryLowCharge_DelayWithWatchdog(BATTERY_LOW_CHARGE_CONFIRM_INTERVAL_MS);
		}
	}
	return 1;
}

static void BatteryLowCharge_ExecuteShutdown(void)
{
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    if (g_BatteryLabelWarning)
        BATTERY_PRINT_LOG("SHUTDOWN", 0);
    else
#endif
        BATTERY_IDLE_LOG("SHUTDOWN");
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***4***\r\n");
#endif		
	g_BatteryLowChargeShutdownRequest = 0;
	s_PrintrConfig.m_LowChargeShutdownLatch = BATTERY_LOW_CHARGE_LATCH_MAGIC;
	SavePrinterConfig();
	ClearDisplayBuf();
	BatteryLowCharge_ShowLowCharge();
	PRINTER_PDL.SYS._BEEP_EN();
	BatteryLowCharge_DelayWithWatchdog(1000);
	PRINTER_PDL.SYS._BEEP_DIS();
	PRINTER_PDL.SYS._POWER_DIS();
	soft_reset();
}

#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
static void BatteryLowCharge_LabelChargerReset(void)
{
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***5***\r\n");
#endif	
    unsigned char changed = (g_BatteryLabelLowCount || g_BatteryLabelWarning || g_BatteryLowChargeShutdownRequest);
    g_BatteryLabelChargerResetDue = 0;
    g_BatteryLabelLowCount = 0;
    g_BatteryLabelWarning = 0;
    g_BatteryCount = 0;
    g_BatteryLowChargeShutdownRequest = 0;
    if (changed)
        BATTERY_PRINT_LOG("CHARGER_RESET", 0);
}

unsigned char BatteryLowCharge_PrintWarning(void)
{
    if (g_BatteryLabelChargerResetDue || PRINTER_PDL.USB._GetUSBDet() != 0)
		{
        BatteryLowCharge_LabelChargerReset();			// reset flags 
		}
    if (!g_BatteryLabelWarning)
		{
        return 0;
		}
    DisplayStr8x16char("                ", 0, 14, 0);
    BatteryLowCharge_ShowLowCharge();
    return 1;
}

void BatteryLowCharge_LabelStarted(void)
{
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***6***\r\n");
#endif	
    if (g_BatteryLabelChargerResetDue || PRINTER_PDL.USB._GetUSBDet() != 0)
        BatteryLowCharge_LabelChargerReset();
#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
    g_BatteryDebugLabelId++;
#endif
    g_BatteryLabelActive = 1;
    g_BatteryLabelFinalQueued = 0;
    g_BatteryLabelFinalBuffer = 0;
    g_BatteryLabelComplete = 0;
    g_BatteryCount = 0;
    g_BatteryLowChargeShutdownRequest = 0;
    g_BatteryLabelWarning = (g_BatteryLabelLowCount >= BATTERY_LOW_CHARGE_LABEL_LIMIT);
    BATTERY_PRINT_LOG("LABEL_START", 0);
    BatteryLowCharge_PrintWarning();
}

void BatteryLowCharge_LabelFinalQueued(unsigned char buffer)
{
    if (g_BatteryLabelActive)
    {
        g_BatteryLabelFinalBuffer = buffer;
        g_BatteryLabelFinalQueued = 1;
    }
}

unsigned char BatteryLowCharge_LabelFinalBufferMatches(unsigned char buffer)
{
    return g_BatteryLabelActive && g_BatteryLabelFinalQueued &&
           g_BatteryLabelFinalBuffer == buffer;
}

/* ISR notification after the final strobe has completed. No hardware I/O. */
void BatteryLowCharge_LabelStopped(unsigned char buffer)
{
    if (BatteryLowCharge_LabelFinalBufferMatches(buffer))
        g_BatteryLabelComplete = 1;
}

void BatteryLowCharge_LabelAborted(void)
{
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***7***\r\n");
#endif	
    unsigned char was_active = g_BatteryLabelActive;
    g_BatteryLabelActive = 0;
    g_BatteryLabelFinalQueued = 0;
    g_BatteryLabelFinalBuffer = 0;
    g_BatteryLabelComplete = 0;
    g_BatteryLabelWarning = 0;
    g_BatteryCount = 0;
    g_BatteryLowChargeShutdownRequest = 0;
    if (was_active)
        BATTERY_PRINT_LOG("LABEL_ABORT", 0);
    /* An aborted label contributes no sample; the completed-label streak
     * is retained, so a previously armed warning applies to the next label. */
}

/* Foreground-only media validation also observes faults during final-buffer
 * waits, before a completed buffer can be mistaken for a successful label. */
static unsigned char BatteryLowCharge_LabelMediaPresent(void)
{
    g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
    if (PRINTER_PDL.SYS._GetHSStatus() || GetPaper() < g_PE_Standard)
    {
        BatteryLowCharge_LabelAborted();
        return 0;
    }
    return 1;
}

void BatteryLowCharge_LabelFinished(void)
{
    if (!g_BatteryLabelComplete)
        return;
    if (g_BatteryLabelComplete == 2)
    {
        BatteryLowCharge_LabelAborted();
        return;
    }
    if (!BatteryLowCharge_LabelMediaPresent())
        return;
#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
    g_BatteryDebugCompletedLabels++;
#endif
    g_BatteryLabelComplete = 0;
    g_BatteryLabelActive = 0;
    g_BatteryLabelFinalQueued = 0;
    g_BatteryLabelFinalBuffer = 0;
    g_BatteryCount = 0;
    if (g_BatteryLabelChargerResetDue || PRINTER_PDL.USB._GetUSBDet() != 0)
    {
        BatteryLowCharge_LabelChargerReset();
        BATTERY_PRINT_LOG("LABEL_DONE_CHARGING", 0);
        return;
    }
    g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
    if (g_BatteryLabelWarning)
    {
        /* The extra warning label is fully printed. Foreground service
         * now owns shutdown, before the next label can be parsed. */
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***Set8***\r\n");
#endif			
        g_BatteryLowChargeShutdownRequest = 1;
        BATTERY_PRINT_LOG("LABEL_DONE", 1);
        return;
    }
    if (g_PowerVol < BATTERY_POWER_00)
    {
        if (g_BatteryLabelLowCount < BATTERY_LOW_CHARGE_LABEL_LIMIT)
            g_BatteryLabelLowCount++;
    }
    else
        g_BatteryLabelLowCount = 0;
    BATTERY_PRINT_LOG("LABEL_DONE", 1);
}

void BatteryLowCharge_Service(void)
{
    if (g_BatteryLabelChargerResetDue || PRINTER_PDL.USB._GetUSBDet() != 0)
		{
        BatteryLowCharge_LabelChargerReset();
		}
    BatteryLowCharge_ServiceVoltageSample();
    if (g_BatteryLowChargeShutdownRequest && !g_BatteryLabelActive && s_PrinterMOTOPar.m_StopPrint == ENABLE_VAR)
		{
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("***9***\r\n");
#endif			
   		BatteryLowCharge_ExecuteShutdown();
		}
}
#endif

/* Preserve the low-charge latch until charging recovery completes.
 * Manual shutdown follows the normal power-key sound preference; the
 * unanswered Turn ON prompt always gives the requested shutdown beep. */
static void BatteryLowCharge_PowerOff(unsigned char timeout_shutdown)
{
    unsigned char sound_enabled = 1;
#ifdef SYSTEM_SOUND_SETTING
    sound_enabled = timeout_shutdown || g_System_Sound_Flag;
#endif
    if (sound_enabled)
    {
        PRINTER_PDL.SYS._BEEP_EN();
        BatteryLowCharge_DelayWithWatchdog(1000);
    }
    PRINTER_PDL.SYS._BEEP_DIS();
    ClearDisplayBuf();
    DisplayBufEn();
    /* As in normal manual shutdown, release the power key before cutting
     * power so a held key cannot immediately start the printer again. */
    while (PRINTER_PDL.SYS._GetKey_POW() == 0)
        BatteryLowCharge_DelayWithWatchdog(BATTERY_LOW_CHARGE_POLL_MS);
    PRINTER_PDL.SYS._POWER_DIS();
    soft_reset();
}

/* Disabled for now: retained Turn ON / User button confirmation flow.
 * This block is excluded from firmware; the active guard below auto-starts. */
#if 0
static void BatteryLowCharge_BootGuard_UserConfirmation(void)
{
    unsigned char stable_samples = 0;
    unsigned char ready = 0;
    unsigned char power_key_armed = 0;
    unsigned char user_key_armed = 0;
    unsigned short power_hold_ms = 0;
    unsigned short power_release_ms = 0;
    unsigned short user_hold_ms = 0;
    unsigned short user_release_ms = 0;
    unsigned int last_sample_ms;
    unsigned int prompt_start_ms = 0;

    if (s_PrintrConfig.m_LowChargeShutdownLatch != BATTERY_LOW_CHARGE_LATCH_MAGIC)
        return;

    if (PRINTER_PDL.USB._GetUSBDet() == 0)
    {
        BatteryLowCharge_ShowText("Checking", "Battery");
        if (BatteryLowCharge_ConfirmBelow(BATTERY_POWER_00 + BATTERY_LOW_CHARGE_RESTART_MARGIN_MV))
            BatteryLowCharge_ExecuteShutdown();
        return;
    }

    BatteryLowCharge_ShowText("Please wait", "Charging");
    /* Sample immediately, then keep the existing three-second spacing.
     * Only the short polling delay blocks: keys/USB stay responsive. */
    last_sample_ms = g_sysTime - BATTERY_LOW_CHARGE_CONFIRM_INTERVAL_MS;
    while (1)
    {
        if (PRINTER_PDL.USB._GetUSBDet() == 0)
        {
            BatteryLowCharge_ExecuteShutdown();
            return;
        }

        /* Ignore the original power-on hold until the button is released. */
        if (PRINTER_PDL.SYS._GetKey_POW() != 0)
        {
            if (power_release_ms < BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                power_release_ms += BATTERY_LOW_CHARGE_POLL_MS;
            if (power_release_ms >= BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                power_key_armed = 1;
            power_hold_ms = 0;
        }
        else
        {
            power_release_ms = 0;
            if (power_key_armed)
                power_hold_ms += BATTERY_LOW_CHARGE_POLL_MS;
            if (power_hold_ms >= BATTERY_LOW_CHARGE_POWER_HOLD_MS)
            {
                BatteryLowCharge_PowerOff(0);
                return;
            }
        }

        if ((g_sysTime - last_sample_ms) >=
            (ready ? 250U : BATTERY_LOW_CHARGE_CONFIRM_INTERVAL_MS))
        {
            last_sample_ms = g_sysTime;
            g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
            if (g_PowerVol >= BATTERY_LOW_CHARGE_RECOVERY_MV)
            {
                if (!ready && ++stable_samples >= BATTERY_LOW_CHARGE_CONFIRM_SAMPLES)
                {
                    ready = 1;
                    user_key_armed = 0;
                    user_hold_ms = 0;
                    user_release_ms = 0;
                    prompt_start_ms = g_sysTime;
                    BatteryLowCharge_ShowText("Turn ON", "User button");
                }
            }
            else
            {
                stable_samples = 0;
                if (ready)
                {
                    ready = 0;
                    BatteryLowCharge_ShowText("Please wait", "Charging");
                }
            }
        }

        if (ready)
        {
            if ((g_sysTime - prompt_start_ms) >= BATTERY_LOW_CHARGE_PROMPT_TIMEOUT_MS)
            {
                BatteryLowCharge_PowerOff(1);
                return;
            }
            if (PRINTER_PDL.SYS._GetKey_LF() != 0)
            {
                if (user_release_ms < BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                    user_release_ms += BATTERY_LOW_CHARGE_POLL_MS;
                if (user_release_ms >= BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                {
                    if (user_key_armed && user_hold_ms >= BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                    {
                        /* Consume a stable press/release so startup calibration
                         * cannot interpret confirmation as another key action. */
                        s_PrintrConfig.m_LowChargeShutdownLatch = 0;
                        SavePrinterConfig();
                        return;
                    }
                    user_key_armed = 1;
                }
                if (user_hold_ms < BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                    user_hold_ms = 0;
            }
            else
            {
                user_release_ms = 0;
                if (user_key_armed && user_hold_ms < BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                    user_hold_ms += BATTERY_LOW_CHARGE_POLL_MS;
            }
        }

        BatteryLowCharge_DelayWithWatchdog(BATTERY_LOW_CHARGE_POLL_MS);
    }
}
#endif

static void BatteryLowCharge_BootGuard(void)
{
    unsigned char stable_samples = 0;
    unsigned char power_key_armed = 0;
    unsigned char below;
    unsigned short power_hold_ms = 0;
    unsigned short power_release_ms = 0;
    unsigned int last_sample_ms;
    unsigned int check_start_ms;
    unsigned int check_elapsed_ms;

    if (s_PrintrConfig.m_LowChargeShutdownLatch != BATTERY_LOW_CHARGE_LATCH_MAGIC)
		{
        return;
		}
    if (PRINTER_PDL.USB._GetUSBDet() == 0)
    {
        BatteryLowCharge_ShowText("Checking", "Battery");
        check_start_ms = g_sysTime;
        below = BatteryLowCharge_ConfirmBelow(BATTERY_POWER_00 + BATTERY_LOW_CHARGE_RESTART_MARGIN_MV);
        /* Keep the status readable even when the first reading passes.
         * The existing spaced confirmation already exceeds this for low readings. */
        check_elapsed_ms = g_sysTime - check_start_ms;
        if (check_elapsed_ms < BATTERY_LOW_CHARGE_CHECK_DISPLAY_MS)
            BatteryLowCharge_DelayWithWatchdog((unsigned short)(BATTERY_LOW_CHARGE_CHECK_DISPLAY_MS - check_elapsed_ms));
        if (below)
        {
            BatteryLowCharge_ExecuteShutdown();
            return;
        }
        /* A successful battery check resolves the previous low-charge shutdown.
         * Persist this so the next normal power-on skips the recovery guard. */
        s_PrintrConfig.m_LowChargeShutdownLatch = 0;
        SavePrinterConfig();
        return;
    }

    BatteryLowCharge_ShowText("Please wait", "Charging");
    last_sample_ms = g_sysTime - BATTERY_LOW_CHARGE_CONFIRM_INTERVAL_MS;
    while (1)
    {
        if (PRINTER_PDL.USB._GetUSBDet() == 0)
        {
            BatteryLowCharge_ExecuteShutdown();
            return;
        }

        /* Ignore the original power-on hold until the button is released. */
        if (PRINTER_PDL.SYS._GetKey_POW() != 0)
        {
            if (power_release_ms < BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                power_release_ms += BATTERY_LOW_CHARGE_POLL_MS;
            if (power_release_ms >= BATTERY_LOW_CHARGE_KEY_DEBOUNCE_MS)
                power_key_armed = 1;
            power_hold_ms = 0;
        }
        else
        {
            power_release_ms = 0;
            if (power_key_armed)
                power_hold_ms += BATTERY_LOW_CHARGE_POLL_MS;
            if (power_hold_ms >= BATTERY_LOW_CHARGE_POWER_HOLD_MS)
            {
                BatteryLowCharge_PowerOff(0);
                return;
            }
        }

        if ((g_sysTime - last_sample_ms) >= BATTERY_LOW_CHARGE_CONFIRM_INTERVAL_MS)
        {
            last_sample_ms = g_sysTime;
            g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
            if (g_PowerVol >= BATTERY_LOW_CHARGE_RECOVERY_MV)
            {
                if (++stable_samples >= BATTERY_LOW_CHARGE_CONFIRM_SAMPLES)
                {
                    /* Recovery now continues directly to normal startup. */
                    s_PrintrConfig.m_LowChargeShutdownLatch = 0;
                    SavePrinterConfig();
                    return;
                }
            }
            else
                stable_samples = 0;
        }

        BatteryLowCharge_DelayWithWatchdog(BATTERY_LOW_CHARGE_POLL_MS);
    }
}
#endif



/**
* @FuncName  SysTickProFun
* @param     void
* @retval    void
* @LocalVar  None
* @GlobalVar gDelay1ms, g_sysTime, g_KeyTimeOutFlag, g_PowerTimerCount, g_LedCount,
*            g_SetModel1ms, gTimer_Print, g_PowerOffCount, g_CharCount, g_BeepCount,
*            g_BatDisCount, g_BT1msCount, g_DlinkCount, g_optionCount, POW_Key_Beep_EN_Time,
*            g_PowerOffTimeOut, g_PowerVol, BATTERY_POWER_00, g_BatteryCount, g_PowerFlag,
*            g_PowerOn1msCount, g_OptionStatus, g_KeySoundEnable, PWR_KEY_BEEP_EN_TIME,
*            g_System_Sound_Flag, gTimer_1ms, SysTickFlag, g_TimeOutFlag, gTimer_cutter1ms, PRINTER_PDL
* @Description System SysTick 1ms Hardware Interrupt Handler. Serves as the central timebase
*              heartbeat for the entire firmware architecture.
*
*              Functional Breakdown:
*              1. Heartbeat Counters: Increments 14 global 1ms counters for system uptime, serial timeouts,
*                 Bluetooth polling, battery refresh, and menu idle timing.
*              2. Non-Blocking Key Audio: Decrements `POW_Key_Beep_EN_Time` and turns off buzzer when zero.
*              3. Inactivity Auto Power-Off: Monitors `g_PowerOffCount` against `g_PowerOffTimeOut`. Plays a 1-second
*                 warning alert, clears OLED, cuts power rail (`_POWER_DIS()`), and performs `soft_reset()`.
*              4. Low-Battery Auto Shutdown: Monitors battery ADC voltage when USB is disconnected. Triggers power-down
*                 if voltage remains below cutoff (`BATTERY_POWER_00`) for 10 seconds.
*              5. Power Key State Machine:
*                 - Short Press (50ms–1000ms): Enters OLED Settings Menu (`g_OptionStatus = 0xA1`).
*                 - Long Press (>400ms): Plays power-off tone, waits for button release, feeds watchdog,
*                   disables system power, and soft-resets MCU.
*              6. Serial Timeout Flagging: Monitors 600ms communication timeout window during parameter config.
*/
void SysTickProFun(void)
{
#ifdef BATTERY_INDICATION_SOLUTION	
		unsigned short m_w;	
#endif
	
	gDelay1ms++;				// General 1ms blocking delay counter
	g_sysTime++;				// Master system uptime tick counter
#ifdef PSF20_R002_TSPL_PROTOCOL
    ++g_tsplMilliseconds;
#endif
	g_KeyTimeOutFlag++;	// Button press timeout counter
	g_PowerTimerCount++;	// Power button press duration timer
	g_LedCount++;
	g_SetModel1ms++;
	gTimer_Print++;
	g_PowerOffCount++;	// Inactivity power-off timeout accumulator
	g_CharCount++;
	g_BeepCount++;			// Power-off warning beep duration counter
	g_BatDisCount++;		// Battery display refresh interval counter
	g_BT1msCount++;
	g_DlinkCount++;
	g_optionCount++;		// OLED Menu screen idle timeout counter
	
#ifdef PWR_USER_BTN_KEY_AUDIO_EN		
	  if (POW_Key_Beep_EN_Time > 0)
    {
        POW_Key_Beep_EN_Time--;
        if (POW_Key_Beep_EN_Time == 0)
        {
            PRINTER_PDL.SYS._BEEP_DIS(); // Turn off when timer hits 0
        }
    }
#endif
	
	//Ê®·ÖÖÓ¹Ø»ú // 10-minute timer shutdown
#ifdef BT_NACK
	if((g_PowerOffCount > g_PowerOffTimeOut) && (g_PowerOffTimeOut != 0) && (PRINTER_PDL.USB._GetUSBDet() == 0))
#else
	if((g_PowerOffCount > g_PowerOffTimeOut) && (g_PowerOffTimeOut != 0))
#endif
	{
		if(g_PowerOffCount < (g_PowerOffTimeOut + 1000))
		{
				PRINTER_PDL.SYS._BEEP_EN();
				return;
		}
		else
		{
				ClearDisplayBuf();
				DisplayBufEn();
				PRINTER_PDL.SYS._BEEP_DIS();						
				PRINTER_PDL.SYS._POWER_DIS(); 
				soft_reset();
		}
		return;
	}


#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING	
#ifndef DISPLAY_FREEZE_ISSUE_SOLUTION	
	BatteryLowCharge_1msTick();
#endif	
#else	
	if(PRINTER_PDL.USB._GetUSBDet() != 0)
	{	
#ifdef BT_NACK	// Reset auto power-off while USB is plugged in
		//g_PowerOffCount = 0;  // Allow timer to count for OLED refresh
#else
		g_PowerOffCount = 0;
#endif
	}	
	else		
	{
#ifdef BATTERY_INDICATION_SOLUTION	
			if ( Receive_count == 0 && g_PowerVol <= BATTERY_POWER_00 ) //while printing do not shutdown based on the sudden voltage drop
			{
				//  Auto shutdown on low battery
				g_BatteryCount++;
				if (g_BatteryCount > 10000)			//if the battery level (g_PowerVol) stays low for more than 10 sec Turn OFF printer
				{
					if (g_BatteryCount < 11000) 	// mukesh - plays BEEP for 1 sec and then goes OFF
					{
						PRINTER_PDL.SYS._BEEP_EN();
						ClearDisplayBuf();
						m_w = GetStr8x16Wide((char*)"Low charge",1);
						DisplayStr8x16char("Low charge",(128-m_w)/2,14,1);
						DisplayBufEn();
						return;
					}
					else
					{
//						ClearDisplayBuf();
//						m_w = GetStr8x16Wide((char*)"Low charge",1);
//						DisplayStr8x16char("Low charge",(128-m_w)/2,14,1);
//						DisplayBufEn();
						ClearDisplayBuf();
						PRINTER_PDL.SYS._BEEP_DIS();
						PRINTER_PDL.SYS._POWER_DIS();	// mukesh - signal controller to Turn Off the Printer power for Low Charge < 6.5 V for 10secs
						soft_reset();
					}
					return;
				}
			}	// end of : (g_PowerVol <= BATTERY_POWER_00)
			else
			{
				g_BatteryCount = 0;		// mukesh - reset g_BatteryCount if the battery level is above BATTERY_POWER_00 
			}
#else		
		//Î´²åusb low charge
		// Low charge when USB not inserted
		if(g_PowerVol <= BATTERY_POWER_00)
		{
			//µçÁ¿µÍ×Ô¶¯¹Ø»ú
			// Auto shutdown on low battery
			g_BatteryCount++;
			if(g_BatteryCount > 10000)
			{
				if(g_BatteryCount < 11000)
				{
					PRINTER_PDL.SYS._BEEP_EN();
					return;
				}
				else
				{		
					ClearDisplayBuf();
					DisplayBufEn();
					PRINTER_PDL.SYS._BEEP_DIS();						
					PRINTER_PDL.SYS._POWER_DIS(); 
					soft_reset();
				}
				return;
			}
		}
		else
		{
			g_BatteryCount = 0;
		}
#endif		
		
	}
#endif	
	if(g_PowerFlag == 0xA5E3 )
	{
		if(PRINTER_PDL.SYS._GetKey_POW())
		{
		 if((g_PowerOn1msCount > 50) &&  (g_PowerOn1msCount < 1000))
		 {
			 g_OptionStatus = 0xA1;
			 g_optionCount = 0;
			 g_PowerOn1msCount = 0;
			 
#ifdef PWR_USER_BTN_KEY_AUDIO_EN
    if(g_KeySoundEnable) 
    {			 
			PRINTER_PDL.SYS._BEEP_EN();     // Turn buzzer ON immediately
			POW_Key_Beep_EN_Time = PWR_KEY_BEEP_EN_TIME;   	// Set timer for 30ms. SysTick handles the turn-off.
		}
#endif			 
		 }
		 
		 if(g_optionCount > 5000)
		 {
			 g_OptionStatus = 0;
		 }
		 
			if(g_PowerTimerCount > 400)
			{
				PRINTER_PDL.SYS._BEEP_DIS();	
			}
			g_PowerTimerCount = 0;
		}
		else
		{
			g_PowerOn1msCount++;
			PRINTER_PDL.SYS._LED_Stauts_OFF();
			g_BatDisCount= 0;
			
			if(g_PowerTimerCount > 400)
			{
				if(g_BeepCount < 1000)
				{
#ifdef SYSTEM_SOUND_SETTING					
				if(g_System_Sound_Flag) {PRINTER_PDL.SYS._BEEP_EN();}
				else {PRINTER_PDL.SYS._POWER_DIS();}	
#else					
					PRINTER_PDL.SYS._BEEP_EN(); //Manual power off buzzer beep
#endif
					return;
				}
				PRINTER_PDL.SYS._BEEP_DIS();	
				ClearDisplayBuf();
				DisplayBufEn();
				while( PRINTER_PDL.SYS._GetKey_POW() == 0)
				{
					PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog ÖØ×°ÔØ¿´ÃÅ¹·¶¨Ê±Æ÷	Reload the watchdog timer
				}
				PRINTER_PDL.SYS._POWER_DIS();
				soft_reset();
			}
			else
			{
				PRINTER_PDL.SYS._BEEP_DIS();	
				g_BeepCount = 0;
			}
			return;
		}
	}	
	gTimer_1ms++;
	if(SysTickFlag == 0xAAAA)
	{
   	if(gTimer_1ms > 600)
   	{
   		g_TimeOutFlag = 0xaa;
   	}
  }
	gTimer_cutter1ms++;
		
} // end SysTick_Handler

//µÚÒ»Ïî±íÊ¾×Ü¹²µÄÏîÄ¿Êý
//const unsigned char ParSetlist[]={2,4,6,8,10,12,16};
const unsigned char ItemIndexCount[]={6,11,4,2,2,7,3};
unsigned short m_ProKeyValue;
void ProKeySetModel(void)
{
	unsigned short m_itemMax,i,j;

	
	if(PRINTER_PDL.SYS._Get_Plus_LF())
	{
#ifdef PWR_USER_BTN_KEY_AUDIO_EN			
			if(g_KeySoundEnable)
			{
				Audio_Beep_Custom(1, 30, 0);
			}
#endif		
		m_ProKeyValue++;
		__SETPRIMASK();
		SysTickFlag = 0xAAAA;
		gTimer_1ms = 0;
		g_TimeOutFlag = 0;
		__RESETPRIMASK();
	//	TIM_DISABLE(TIM6);
	//	TIM_ENABLE(TIM6,6000);//ÑÝÊ¾400ms
	}
	if(g_TimeOutFlag != 0)
	{
		//SendChar(m_ProKeyValue);
		g_TimeOutFlag = 0;//Çå¶¨Ê±±êÖ¾
		SysTickFlag = 0;
		if(g_ProFuncValue == 0)
		{
			ProTraehdat(0);
		//µÚÒ»´Î½øÈëÅÐ¶Ï°´¼üÖµÊÇ·ñ´óÓÚµÈÓÚ9,ÊÇ±íÊ¾½øÈëÉèÖÃ×´Ì¬
			if(m_ProKeyValue > 9)
			{
				s_PrintParameters.m_HoriZoomOut = 1;//ºáÏò·Å´ó±¶ÊýÎª1
				s_PrintParameters.m_VertZoomOut = 1;//×ÝÏò´òÓ¡·Å´óÎª1
				s_PrintParameters.m_CharType = TYPE_ENGLISH;//TYPE_LITTLECHAR;//Ä¬ÈÏÎª0±íÊ¾´òÓ¡µÄÊÇºº×Ö,Ð¡ÓÚ80µÄ±íÊ¾´òÓ¡µÄÊÇ×Ö·û
				s_PrintParameters.m_Charlist = 1;//Ä¬ÈÏ´òÓ¡×Ö·û¼¯1
				g_ProFuncValue = 0x0100 ;//µÚÒ»ÏîÄ¬ÈÏÊÇ²¨ÌØÂÊ
				#ifdef RD_TYPE_PARALLEL
				g_ProFuncValue = 0x0400 ;//µÚÒ»ÏîÄ¬ÈÏÊÇ²¨ÌØÂÊ
				#endif

				i = (g_ProFuncValue >> 8) & 0xFF;//µÃµ½ÏîÄ¿Öµ

				g_ProFuncValue |= g_cruve_arry[i];

				m_ProKeyValue = 0;
			}
			else
			{
				m_ProKeyValue = 0;
				return ;
			}

		}
		if(m_ProKeyValue == 1)
		{
			g_ProFuncValue++;

			i = (g_ProFuncValue >> 8) & 0xFF;//µÃµ½ÏîÄ¿Öµ
			m_itemMax = ItemIndexCount[i];
			j =  g_ProFuncValue & 0xFF;
			if(j >= m_itemMax)
			{
				g_ProFuncValue &= 0xFF00;
			}
			g_cruve_arry[i] = g_ProFuncValue & 0xFF;
		}
#ifdef MULTIPLE_USER_BUTTON_PRESS
		if(m_ProKeyValue == 2)
		{
			g_ProFuncValue += 0x0100;//ÏîÄ¿ÀÛ¼Ó
			i = (g_ProFuncValue >> 8) & 0xFF;//µÃµ½ÏîÄ¿Öµ
			if(i> ItemIndexCount[0])
			{
				g_ProFuncValue = 0x0100;//´óÓÚ×î´óÏî»áµ½µÚÒ»Ïî
				#ifdef RD_TYPE_PARALLEL
				g_ProFuncValue = 0x0400 ;//µÚÒ»ÏîÄ¬ÈÏÊÇ²¨ÌØÂÊ
				#endif
			}
			g_ProFuncValue &= 0xFF00;
			i = (g_ProFuncValue >> 8) & 0xFF;//µÃµ½ÏîÄ¿Öµ
			g_ProFuncValue |= g_cruve_arry[i];

		}
			
		if(m_ProKeyValue > 5)
		{
			//»Ø¸´Ä¬ÈÏÖµ
			PrinterConfigInt();
#ifndef PAPER_FEED_SOLUTION			
			SavePrinterConfig();
#endif			
			DefaultSet();
			g_ProFuncValue = 0;
		}
		if(m_ProKeyValue < 3)
		{
			i = (g_ProFuncValue >> 8) & 0xFF;//µÃµ½ÏîÄ¿Öµ
			j = g_ProFuncValue & 0xFF;
			ProTraehdat(1);
#ifndef PAPER_FEED_SOLUTION				
			SavePrinterConfig();
#endif			
			(*StandardSetPro[i])(j);


		}
#endif		
		m_ProKeyValue = 0;
	}



}


void  Delay1ms(unsigned short mdelay)
{
#ifdef AUDIO_BEEP_SEQUENCE
	unsigned int start_time = g_sysTime;
	while((g_sysTime - start_time) < mdelay)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter();
		Process_Buzzer_State_Machine();
	}
	
#else	
	__SETPRIMASK();
	gDelay1ms = 0;
	__RESETPRIMASK();
	while(gDelay1ms <mdelay )
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog ÖØ×°ÔØ¿´ÃÅ¹·¶¨Ê±Æ÷
	}
#endif

}




/****************************************************************************
* º¯ÊýÃû£ºPrinterConfigInt()
* ¹¦  ÄÜ£º´òÓ¡»ú²ÎÊý³õÊ¼»¯
*
* Ëµ  Ã÷£º
****************************************************************************/
/**
* @FuncName  PrinterConfigInt
* @param     void
* @retval    void
* @LocalVar  unsigned char idx - Array index iterator for OLED menu parameter table setup
* @GlobalVar s_PrintrConfig, g_0D0AHigh, g_HTCharSpace
* @Description Initializes the global `s_PrintrConfig` structure with default factory configuration
*              parameters and sets up default non-volatile memory defaults.
*
*/
void PrinterConfigInt(void)
{
  unsigned char idx = 0;
	
		s_PrintrConfig.m_InterFace = 1;//Á¬ÐøÖ½	// Default to Continuous Paper (1)
#ifndef PRINTER_LABEL
		 s_PrintrConfig.m_InterFace = 1;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½; 
#endif
		#ifdef PRINTER_LABEL
			s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½;	// Set to Label Media (0)
#ifdef PAPER_LABEL
			 s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½; 
#endif
	#ifdef PAPER_CONTINUE
			 s_PrintrConfig.m_InterFace = 1;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½; 
	#endif	
#endif	
	
//	s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½
	 /* Character & Line Spacing Defaults */
	s_PrintrConfig.m_CharSpace = 0;//×Ö¼ä¾àÄ¬ÈÏÎª0	// Character spacing (0 dots)

	s_PrintrConfig.m_LineSpace = 3;//Ä¬ÈÏÐÐ¼ä¾à0	 // Line spacing (3 dots)

#ifdef PRINTER_GB2312
	s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//Ä¬ÈÏ¹ú±êÒ»¶þ¼¶
#endif
#ifdef PRINTER_GBK
	s_PrintrConfig.m_ChineseLib = TYPE_GBK;//Ä¬ÈÏGBK
#endif

#ifdef PRINTER_CHINESE_24
	s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//Ä¬ÈÏ24µãÕó»úÐÍ
	s_PrintrConfig.m_DefaultChar = TYPE_ASC12X24;//Ä¬ÈÏ´òÓ¡×Ö·û
	s_PrintrConfig.m_DefaultChinese = TYPE_CHIN24X24;//Ä¬ÈÏ´òÓ¡ºº×Ö
#endif
#ifdef PRINTER_CHINESE_16
	s_PrintrConfig.m_ChineseFont = PRINTTYPE_16;//Ä¬ÈÏ24µãÕó»úÐÍ
	s_PrintrConfig.m_DefaultChar = TYPE_ASC8X16;//Ä¬ÈÏ´òÓ¡×Ö·û
	s_PrintrConfig.m_DefaultChinese = TYPE_CHIN16X16;//Ä¬ÈÏ´òÓ¡ºº×Ö
#endif

#ifdef REVERSE_PRINT_SOLUTION
    s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;
#else
	s_PrintrConfig.m_PrintDir = PRINT_DIRECT_FX;//´òÓ¡»ú´òÓ¡·½Ïò Ì¨Ê½ Ãæ°å

#ifdef PRINTER_DIRECT_NOR

	s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;//´òÓ¡»ú´òÓ¡·½Ïò Ì¨Ê½ Ãæ°å

#endif
#endif

	s_PrintrConfig.m_HoriZoomOut = 1;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
	s_PrintrConfig.m_VertZoomOut = 1;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý

	g_0D0AHigh = 24;//
	g_HTCharSpace = 12;//¸ù¾Ý´òÓ¡µÄ×Ö·ûÈ·¶¨¿Õ¸ñ¿í¶È

	if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_16)
	{
		s_PrintrConfig.m_HoriZoomOut = 2;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
		s_PrintrConfig.m_VertZoomOut = 2;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý
		g_0D0AHigh = 16;//
 	}



	s_PrintrConfig.m_Liftpos = 0;//Ä¬ÈÏ´òÓ¡»ú×óÏÞ
	s_PrintrConfig.m_Righttpos = PRINT_HEAD_LENGTH;//Ä¬ÈÏ´òÓ¡»úÓÒÏÞ
	s_PrintrConfig.m_BaudRate = 0;//´®¿Ú»úÐÍ²¨ÌØÂÊ

	s_PrintrConfig.m_BaudRate = 6;//´®¿Ú»úÐÍ²¨ÌØÂÊ
	#ifdef IF_TYPE_BL
	s_PrintrConfig.m_BaudRate = 10;//´®¿Ú»úÐÍ²¨ÌØÂÊ115200
	#endif
		#ifdef PRINT_BPS_150
		s_PrintrConfig.m_BaudRate = 0x0;
		#endif
		#ifdef PRINT_BPS_300
		s_PrintrConfig.m_BaudRate = 0x1;
		#endif
		#ifdef PRINT_BPS_600
		s_PrintrConfig.m_BaudRate = 0x2;
		#endif
		#ifdef PRINT_BPS_1200
		s_PrintrConfig.m_BaudRate = 0x3;
		#endif
		#ifdef PRINT_BPS_2400
		s_PrintrConfig.m_BaudRate = 0x4;
		#endif
		#ifdef PRINT_BPS_4800
		s_PrintrConfig.m_BaudRate = 0x5;
		#endif
		#ifdef PRINT_BPS_9600
		s_PrintrConfig.m_BaudRate = 0x6;
		#endif
		#ifdef PRINT_BPS_19200
		s_PrintrConfig.m_BaudRate = 0x7;
		#endif
		#ifdef PRINT_BPS_38400
		s_PrintrConfig.m_BaudRate = 0x8;
		#endif
		#ifdef PRINT_BPS_57600
		s_PrintrConfig.m_BaudRate = 0x9;
		#endif
		#ifdef PRINT_BPS_115200
			s_PrintrConfig.m_BaudRate = 0x0A;
		#endif
		#ifdef PRINT_BPS_230400
			s_PrintrConfig.m_BaudRate = 0x0B;
		#endif
		
	
	#ifdef PRINT_BPS_XONXOFF
		s_PrintrConfig.m_BaudRate |= 0x40;
	#endif



	//Odd ÆæÐ§Ñé
	#ifdef PRINT_SERIAL_ODD
		s_PrintrConfig.m_BaudRate |= 0x20;
	#endif
	//Even Å¼Ð§Ñé
	#ifdef PRINT_SERIAL_EVEN
		s_PrintrConfig.m_BaudRate |= 0x10;
	#endif
//7Î»Å¼Ð§Ñé
	#ifdef PRINT_SERIAL_7_EVEN
		s_PrintrConfig.m_BaudRate |= 0x30;
	#endif




	s_PrintrConfig.m_SoftwarePart = 2;//Èí¼þ×î´ó·ÖÇø
	#ifdef PRINTER_LOW_16
	s_PrintrConfig.m_SoftwarePart = 16;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_12
	s_PrintrConfig.m_SoftwarePart = 12;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_10
	s_PrintrConfig.m_SoftwarePart = 10;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_8
	s_PrintrConfig.m_SoftwarePart = 8;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_6
	s_PrintrConfig.m_SoftwarePart = 6;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_4
	s_PrintrConfig.m_SoftwarePart = 4;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_2
	s_PrintrConfig.m_SoftwarePart = 2;//Èí¼þ×î´ó·ÖÇø
	#endif



	s_PrintrConfig.m_HotDeep = 0;//¶îÍâ¸½¼Ó¼ÓÈÈÉî¶È
	s_PrintrConfig.m_PrintSub = 3;//3;//´òÓ¡»úÏ¸·Ö¶È

	s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS;//ÉèÖÃÎª0±íÊ¾ÎÞ±êÇ©´òÓ¡
	s_PrintrConfig.m_LabelWide = 50;//±íÊ¾´òÓ¡µÄÖ½¿í,Òª¸ù¾ÝÆÕÍ¨Ö½58mmÖ½¿í´òÓ¡¿í¶È48mm 58-50 = 8mm * 8 ¿Õ64µã
	s_PrintrConfig.m_LabelLength = 200;//30mm * 32 1mm=32²½
	s_PrintrConfig.m_BarPaGap = 2;////3mm * 32 1mm=32²½
	//s_PrintrConfig.m_PE_LabValue_L Ö½·ìÅÐ¶ÏÖµ ¸ù¾Ý×Ô¼ì²âÊÔ×Ô¶¯¼ÆËã
	//s_PrintrConfig.m_PE_LabValue_H
	#ifdef PRINTER_LABEL
	s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN;//ÉèÖÃÎª±êÇ©´òÓ¡
	#endif
	


	
	s_PrintrConfig.m_HotLineTOPD = 9;//8 ; //¼ÓÈÈÏßµ½´«¸ÐÆ÷µÄÎ»ÖÃ
	s_PrintrConfig.m_HotLineTOOUT = 8;//6;//7 ;//¼ÓÈÈÏßµ½³öÖ½¿ÚµÄÎ»ÖÃ	


	#ifdef PRINTER_BUFF_RAM
	s_PrintrConfig.m_ReceiveBuffrWor = RECEIVE_RAM;//RECEIVE_FLASH;
	#endif

	#ifdef PRINTER_BUFF_FLASH
	s_PrintrConfig.m_ReceiveBuffrWor = RECEIVE_FLASH;//RECEIVE_FLASH;
	#endif
	s_PrintrConfig.m_CharType = TYPE_CHINESE;//Ä¬ÈÏÖÐÎÄ»úÐÍ
	#ifdef PRINTER_ENGLISH
	s_PrintrConfig.m_CharType = TYPE_ENGLISH;//Ä¬ÈÏÓ¢ÎÄ»úÐÍ
	#endif
	s_PrintrConfig.m_PrintEndFeed = 0;//Ä¬ÈÏ²»×ßÖ½
	s_PrintrConfig.m_PrintEnCode = 0;//Ä¬ÈÏ²»¼ÓÃÜ
	
	
	 s_PrintrConfig.m_PE_LabValue_H = 5;
	
	s_PrintrConfig.m_Delayms_End = 2;
	s_PrintrConfig.m_Delayms_Head = 10;	
	
	
#ifdef NEW_UI_MENU_SETTINGS
       //1. MAC Address
		s_PrintrConfig.m_SaveConPar[idx++] = 0x0100;
			// 2. Sound (ID 2)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0200;
			// 3. Key Sound (ID 3)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0300;
			// 4. Alerts (ID 4)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0400;
			// 5. Shutdown (ID 5, Default 10Min)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0501;
			// 6. Paper (ID 6)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0600;
			// 7. Calibration (ID 7)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0700;
			// 8. Darkness (ID 8)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0804;
			// 9. Speed (ID 9)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0904;
			// 10. Self Test (ID 10)
			s_PrintrConfig.m_SaveConPar[idx++] = 0x0A00;

#else
	s_PrintrConfig.m_SaveConPar[0] = 0x0100;//ÏÔÊ¾macµØÖ·
	s_PrintrConfig.m_SaveConPar[1] = 0x0200;//ÉùÒô¿ª
	s_PrintrConfig.m_SaveConPar[2] = 0x0301;//10·ÖÖÓ¹Ø»ú
	s_PrintrConfig.m_SaveConPar[3] = 0x0400;//±êÇ©Ö½
	s_PrintrConfig.m_SaveConPar[4] = 0x0500;//Calibration	
	s_PrintrConfig.m_SaveConPar[5] = 0x0600;//Å¨¶È 5
	s_PrintrConfig.m_SaveConPar[6] = 0x0704;//ËÙ¶È
	s_PrintrConfig.m_SaveConPar[7] = 0x0800;//×Ô¼ì
#endif	

}

void Set_default_printer_setting_at_power_on(void)
{
  unsigned char idx = 0;
	
		s_PrintrConfig.m_InterFace = 1;//Á¬ÐøÖ½	// Default to Continuous Paper (1)
#ifndef PRINTER_LABEL
		 s_PrintrConfig.m_InterFace = 1;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½; 
#endif
		#ifdef PRINTER_LABEL
			s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½;	// Set to Label Media (0)
#ifdef PAPER_LABEL
			 s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½; 
#endif
	#ifdef PAPER_CONTINUE
			 s_PrintrConfig.m_InterFace = 1;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½; 
	#endif	
#endif	
	
//	s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½
	 /* Character & Line Spacing Defaults */
	s_PrintrConfig.m_CharSpace = 0;//×Ö¼ä¾àÄ¬ÈÏÎª0	// Character spacing (0 dots)

	s_PrintrConfig.m_LineSpace = 3;//Ä¬ÈÏÐÐ¼ä¾à0	 // Line spacing (3 dots)

#ifdef PRINTER_GB2312
	s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//Ä¬ÈÏ¹ú±êÒ»¶þ¼¶
#endif
#ifdef PRINTER_GBK
	s_PrintrConfig.m_ChineseLib = TYPE_GBK;//Ä¬ÈÏGBK
#endif

#ifdef PRINTER_CHINESE_24
	s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//Ä¬ÈÏ24µãÕó»úÐÍ
	s_PrintrConfig.m_DefaultChar = TYPE_ASC12X24;//Ä¬ÈÏ´òÓ¡×Ö·û
	s_PrintrConfig.m_DefaultChinese = TYPE_CHIN24X24;//Ä¬ÈÏ´òÓ¡ºº×Ö
#endif
#ifdef PRINTER_CHINESE_16
	s_PrintrConfig.m_ChineseFont = PRINTTYPE_16;//Ä¬ÈÏ24µãÕó»úÐÍ
	s_PrintrConfig.m_DefaultChar = TYPE_ASC8X16;//Ä¬ÈÏ´òÓ¡×Ö·û
	s_PrintrConfig.m_DefaultChinese = TYPE_CHIN16X16;//Ä¬ÈÏ´òÓ¡ºº×Ö
#endif

#ifdef REVERSE_PRINT_SOLUTION
    s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;
#else
	s_PrintrConfig.m_PrintDir = PRINT_DIRECT_FX;//´òÓ¡»ú´òÓ¡·½Ïò Ì¨Ê½ Ãæ°å

#ifdef PRINTER_DIRECT_NOR

	s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;//´òÓ¡»ú´òÓ¡·½Ïò Ì¨Ê½ Ãæ°å

#endif
#endif

	s_PrintrConfig.m_HoriZoomOut = 1;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
	s_PrintrConfig.m_VertZoomOut = 1;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý

	g_0D0AHigh = 24;//
	g_HTCharSpace = 12;//¸ù¾Ý´òÓ¡µÄ×Ö·ûÈ·¶¨¿Õ¸ñ¿í¶È

	if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_16)
	{
		s_PrintrConfig.m_HoriZoomOut = 2;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
		s_PrintrConfig.m_VertZoomOut = 2;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý
		g_0D0AHigh = 16;//
 	}



	s_PrintrConfig.m_Liftpos = 0;//Ä¬ÈÏ´òÓ¡»ú×óÏÞ
	s_PrintrConfig.m_Righttpos = PRINT_HEAD_LENGTH;//Ä¬ÈÏ´òÓ¡»úÓÒÏÞ
	s_PrintrConfig.m_BaudRate = 0;//´®¿Ú»úÐÍ²¨ÌØÂÊ

	s_PrintrConfig.m_BaudRate = 6;//´®¿Ú»úÐÍ²¨ÌØÂÊ
	#ifdef IF_TYPE_BL
	s_PrintrConfig.m_BaudRate = 10;//´®¿Ú»úÐÍ²¨ÌØÂÊ115200
	#endif
		#ifdef PRINT_BPS_150
		s_PrintrConfig.m_BaudRate = 0x0;
		#endif
		#ifdef PRINT_BPS_300
		s_PrintrConfig.m_BaudRate = 0x1;
		#endif
		#ifdef PRINT_BPS_600
		s_PrintrConfig.m_BaudRate = 0x2;
		#endif
		#ifdef PRINT_BPS_1200
		s_PrintrConfig.m_BaudRate = 0x3;
		#endif
		#ifdef PRINT_BPS_2400
		s_PrintrConfig.m_BaudRate = 0x4;
		#endif
		#ifdef PRINT_BPS_4800
		s_PrintrConfig.m_BaudRate = 0x5;
		#endif
		#ifdef PRINT_BPS_9600
		s_PrintrConfig.m_BaudRate = 0x6;
		#endif
		#ifdef PRINT_BPS_19200
		s_PrintrConfig.m_BaudRate = 0x7;
		#endif
		#ifdef PRINT_BPS_38400
		s_PrintrConfig.m_BaudRate = 0x8;
		#endif
		#ifdef PRINT_BPS_57600
		s_PrintrConfig.m_BaudRate = 0x9;
		#endif
		#ifdef PRINT_BPS_115200
			s_PrintrConfig.m_BaudRate = 0x0A;
		#endif
		#ifdef PRINT_BPS_230400
			s_PrintrConfig.m_BaudRate = 0x0B;
		#endif
		
	
	#ifdef PRINT_BPS_XONXOFF
		s_PrintrConfig.m_BaudRate |= 0x40;
	#endif



	//Odd ÆæÐ§Ñé
	#ifdef PRINT_SERIAL_ODD
		s_PrintrConfig.m_BaudRate |= 0x20;
	#endif
	//Even Å¼Ð§Ñé
	#ifdef PRINT_SERIAL_EVEN
		s_PrintrConfig.m_BaudRate |= 0x10;
	#endif
//7Î»Å¼Ð§Ñé
	#ifdef PRINT_SERIAL_7_EVEN
		s_PrintrConfig.m_BaudRate |= 0x30;
	#endif




	s_PrintrConfig.m_SoftwarePart = 2;//Èí¼þ×î´ó·ÖÇø
	#ifdef PRINTER_LOW_16
	s_PrintrConfig.m_SoftwarePart = 16;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_12
	s_PrintrConfig.m_SoftwarePart = 12;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_10
	s_PrintrConfig.m_SoftwarePart = 10;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_8
	s_PrintrConfig.m_SoftwarePart = 8;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_6
	s_PrintrConfig.m_SoftwarePart = 6;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_4
	s_PrintrConfig.m_SoftwarePart = 4;//Èí¼þ×î´ó·ÖÇø
	#endif
	#ifdef PRINTER_LOW_2
	s_PrintrConfig.m_SoftwarePart = 2;//Èí¼þ×î´ó·ÖÇø
	#endif



	s_PrintrConfig.m_HotDeep = 0;//¶îÍâ¸½¼Ó¼ÓÈÈÉî¶È
	s_PrintrConfig.m_PrintSub = 3;//3;//´òÓ¡»úÏ¸·Ö¶È

	s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS;//ÉèÖÃÎª0±íÊ¾ÎÞ±êÇ©´òÓ¡
	s_PrintrConfig.m_LabelWide = 50;//±íÊ¾´òÓ¡µÄÖ½¿í,Òª¸ù¾ÝÆÕÍ¨Ö½58mmÖ½¿í´òÓ¡¿í¶È48mm 58-50 = 8mm * 8 ¿Õ64µã
	s_PrintrConfig.m_LabelLength = 200;//30mm * 32 1mm=32²½
	s_PrintrConfig.m_BarPaGap = 2;////3mm * 32 1mm=32²½
	//s_PrintrConfig.m_PE_LabValue_L Ö½·ìÅÐ¶ÏÖµ ¸ù¾Ý×Ô¼ì²âÊÔ×Ô¶¯¼ÆËã
	//s_PrintrConfig.m_PE_LabValue_H
	#ifdef PRINTER_LABEL
	s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN;//ÉèÖÃÎª±êÇ©´òÓ¡
	#endif
	


	
	s_PrintrConfig.m_HotLineTOPD = 9;//8 ; //¼ÓÈÈÏßµ½´«¸ÐÆ÷µÄÎ»ÖÃ
	s_PrintrConfig.m_HotLineTOOUT = 8;//6;//7 ;//¼ÓÈÈÏßµ½³öÖ½¿ÚµÄÎ»ÖÃ	


	#ifdef PRINTER_BUFF_RAM
	s_PrintrConfig.m_ReceiveBuffrWor = RECEIVE_RAM;//RECEIVE_FLASH;
	#endif

	#ifdef PRINTER_BUFF_FLASH
	s_PrintrConfig.m_ReceiveBuffrWor = RECEIVE_FLASH;//RECEIVE_FLASH;
	#endif
	s_PrintrConfig.m_CharType = TYPE_CHINESE;//Ä¬ÈÏÖÐÎÄ»úÐÍ
	#ifdef PRINTER_ENGLISH
	s_PrintrConfig.m_CharType = TYPE_ENGLISH;//Ä¬ÈÏÓ¢ÎÄ»úÐÍ
	#endif
	s_PrintrConfig.m_PrintEndFeed = 0;//Ä¬ÈÏ²»×ßÖ½
	s_PrintrConfig.m_PrintEnCode = 0;//Ä¬ÈÏ²»¼ÓÃÜ
	
	
	 s_PrintrConfig.m_PE_LabValue_H = 5;
	
	s_PrintrConfig.m_Delayms_End = 2;
	s_PrintrConfig.m_Delayms_Head = 10;	

}


/****************************************************************************
* º¯ÊýÃû£ºSavePrinterConfig()
* ¹¦  ÄÜ£º´òÓ¡»ú²ÎÊý³õÊ¼»¯
*
* Ëµ  Ã÷£º
****************************************************************************/
#if 0
void SavePrinterConfig(void)
{
	short i;
	Buffer_writer_I = &(s_ListParmeter_B.m_ListBuffer[700]);//ÉèÖÃÖ¸ÕëÎ»ÖÃ
	p_PrCon = &s_PrintrConfig.m_InterFace;

	for(i=0;i<128;i++)
	{
		Buffer_writer_I[i] = 0;
	}
	Buffer_writer_I[0] = CompileTime[0] << 8;
	Buffer_writer_I[0] |= CompileTime[1];
	Buffer_writer_I[1] = CompileTime[3] << 8;
	Buffer_writer_I[1] |= CompileTime[4];


	for(i=0;i< CONFIG_PAR_LENGTH;i++)
	{
		Buffer_writer_I[i+2] = p_PrCon[i];
	}
	ERASE_4K(DL_PRINTER_PAR_ADDR);
	AT91F_DataFlashPageWrite_NE(DL_PRINTER_PAR_ADDR,Buffer_writer_I);//Ð´ÈëÊý¾Ý


}
#endif
void SavePrinterConfig(void)
{
	char mbuf[64];
	memset(mbuf,0,64);
	strcpy(mbuf,(char*)CompileDate);
	strcat(mbuf,(char*)CompileTime);
	s_PrintrConfig.m_CRC = Cal_CRC16((unsigned char*)mbuf,strlen(mbuf));
	PRINTER_PDL.SYS._SavePrinterConfig((unsigned char*)&s_PrintrConfig,sizeof(PrintrConfig));
}

void ReadPrinterConfig(void)
{
	short i;//,j;
	unsigned char *p_con;
	PRINTER_PDL.SYS._ReadPrinterConfig((unsigned char*)&s_PrintrConfig,sizeof(PrintrConfig));
	p_con = (unsigned char*)&s_PrintrConfig;
	
#ifdef SYSTEM_SOUND_SETTING
    unsigned char saved_system_sound = s_PrintrConfig.m_SaveConPar[1] & 0xFF;
    // Map UI state (0=On, 1=Off) to logic flag (1=True, 0=False)
    g_System_Sound_Flag = (saved_system_sound == 0) ? 1 : 0;
#endif

    // --- Key Press Sound (Item #3 -> Index 2) ---
#ifdef AUDIO_BEEP_SEQUENCE
    extern unsigned char g_KeySoundEnable;
    // In the new sequence, Key Sound is the 3rd item (Index 2)
    unsigned char saved_key_state = s_PrintrConfig.m_SaveConPar[2] & 0xFF;
    g_KeySoundEnable = (saved_key_state == 0) ? 1 : 0;    
#endif	
	
#ifdef MY_DEBUG_ENABLE	
	for(i=0;i<sizeof(PrintrConfig);i++)
	{
		printf("%02x ",p_con[i]);
	}
	printf("\r\n");
#endif
}

/****************************************************************************
* º¯ÊýÃû£ºConfigToPrinter()
* ¹¦  ÄÜ£ºÅäÖÃ´òÓ¡»ú
*
* Ëµ  Ã÷£º
****************************************************************************/
void ConfigToPrinter(void)
{

	//s_PrintrConfig.m_InterFace = 0;//Èí¼þ¹Ì¶¨½Ó¿Ú·½Ê½

	s_PrintParameters.m_CharSpace =	s_PrintrConfig.m_CharSpace;//×Ö¼ä¾àÄ¬ÈÏÎª0
	//s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//Ä¬ÈÏ¹ú±êÒ»¶þ¼¶

	//s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//Ä¬ÈÏ24µãÕó»úÐÍ

	s_PrintParameters.m_CharRepertoire = s_PrintrConfig.m_DefaultChar;// = TYPE_ASC12X24;//Ä¬ÈÏ´òÓ¡×Ö·û
	s_PrintParameters.m_ChineseCharType = s_PrintrConfig.m_DefaultChinese;// = TYPE_CHIN24X24;//Ä¬ÈÏ´òÓ¡ºº×Ö


	s_PrintParameters.m_PrintDir = s_PrintrConfig.m_PrintDir;// = PRINT_DIRECT_NOR;//´òÓ¡»ú´òÓ¡·½Ïò Ì¨Ê½ Ãæ°å

	s_PrintParameters.m_HoriZoomOut = s_PrintrConfig.m_HoriZoomOut;// = 1;//Ä¬ÈÏºáÏò·Å´ó±¶Êý
	s_PrintParameters.m_VertZoomOut = s_PrintrConfig.m_VertZoomOut;// = 1;//Ä¬ÈÏ×ÝÏò·Å´ó±¶Êý


	s_PrintParameters.m_Liftpos = s_PrintrConfig.m_Liftpos;//  = 0;//Ä¬ÈÏ´òÓ¡»ú×óÏÞ
	s_PrintParameters.m_Righttpos = s_PrintrConfig.m_Righttpos;// = 384;//Ä¬ÈÏ´òÓ¡»úÓÒÏÞ

	//s_PrintrConfig.m_BaudRate = 0;//´®¿Ú»úÐÍ²¨ÌØÂÊ



	g_ParTypeValue = PRINT_HEAD_LENGTH/s_PrintrConfig.m_SoftwarePart;//s_PrintrConfig.m_SoftwarePart = 4;//Èí¼þ×î´ó·ÖÇø


	g_THDeep = s_PrintrConfig.m_HotDeep;// = 0;//¶îÍâ¸½¼Ó¼ÓÈÈÉî¶È

	g_PrinterDip = s_PrintrConfig.m_PrintSub;// = 3;//´òÓ¡»úÏ¸·Ö¶È



	s_PrintParameters.m_LineSpace = s_PrintrConfig.m_LineSpace;

	s_PrintParameters.m_CharType = s_PrintrConfig.m_CharType;

}

/****************************************************************************
* º¯ÊýÃû£ºPrinterParameterInt()
* ¹¦  ÄÜ£º´òÓ¡»ú²ÎÊý³õÊ¼»¯
*
* Ëµ  Ã÷£º
****************************************************************************/
 void  PrinterParameterInt(void)
 {
	int i;
	s_PrintParameters.m_AlignUPDown = 0x8000;//=1±íÊ¾ÏÂ¶ÔÆë
	g_Char24_17 = 0;
	print_head_length = PRINT_HEAD_LENGTH;
	s_PrintParameters.m_CharSpace = 0;//×Ö¼ä¾àÄ¬ÈÏÎª0

	s_PrintParameters.m_CharType = TYPE_CHINESE;//Ä¬ÈÏÎª0±íÊ¾´òÓ¡µÄÊÇºº×Ö,Ð¡ÓÚ80µÄ±íÊ¾´òÓ¡µÄÊÇ×Ö·û
	s_PrintParameters.m_ChineseCharType = TYPE_CHIN24X24;
	s_PrintParameters.m_CharRepertoire = TYPE_ASC12X24;
	s_PrintParameters.m_Charlist = 1;//Èç¹û´òÓ¡ANK×Ö·û,Ä¬ÈÏ´òÓ¡×Ö·û¼¯1

	s_PrintParameters.m_HoriZoomOut = 1;//ºáÏò·Å´ó±¶ÊýÎª1
	s_PrintParameters.m_VertZoomOut = 1;//×ÝÏò´òÓ¡·Å´óÎª1

	s_PrintParameters.m_boldface = VALUE_DISABLE;//È¡Ïû¼Ó´ÖÄ£Ê½

	s_PrintParameters.m_Inverse = 0;//·´°×Îª0²»·´°×

	s_PrintParameters.m_Liftpos = 0;//×óÏÞµÄÎ»ÖÃÄ¬ÈÏÎªÆðÊ¼Î»ÖÃ0
	s_PrintParameters.m_Righttpos = print_head_length;//´òÓ¡»úÓÒÏÞ

	s_PrintParameters.m_LineSpace = 3;//ÐÐ¼ä¾àÄ¬ÈÏÎª3

	s_PrintParameters.m_PageLength = 255;//Ä¬ÈÏÎª255
	s_PrintParameters.m_Pagecount = 0;//´òÓ¡Ò³³¤¼ÆÊýÆ÷Îª0

	s_PrintParameters.m_positions = print_head_length;//´òÓ¡»úÄÜ´òÓ¡µÄÎ»ÖÃÎª´òÓ¡Í·ÄÜ¹»´òÓ¡µÄµãÊý


	s_PrintParameters.m_PrintDir = PRINT_DIRECT_NOR;//Õý³£Ì¨Ê½´òÓ¡»ú´òÓ¡·½Ïò

	s_PrintParameters.m_Prstate = 0;//´òÓ¡»úµ±Ç°×´Ì¬

	s_PrintParameters.m_RotDegree = 0;//Ðý×ª½Ç¶ÈÄ¬ÈÏÎª0¶È
	s_PrintParameters.m_alignType = 0;//×ó¶ÔÆë
	for (i=0;i<20;i++)
	{
		s_PrintParameters.m_TabPos[i] = 0;//Çå¿ÕË®Æ½Ôì±íÎ»ÖÃ×îÐ¡ÖµÎª1
	}
	s_PrintParameters.m_TabCount = 0;//Ë®Æ½Ôì±í¼ÆÊýÆ÷Îª0

	s_CodeBarCon.m_BarHigh = 50;//ÌõÂë¸ß¶ÈÄ¬ÈÏ50
	s_CodeBarCon.m_Barwide = 2;//¿í¶ÈÄ¬ÈÏ2
	s_CodeBarCon.m_CharSpace = 2;//ÌõÂë×Ö·ûÔÚÉÏÃæ
	s_CodeBarCon.m_BarSpace = 0;
	g_DriverAFlag = 0;

	

	
	
	g_THDeep = 0;//Ä¬ÈÏÔö¼Ó¼ÓÈÈÉî¶È0
	g_ThType = 0;//¼ÓÈÈÀàÐÍ
	g_SpeedChoice = 0;//È¡ÏûÉè¶¨ËÙ¶È

	g_CodePage = 255;//0;
	g_InternationChar = 0;

	s_PrintParameters.m_QRZoom = 0x04;

	//µÃµ½ÅäÖÃ±í
	ConfigToPrinter();
	g_0D0AHigh = 24;//
	g_HTCharSpace = 12;//¸ù¾Ý´òÓ¡µÄ×Ö·ûÈ·¶¨¿Õ¸ñ¿í¶È

	if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_16)
	{
		g_0D0AHigh = 16;//
	}

	g_unicodeType = 0;//asc±àÂë	
	
	
 }
 
void delaynms(unsigned int delay)
{
		__SETPRIMASK();
	gTimer_1ms = 0;
	__RESETPRIMASK();
	while(gTimer_1ms < delay)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter();//Î¹¹·
	}
} 
 
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
/**
 * @FuncName  Oled_UpdateThermalStatus 
 * @param     state : ThermalState_t
 * @retval    void
 * @Description Updates the OLED status line ONCE upon thermal state transitions.
 */
static void Oled_UpdateThermalStatus (ThermalState_t thermal_state)
{
	unsigned char m_w;
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    if (BatteryLowCharge_PrintWarning())
        return;
#endif	
	/* Clear text row at Y=14 while keeping top status icons intact */
	DisplayStr8x16char("                ", 0, 14, 0);
	switch (thermal_state)
	{
		case THERMAL_STATE_WARNING:
			m_w = GetStr8x16Wide((char *)"Too Hot", 1);
			DisplayStr8x16char("Too Hot", (128 - m_w) / 2, 14, 1);
			break;

		case THERMAL_STATE_STOP:
			m_w = GetStr8x16Wide((char *)"Cooling down", 1);
			DisplayStr8x16char("Cooling down", (128 - m_w) / 2, 14, 1);
			break;

		case THERMAL_STATE_PRINT:
		default:
			m_w = GetStr8x16Wide((char *)"printing", 1);
			DisplayStr8x16char("printing", (128 - m_w) / 2, 14, 1);
			break;
	}
	DisplayBufEn();
}

/**
 * @FuncName  Thermal_State_Machine_Update
 * @param     void
 * @retval    void
 * @Description Non-blocking periodic thermal monitoring state machine.
 *              - Runs every 100ms using system uptime tick (g_sysTime).
 *              - Applies hysteresis and 2-sample debounce filter.
 *              - Triggers OLED update ONLY on confirmed state transitions.
 */
static void Thermal_State_Machine_Update(void)
{
	static uint32_t last_check_time = 0;
	static ThermalState_t pending_state = THERMAL_STATE_PRINT;
	static unsigned char debounce_count = 0;
	unsigned short adc_VAL;
	unsigned short current_temp;
	ThermalState_t target_state;

	/* Run periodic sampling every 100ms */
	if ((g_sysTime - last_check_time) < 100)
	{
		return;
	}
	last_check_time = g_sysTime;

	adc_VAL = ADC_Quick_GetVoltage(ADC_CHANNEL_0, ADC_GPIO);
	current_temp = Read_Thermistor_Temperature(adc_VAL);
	g_CurrentThermalTemp = current_temp;

	target_state = g_ThermalState;

	switch (g_ThermalState)
	{
		case THERMAL_STATE_PRINT:
			if (current_temp > THERMAL_STOP_TEMP) // > 47.0 C
			{
				target_state = THERMAL_STATE_STOP;
#ifdef TEMP_STATUS_ACK_TO_SOFTWARE_APP
Get_Temperature_BT_Response("PAUSED", current_temp);
#endif				
			}
			else if (current_temp > THERMAL_WARNING_TEMP) // > 43.0 C
			{
				target_state = THERMAL_STATE_WARNING;
			}
			break;

		case THERMAL_STATE_WARNING:
			if (current_temp > THERMAL_STOP_TEMP) // > 47.0 C
			{
				target_state = THERMAL_STATE_STOP;
			}
			else if (current_temp <= THERMAL_WARNING_TEMP) // <= 43.0 C
			{
				target_state = THERMAL_STATE_PRINT;
			}
			break;

		case THERMAL_STATE_STOP:
			if (current_temp <= THERMAL_RESUME_TEMP) // <= 38.0 C
			{
				target_state = THERMAL_STATE_PRINT;
#ifdef TEMP_STATUS_ACK_TO_SOFTWARE_APP
Get_Temperature_BT_Response("RESUMED", current_temp);
#endif				
			}
			break;

		default:
			target_state = THERMAL_STATE_PRINT;
			break;
	}

	/* State transition debounce & hysteresis filter */
	if (target_state != g_ThermalState)
	{
		if (target_state == pending_state)
		{
			debounce_count++;
			if (debounce_count >= THERMAL_DEBOUNCE_COUNT)
			{
				g_ThermalState = target_state;
				if (target_state == THERMAL_STATE_STOP)
				{
					/* Do not interrupt an active GS v 0 label. */
					g_ThermalStopPending = 1;
				}
				else if (target_state == THERMAL_STATE_PRINT)
				{
					/* The head recovered before a boundary was reached. */
					g_ThermalStopPending = 0;
				}
				debounce_count = 0;
				Oled_UpdateThermalStatus (g_ThermalState);
			}
		}
		else
		{
			pending_state = target_state;
			debounce_count = 1;
		}
	}
	else
	{
		pending_state = g_ThermalState;
		debounce_count = 0;
	}
}

/**
 * @FuncName  Thermal_Head_Protection_Check
 * @param     void
 * @retval    void
 * @Description Non-blocking print-path thermal protection check.
 *              Consumes pre-computed thermal state (g_ThermalState).
 *              Ensures dot-line transmission finishes before pausing printing.
 */
static void Thermal_Head_Protection_Check(void)
{
	/* Periodic non-blocking thermal state update (returns instantly if 100ms hasn't elapsed) */
	Thermal_State_Machine_Update();

	/* An active raster label owns the paper position until its final block has
	 * printed.  Keep receiving bytes in the ring buffer, but do not pause the
	 * motor/head half-way through that label. */
	if ((g_ThermalState == THERMAL_STATE_STOP) &&
		(g_ThermalRasterLabelActive != 0) &&
		(g_ThermalBoundaryPause == 0))
	{
		return;
	}

	/* Pause print path if thermal state machine triggered STOP or interrupt has paused for cooling */
	if (g_ThermalState == THERMAL_STATE_STOP || g_ThermalPrintTrack.thermal_pause_active)
	{
		/* If motor is actively printing, wait until motor interrupt finishes current line and pauses */
		if (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR && g_ThermalPrintTrack.thermal_pause_active == 0)
		{
			while (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR && g_ThermalPrintTrack.thermal_pause_active == 0)
			{
				PRINTER_PDL.SYS._WDG_ReloadCounter();
			}
		}
		else if (s_PrinterMOTOPar.m_StopPrint == ENABLE_VAR && g_ThermalPrintTrack.thermal_pause_active == 0)
		{
			/* Printer is currently idle.  On this board _Drive_Power_DIS() also
			 * removes the printer supply, which powers the unit off.  A thermal
			 * pause must only remove heat from the head, so clear the strobes and
			 * retain the drive supply for the cooling/resume path. */
			PRINTER_PDL.SYS._STB_ALL_CLEAR();
		}

		/* Non-blocking cooling loop: service watchdog until state machine recovers to NORMAL (<= 38.0 deg C) */
		while (g_ThermalState == THERMAL_STATE_STOP)
		{
			g_PowerOffCount = 0; /* Prevent auto power-off while cooling */
			PRINTER_PDL.SYS._WDG_ReloadCounter();

			Thermal_State_Machine_Update();

			// mukesh - updated_23_09: keep the OLED temperature sourced from the ADC while the print path is paused.
			DisplayTemp(DIS_TEMP_X, DIS_TEMP_Y);

			Delay1ms(10);
		}

		/* Keep the drive supply asserted.  It was intentionally left on during
		 * cooling because disabling it is a full printer power-off on this board. */
		PRINTER_PDL.SYS._WDG_ReloadCounter();

		/* A GS v 0 label ended before this cooling pause. Its final line was
		 * already strobed and motor outputs were cleared. Let the parser begin
		 * the next queued label instead of restarting an IRQ. */
		if (g_ThermalBoundaryPause)
		{
			g_ThermalBoundaryPause = 0;
			g_ThermalStopPending = 0;
			g_ThermalPrintTrack.thermal_pause_active = 0;
			return;
		}

		/* If printing was interrupted mid-job, resume cleanly from the exact next unprinted line */
		if (g_ThermalPrintTrack.thermal_pause_active)
		{
			// mukesh - updated_23_09: restore the held motor phase before the next interrupt advances it.
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7], s_PrintParameters.m_CurMotoPWM);
			g_ThermalPrintTrack.thermal_pause_active = 0;
			s_PrinterMOTOPar.m_StopPrint = DISABLE_VAR;
			s_PrinterMOTOPar.m_MotoRun = ENABLE_VAR;
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(10);
		}
	}
}
#endif

extern unsigned short g_PowerVol;
/****************************************************************************
* º¯ÊýÃû£ºmain()
* ¹¦  ÄÜ£ºÖ÷³ÌÐò
*
* Ëµ  Ã÷£º
RD_USART1_EN
****************************************************************************/


int Handler_main()
{
#ifdef PSF20_R002_TSPL_PROTOCOL
    TsplNative_Init();
    TsplFirmware_Init();
#endif

	unsigned char mbuf[64];
	unsigned int i, m_w;
	unsigned short adc_VAL ;
  unsigned short m_ThermalTemp;
	
	oled_init();															// initialization of the OLED
	ClearDisplayBuf();												//clears the display buffer so the screen starts blank.

	if (PRINTER_PDL.SYS._GetKey_POW())
	{
		PRINTER_PDL.SYS._POWER_DIS();					// turns the 3.3v OFF and hold it LOW, MCU_PWR_EN flag is attached to this GPIO-42 (PC10)
	}
	PRINTER_PDL.SYS._POWER_EN();						// turns the 3.3v ON and hold it HIGH, MCU_PWR_EN flag is attached to this GPIO-42 (PC10)
	g_BatDisCount = 0;
	g_BatDisCount = 0;
	g_PowerFlag = 0;
	
#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING
	ReadPrinterConfig();
	g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
	BatteryLowCharge_BootGuard();
#else
	g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
#endif	
	
#ifdef EMS_TEST_CODE //(Modification for EMS Vendor test code: Kajal 02 April 2026)
	Display_ALLSEG_ON();
	PRINTER_PDL.SYS._BEEP_EN();
	Delay1ms(2000);
#endif

	memset(mbuf, 0, 32);						// mukesh - clears the buffer to '0'
	// sprintf(mbuf,"v:%d",g_PowerVol);
	//	DisplayStr6x8(mbuf,24,8);
	// DisplayLogo();

#ifdef POSIFLOW_LOGO_AND_NAME
	DisplayLogo();
	DisplayBufEn();
	Delay1ms(LOGO_DISPLAY_TIME); // Display Logo for 2 Sec
	ClearDisplayBuf();
#else
	DisplayStrchar("POSIFLOW", 16, 0);
	DisplayStr6x8("Made in India", 25, 24);
	DisplayBufEn();
#endif
	// Initial 2-Beep Sequence when Power/ Turn ON 
	PRINTER_PDL.SYS._BEEP_EN();				
	Delay1ms(100);
	PRINTER_PDL.SYS._BEEP_DIS();
	Delay1ms(100);
	PRINTER_PDL.SYS._BEEP_EN();
	Delay1ms(100);
	PRINTER_PDL.SYS._BEEP_DIS();
	Delay1ms(100);
	// mukesh - if someone presses and hold the Power button, just after LOGO is display then the below loop gets executed and we see the LOGO screen only 
	while (PRINTER_PDL.SYS._GetKey_POW() == 0)		// mukesh - once the printer is Powered ON, still we keep Holding the Power Button then this loop handles it 
	{
		g_PowerVol = PRINTER_PDL.ADC._GetDriveVoltage();
		PRINTER_PDL.SYS._WDG_ReloadCounter(); // reload the watchdog ???????Z??????
	} // mukesh - the false press and hold handling ends here
	g_BatDisCount = 0;
	g_BatDisCount = 0;

	PRINTER_PDL.SYS._WDG_ReloadCounter();

	g_PowerFlag = 0xA5E3;		// mukesh - set the PowerFlag 

#ifdef POWER_ON_CAL
	// Calibration initialization
	Power_ON_CAL_Status = 0;
	Calibration_Mode = Cal_Mode_Auto; // Initial Calibration Automatic

	// MyPrintf("CAL_Mode:%d\r\n",Calibration_Mode);
	g_CAL_Mode_statusFlag = 1;

	g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();		// mukesh - reads the ADC values for Paper Detection -> Gives Media Status - Present or Absent  
	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		// mukesh - reads the ADC values for Gap Detection -> Gives Media type - Paper or Label 

	
#ifdef AUTO_CAL_SELECTION						// simplify auto calibratio to "CALIBRATE: YES /NO" 
				
				ClearDisplayBuf();
        m_w = GetStr8x16Wide((char*)"Calibrate: YES", 0);
        DisplayStr8x16char("Calibrate: YES", (128 - m_w) / 2, 14, 0);
        DisplayBufEn();
				
				uint32_t selection_timer = 0;
        while(selection_timer < CAL_MODE_SELECTION_TIME) // Selection timeout (3 seconds)
        {
            if(PRINTER_PDL.SYS._GetKey_LF() == 0) // Feed button pressed
            {
                ClearDisplayBuf();
                if(Calibration_Mode == Cal_Mode_Auto)
                {
                    Calibration_Mode = Cal_Mode_Manual; // Toggle to NO (Skip)
                    m_w = GetStr8x16Wide((char*)"Calibrate: NO", 0);
                    DisplayStr8x16char("Calibrate: NO", (128 - m_w) / 2, 14, 0);
                }
                else
                {
                    Calibration_Mode = Cal_Mode_Auto;   // Toggle to YES (Proceed)
                    m_w = GetStr8x16Wide((char*)"Calibrate: YES", 0);
                    DisplayStr8x16char("Calibrate: YES", (128 - m_w) / 2, 14, 0);
                }
                DisplayBufEn();

                selection_timer = 0; // Reset countdown after user interaction

                // Wait for Feed button release (debounce)
                while(PRINTER_PDL.SYS._GetKey_LF() == 0)
                {
                    PRINTER_PDL.SYS._WDG_ReloadCounter();
                }
            }
            Delay1ms(1);
            selection_timer++;
            PRINTER_PDL.SYS._WDG_ReloadCounter();
        }
	
#else	
	//  CAL MODE SELECTION
	ClearDisplayBuf();		// mukesh - Clear the OLED screen 
	m_w = GetStr8x16Wide((char *)"CAL Mode: Auto", 0);
	DisplayStr8x16char("CAL Mode: Auto", (128 - m_w) / 2, 14, 0);
	DisplayBufEn();				// mukesh - This will show the above Message on the screen 

	uint32_t selection_timer = 0;
	while (selection_timer < 3000)
	{
		if (PRINTER_PDL.SYS._GetKey_LF() == 0) // mukesh - Feed button detected ( if Feed Button is pressed -> we toggle the Calibration Mode
		{
			if (Calibration_Mode == Cal_Mode_Auto)
			{
				Calibration_Mode = Cal_Mode_Manual;
				ClearDisplayBuf();
				m_w = GetStr8x16Wide((char *)"CAL Mode:Manual", 0);
				DisplayStr8x16char("CAL Mode:Manual", (128 - m_w) / 2, 14, 0);
			}
			else
			{
				Calibration_Mode = Cal_Mode_Auto;
				ClearDisplayBuf();
				m_w = GetStr8x16Wide((char *)"CAL Mode: Auto", 0);
				DisplayStr8x16char("CAL Mode: Auto", (128 - m_w) / 2, 14, 0);
			}
			DisplayBufEn();
			selection_timer = 0; // Reset timeout to give user more time after a change
			while (PRINTER_PDL.SYS._GetKey_LF() == 0)		// mukesh - This is a loop if the Feed Button Key gets stuck this handles it 
			{
				PRINTER_PDL.SYS._WDG_ReloadCounter();
			}
		}
		Delay1ms(1);
		selection_timer++;
		PRINTER_PDL.SYS._WDG_ReloadCounter();
	}
#endif

#endif  // Auto cal selection

#ifdef EMS_TEST_CODE
	PRINTER_PDL.SYS._Drive_Power_EN();
#endif

	//***********????'??**********************
	g_ConfigRCC = 8;
	g_CutterErr = 0;
	g_LabPrintEndF = 0;
	g_BarCodeDegree = 0;
	g_BarCharHigh = 24;

#ifdef TEMP_DEBUG_FOR_PF	  //(Modify by Kajal: 08-Sep-2026)
MyPrintf("-------------Before Configuration:--------------\r\n");
Before_PrinterCOnfig();	
#endif

#ifndef PAPER_FEED_SOLUTION
	ReadPrinterConfig();
#endif

#ifdef TEMP_DEBUG_FOR_PF	  //(Modify by Kajal: 08-Sep-2026)
MyPrintf("-------------1) After read Configuration:--------------\r\n");
Before_PrinterCOnfig();	

MyPrintf("\r\nBefore Memset_mBufValues:\r\n");
for(int k=0; k<64; k++)
{
	 MyPrintf("%X ",mbuf[k]);
}
MyPrintf("\n");
#endif

	memset(mbuf, 0, 64);
	
#ifdef TEMP_DEBUG_FOR_PF    //(Modify by Kajal: 08-Sep-2026)
MyPrintf("\r\nPowerON_mbuf_Values\r\n");
for(int k=0; k<64; k++)
{
	 MyPrintf("%X ",mbuf[k]);
}
MyPrintf("\n");
#endif	
	
	
	strcpy(mbuf, (char *)CompileDate);
	strcat(mbuf, (char *)CompileTime);
	
#ifdef TEMP_DEBUG_FOR_PF    //(Modify by Kajal: 08-Sep-2026)
MyPrintf("\r\nCompileDate:%s & -->%X \r\n",CompileDate,CompileDate);
MyPrintf("\r\nCompileTime:%s & -->%X \r\n",CompileTime,CompileTime);
MyPrintf("\r\nmbuf_Values_After Date & Time function\r\n");

for(int k=0; k<64; k++)
{
	 MyPrintf("%X ",mbuf[k]);
}
MyPrintf("\n");

MyPrintf("\r\nCal_CRC=%X & m_CRC=%X\r\n",i,s_PrintrConfig.m_CRC);  

#endif	

#ifdef PAPER_FEED_SOLUTION
#ifdef MY_DEBUG_ENABLE
	MyPrintf("-------------Before Configuration:--------------\r\n");
	Before_PrinterCOnfig();
#endif

	//  s_PrintrConfig.m_CRC = Cal_CRC16((unsigned char*)mbuf,strlen(mbuf));
	PrinterConfigInt();
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	SavePrinterConfig(); // In PSF20 Code Need to save this initial configuration for Display menu setting option
#ifdef MY_DEBUG_ENABLE
	MyPrintf("-------------After Configuration:--------------\r\n");
	Before_PrinterCOnfig();
#endif
#endif

#ifdef FOR_PSF20_PAPER_FEED_SOLUTION
	// PSF20 Paper Feed Solution:
	// To prevent continuous paper feed caused by corrupted configuration parameters,
	// only the parameters that affect paper feed operation will be initialized and
	// configured during every power-on. Other user settings stored in flash (Settings Menu)
	// will remain unchanged to preserve user configurations.
	s_PrintrConfig.m_LineSpace = 3;
#endif

	i = Cal_CRC16((unsigned char *)mbuf, strlen(mbuf));
#ifdef TEMP_DEBUG_FOR_PF    //(Modify by Kajal: 08-Sep-2026)
MyPrintf("\r\nCal_CRC=%X & m_CRC=%X\r\n",i,s_PrintrConfig.m_CRC);  
#endif
#ifndef PAPER_FEED_SOLUTION
	if (i != s_PrintrConfig.m_CRC)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter();
		// ??????????????'??????
#ifdef MY_DEBUG_ENABLE
		printf("init\r\n");
#endif
		PrinterConfigInt();
		PRINTER_PDL.SYS._WDG_ReloadCounter();
		SavePrinterConfig();
	}
#endif
	
#ifdef TEMP_DEBUG_FOR_PF	 //(Modify by Kajal: 08-Sep-2026)
MyPrintf("-------------2) After read Configuration:--------------\r\n");
Before_PrinterCOnfig();	
#endif	
	
#ifdef FOR_PSF20_PAPER_FEED_SOLUTION		 //(Modify by Kajal: 08-Sep-2026)
		Set_default_printer_setting_at_power_on();
	
#ifdef TEMP_DEBUG_FOR_PF	  //(Modify by Kajal: 08-Sep-2026)
MyPrintf("-------------3) After Reconfiguration of default setting:--------------\r\n");
Before_PrinterCOnfig();	
#endif	
#endif	
	
	PRINTER_PDL.SYS._WDG_ReloadCounter();

#ifndef FOR_PSF20_PAPER_FEED_SOLUTION    //(Modify by Kajal: 08-Sep-2026)
		ReadPrinterConfig(); 
#endif

	//*****************??????*******************************
	g_Receive_count = 0; // ?????????????
	Receive_count = 0;	 // ??????????
	g_Order_count = 0;	 // ???????J???

#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("*1-->BFR SR:%d & LA:%d & SP:0x%4X\r\n",g_BatteryLowChargeShutdownRequest,g_BatteryLabelActive,s_PrinterMOTOPar.m_StopPrint);
//g_BatteryLowChargeShutdownRequest = 1;
//g_BatteryLabelActive = 1;
//s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
#endif

	PrinterCore = PrinterCore_RAM;			// mukesh - PrintCore is a function pointer to the PrinterCore_RAM function 
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("*1-->AFT SR:%d & LA:%d & SP:0x%4X\r\n",g_BatteryLowChargeShutdownRequest,g_BatteryLabelActive,s_PrinterMOTOPar.m_StopPrint);
//g_BatteryLowChargeShutdownRequest = 1;
//g_BatteryLabelActive = 0;
//s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
#endif	
	p_OcAndGDAdr = &g_Order_count;		// ?????????buffer ???t???? g_GetDataFromFlashAddr
	p_REAndSAcount = &Receive_count;	// ????????J???g_InterFaceBufferCount
	p_SaveDrREcount = &g_Receive_count; // ?????????t???? g_SaveDataOffset

	g_PaperType = s_PrintrConfig.m_InterFace;
//	printf("PT:%d\r\n",g_PaperType);
	g_ProFuncValue = 0;		// mukesh - this variable is used when we press Feed button more than 9 times 
	g_CurveCountFlag = 0;

	g_DetPaperNoprint = 0;
	g_NotStbFlag = 0;
	g_DetPaperGapSpace = 0;

	g_ProFuncValue = 0;
	g_CurveCountFlag = 0;
	g_ProFlagVelue = 0xaa00;

	//=======================================

#ifdef POWER_ON_CAL
	// Load physical printer mechanism offsets (in mm) from the saved configuration
	g_HotLineTOPD = s_PrintrConfig.m_HotLineTOPD;
	g_HotLineTOOUT = s_PrintrConfig.m_HotLineTOOUT;

	if (Calibration_Mode == Cal_Mode_Auto)			// mukesh - if the mode is Auto - Start the calibration
	{
		Power_ON_CAL_Status = PowerON_Calibration();
		//	MyPrintf("PCS:%d\r\n",Power_ON_CAL_Status);
		PRINTER_PDL.SYS._WDG_ReloadCounter();
	}
	else // Manual
	{
#ifdef AUTO_CAL_SELECTION

	//Display Error msg on OLED for skipping Calibration
	
	ClearDisplayBuf();
	m_w = GetStr8x16Wide((char*)"Calibration", 0);
	DisplayStr8x16char("Calibration", (128-m_w)/2, 0, 0);
	m_w = GetStr8x16Wide((char*)"Discarded", 0);
	DisplayStr8x16char("Discarded", (128-m_w)/2, 16, 0);
	
	DisplayBufEn();		
	
	PRINTER_PDL.SYS._WDG_ReloadCounter();	
	Delay1ms(2000);
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	
	ClearDisplayBuf();
	m_w = GetStr8x16Wide((char*)"Media: Unknown", 0);
	DisplayStr8x16char("Media: Unknown", (128-m_w)/2, 0, 0);
	m_w = GetStr8x16Wide((char*)"Size: Unknown", 0);
	DisplayStr8x16char("Size: Unknown", (128-m_w)/2, 16, 0);
	DisplayBufEn();	
	
	PRINTER_PDL.SYS._WDG_ReloadCounter();	
	Delay1ms(2000);
	PRINTER_PDL.SYS._WDG_ReloadCounter();	
	
	ClearDisplayBuf();
			
		// In single-step mode, selecting "NO" skips calibration directly!
		Power_ON_CAL_Status = 2; // 2 = Skipped
		g_CAL_Mode_statusFlag = 0;
		
#ifndef DISCARDING_CAL_SET_PREV_SR20_FUNCTIONALITY		   //[Modify by: Kajal ---> 10 Oct 2026] ---> see macro definition
    	//user loaded paper and skiped calibration  //FIX: For Label feed when calibraion was skipped (derick)
		g_PaperType = 1; 
		s_PrintrConfig.m_InterFace = 1;               // Continuous Paper
		s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS;// Disable label mode
#else		
		g_PaperType = s_PrintrConfig.m_InterFace;//Default set to label if the calibration discarded by the User
		//MyPrintf("*PT:%d\r\n",g_PaperType);
#endif
    mediaType = "P";

#else		
		ClearDisplayBuf();
		m_w = GetStr8x16Wide((char *)"Calibration: YES", 0);
		DisplayStr8x16char("Calibration: YES", (128 - m_w) / 2, 14, 0);
		DisplayBufEn();

		unsigned char manual_cal_choice = 1;
		uint32_t manual_timer = 0;
		while (manual_timer < 2000)
		{
			if (PRINTER_PDL.SYS._GetKey_LF() == 0)		// mukesh - Read the Feed Button and if it is pressed then toggle the Manual choice menu
			{ // mukesh - if Feed Button Pressed 
				manual_cal_choice = !manual_cal_choice; // mukesh - manual_cal_choice = 0 
				ClearDisplayBuf();
				if (manual_cal_choice)			// mukesh - if manual_cal_choice == 1 
					DisplayStr8x16char("Calibration: YES", (128 - GetStr8x16Wide("Calibration: YES", 1)) / 2, 14, 0);
				else												// mukesh - if manual_cal_choice == 0 
					DisplayStr8x16char("Calibration: NO", (128 - GetStr8x16Wide("Calibration: NO", 1)) / 2, 14, 0);
				DisplayBufEn();
				manual_timer = 0;			// mukesh - if the Feed Button is pressed RESET the manual_timer to 0 giving again time to toggle 
				while (PRINTER_PDL.SYS._GetKey_LF() == 0)
					PRINTER_PDL.SYS._WDG_ReloadCounter();
			}
			Delay1ms(1);
			manual_timer++;
			PRINTER_PDL.SYS._WDG_ReloadCounter();
		}

		if (manual_cal_choice)		// mukesh - this will initiaite the Calibration Process
		{
			Power_ON_CAL_Status = PowerON_Calibration();
			PRINTER_PDL.SYS._WDG_ReloadCounter();
		}
		else
		{
			Power_ON_CAL_Status = 2; // Skipped
		}
		#endif	// Auto cal selection 
	}

	if (Power_ON_CAL_Status == 1) // mukesh - Media Detected Successfully  & Only show media types on Success
	{
		// Media Found
		ClearDisplayBuf();
		if (g_boundary_detection_flag == 1)		// mukesh - The detected media was Label 
		{
			Label_Size_Calculation(g_boundary_gap_count_val); // calculate lab size
			mediaType = "L";
			// Update Persistent Settings
			s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN;
			s_PrintrConfig.m_BarPaGap = 3; // Standard Gap 3mm
			s_PrintrConfig.m_LabelLength = g_label_size_final;

			m_w = GetStr8x16Wide((char *)"Media: Label", 1);
			DisplayStr8x16char("Media: Label", (128 - m_w) / 2, 0, 1);

			char size_buf[32];									// Temporary buffer for formatting
			sprintf(size_buf, "Size: @%d mm", label_length_mm); // label_length_mm

			m_w = GetStr8x16Wide(size_buf, 1);
			DisplayStr8x16char(size_buf, (128 - m_w) / 2, 16, 1);
			DisplayBufEn();
			PRINTER_PDL.SYS._WDG_ReloadCounter();
		}
		else
		{ 
#ifndef MEDIA_UNKNOWN_DETECTION_IN_SETTING_MENU			
			if(Power_ON_CAL_Status == 2)   //For unknown media if calibration discarded at power on
			{
				  m_w = GetStr8x16Wide((char *)"Media: Unknown", 1);
					DisplayStr8x16char("Media: Unknown", (128 - m_w) / 2, 14, 1);
			}
			else{
				// For Paper Media 
			    mediaType = "P";
#ifndef DISCARDING_CAL_SET_PREV_SR20_FUNCTIONALITY		   //[Modify by: Kajal ---> 10 Oct 2026] ---> see macro definition			
			s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS;
#endif			
					m_w = GetStr8x16Wide((char *)"Media: Paper", 1);
					DisplayStr8x16char("Media: Paper", (128 - m_w) / 2, 14, 1);
			}
#else			 //Previous logic for paper detection only
			// mukesh - For Paper Media 
			mediaType = "P";
#ifndef DISCARDING_CAL_SET_PREV_SR20_FUNCTIONALITY		   //[Modify by: Kajal ---> 10 Oct 2026] ---> see macro definition			
			s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS;
#endif			
			m_w = GetStr8x16Wide((char *)"Media: Paper", 1);
			DisplayStr8x16char("Media: Paper", (128 - m_w) / 2, 14, 1);
			
#endif			
		}
		DisplayBufEn();
		Delay1ms(2000);

		// Save results to Flash
#ifndef PF_ISSUE_SOLUTION // No need to save flash
		SavePrinterConfig();
#endif

		// PWR_Tag13   Check Power on calibration status
		ClearDisplayBuf();
		m_w = GetStr8x16Wide((char *)"Calibration Done", 0);
		DisplayStr8x16char("Calibration Done", (128 - m_w) / 2, 14, 0);
		DisplayBufEn();
		Delay1ms(2000); // wait 2secs

		g_CAL_Mode_statusFlag = 0; // Reset calibration mode
	}
	else if (Power_ON_CAL_Status == 3 || Power_ON_CAL_Status == 4) // Failure codes
	{
#ifdef AUDIO_BEEP_SEQUENCE
		BuzzerManager_Init((Power_ON_CAL_Status == 3), (Power_ON_CAL_Status == 4));
#endif
		ClearDisplayBuf();
		m_w = GetStr8x16Wide((char *)"CAL Failed", 1);
		DisplayStr8x16char("CAL Failed", (128 - m_w) / 2, 14, 1);
		DisplayBufEn();
		Delay1ms(2000);

		ClearDisplayBuf();
		if (Power_ON_CAL_Status == 3)
		{
			mediaType = "A";
			m_w = GetStr8x16Wide((char *)"Media Absent", 1);
			DisplayStr8x16char("Media Absent", (128 - m_w) / 2, 14, 1);
		}
		else if (Power_ON_CAL_Status == 4)
		{
			mediaType = "LO";
			m_w = GetStr8x16Wide((char *)"Lid Open", 1);
			DisplayStr8x16char("Lid Open", (128 - m_w) / 2, 14, 1);
		}
		DisplayBufEn();
		Delay1ms(2000);
	}

	// Home screen for both successful and skipped calibration
#else
	ClearDisplayBuf();
	SetPrinterStatus(DIS_PE_X, DIS_PE_Y, 1);
	SetCoverStatus(DIS_COV_X, DIS_COV_Y, 1);
	DisplayBTPro();

#ifdef POSIFLOW_LOGO_AND_NAME
	m_w = GetStr8x16Wide((char *)PRODUCT_NAME, 1);
	DisplayStr8x16char((char *)PRODUCT_NAME, (128 - m_w) / 2, 14, 1);
#else
	m_w = GetStr8x16Wide((char *)"SR20", 1);
	DisplayStr8x16char("SR20", (128 - m_w) / 2, 14, 1);
#endif

	DisplayBatty(DIS_BAT_X, DIS_BAT_Y);
/* Show the measured head temperature on the initial home screen only in the protected build. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
	DisplayTemp(DIS_TEMP_X, DIS_TEMP_Y);
#endif

#endif // Power on cal end

	//=======================================
	// ???????  mukesh - Setting the Automatic Shutdown Timer 
	if (s_PrintrConfig.m_SaveConPar[4] == 0x0300)
	{
		g_PowerOffTimeOut = 0;
	}
	else if (s_PrintrConfig.m_SaveConPar[4] == 0x0301)
	{
		g_PowerOffTimeOut = 10 * 60 * 1000;
	}
	else if (s_PrintrConfig.m_SaveConPar[4] == 0x0302)
	{
		g_PowerOffTimeOut = 20 * 60 * 1000;
	}
	else if (s_PrintrConfig.m_SaveConPar[4] == 0x0303)
	{
		g_PowerOffTimeOut = 30 * 60 * 1000;
	}
	else if (s_PrintrConfig.m_SaveConPar[4] == 0x0304)
	{
		g_PowerOffTimeOut = 60 * 60 * 1000;
	}
	else if (s_PrintrConfig.m_SaveConPar[4] == 0x0305)
	{
		g_PowerOffTimeOut = 6 * 60 * 60 * 1000;
	}
	else
	{
		s_PrintrConfig.m_SaveConPar[4] = 0x0301;
		g_PowerOffTimeOut = 10 * 60 * 1000;
	}

	// This commented becuase it feeds 20 mm paper
	//	if((s_PrintrConfig.m_SaveConPar[3] & 0xF) == 0)
	//	{
	//		g_PaperType = 0;
	//		s_PrintrConfig.m_InterFace = 0;
	//	}
	//	else
	//	{
	//		g_PaperType = 1;
	//		s_PrintrConfig.m_InterFace = 1;
	//	}

	//	//speed
	i = s_PrintrConfig.m_SaveConPar[8] & 0x7;
	if (i > 4)
	{
		i = 4;
		s_PrintrConfig.m_SaveConPar[8] &= 0xFF00;
		s_PrintrConfig.m_SaveConPar[8] |= i;
	}
	g_Speed = (4 - i) * 120;

	g_Item = 0x0100;

	//=======================================
	g_receiveFlage = 0;
#ifdef PRINTER_AND_USB
	// USB ????g??
	//	PRINTER_PDL.USB._Init_Fun();
	// Delay1ms(100);
#endif

#ifdef RD_TYPE_SERIAL

	FlowFlag = s_PrintrConfig.m_BaudRate;

#ifdef SWITCH_BAUD
	ReadSwStatus();
#endif
	FlowFlag = 0x0A;
// ??'??????
#ifdef RD_USART1_EN
	PRINTER_PDL.RS232._Init_Fun(FlowFlag);
#endif
#ifdef RD_USART0_EN
	PRINTER_PDL.TTL._Init_Fun(FlowFlag);
#endif
#ifdef RD_USART3_EN
	PRINTER_PDL.TTL._Init_Fun(FlowFlag);
#endif

#endif

	PRINTER_PDL.ADC._GPIO_Init_Fun(4);
	//***************************************

	g_printChar = 0;

	//***************************************

	gTimer_Print = 60000;

	PRINTER_PDL.SYS._WDG_ReloadCounter(); // reload the watchdog ???????Z??????

	//	WRDI();

	g_PrinterDip = 3; // I??4??h????
	//	g_PrinterDip = 1;//I??2??h????

	g_cruvefalg = 0xAAAA;
	for (i = 0; i < 20; i++)
	{
		g_cruve_arry[i] = 0;
	}

	Erase_Sector = 0x300000; // ???????????????????'?

	g_DetPaperValueFlag = 0xA5EF;
	g_DetPV_Count = 0;

	//******************************************
	g_Receive_count = 0; // ?????????????
	Receive_count = 0;	 // ??????????
	g_Order_count = 0;	 // ???????J???

	g_SaveDataToFlashAddr = ADDRPRO_OR;
	g_GetDataFromFlashAddr = ADDRPRO_OR;
	g_SaveDataOffset = 0;
	g_GetDataOffset = 0;
	g_InterFaceBufferCount = 0;
	Order_Erase_Sector = 0x300000; // ??????????????????'?

	g_DumpReceiveFlage = 0;

	//	g_DumpReceiveFlage = 0xAAAA;

	g_BufferSwitch = 0;				  // ??'????????????????? setting BufferSwitch
	g_0D0AFlag = 0;					  // ??0d 0a???
	g_dotSwitch = 0;				  // ?????
	s_PrintOrderstr.m_DataFlag = 0;	  // ????????????
	s_PrintOrderstr.m_GetdataPos = 0; // ???????????????
	//=====?????????'??
	g_TMIndex = PRINTER_PDL.SYS._GetCurTemperatureLevel();		// mukesh - ADC_TEMP

	g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
	PrinterParameterInt();
	// ??????????????????????h??
	s_PrintParameters.m_ReceiveBuffrWor = s_PrintrConfig.m_ReceiveBuffrWor; // ??????????????

	s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;	 // ????????
	s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR; // ?????????
	s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;	 // ????????

	g_PaperRunToOut = 8000;

	g_PE_Standard = 300; // ????????AD????????

	// ???????????
	PRINTER_PDL.SYS._PeripheralVcc_EN();		// empty function call 
	PRINTER_PDL.SERVICE._PrinterIdle();			// empty function call 

	PRINTER_PDL.SYS._LED_Stauts_OFF();			// empty function call 

	PRINTER_PDL.SYS._WDG_ReloadCounter(); // reload the watchdog ???????Z??????
	//======?????????????=================================
	//	s_PrintParameters.m_ReceiveBuffrWor = RECEIVE_RAM;//RECEIVE_FLASH;

	//	if(s_PrintParameters.m_ReceiveBuffrWor == RECEIVE_FLASH)
	//	{
	//		PrinterCore = PrinterCore_Flash;//????????????
	//		p_OcAndGDAdr = &g_GetDataFromFlashAddr ;//g_Order_count
	//		p_REAndSAcount = &g_InterFaceBufferCount; //Receive_count
	//		p_SaveDrREcount = &g_SaveDataOffset;//?????????t???? g_Receive_count
	//	}
	//	else
	{	
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("*2-->BFR SR:%d & LA:%d & SP:0x%4X\r\n",g_BatteryLowChargeShutdownRequest,g_BatteryLabelActive,s_PrinterMOTOPar.m_StopPrint);
//g_BatteryLowChargeShutdownRequest = 1;
//g_BatteryLabelActive = 0;
//s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
#endif		
		PrinterCore = PrinterCore_RAM;
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
MyPrintf("*2-->AFT SR:%d & LA:%d & SP:0x%4X\r\n",g_BatteryLowChargeShutdownRequest,g_BatteryLabelActive,s_PrinterMOTOPar.m_StopPrint);
//g_BatteryLowChargeShutdownRequest = 1;
//g_BatteryLabelActive = 0;
//s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
#endif		
		p_OcAndGDAdr = &g_Order_count;		// ?????????buffer ???t???? g_GetDataFromFlashAddr
		p_REAndSAcount = &Receive_count;	// ????????J???g_InterFaceBufferCount
		p_SaveDrREcount = &g_Receive_count; // ?????????t???? g_SaveDataOffset
	}
	
#ifdef DISPLAY_FREEZE_ISSUE_SOLUTION	
    g_Display_freeze_issue_flag = 1;
#endif	
	
	g_TMIndex = PRINTER_PDL.SYS._GetCurTemperatureLevel();

	g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
	Delay1ms(50);

	g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
	Delay1ms(50);
	g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
	Delay1ms(50);
	g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
	Delay1ms(50);
	g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
	//		PRINTER_PDL.RS232._SendChar(0xA1);
	//		PRINTER_PDL.RS232._SendChar(g_PowerVol);
	//		PRINTER_PDL.RS232._SendChar(g_PowerVol>>8);

	//		s_PrintParameters.m_CurMotoRunTime = VolRUNTime[g_CurrentVol] - 100;//????j?????????????
	//		s_PrintParameters.m_PrHeadHottime = VolTHSTB[g_CurrentVol] - 100;//???????
	//		if(s_PrintrConfig.m_SoftwarePart == 2)
	//		{
	//			s_PrintParameters.m_PrHeadHottime = VolTHSTB_2[g_CurrentVol];//???????
	//		}
	s_PrintParameters.m_CurMotoPWM = VolRUNPWM[g_CurrentVol] - 100; // ????j??????PWM?
	s_PrintParameters.m_CurFreeTime = VolFreeTime[g_CurrentVol];	// ????j???????????????

#ifdef MY_DEBUG_ENABLE
	printf("g_pinrtStb:%d\r\n", g_pinrtStb);
	printf("g_CurrentVol:%d\r\n", g_CurrentVol);
	printf("g_TMIndex:%d\r\n", g_TMIndex);
#endif
	//======???????????=================================
	for (i = 0; i < BUFFER_SHORT_LENGTH; i++)
	{
		g_ListTable[i] = 0;		// clear the g_ListTable array
	}
	for (i = 0; i < 1000; i++)
		;
	g_ListSaveToFlashCount = 0;
	g_ListPrintFinish = 0;
	g_ListControlFor = 0;
	g_PrintListOrNorm = 0; // ?????????
	//=======================================
	g_SelfRunFlag = 0; // ?????????????

	g_HotLineTOPD = s_PrintrConfig.m_HotLineTOPD;
	g_HotLineTOOUT = s_PrintrConfig.m_HotLineTOOUT;

	// g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();

	g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();

	g_PaperWide = 57;

	g_PaperRunToOut = 8000; // s_PrintrConfig.m_LabelLength + (s_PrintrConfig.m_BarPaGap /2 );
	//=======================================

	//	while(1)
	//	{
	//		PRINTER_PDL.SYS._WDG_ReloadCounter();

	//		while(PRINTER_PDL.SYS._GetKey_LF() == 0)
	//		{
	//			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog ???????Z??????
	//

	//			{//?????????g?
	//					PRINTER_PDL.SYS._Drive_Power_EN();
	//					PRINTER_PDL.MOTO._MotoRun_RW(&g_MotoAll,500);
	//					PRINTER_PDL.SYS._Drive_Power_DIS();
	//			}
	//
	//
	//		}GetDETPaper
	//	g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
	//	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();
	//		Delay1ms(1000);
	//		MyPrintf("GetPaper:%d\r\n",GetPaper());
	//		MyPrintf("GetDETPaper:%d\r\n",GetDETPaper());
	//
	//	}
	for (i = 0; i < 32; i++)
	{
		Delay1ms(2);
		SetFreValue(PRINTER_PDL.SYS._PaperGapDet());
	}
	g_pinrtStb = CalStbTime();
	if (PRINTER_PDL.SYS._GetKey_LF() == 0)			// mukesh - When we press the Feed button after Calibration done message printer enter into the self-print mode and generate a self print 
	{
		Delay1ms(100);
		if (PRINTER_PDL.SYS._GetKey_LF() == 0)		// mukesh - Feed button kept pressed for 0.1 sec 
		{

#ifdef PRINTER_AND_USB
			PRINTER_PDL.USB._ReceiveNBytes = PUSB_Dump_ReceiveNBytes;
#endif

#ifdef RD_TYPE_PARALLEL
			PRINTER_PDL.PARALLEL._EXTInterruptIRQHandler = PARALLEL_DUMP_ReceiveNBytes;
#endif

			// ??????'??FLASH??,dunmpg?????????????
			//				MyPrintf("Star.....Selftest.....\r\n");
			// g_PaperType = 1;//???????????
			//	SysTick_Init();
			// g_PaperType = 1;//???????????
			PRINTER_PDL.BT._ReceiveNBytes = BT_ReceiveDumpNBytes;
			SetSelftestData();				// mukesh - self print after Calibration done message 
		}
	}

	g_optionCount = 0;
	
#ifdef DUMMY_FLAGS_SET_TO_CREATE_ISSUE
	MyPrintf("*1--> g_PwrVol:%d\r\n",g_PowerVol);
#endif	
	
	
#ifdef MY_DEBUG_ENABLE
	printf("Star.....3..\r\n");
#endif
	//=======================================
#ifdef PSF20_R002_TSPL_PROTOCOL
    /* Startup self-tests do not change the incoming language or callbacks. */
    PRINTER_PDL.BT._ReceiveNBytes = BT_ReceiveNBytes;
    PRINTER_PDL.USB._ReceiveNBytes = PUSB_ReceiveNBytes;
    g_DumpReceiveFlage = 0U;
    for (;;) {
        TsplCore_Service();
        TsplNative_Service();
        TsplFirmware_Service();
    }
#else // legacy flow
	while (1)
	{
#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
        BatteryLowCharge_Service();
#else
		BatteryLowCharge_ServiceVoltageSample();
		if (g_BatteryLowChargeShutdownRequest != 0)
			BatteryLowCharge_ExecuteShutdown();
#endif
#endif	
		
		/* Log the initial thermistor reading only when temperature-safe printing is compiled in. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
		
		adc_VAL = ADC_Quick_GetVoltage(ADC_CHANNEL_0,ADC_GPIO);
#ifndef TEMP_SET_ADC_VALUE_DEFAULT80_ISSUE		
		MyPrintf("TMP:%d\r\n",adc_VAL);
#endif		
		m_ThermalTemp = Read_Thermistor_Temperature(adc_VAL);
	//	MyPrintf("Initial ADC: %d\tInitial TEMP: %d.%d C\r\n",adc_VAL,m_ThermalTemp / 1,m_ThermalTemp % 10);
#endif
		
	ProcessStart:
		__SETPRIMASK();			// mukesh - disable interrupts 
		g_PrintStatusReg = PRASE_PROCESS;
		__RESETPRIMASK();		// mukesh - enable interrupts
		if (g_BufferSwitch != 0)		// mukesh - this is set as '0' by default
			p_ListPrease = &s_ListParmeter_B;
		else
			p_ListPrease = &s_ListParmeter_A;
		HTCount = 0;					// mukesh - count of Horizontal Tabs 
		p_ListPrease->m_PrintLineSpace = s_PrintParameters.m_LineSpace;
		p_ListPrease->m_HotTime = s_PrintParameters.m_PrHeadHottime; // ???????l??????
		p_ListPrease->m_PrintDir = s_PrintParameters.m_PrintDir;	 // ??????k???
		p_ListPrease->m_PrintLineHigh = 8;							 // I??h??????8;

		p_ListPrease->m_PrinterDip = g_PrinterDip;
		p_ListPrease->m_LableStopFlag = 0;
		p_ListPrease->m_ThType = 0;
		if (s_PrintParameters.m_boldface == VALUE_ENABLE)			// mukesh - If bold printing is required 
		{
			p_ListPrease->m_ThType = 1;
		}

		p_ListPrease->m_MotoRunTime = g_SpeedChoice;	   // ?????????????? mukesh - set the current printing speed here 
		p_ListDataDot = &(p_ListPrease->m_ListBuffer[0]);  // ????????????????
		p_IndexListData = &(p_ListPrease->m_ListIndex[0]); // ????????????????
		if (p_ListPrease->m_PrintDir != PRINT_DIRECT_NOR)
		{
			*p_IndexListData++ = LIST_END; // ???????????????'????????????i
		}
		p_ListPrease->m_MotoRunDir = MOTO_CLOCKWISE; // ?????????? mukesh - set the motor direction 
		p_ListPrease->m_LableFeedFlag = 0;
		p_ListPrease->m_LeftFreeDot = s_PrintParameters.m_Liftpos;		// mukesh - set as '0' 
		g_CalcPrintPos = s_PrintParameters.m_Liftpos; // ??????????'???	// mukesh - set as '0'
		g_SavePrintPos = g_CalcPrintPos;
		m_SaverightPos = s_PrintParameters.m_Righttpos; // ??????	mukesh - set as '384'
		g_alignType = s_PrintParameters.m_alignType;	// ???????		mukesh - set as '0' 
		for (i = 0; i < BUFFER_SHORT_LENGTH; i++)		// mukesh - BUFFER_SHORT_LENGTH -> 24
		{
			p_ListPrease->m_LineProArry[i] = g_ListTable[i];		// mukesh - sets the m_LineProArry to '0'
		}
		__SETPRIMASK();			// mukesh - disable interrupts 
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();				// detects presense of media : paper/label
		__RESETPRIMASK();		// mukesh - enable interrupts 
		PrinterLPS();
    
		if (g_PaperType == 0)	// mukesh - g_PaperType = 0 -> Label Media 
		{// mukesh - The purpose of this section is to calculate the left and right unused printable areas when label media is being used.

			p_ListPrease->m_LeftFreeDot = 0;
			p_ListPrease->m_RightFreeDot = 0;
			if (g_PaperWide > 48)
			{
				if (g_HorShift & 0xF00)
				{
					// ?????z??????????????????
					p_ListPrease->m_LeftFreeDot += ((g_HorShift & 0xFF) * 4);
				}
			}
			else
			{
				p_ListPrease->m_RightFreeDot = (48 - g_PaperWide) * 8; // ?????????

				if (g_HorShift & 0xF00)
				{
					// ?????????,?????????
					if (p_ListPrease->m_LeftFreeDot > ((g_HorShift & 0xFF) * 4))
					{
						p_ListPrease->m_LeftFreeDot -= ((g_HorShift & 0xFF) * 4);
					}
					else
					{
						p_ListPrease->m_LeftFreeDot = 0;
					}
				}
				else
				{
					// ?????z??????????????????
					p_ListPrease->m_LeftFreeDot += ((g_HorShift & 0xFF) * 4);

					if (p_ListPrease->m_RightFreeDot > ((g_HorShift & 0xFF) * 4))
					{
						p_ListPrease->m_RightFreeDot -= ((g_HorShift & 0xFF) * 4);
					}
					else
					{
						p_ListPrease->m_RightFreeDot = 0;
					}

					// g_PaperWide -= g_HorShift;
					// i = g_PaperWide * 8 - (g_HorShift & 0xFF) * 4;
					// print_head_length = i;//??????????
				}
			}
		} // Label Media Handling ends here 

	ProcessLoop:
#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
        BatteryLowCharge_Service();
#else
		BatteryLowCharge_ServiceVoltageSample();
		if (g_BatteryLowChargeShutdownRequest != 0)
			BatteryLowCharge_ExecuteShutdown();
#endif
#endif		
		
/* Update thermal state and request a safe pause before parsing more print data. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
		Thermal_Head_Protection_Check();
#endif
		/* Periodically update temperature and battery icons (every 500ms) to prevent inner-loop OLED lag */
		{
			static uint32_t last_disp_time = 0;
			if ((g_sysTime - last_disp_time) >= 500)
			{
				last_disp_time = g_sysTime;
				/* Keep the OLED temperature readout synchronized with the protected print state. */
				#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
				DisplayTemp(DIS_TEMP_X, DIS_TEMP_Y);
				#endif
				DisplayBatty(DIS_BAT_X, DIS_BAT_Y);
			}
		}
		
		// mukesh - logging logic -> log on Feed Button press
		/*
		if (PRINTER_PDL.SYS._GetKey_LF() == 0)
		{
			unsigned short m_ThermalTemp;
			unsigned short adc_VAL;
			adc_VAL = ADC_Quick_GetVoltage(ADC_CHANNEL_0,ADC_GPIO);
			m_ThermalTemp = Read_Thermistor_Temperature(adc_VAL);
			PRINTER_PDL.SYS._WDG_ReloadCounter();
			MyPrintf("ADC: %d\tTEMP: %d.%d C\r\n",
											adc_VAL,
											m_ThermalTemp / 10,
											m_ThermalTemp % 10);
			while (PRINTER_PDL.SYS._GetKey_LF() == 0)		// mukesh - This is a loop if the Feed Button Key gets stuck this handles it 
			{
				PRINTER_PDL.SYS._WDG_ReloadCounter();
			}
		}
		*/
		
		if (p_ListPrease->m_PrintDir != PRINT_DIRECT_NOR)		// mukesh - it is set to NOR thus false 
		{
			if (s_PrintParameters.m_AlignUPDown == 0)
			{
				m_UPDown = 0x8000;		// mukesh - Used as a Flag
			}
			else
			{
				m_UPDown = 0;
			}
		}
		else		// mukesh - Handling of PRINT_DIREC_NOR
		{
			m_UPDown = s_PrintParameters.m_AlignUPDown;
		}
		// m_OrderSave = g_Order_count;
		m_OrderSave = (*p_OcAndGDAdr);		// mukesh - read and store the value stored at p_OcAndGDAdr to m_OrderSave
		// ???? unicode??J???
		ProUnicodeChar();
		m_GetData = PrinterCore(); // ?????????
#ifdef PRINTER_PROTOCOL_9A9A
		if ((m_GetData == 0x9A) || (m_GetData == 0x9D))
		{
			m_GetData = 0;
		}
#endif
		if (g_DumpReceiveFlage == 0xAAAA)		// mukesh - dump data 
		{
			if (m_GetData == 0x20)
			{
				if (g_SavePrintPos == g_CalcPrintPos)
				{
					m_GetData = 0;
				}
			}
		}
	Process_In_Loop:
		m_ProcessFlag = PROCESS_CONTINUE; // I????????????
		if (m_GetData > 0x1F)
		{
			m_ProcessStatus = __PrcessCharCh();	// mukesh - all the printable bytes such as text, space, digits, punctuations, symbols etc are send to this ;
		}
		else
		{
			switch (m_GetData)
			{
			// case 0x03:{ m_ProcessStatus = __03_Order(); }break; //???
			case 0x09:		// mukesh - Horizontal Tab(HT)
			{
				m_ProcessStatus = __HT_Order();
			}
			break; // ?????
			case 0x0A:		// mukesh - Line Feed (LF)
			{
				m_ProcessStatus = __OAOD_Order();	
			}
			break; // ???????
			case 0x0D:		// mukesh - Carriage Feed (CR)
			{
				m_ProcessStatus = __OAOD_Order();
			}
			break; // ???????
			case 0x0E:		// mukesh - Shift Out (SO)
			{
				m_ProcessStatus = __SO_Order();
			}
			break; // ?????
			case 0x0C:		// mukesh - Form Feed (FF)
			{
				m_ProcessStatus = __FF_Order();
			}
			break; // ???
			case 0x10:		// mukesh - Data Line Escape (DLE)
			{
				m_ProcessStatus = __DC0_Order();
			}
			break; //
			case 0x12:		// mukesh - Device Control 2 (DC2)
			{
				m_ProcessStatus = __DC2_Order();
			}
			break; //
			case 0x14:		// mukesh - Device Control 4 (DC4) 
			{
				m_ProcessStatus = __DC4_Order();
			}
			break; // ???????
			case 0x18:		// mukesh - CANCEL (CAN) 
			{
				m_ProcessStatus = __18_Order();
			}
			break; // ???h??
			case 0x1B:		// mukesh - Escape (ESC)
			{
				m_ProcessStatus = __ESC_Order();
			}
			break; // ????ESC???
			case 0x1C:		// mukesh - File Separator (FS)
			{
				m_ProcessStatus = __FS_Order();
			}
			break; // ????FS???
			case 0x1D:		// mukesh - Group Separator (GS)
			{
				m_ProcessStatus = __GS_Order();
			}
			break; // ????GS???
			case 0x1F:		// mukesh - Unit Separator (US)
			{
				m_ProcessStatus = __US_Order();
			}
			break; // ????US???
			default:
				m_ProcessStatus = __OrderReceive();
			}
		}
		if (m_ProcessStatus == PROCESS_START)
		{
			goto ProcessStart;
		}
		else if (m_ProcessStatus == PROCESS_IN_LOOP)
		{
			goto Process_In_Loop;
		}
		else if (m_ProcessStatus == PROCESS_LOOP)
		{
			goto ProcessLoop;
		}
		else if (m_ProcessStatus == PROCESS_CONTINUE)
		{
			goto ProcessLoop;
		}
		else if (m_ProcessStatus == PROCESS_FINISH)
		{
			//=========??????????????=================================

			//			if((g_THDeep & 0x8000) == 0)
			//			{
			//				p_ListPrease->m_HotTime = s_PrintParameters.m_PrHeadHottime + g_THDeep;//???????l??????
			//			}
			//			else
			//			{
			//				p_ListPrease->m_HotTime = s_PrintParameters.m_PrHeadHottime - (g_THDeep & 0x7FFF);//???????l??????
			//			}

			//		if(g_PaperType == 0)
			//		{
			//
			//			p_ListPrease->m_HotTime = p_ListPrease->m_HotTime * 2;
			//
			//		}
			//

			p_ListPrease->m_RightFreeDot = print_head_length - g_CalcPrintPos;

			if (g_alignType == 1) // ????
			{
				i = p_ListPrease->m_RightFreeDot + p_ListPrease->m_LeftFreeDot;
				p_ListPrease->m_RightFreeDot = i >> 1;
				p_ListPrease->m_LeftFreeDot = i - p_ListPrease->m_RightFreeDot;
			}
			if (g_alignType == 2) // ????
			{
				p_ListPrease->m_LeftFreeDot = p_ListPrease->m_RightFreeDot + p_ListPrease->m_LeftFreeDot;
				p_ListPrease->m_RightFreeDot = 0;
			}

			if (p_ListPrease->m_PrintDir == PRINT_DIRECT_NOR)
			{
				p_ListPrease->m_PrintStartPos = 0; // ??????????0??'
				*p_IndexListData++ = LIST_END;	   // ???????
			}
			else
			{
				p_ListPrease->m_PrintStartPos = p_IndexListData - p_ListPrease->m_ListIndex - 1; // ????'???,?p_IndexListData++ ???????h
			}
			p_ListPrease->m_FeedTime = 0;
#ifdef TM_ENABLE

			__SETPRIMASK();
			g_TMIndex = PRINTER_PDL.SYS._GetCurTemperatureLevel();
			__RESETPRIMASK();

			s_PrintParameters.m_VolTemAdd = 130; // ?9%
			s_PrintParameters.m_VolTemSubb = 90; // ?6%

			s_PrintParameters.m_VolTemAdd = (s_PrintParameters.m_PrHeadHottime * 9) / 100;
			s_PrintParameters.m_VolTemSubb = (s_PrintParameters.m_PrHeadHottime * 6) / 100;

			p_ListPrease->m_TMIndex = g_TMIndex; // PRINTER_PDL.SYS._GetCurTemperatureLevel();//????j????

			if (p_ListPrease->m_TMIndex > 9)
			{
				p_ListPrease->m_HotTime += (p_ListPrease->m_TMIndex - 9) * s_PrintParameters.m_VolTemAdd * 2; // ???????
			}
			else
			{
				p_ListPrease->m_FeedTime = (9 - (p_ListPrease->m_TMIndex)) * s_PrintParameters.m_VolTemSubb; // ???????
				p_ListPrease->m_HotTime -= p_ListPrease->m_FeedTime;
			}

#endif

			if (g_PrintListOrNorm == 0xAA55)
			{
				p_ListPrease->m_PrintLineSpace = 0;
			}

			//=============================================================
			//---?????????????
			// ???????????????????????????????????
			// ???????????????????????????????
			// ????????????????????????????
			g_CurveCountFlag = 0; // ?????????????
#ifdef PRINTER_AND_USB
			returnstatus(0);

#endif
			__SETPRIMASK();
			g_PrintStatusReg = FINISH_PROCESS;
			__RESETPRIMASK();
#ifdef RD_OEMPRINTER
			SET_H_POWER; // ?????????
#endif

#ifdef RD_INDEP_CONTROL

			SET_H_POWER; // ?????????

#endif

#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
            if (g_BatteryLabelFinalQueued)
            {
                while (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)
                {
                    SaveDataToFlash();
                    PRINTER_PDL.SYS._WDG_ReloadCounter();
                    if (g_BatteryLabelActive)
                        BatteryLowCharge_LabelMediaPresent();
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
                    Thermal_Head_Protection_Check();
#endif
                }
                BatteryLowCharge_LabelFinished();
                BatteryLowCharge_Service();
            }
#endif

			if (s_PrinterMOTOPar.m_PrintEnd == DISABLE_VAR) // ???????????????
			{
			/*
			 * Finish the final raster block of a label before a thermal pause can
			 * take ownership of the motor and print-head strobes.
			 */
			#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
				/* The final raster block belongs to this label. Do not consume
				 * the next GS v 0 header until it has physically completed. */
				if (g_ThermalRasterFinalChunkQueued != 0)
				{
					while (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)
					{
						SaveDataToFlash();
						Thermal_Head_Protection_Check();
					}

					if (g_ThermalBoundaryPause != 0)
					{
						Thermal_Head_Protection_Check();
					}
					g_ThermalRasterFinalChunkQueued = 0;
					g_ThermalRasterLabelActive = 0;
					g_ThermalRasterFinalBuffer = 0;
					goto ProcessStart;
				}

				/* Original print flow after a normal buffer completes. */
				while (s_PrinterMOTOPar.m_MutualFlag != ENABLE_VAR)
				{
					SaveDataToFlash();
					Thermal_Head_Protection_Check();
				}
				s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;
			#else
				/*
				 * Legacy print flow: wait for the normal producer/consumer handshake.
				 * No thermistor is sampled and no thermal pause can be requested here.
				 */
				// ?????j?????????????????
				while (s_PrinterMOTOPar.m_MutualFlag != ENABLE_VAR) // ?????????????
				{
					SaveDataToFlash();
				}
				s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR; // ?????????
			#endif
			}
			else
			{
			/* While the normal print engine is active, continuously service thermal protection. */
			#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
				// ????????
				// ???????'???
				while (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR) //
				{
					SaveDataToFlash();
					Thermal_Head_Protection_Check();
				}
				if (g_PrintStatusReg == FINISH_PROCESS)
				{
					Thermal_Head_Protection_Check();
					s_PrinterMOTOPar.m_In_printing = ENABLE_VAR;
					s_PrinterMOTOPar.m_InFirstPrint = ENABLE_VAR;
#ifdef BATTERY_INDICATION_SOLUTION
					BatteryBar_PrintStarted();
#endif					

					g_RunFlag = 0;
					//--------------????????????
					// TIM_ENABLE(TIM_Run,10);//??'??h?e?ARR//???????????1

					PRINTER_PDL.SYS._SystemPrintIRQ_EN(10);
					s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;		// ?????????
					while (s_PrinterMOTOPar.m_MutualFlag != ENABLE_VAR) // ?????????????
					{
						SaveDataToFlash();
					}
					s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR; // ?????????
				}
			#else
				/*
				 * Legacy print flow: wait only for the existing print-complete signal,
				 * then restart the print IRQ using the original scheduling sequence.
				 */
				while (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)
				{
					SaveDataToFlash();
				}
				if (g_PrintStatusReg == FINISH_PROCESS)
				{
					s_PrinterMOTOPar.m_In_printing = ENABLE_VAR;
					s_PrinterMOTOPar.m_InFirstPrint = ENABLE_VAR;

					g_RunFlag = 0;
					PRINTER_PDL.SYS._SystemPrintIRQ_EN(10);
					s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;
					while (s_PrinterMOTOPar.m_MutualFlag != ENABLE_VAR)
					{
						SaveDataToFlash();
					}
					s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;
				}
			#endif
			}
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
            /* An idle engine may have started the final (or only) buffer
             * above. Wait here too before parsing the next label header. */
            if (g_BatteryLabelFinalQueued)
            {
                while (s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)
                {
                    SaveDataToFlash();
                    PRINTER_PDL.SYS._WDG_ReloadCounter();
                    if (g_BatteryLabelActive)
                        BatteryLowCharge_LabelMediaPresent();
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
                    Thermal_Head_Protection_Check();
#endif
                }
                BatteryLowCharge_LabelFinished();
                BatteryLowCharge_Service();
            }
#endif			
			
			goto ProcessStart;
		}
		else
		{
			goto ProcessLoop;
		}
	} //* End
#endif
}


//2·ÖÇø
const unsigned short Par_STB_1[]={0xFFFF};
const unsigned short Par_STB_2[]={0xAAAA,0x5555};
const unsigned short Par_STB_4[]={0x1111,0x2222,0x4444,0x8888};
const unsigned short Par_STB_6[]={0x1041,0x2082,0x4104,0x8208,0x0410,0x0820};
const unsigned short Par_STB_8[]={0x0101,0x0202,0x0404,0x0808,0x1010,0x2020,0x4040,0x8080};
const unsigned short Par_STB_10[]={0x0401,0x0802,0x1004,0x2008,0x4010,0x8020,0x0040,0x0080,0x0100,0x0200}  ;
const unsigned short Par_STB_12[]={0x1001,0x2002,0x4004,0x8008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800};
const unsigned short Par_STB_16[]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};


/*
unsigned short VolDetStand[]= {112,120,128,136,145,150,159,167,175,182,190,197,205,212,220,228,235,243,250,300};
*/



const unsigned short g_RunFlagArry[]={
0,
0xAA00,
0xAA00,
0,
0xAA01,//4
0,
0xAA02,//6
0,
0xAA03,//8
0,
0xAA04,//10
0,
0xAA05,//12
0,
0,
0,
0xAA07,//16
};
const unsigned short g_ForArry[]={
1,
1,
1,
1,
2,//4
0,
3,//6
0,
4,//8
0,
5,//10
0,
6,
0,
0,
0,
8,
};
const unsigned short *  bySTBExp[] = {
0,
Par_STB_1,
Par_STB_2,
0,
Par_STB_4,//4
0,
Par_STB_6,//6
0,
Par_STB_8,//8
0,
Par_STB_10,//10
0,
Par_STB_12,//12
0,
0,
0,
Par_STB_16,//16

};


/*
Ð¡ÓÚ90 2·ÖÇø
´óÓÚ90 Ð¡ÓÚ 180 4·ÖÇø
´óÓÚ 180 Ð¡ÓÚ 270  6·ÖÇø
´óÓÚ270 8·ÖÇø

384/2 = 192 C0
384/4 = 192 60
384/6 = 192 40
384/8 = 192 30
384/10 = 192 27
384/12 = 192 20
384/16 = 192 18



*/






unsigned char GetPaerStb(unsigned short BitCount)
{
	if(BitCount == 0)
	{
		return 0;
	}
	else if(BitCount < (g_ParTypeValue*2 + 1))
	{
		return 2;//2·ÖÇø
	}
	else if(BitCount < (g_ParTypeValue*4 + 1))
	{
		return 4;//
	}
	else if(BitCount < (g_ParTypeValue*6 + 1))
	{
		return 6;
	}
	else if(BitCount < (g_ParTypeValue*8 + 1))
	{
		return 8;
	}
	else if(BitCount < (g_ParTypeValue*10 + 1))
	{
		return 10;
	}
	else if(BitCount < (g_ParTypeValue*12 + 1))
	{
		return 12;
	}
	else
	{
		return 16;
	}

}













unsigned short g_RunCuStep,g_runLstep;//µ±Ç°ÔËÐÐµÄ²½Êý
const unsigned short * p_GetParSTB;//
/****************************************************************************
* º¯ÊýÃû£ºTimer0ControlMoto
* ¹¦  ÄÜ£º¿ØÖÆµç»ú×ß²½£¬¿ØÖÆ´òÓ¡ÐÐÊý
*
* Ëµ  Ã÷£ºÇÐ»»´òÓ¡»º³åÇø£¬´¦Àí´òÓ¡Á´±íÍ·£¬¿ØÖÆ²½½øµç»ú×ß²½
*			Moto_Step µç»ú×ß²½¼ÆÊýÆ÷£¬ÔÚÍâ±ß¸³Öµ


void KeyRead( void )
{
    unsigned char ReadData = PINB^0xff;// 1
    Trg = ReadData & (ReadData ^ Cont);// 2
    Cont = ReadData;// 3


****************************************************************************/
unsigned short g_SpeedEnstu;
unsigned short g_lindotnu = 0;
unsigned short g_SpaceStatus;
unsigned short g_falg;
unsigned short g_CJCalfalg=0;
unsigned short g_FeedStepCount = 0;

extern unsigned short	g_Lab_head_Feed;
unsigned short g_GapProEn=0;
StbProDataCon gStbProDataCon;

unsigned short g_EnstbFlag = 0;

/**
* @FuncName  ProcessMotoLineData
* @param     void
* @retval    void
* @LocalVar  temp, i, h, a
* @GlobalVar s_PrinterMOTOPar, p_ListParmeters, g_SendFlag, g_DetPaperValueFlag,
*            g_DetPV_Count, g_CJCalfalg, g_SpaceStatus, g_GapProEn, s_PrintrConfig,
*            g_PaperRunToOut, g_HotLineTOPD, g_HotLineTOOUT, g_DetPaperGapSpace,
*            g_PrintStatusReg, g_BufferSwitch, s_ListParmeter_A, s_ListParmeter_B,
*            g_PrintLineCount, g_MotoAll, SendBufferToSPI, SaveToBuffer, g_StandbyPar,
*            g_lineBitCount, g_lindotnu, g_RunLTime, g_pinrtStb, gStbProDataCon,
*            g_Speed, g_FeedStepCount, PRINTER_PDL
* @Description Master Stepper Motor & Print Head Line-Printing Interrupt Handler.
*              Synchronizes paper feeding with thermal print head element heating per printed line.
*
*/
void ProcessMotoLineData(void)
{
	unsigned short temp,i,h;

	double a;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	PRINTER_PDL.SYS._Drive_Power_Trig();

//	if(g_EnstbFlag == 0xA123)
//	{
//		
//			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
//			return;		
//	}
//	
	
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		if(g_SendFlag == DATA_IN_SEND)
		{
			//µÈ´ýÒ»ÐÐÊý¾Ý´¦ÀíÍê³É	
// Wait until one line of data processing completes
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
			return;
		}
		/* Do not pause between DMA/latch/strobe phases; the current dot line must finish first. */
#if defined(PSF20_R002_TEMPERATURE_SAFE_PRINTING) || defined(BATTERY_LOW_CHARGE_LABEL_SHUTDOWN)
		// mukesh - updated_23_09: do not accept a thermal STOP until the previous dot line's DMA/latch/strobe sequence has finished.
		if (g_EnstbFlag == 0xA123)
		{
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
			return;
		}

#endif
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
		g_ThermalPrintTrack.unit_in_transmission = 0;		// mukesh - indicates no data is being printing 
#endif
	}
// =============================================================================
		if(g_DetPaperValueFlag == 0xA5EF)
		{
			if(g_DetPV_Count < 2)
			{
				g_DetPV_Count++;
			}
		}	
// =============================================================================
		if(g_CJCalfalg == 0xA565)
		{
			linearRegression_0_30();
			g_CJCalfalg = 0;
		}
		if((s_PrinterMOTOPar.m_MotoStep & 1) == 0)
		{
			SetFreValue(GetDETPaper());//ÉèÖÃ²ÉÑù¹âµçÖµ	// Set printhead strobe voltage
			if(g_DetPV_Count > 1 )
			{
				g_CJCalfalg = 0xA565;
				linearRegression_32(&a);
//				if(g_GapProEn == 0xA537)
//				{
//					a = 0;
//				}
				if(a > s_PrintrConfig.m_PE_LabValue_H)
				{
					g_SpaceStatus |= 1;
					g_GapProEn = 0xA537;//±íÊ¾¼ì²âµ½Ö½·ì
				}
				else
				{
					g_SpaceStatus = 0xA500;
					
				}
			//	printf("%2.3f ",a);
			}				
		}		
	#ifdef PRINTER_LABEL		
		if((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
		{


			//±êÇ©´òÓ¡
			//ÔÚÃ¿Ò»²½µÄ¿ªÍ·¼ÆËã
			if(g_PaperRunToOut)
			{
				g_PaperRunToOut--;
			}

			if(g_SpaceStatus == 0xA501)
			{
				//¼ì²âµ½±êÇ©µÄÆðÊ¼´òÓ¡Î»ÖÃ

				g_falg = 0x0303;

				g_SpaceStatus = 0;//Çå¿Õ¼à²â×´Ì¬
				g_DetPaperGapSpace = 0xaaa;//¼ì²âµ½±êÇ©Í·
				//¼à²âµ½Ö½·ìºóÍùÇ°Ëù×ßµÄ²½Êý
				//Ö»×ßµ½¼ÓÈÈÏß´¦£¬³öÖ½²¿·ÖÓÉ³¬Ê±¿ØÖÆ

				//g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8;//  - 8- 4;//¼à²âµ½Ö½·ìÖØÐÂÐÞÕý×ßµ½¼ÓÈÈÏßµÄ²½Êý¡¢
				g_PaperRunToOut = g_HotLineTOPD * 8;//  - 8- 4;//¼à²âµ½Ö½·ìÖØÐÂÐÞÕý×ßµ½¼ÓÈÈÏßµÄ²½Êý¡¢
				if(p_ListParmeters->m_LableStopFlag == 0xA5A1)
				{
					if((*p_REAndSAcount) < 5)
					{
						//g_PaperRunToOut +=  s_PrintrConfig.m_HotLineTOOUT * 8;						
						g_PaperRunToOut +=  g_HotLineTOOUT * 8;						
					}
					
				}
				//if(g_Lab_head_Feed == 0x5523)
//				{
//					if((*p_REAndSAcount) < 5)
//					{
//						//Ã»ÓÐ½ÓÊÕµ½Êý¾Ý
//						g_PaperRunToOut +=  s_PrintrConfig.m_HotLineTOOUT * 8;
//						g_Lab_head_Feed = 0xA537;
//					}
//				}
			}


			if(g_PaperRunToOut == 0)
			{
				if(g_GapProEn == 0xA537)
				{
					g_GapProEn = 0;//¼ì²âµ½Ö½·ìÇÒÔËÐÐµ½0£¬ÇåµôÖ½·ì¼ì²â
				}
				//¼ì²âµ½Ö½·ì²¢ÇÒ×ßµ½³öÖ½Î»ÖÃ
				if(g_DetPaperGapSpace == 0xAAA)
				{
					if(p_ListParmeters->m_LableStopFlag == 0xA5A1)
					{
						//×ßµ½0µã£¬ÉèÖÃ³¤¶ÈÎªÖµµÄ³¤¶È+Ö½·ìµÄ³¤¶È
						g_PaperRunToOut = 8000;//8000²½ 1000mm ÉèÖÃÄ¬ÈÏ×ß1Ã×³¤¶È

						if(g_PrintStatusReg == FINISH_PROCESS)
						{
							goto first_Pro_printDataList;
						}
						else
						{
							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//´òÓ¡»úÍ£»ú;
							p_ListParmeters->m_LableStopFlag = 0;
						}
					}
					else
					{
//						if(g_SelfRunFlag != 0xAA)
//						{
//							g_RunToGapFlag = 0xA55A;
//							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//´òÓ¡»úÍ£»ú;
//							p_ListParmeters->m_LableStopFlag = 0;
//						}
					}
					
					
					
					//ÖØÐÂ´ÓÒ»¸ö±íµ¥¿ªÊ¼´òÓ¡¼ÇÊý
					g_DetPaperGapSpace = 0;


				}
			}
	
		}

	#endif
	
	
	
	
//	g_MotoSTBStatus = 0;
	if (s_PrinterMOTOPar.m_In_printing == ENABLE_VAR)//µÈÓÚENABLE_VAR ±íÊ¾µÚÒ»´Î½øÈë´òÓ¡³ÌÐòÐèÒªÔ¤´¦ÀíºÜ¶à¶«Î÷
	{

first_Pro_printDataList:
		//µÃµ½´òÓ¡Êý¾ÝÁ´±í
		if(g_PrintStatusReg != FINISH_PROCESS)
		{
			goto Pro_printDataList_EX;
		}
		/* Pause at a dot-line boundary only when no raster label is being protected as one unit. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
		if ((g_ThermalState == THERMAL_STATE_STOP) && (g_ThermalRasterLabelActive == 0))
		{
			g_ThermalPrintTrack.current_dot_line = g_PrintLineCount;
			g_ThermalPrintTrack.next_dot_line = 0;
			g_ThermalPrintTrack.unit_in_transmission = 0;
			g_ThermalPrintTrack.thermal_pause_active = 1;

			PRINTER_PDL.SYS._STB_ALL_CLEAR();
			// mukesh - updated_23_09: keep the active motor phase energized so paper cannot move during thermal cooling.
			s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
			PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
			return;
		}
#endif
		
		g_PrintStatusReg = 0;
		g_BufferSwitch = ~g_BufferSwitch;//È¡·´´òÓ¡Á´±íÇÐ»»±êÖ¾
		if(g_BufferSwitch != 0)
		{
			p_ListParmeters = &s_ListParmeter_A;
		}
		else
		{
			p_ListParmeters = &s_ListParmeter_B;
		}

		s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//ÊÖ¶¯±êÖ¾Ê¹ÄÜ
		g_PrintLineCount = 0; //´òÓ¡µÚÒ»ÐÐµÄÊý¾Ý
		s_PrinterMOTOPar.m_PrintLSbit_f = DISABLE_VAR;//ÐÐ¼ä¾à±êÖ¾ÇåÁã
		s_PrinterMOTOPar.m_PrintLSbit = DISABLE_VAR;//ÎÞÐÐ¼ä¾à ±êÖ¾
		s_PrinterMOTOPar.m_MotoRun = ENABLE_VAR;//µç»úÔËÐÐ±êÖ¾Ê¹ÄÜ
		s_PrinterMOTOPar.m_StopPrint = DISABLE_VAR;//Í£Ö¹´òÓ¡½ûÖ¹
		s_PrinterMOTOPar.m_PrintEnd = DISABLE_VAR;//´òÓ¡½áÊø½ûÖ¹
		s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//µÈÓÚ DISABLE_VAR ±íÊ¾´òÓ¡»úÕýÔÚ´òÓ¡
		if(s_PrinterMOTOPar.m_InFirstPrint == ENABLE_VAR)
		{
			//µÚÒ»´Î½øÈë´òÓ¡Ô¤´¦Àí
			s_PrinterMOTOPar.m_InFirstPrint = DISABLE_VAR;//Çå³ýµÚÒ»´Î´òÓ¡±êÖ¾
			s_PrinterMOTOPar.m_MotoStep = 0;//¿ªÊ¼´òÓ¡motoÔËÐÐ²½ÊýÇå¿Õ
		//	TIM_ENABLE(TIMx,20000);//20msÆô¶¯Ê±Ðò
		//	TIM_ENABLE(TIM3,20);//Æô¶¯¶¨Ê±Æ÷3´¦ÀíÒ»ÐÐÊý¾Ý
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(20000);
			PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],s_PrintParameters.m_CurMotoPWM);
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			
			return;
		}
		//TIM_ENABLE(TIM3,10);//Æô¶¯¶¨Ê±Æ÷3´¦ÀíÒ»ÐÐÊý¾Ý
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
		goto timer_run;
	}
Pro_printDataList_EX:
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		/* Apply the same safe stop rule while advancing the motor through the next dot line. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
		if ((g_ThermalState == THERMAL_STATE_STOP) && (g_ThermalRasterLabelActive == 0))
		{
			g_ThermalPrintTrack.current_dot_line = g_PrintLineCount;
			g_ThermalPrintTrack.next_dot_line = g_PrintLineCount;
			g_ThermalPrintTrack.unit_in_transmission = 0;
			g_ThermalPrintTrack.thermal_pause_active = 1;

			PRINTER_PDL.SYS._STB_ALL_CLEAR();
			// mukesh - updated_23_09: keep the active motor phase energized so paper cannot move during thermal cooling.
			s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
			PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
			return;
		}
#endif
		if(g_QR_enable == 0xA123)
		{
			g_QR_SpaceCount++;
		}
		if(g_BarCode_enable == 0xA123)
		{
			g_BarCode_SpaceCount++;
		}
		//*********************************************
		//ÅÐ¶ÏÍ£Ö¹´òÓ¡
		if(s_PrinterMOTOPar.m_PrintEnd == ENABLE_VAR)
		{
			
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
            if (BatteryLowCharge_LabelFinalBufferMatches((p_ListParmeters == &s_ListParmeter_A) ? 1 : 2))
            {
                if (g_PrintLineCount >= p_ListParmeters->m_PrintLineHigh)
                    BatteryLowCharge_LabelStopped((p_ListParmeters == &s_ListParmeter_A) ? 1 : 2);
                else
                    g_BatteryLabelComplete = 2; /* Early stop: foreground must discard this sample. */
            }
#endif			
			
			/* If a hot head reaches a label boundary, stop only after its last line has completed. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
			/* The final line of one GS v 0 label has completed: g_EnstbFlag
			 * was observed clear at the top of this interrupt. If thermal stop
			 * is pending, disable motor output and cool at this boundary. */
			if ((g_ThermalRasterFinalChunkQueued != 0) &&
				(g_ThermalRasterFinalBuffer == ((p_ListParmeters == &s_ListParmeter_A) ? 1 : 2)) &&
				(g_ThermalStopPending != 0))
			{
				g_ThermalPrintTrack.current_dot_line = g_PrintLineCount;
				g_ThermalPrintTrack.next_dot_line = 0;
				g_ThermalPrintTrack.unit_in_transmission = 0;
				g_ThermalPrintTrack.thermal_pause_active = 1;
				g_ThermalBoundaryPause = 1;
				PRINTER_PDL.SYS._STB_ALL_CLEAR();
				/* Do not lower O_SWITCH here: on this board it turns the printer
				 * off. Clear only motor PWM/coil outputs during cooling. */
				PRINTER_PDL.MOTO._MotoDisable();
				s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;
				PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
				return;
			}
#endif
			s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//;;;;;Ã»ÓÐ¿ÉÒÔ´òÓ¡µÄÊý¾Ý
			//Çå³ý¼ÓÈÈÏßºÍµç»úÇý¶¯ÐÅºÅÏß
		//	SET_L_STB1;SET_L_STB2;//Çå¼ÓÈÈÏß
				PRINTER_PDL.SYS._STB_ALL_CLEAR();
		//	ClearMoto();
				PRINTER_PDL.MOTO._MotoDisable();
			s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//·­×ª
			s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;//´òÓ¡ÒÑ¾­Í£Ö¹
			//TIM_DISABLE(TIMx);//¹Ø±Õ¶¨Ê±Æ÷//Í£Ö¹¼ÆÊýÆ÷Ê±ÖÓ
			PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			g_FeedStepCount = 0;
			return;
		}
		//*********************************************
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}
			g_StandbyPar = GetPaerStb(g_lineBitCount);//µÃµ½·ÖÇø±ê×¼

		
		g_lindotnu = g_lineBitCount;
		//*********************************************
		s_PrinterMOTOPar.m_PrintLSbit_f = s_PrinterMOTOPar.m_PrintLSbit;
		//*********************************************
		//ÐÐ¼ÆÊýÆ÷¼ÓÒ»
		g_PrintLineCount++;//×¼±¸´¦ÀíÏÂÒ»ÐÐµÄÊý¾Ý£¬ÌáÇ°¼ÓÒ»

		// *********************************************
		//´òÓ¡µ½×îºóÒ»ÐÐ,´ËÐÐ¿ªÊ¼´òÓ¡×Ö¼ä¾à
		if(g_PrintLineCount >= p_ListParmeters->m_PrintLineHigh)//
		{
			if(p_ListParmeters->m_PrintLineSpace != 0)
			{
				p_ListParmeters->m_PrintLineHigh = p_ListParmeters->m_PrintLineSpace;
				p_ListParmeters->m_PrintLineSpace = 0;
				g_PrintLineCount = 0;
				s_PrinterMOTOPar.m_PrintLSbit = ENABLE_VAR;//Ê¹ÄÜ¼ä¾à ±êÖ¾
			}
			else
			{
				
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
                if (BatteryLowCharge_LabelFinalBufferMatches((p_ListParmeters == &s_ListParmeter_A) ? 1 : 2))
                {
                    s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;
                    goto timer_run;
                }
#endif				
				/*
			 * Protected build: retain the current list buffer until the final raster
			 * block has physically printed, preventing the next label from starting early.
			 */
			#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
				if ((g_ThermalRasterFinalChunkQueued != 0) &&
					(g_ThermalRasterFinalBuffer == ((p_ListParmeters == &s_ListParmeter_A) ? 1 : 2)))
				{
					/* Finish this raster label before switching to the next
					 * print-list buffer, even if more Bluetooth data is ready. */
					s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;
					goto timer_run;
				}
				else
				if (g_PrintStatusReg == FINISH_PROCESS)
				{
					//Ìí¼ÓÈ±Ö½ºÍ°´¼ü±êÖ¾ -> // Add buzzer and key flag

					//*********************************************
					goto first_Pro_printDataList;
				}
				else
				{
					s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR; //Í£»ú;
					goto timer_run;
				}
			#else
				/*
				 * Legacy build: use the original list-buffer transition immediately
				 * after the current buffer is complete; no raster-boundary state exists.
				 */
				if (g_PrintStatusReg == FINISH_PROCESS)
				{
					goto first_Pro_printDataList;
				}
				else
				{
					s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;
					goto timer_run;
				}
			#endif
			}
		}
		//****************Ê¹ÄÜÊý¾Ý·¢ËÍ*****************************
		 //Æô¶¯·¢ËÍ³ÌÐò
		//¶¨Ê±Æ÷3Ê±ÖÓÊ¹ÄÜ·¢ËÍÊý¾Ý
	/* Mark the line as actively transferring so thermal protection can wait for a safe boundary. */	
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
		g_ThermalPrintTrack.unit_in_transmission = 1;
#endif
		
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
	}//if((Moto_Step & 0x3) == 0)
timer_run:
	
	g_RunFlag = 0;
	g_runLstep = 0;
	
	g_RunLTime = g_pinrtStb;

//#ifndef FIVE_DARKNESS_LEVEL_CONFIG
//		i = s_PrintrConfig.m_Density;
//#else	
		i = s_PrintrConfig.m_SaveConPar[7] & 0x1F;
//#endif
	
	h = g_RunLTime/10;
	
	if(i < 3)
	{
		g_RunLTime -= h * (3 - i);
		g_runLstep +=  h * (3 - i);
	}
	else
	{
		g_RunLTime += h * (i - 3);
	}
	

	
	if(s_PrintrConfig.m_InterFace == 0)
	{
		g_RunLTime = g_RunLTime * 2;
	}	
	
	s_PrintParameters.m_VolTemAdd = (g_RunLTime * 15)/100;
	s_PrintParameters.m_VolTemSubb = (g_RunLTime * 6)/100;	
	
//	if(p_ListParmeters->m_TMIndex > 9)
//	{
//		g_RunLTime += ( p_ListParmeters->m_TMIndex - 9) * s_PrintParameters.m_VolTemAdd*2;//¼ÓÈÈÊ±¼ä
//	}
//	else
//	{
//		g_runLstep = (9 - (p_ListParmeters->m_TMIndex)  ) * s_PrintParameters.m_VolTemSubb;//¼ÓÈÈÊ±¼ä
//		g_RunLTime -= g_runLstep;
//	}	

//	

	
	
	gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
	gStbProDataCon.m_SoftwarePart = s_PrintrConfig.m_SoftwarePart;//ÏµÍ³·ÖÇø·½Ê½
	gStbProDataCon.m_StandbyPar = 2;//µ±Ç°µÄ·ÖÇø·½Ê½

	g_RunCuStep = s_PrinterMOTOPar.m_MotoStep & 1;
	
	

//	printf("%d",g_StandbyPar);
//	if(s_PrinterMOTOPar.m_PrintLSbit_f != ENABLE_VAR)
	{
		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			if(g_StandbyPar == 0)
			{
				gStbProDataCon.m_StbHot = s_PrintParameters.m_CurFreeTime;//¿Õ×ßÖ½ËÙ¶È
				gStbProDataCon.m_StatusFlag = 1;
			}
			else
			{
				
				g_RunLTime += (g_RunLTime >> 4) * g_StandbyPar;//
				gStbProDataCon.m_StandbyPar = g_StandbyPar;
				//-----
				gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
				//-----
				//g_RunFlag = g_RunFlagArry[g_StandbyPar];
				gStbProDataCon.m_StatusFlag = g_RunFlagArry[g_StandbyPar] + 1;
				//-----
				gStbProDataCon.m_MarkOffset = g_RunCuStep * g_ForArry[g_StandbyPar];//µÃµ½Æ«ÒÆ±¶Êý
				//------
				//p_GetParSTB = bySTBExp[g_StandbyPar]; 
				gStbProDataCon.p_GetParSTB =  bySTBExp[g_StandbyPar];//µÃµ½·ÖÇøÑÚÂë±í
				
				gStbProDataCon.m_RunStatus = 0;//µÚÒ»²½¿ªÊ¼´¦Àí
				
				/* TIM3 has higher priority than the motor timer.
				* Always mark the line as active before starting TIM3.
				* If not then, the first head transfer of a label
				* could be interrupted before g_EnstbFlag is set. */
				g_EnstbFlag = 0xA123; 		// mukesh - reordered to avoid distortion during printing 
				ProcessStbSingle();	
				//g_EnstbFlag = 0xA123;
			}
			

		}
		else
		{
			if(g_lindotnu > 256)
			{
				g_StandbyPar = 4;
			}
			else if(g_lindotnu < 100)
			{
				g_StandbyPar = 1;
			}					
			else
			{
				g_StandbyPar = 2;//µ±Ç°µÄ·ÖÇø·½Ê½	
			}
			if(g_lindotnu)
			{		
				//g_RunLTime += (g_RunLTime >> 2) * g_StandbyPar;//
				if(g_StandbyPar == 4)
				{
					g_RunLTime += (g_RunLTime >> 3) * g_StandbyPar;//
				}				

				gStbProDataCon.m_StandbyPar = g_StandbyPar;
				//-----
				gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
				//-----
				//g_RunFlag = g_RunFlagArry[g_StandbyPar];
				gStbProDataCon.m_StatusFlag = g_RunFlagArry[g_StandbyPar] + 1;
				//-----
				gStbProDataCon.m_MarkOffset = g_RunCuStep * g_ForArry[g_StandbyPar];//µÃµ½Æ«ÒÆ±¶Êý
				//------
				//p_GetParSTB = bySTBExp[g_StandbyPar]; 
				gStbProDataCon.p_GetParSTB =  bySTBExp[g_StandbyPar];//µÃµ½·ÖÇøÑÚÂë±í
				
				gStbProDataCon.m_RunStatus = 0;//µÚÒ»²½¿ªÊ¼´¦Àí
				

				ProcessStbSingle();
				g_EnstbFlag = 0xA123;
			}
		}
	}
	if(g_lindotnu)
	{
		g_RunLTime = (gStbProDataCon.m_StbHot + g_runLstep)  * (gStbProDataCon.m_StatusFlag & 0xF);
	}	
	//g_RunLTime = (gStbProDataCon.m_StbHot + g_runLstep)  * (gStbProDataCon.m_StatusFlag & 0xF);
	//********************************************
	if(g_lindotnu == 0)
	{
		g_MotoAll++;
		g_RunLTime = 350;// g_pinrtStb / 3;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],10);
	}
	else
	{
		if(s_PrintrConfig.m_SoftwarePart == 2)
		{
			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],100);
		}
		else
		{
			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],s_PrintParameters.m_CurMotoPWM);
		}
	}
	if(s_PrintrConfig.m_SoftwarePart != 2)
	{
		if(g_FeedStepCount < 40)
		{
			g_RunLTime += 5 * (40 - g_FeedStepCount);
			g_FeedStepCount++;
		}		
	}	
	PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime+g_Speed);
	s_PrinterMOTOPar.m_MotoStep++;
	
	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
	
}

#if 0
void ProcessMotoLineData11(void)
{
	unsigned short temp;

	double a;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	PRINTER_PDL.SYS._Drive_Power_Trig();

	if(g_EnstbFlag == 0xA123)
	{
		
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
			return;		
	}
	if(g_lindotnu)
	{
		PRINTER_PDL.SYS._STB_A_EN();
		PRINTER_PDL.SYS._STB_B_EN();		
	}	
	
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		if(g_SendFlag == DATA_IN_SEND)
		{
			//µÈ´ýÒ»ÐÐÊý¾Ý´¦ÀíÍê³É
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
			return;
		}
	}
// =============================================================================
	#ifdef PRINTER_LABEL		
	if(g_DetPaperValueFlag == 0xA5EF)
		{
			if(g_DetPV_Count < 2)
			{
				g_DetPV_Count++;
			}
		}	
// =============================================================================
		if(g_CJCalfalg == 0xA565)
		{
			linearRegression_0_30();
			g_CJCalfalg = 0;
		}
		if((s_PrinterMOTOPar.m_MotoStep & 1) == 0)
		{
			SetFreValue(GetDETPaper());//ÉèÖÃ²ÉÑù¹âµçÖµ
			if(g_DetPV_Count > 1 )
			{
				g_CJCalfalg = 0xA565;
				linearRegression_32(&a);
//				if(g_GapProEn == 0xA537)
//				{
//					a = 0;
//				}
				if(a > s_PrintrConfig.m_PE_LabValue_H)
				{
					g_SpaceStatus |= 1;
					g_GapProEn = 0xA537;//±íÊ¾¼ì²âµ½Ö½·ì
				}
				else
				{
					g_SpaceStatus = 0xA500;
					
				}
			//	printf("%2.3f ",a);
			}				
		}		

		if((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
		{


			//±êÇ©´òÓ¡
			//ÔÚÃ¿Ò»²½µÄ¿ªÍ·¼ÆËã
			if(g_PaperRunToOut)
			{
				g_PaperRunToOut--;
			}

			if(g_SpaceStatus == 0xA501)
			{
				//¼ì²âµ½±êÇ©µÄÆðÊ¼´òÓ¡Î»ÖÃ

				g_falg = 0x0303;

				g_SpaceStatus = 0;//Çå¿Õ¼à²â×´Ì¬
				g_DetPaperGapSpace = 0xaaa;//¼ì²âµ½±êÇ©Í·
				//¼à²âµ½Ö½·ìºóÍùÇ°Ëù×ßµÄ²½Êý
				//Ö»×ßµ½¼ÓÈÈÏß´¦£¬³öÖ½²¿·ÖÓÉ³¬Ê±¿ØÖÆ

				//g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8;//  - 8- 4;//¼à²âµ½Ö½·ìÖØÐÂÐÞÕý×ßµ½¼ÓÈÈÏßµÄ²½Êý¡¢
				g_PaperRunToOut = g_HotLineTOPD * 8;//  - 8- 4;//¼à²âµ½Ö½·ìÖØÐÂÐÞÕý×ßµ½¼ÓÈÈÏßµÄ²½Êý¡¢
				if(p_ListParmeters->m_LableStopFlag == 0xA5A1)
				{
					if((*p_REAndSAcount) < 5)
					{
						//g_PaperRunToOut +=  s_PrintrConfig.m_HotLineTOOUT * 8;						
						g_PaperRunToOut +=  g_HotLineTOOUT * 8;						
					}
					
				}
				//if(g_Lab_head_Feed == 0x5523)
//				{
//					if((*p_REAndSAcount) < 5)
//					{
//						//Ã»ÓÐ½ÓÊÕµ½Êý¾Ý
//						g_PaperRunToOut +=  s_PrintrConfig.m_HotLineTOOUT * 8;
//						g_Lab_head_Feed = 0xA537;
//					}
//				}
			}


			if(g_PaperRunToOut == 0)
			{
				if(g_GapProEn == 0xA537)
				{
					g_GapProEn = 0;//¼ì²âµ½Ö½·ìÇÒÔËÐÐµ½0£¬ÇåµôÖ½·ì¼ì²â
				}
				//¼ì²âµ½Ö½·ì²¢ÇÒ×ßµ½³öÖ½Î»ÖÃ
				if(g_DetPaperGapSpace == 0xAAA)
				{
					if(p_ListParmeters->m_LableStopFlag == 0xA5A1)
					{
						//×ßµ½0µã£¬ÉèÖÃ³¤¶ÈÎªÖµµÄ³¤¶È+Ö½·ìµÄ³¤¶È
						g_PaperRunToOut = 8000;//8000²½ 1000mm ÉèÖÃÄ¬ÈÏ×ß1Ã×³¤¶È

						if(g_PrintStatusReg == FINISH_PROCESS)
						{
							goto first_Pro_printDataList;
						}
						else
						{
							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//´òÓ¡»úÍ£»ú;
							p_ListParmeters->m_LableStopFlag = 0;
						}
					}
					else
					{
//						if(g_SelfRunFlag != 0xAA)
//						{
//							g_RunToGapFlag = 0xA55A;
//							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//´òÓ¡»úÍ£»ú;
//							p_ListParmeters->m_LableStopFlag = 0;
//						}
					}
					
					
					
					//ÖØÐÂ´ÓÒ»¸ö±íµ¥¿ªÊ¼´òÓ¡¼ÇÊý
					g_DetPaperGapSpace = 0;


				}
			}
	
		}

	#endif
	
	
	
	
//	g_MotoSTBStatus = 0;
	if (s_PrinterMOTOPar.m_In_printing == ENABLE_VAR)//µÈÓÚENABLE_VAR ±íÊ¾µÚÒ»´Î½øÈë´òÓ¡³ÌÐòÐèÒªÔ¤´¦ÀíºÜ¶à¶«Î÷
	{

first_Pro_printDataList:
		//µÃµ½´òÓ¡Êý¾ÝÁ´±í
		if(g_PrintStatusReg != FINISH_PROCESS)
		{
			goto Pro_printDataList_EX;
		}
		g_PrintStatusReg = 0;
		g_BufferSwitch = ~g_BufferSwitch;//È¡·´´òÓ¡Á´±íÇÐ»»±êÖ¾
		if(g_BufferSwitch != 0)
		{
			p_ListParmeters = &s_ListParmeter_A;
		}
		else
		{
			p_ListParmeters = &s_ListParmeter_B;
		}

		s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//ÊÖ¶¯±êÖ¾Ê¹ÄÜ
		g_PrintLineCount = 0; //´òÓ¡µÚÒ»ÐÐµÄÊý¾Ý
		s_PrinterMOTOPar.m_PrintLSbit_f = DISABLE_VAR;//ÐÐ¼ä¾à±êÖ¾ÇåÁã
		s_PrinterMOTOPar.m_PrintLSbit = DISABLE_VAR;//ÎÞÐÐ¼ä¾à ±êÖ¾
		s_PrinterMOTOPar.m_MotoRun = ENABLE_VAR;//µç»úÔËÐÐ±êÖ¾Ê¹ÄÜ
		s_PrinterMOTOPar.m_StopPrint = DISABLE_VAR;//Í£Ö¹´òÓ¡½ûÖ¹
		s_PrinterMOTOPar.m_PrintEnd = DISABLE_VAR;//´òÓ¡½áÊø½ûÖ¹
		s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//µÈÓÚ DISABLE_VAR ±íÊ¾´òÓ¡»úÕýÔÚ´òÓ¡
		if(s_PrinterMOTOPar.m_InFirstPrint == ENABLE_VAR)
		{
			//µÚÒ»´Î½øÈë´òÓ¡Ô¤´¦Àí
			s_PrinterMOTOPar.m_InFirstPrint = DISABLE_VAR;//Çå³ýµÚÒ»´Î´òÓ¡±êÖ¾
			s_PrinterMOTOPar.m_MotoStep = 0;//¿ªÊ¼´òÓ¡motoÔËÐÐ²½ÊýÇå¿Õ
		//	TIM_ENABLE(TIMx,20000);//20msÆô¶¯Ê±Ðò
		//	TIM_ENABLE(TIM3,20);//Æô¶¯¶¨Ê±Æ÷3´¦ÀíÒ»ÐÐÊý¾Ý
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(20000);
			PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],s_PrintParameters.m_CurMotoPWM);
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			
			return;
		}
		//TIM_ENABLE(TIM3,10);//Æô¶¯¶¨Ê±Æ÷3´¦ÀíÒ»ÐÐÊý¾Ý
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
		goto timer_run;
	}
Pro_printDataList_EX:
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		if(g_QR_enable == 0xA123)
		{
			g_QR_SpaceCount++;
		}
		if(g_BarCode_enable == 0xA123)
		{
			g_BarCode_SpaceCount++;
		}
		//*********************************************
		//ÅÐ¶ÏÍ£Ö¹´òÓ¡
		if(s_PrinterMOTOPar.m_PrintEnd == ENABLE_VAR)
		{
			s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//;;;;;Ã»ÓÐ¿ÉÒÔ´òÓ¡µÄÊý¾Ý
			//Çå³ý¼ÓÈÈÏßºÍµç»úÇý¶¯ÐÅºÅÏß
		//	SET_L_STB1;SET_L_STB2;//Çå¼ÓÈÈÏß
			g_lindotnu = 0;
				PRINTER_PDL.SYS._STB_ALL_CLEAR();
		//	ClearMoto();
				PRINTER_PDL.MOTO._MotoDisable();
			s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//·­×ª
			s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;//´òÓ¡ÒÑ¾­Í£Ö¹
			//TIM_DISABLE(TIMx);//¹Ø±Õ¶¨Ê±Æ÷//Í£Ö¹¼ÆÊýÆ÷Ê±ÖÓ
			PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			g_FeedStepCount = 0;
			return;
		}
		//*********************************************
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}
			g_StandbyPar = GetPaerStb(g_lineBitCount);//µÃµ½·ÖÇø±ê×¼

		
		g_lindotnu = g_lineBitCount;
		//*********************************************
		s_PrinterMOTOPar.m_PrintLSbit_f = s_PrinterMOTOPar.m_PrintLSbit;
		//*********************************************
		//ÐÐ¼ÆÊýÆ÷¼ÓÒ»
		g_PrintLineCount++;//×¼±¸´¦ÀíÏÂÒ»ÐÐµÄÊý¾Ý£¬ÌáÇ°¼ÓÒ»

		// *********************************************
		//´òÓ¡µ½×îºóÒ»ÐÐ,´ËÐÐ¿ªÊ¼´òÓ¡×Ö¼ä¾à
		if(g_PrintLineCount >= p_ListParmeters->m_PrintLineHigh)//
		{
			if(p_ListParmeters->m_PrintLineSpace != 0)
			{
				p_ListParmeters->m_PrintLineHigh = p_ListParmeters->m_PrintLineSpace;
				p_ListParmeters->m_PrintLineSpace = 0;
				g_PrintLineCount = 0;
				s_PrinterMOTOPar.m_PrintLSbit = ENABLE_VAR;//Ê¹ÄÜ¼ä¾à ±êÖ¾
			}
			else
			{
				if(g_PrintStatusReg == FINISH_PROCESS)
				{
					//Ìí¼ÓÈ±Ö½ºÍ°´¼ü±êÖ¾

					//*********************************************
					goto first_Pro_printDataList;

				}
				else
				{
					s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//Í£»ú;
					goto timer_run;
				}
			}
		}
		//****************Ê¹ÄÜÊý¾Ý·¢ËÍ*****************************
		 //Æô¶¯·¢ËÍ³ÌÐò
		//¶¨Ê±Æ÷3Ê±ÖÓÊ¹ÄÜ·¢ËÍÊý¾Ý
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
	}//if((Moto_Step & 0x3) == 0)
timer_run:
	
	g_RunFlag = 0;
	g_runLstep = 0;
	
	g_RunLTime = g_pinrtStb;
	if((g_THDeep & 0x8000) == 0)
	{
		g_RunLTime += g_THDeep;//µÃµ½´òÓ¡Í·µÄ¼ÓÈÈÊ±¼ä
	}
	else
	{
		if(g_RunLTime > (g_THDeep & 0x7FFF))
		{
			g_RunLTime -= (g_THDeep & 0x7FFF);//µÃµ½´òÓ¡Í·µÄ¼ÓÈÈÊ±¼ä
		}
	}		
	

//	
//	if(s_PrintrConfig.m_InterFace == 0)
//	{
//		g_RunLTime = g_RunLTime * 2;
//	}	
	
//	s_PrintParameters.m_VolTemAdd = (g_RunLTime * 15)/100;
//	s_PrintParameters.m_VolTemSubb = (g_RunLTime * 6)/100;	
	
//	if(p_ListParmeters->m_TMIndex > 9)
//	{
//		g_RunLTime += ( p_ListParmeters->m_TMIndex - 9) * s_PrintParameters.m_VolTemAdd*2;//¼ÓÈÈÊ±¼ä
//	}
//	else
//	{
//		g_runLstep = (9 - (p_ListParmeters->m_TMIndex)  ) * s_PrintParameters.m_VolTemSubb;//¼ÓÈÈÊ±¼ä
//		g_RunLTime -= g_runLstep;
//	}	

		if(p_ListParmeters->m_ThType	 == 1)
		{
			g_RunLTime = (g_RunLTime * 2)/3;
		}
	
	gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
	gStbProDataCon.m_SoftwarePart = s_PrintrConfig.m_SoftwarePart;//ÏµÍ³·ÖÇø·½Ê½
	gStbProDataCon.m_StandbyPar = 2;//µ±Ç°µÄ·ÖÇø·½Ê½

	g_RunCuStep = s_PrinterMOTOPar.m_MotoStep & 1;
	
	

	
//	if(s_PrinterMOTOPar.m_PrintLSbit_f != ENABLE_VAR)
	{
		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			if(g_StandbyPar == 0)
			{
				gStbProDataCon.m_StbHot = s_PrintParameters.m_CurFreeTime;//¿Õ×ßÖ½ËÙ¶È
				gStbProDataCon.m_StatusFlag = 1;
			}
			else
			{
				
				g_RunLTime += (g_RunLTime >> 4) * g_StandbyPar;//
				
				gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
				gStbProDataCon.m_StandbyPar = g_StandbyPar;
				//-----
				gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
				//-----
				//g_RunFlag = g_RunFlagArry[g_StandbyPar];
				gStbProDataCon.m_StatusFlag = g_RunFlagArry[g_StandbyPar] + 1;
				//-----
				gStbProDataCon.m_MarkOffset = g_RunCuStep * g_ForArry[g_StandbyPar];//µÃµ½Æ«ÒÆ±¶Êý
				//------
				//p_GetParSTB = bySTBExp[g_StandbyPar]; 
				gStbProDataCon.p_GetParSTB =  bySTBExp[g_StandbyPar];//µÃµ½·ÖÇøÑÚÂë±í
				
				gStbProDataCon.m_RunStatus = 0;//µÚÒ»²½¿ªÊ¼´¦Àí
				

				ProcessStbSingle();	
				g_EnstbFlag = 0xA123;
			}
			

		}
		else
		{
			if(g_lindotnu > 260)
			{
				g_StandbyPar = 4;
			}
			else if(g_lindotnu < 130)
			{
				g_StandbyPar = 1;
			}			
			else
			{
				g_StandbyPar = 2;//µ±Ç°µÄ·ÖÇø·½Ê½	
			}
			
			if(g_lindotnu)
			{
				if(g_StandbyPar != 1)
				{
					g_RunLTime += (g_RunLTime >> 2) * g_StandbyPar;//
				}
				gStbProDataCon.m_StandbyPar = g_StandbyPar;
				//-----
				gStbProDataCon.m_StbHot = g_RunLTime;//µÃµ½¼ÓÈÈµÄÊ±¼ä
				//-----
				//g_RunFlag = g_RunFlagArry[g_StandbyPar];
				gStbProDataCon.m_StatusFlag = g_RunFlagArry[g_StandbyPar] + 1;
				//-----
				gStbProDataCon.m_MarkOffset = g_RunCuStep * g_ForArry[g_StandbyPar];//µÃµ½Æ«ÒÆ±¶Êý
				//------
				//p_GetParSTB = bySTBExp[g_StandbyPar]; 
				gStbProDataCon.p_GetParSTB =  bySTBExp[g_StandbyPar];//µÃµ½·ÖÇøÑÚÂë±í
				
				gStbProDataCon.m_RunStatus = 0;//µÚÒ»²½¿ªÊ¼´¦Àí
				

				ProcessStbSingle();
				g_EnstbFlag = 0xA123;
			}
		}
	}
	if(g_lindotnu)
	{
		g_RunLTime = (gStbProDataCon.m_StbHot + g_runLstep)  * (gStbProDataCon.m_StatusFlag & 0xF);
	}
	//********************************************
	if(g_lindotnu == 0)
	{
		g_MotoAll++;
		g_RunLTime = 450;// g_pinrtStb / 3;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],10);
		SendParData(0);
		PRINTER_PDL.SYS._LAT_L();
		PRINTER_PDL.SYS._LAT_H();				
	}
	else
	{
		if(s_PrintrConfig.m_SoftwarePart == 2)
		{
			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],100);
		}
		else
		{
			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],s_PrintParameters.m_CurMotoPWM);
		}
	}
	
//	if(g_FeedStepCount < 40)
//	{
//		g_RunLTime += 5 * (40 - g_FeedStepCount);
//		g_FeedStepCount++;
//	}		
	
	PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime);
	s_PrinterMOTOPar.m_MotoStep++;
	
	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
	
}


#endif
/****************************************************************************
* ÎÄ¼þÃû£ºTimer1PrintDataSend
* ¹¦  ÄÜ£ºÏò´òÓ¡»úÍ··¢ËÍÊý¾Ý
* Èë¿Ú²ÎÊý£º
* Ëµ  Ã÷£ºÔÚ½øÈëtimer1ÖÐ¶Ïºó·¢ËÍÏÂÒ»ÐÐÐèÒª´òÓ¡µÄÊý¾Ý£¬PrintLineCount ´òÓ¡ÐÐÊý¼ÆÊýÆ÷
*ÎªÁËÄÜ²»ÔÚÏò´òÓ¡Í·´®ÐÐ·¢ËÍÊý¾ÝµÄÊ±ºòµ¢ÎóCPUµÄÊ±¼ä£¬Ê¹ÓÃSSCÕâ¸öµ¥Ôª·¢ËÍÊý¾Ý
****************************************************************************/

void SendDICLK(unsigned short u16_data)
{
	unsigned short mdat;
	mdat = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter))|g_QRCodeBuffer[SendBuffer_counter]|g_BarCodeBuffer[SendBuffer_counter];
	SaveToBuffer[SendBuffer_counter] = mdat;
	g_lineBitCount += CalintCount(mdat);
	SendBuffer_counter++;
}

unsigned char buuff[80];
void SendParData(unsigned short u16_anddata)
{
	int i,j;
	unsigned short mdat;

	SendToSPI_counter = 0;
	j = 0;
	for(i=0;i<BUFFER_SHORT_LENGTH;i++)
	{

		mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
//		PRINTER_PDL.SDTH._SendToHead_u16(&mdat,1);
		buuff[j++] = mdat >> 8;
		buuff[j++] = mdat;
		
	}
	PRINTER_PDL.SDTH._DMA_SendToHead_u8(buuff,BUFFER_SHORT_LENGTH*2);
	
	
	
	PRINTER_PDL.SDTH._IS_Send_Empty();

}

void SendDMAData(unsigned short u16_anddata)
{
	unsigned short mdat,j;

	SendToSPI_counter = 0;

	j = 0;
	//1---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//2---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//3---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//4---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//5---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//6---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//7---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//8---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//9---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//10---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//11---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//12---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//13---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//14---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//15---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//16---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//17---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//18---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//19---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//20---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//21---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//22---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//23---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	//24---
	mdat = (SendBufferToSPI[SendToSPI_counter++]& u16_anddata);
	buuff[j++] = mdat >> 8;
	buuff[j++] = mdat;
	
	PRINTER_PDL.SDTH._DMA_SendToHead_u8(buuff,48);

}
// mukesh - fix for distortion due to uncleared raster dots
/* TIM3 runs in microsecond resolution.
 * After DMA signals completion, SPI has already been fed,
 * but the last bits are still shifting out of the register.
 * A short delay ensures the shift-register tail clears before
 * latching head data. These defines set the poll interval and
 * settle time for safe DMA completion handling. 
*/
#define PRINT_HEAD_DMA_POLL_US    6   // Interval before re-checking DMA transfer
#define PRINT_HEAD_DMA_SETTLE_US  6   // Extra delay after DMA reports done

void ProcessStbSingle(void)
{   
    // Clear all strobe outputs before starting
    // PRINTER_PDL.SYS._STB_ALL_CLEAR();

    if ((gStbProDataCon.m_StatusFlag & 0xFF00) == 0xAA00)
    {
        switch (gStbProDataCon.m_RunStatus)
        {
        case 0: 
            // First entry: send the very first data block to DMA
            SendDMAData(gStbProDataCon.p_GetParSTB[gStbProDataCon.m_MarkOffset++]);
            gStbProDataCon.m_RunStatus = 1;
            // Schedule next service interrupt to verify DMA progress
            PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(30);
            break;

        case 1: 
            // Verify whether the first DMA transfer has actually completed
            // End the heating interval of the previous partition by clearing strobes
            PRINTER_PDL.SYS._STB_ALL_CLEAR();

            // DMA reports “fed SPI” but final bits may still be shifting.
            // Poll until DMA empty, then move to settle state.
            if (PRINTER_PDL.SDTH._IS_Send_Empty() == 0) 
            {
                // Retry after poll interval; avoids latching partial data
                PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(PRINT_HEAD_DMA_POLL_US);
                break;
            }

            // DMA is complete; advance to settle state
            gStbProDataCon.m_RunStatus = 3;
            // Wait settle time before touching latch
            PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(PRINT_HEAD_DMA_SETTLE_US);
            break;

        case 3: 
            // DMA complete and shift-register tail settled.
            // Enable heating interval and latch new dot pattern
            PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(gStbProDataCon.m_StbHot);
            PRINTER_PDL.SYS._LAT_L();
            PRINTER_PDL.SYS._STB_A_EN();
            PRINTER_PDL.SYS._STB_B_EN();
            PRINTER_PDL.SYS._LAT_H();

            if ((gStbProDataCon.m_StatusFlag & 0xFF))
            {
                gStbProDataCon.m_StatusFlag--;
                if ((gStbProDataCon.m_StatusFlag & 0xFF))
                {
                    // More data remains; send next block and return to DMA verification
                    SendDMAData(gStbProDataCon.p_GetParSTB[gStbProDataCon.m_MarkOffset++]);
                    gStbProDataCon.m_RunStatus = 1;
                }
                else
                {
                    // No more data; transition to stop state
                    gStbProDataCon.m_RunStatus = 2;
                }
            }
            else
            {
                // No data left; transition to stop state
                gStbProDataCon.m_RunStatus = 2;
            }
            break;

        case 2: 
            // Cancel printing and clear all strobes
            g_EnstbFlag = 0;
            gStbProDataCon.m_StatusFlag = 0;
            gStbProDataCon.m_RunStatus = 2;
            PRINTER_PDL.SYS._STB_ALL_CLEAR();
            break;
        }
    }
    else
    {
        // Status flag not valid; clear strobes defensively
        PRINTER_PDL.SYS._STB_ALL_CLEAR();
    }
}



/****************************************************************************
* ÎÄ¼þÃû£ºTimer1PrintDataSend
* ¹¦  ÄÜ£ºÏò´òÓ¡»úÍ··¢ËÍÊý¾Ý
* Èë¿Ú²ÎÊý£º
* Ëµ  Ã÷£ºÔÚ½øÈëtimer1ÖÐ¶Ïºó·¢ËÍÏÂÒ»ÐÐÐèÒª´òÓ¡µÄÊý¾Ý£¬PrintLineCount ´òÓ¡ÐÐÊý¼ÆÊýÆ÷
*ÎªÁËÄÜ²»ÔÚÏò´òÓ¡Í·´®ÐÐ·¢ËÍÊý¾ÝµÄÊ±ºòµ¢ÎóCPUµÄÊ±¼ä£¬Ê¹ÓÃSSCÕâ¸öµ¥Ôª·¢ËÍÊý¾Ý
****************************************************************************/

/****************************************************************************
* File Name: Timer1PrintDataSend
* Function: Send data to printer
* Parameters: None
* Description: After entering timer1 interrupt, send the next line of print data.
*              PrintLineCount is the line counter.
*              To avoid wasting CPU time when sending data to the printhead line,
*              use SSC unit for transmission.
****************************************************************************/

void OneLinePrintDataSend(void)
{
	p_PrintListPar			p_PrParmeters	;//Á´±í½á¹¹Ö¸Õë		// Pointer to active print line parameter list
	DataCon					m_SSCdata;//SSCÊý¾Ý·¢ËÍ½á¹¹
	int 					dummy;						// temporary
	unsigned short mTempcount,i;
	unsigned int			m_Dotdata;		// Raw 32-bit dot matrix pattern word
	unsigned int			m_Tempdata;
//	int						m_xount;
	unsigned short			addpar;
	unsigned short			m_IndexCounter;//Ë÷Òý±í			// Item index pointer
	unsigned short			*p_PrinterData;//Êý¾ÝÖ¸Õë		// Pointer to font/bitmap dot matrix data
	unsigned short			m_GOrder;//ÃüÁî							// Active list item command token

	unsigned short			m_CharWide;//×Ö·ûµÄ¿í¶È			// Character width in dots
	unsigned short			m_WideZoom;//¿í¶È·Å´ó				// Horizontal scaling multiplier
	unsigned short			m_CharHigh;//×Ö·ûµÄ¸ß¶È			// Character height in dots
	unsigned short			m_HighZoom;//¸ß¶È·Å´ó				// Vertical scaling multiplier used in qr/barcode
	unsigned short			m_calZe;//¼ÆËãÆ÷
	unsigned short			m_calR0;//¼ÆËãÆ÷
	unsigned short			m_alignSpace;//ÉÏÏÂ¶ÔÆëºÍ×Ö¼ä¾à


//---------------Çå³ýÖÐ¶Ï±êÖ¾----------------------------------------
/* 1. Clear & Prepare QR Code and Barcode Line Buffers */
	for(i=0;i<48;i++)
	{
		g_QRCodeBuffer[i] = 0;
		g_BarCodeBuffer[i] = 0;
	}
#ifdef PSF20_R002_ESC_POS_PROTOCOL
	if(g_QR_enable == 0xA123)
	{
		if(g_QR_SpaceCount >= g_QR_y)
		{
			mTempcount = (g_QR_SpaceCount - g_QR_y)/s_PrintParameters.m_QRZoom;
			if(mTempcount < qrcode->width)
			{
				SetQRCodeBuffLine(mTempcount);
			}
			else
			{
				g_QR_enable = 0;
			}
		}
	}
#endif
#ifdef PSF20_R002_ESC_POS_PROTOCOL
	if(g_BarCode_enable == 0xA123)
	{
		if(g_BarCode_SpaceCount >= g_BarCode_y)
		{
			mTempcount = g_BarCode_SpaceCount - g_BarCode_y;
			if(mTempcount < g_BarCodeHigh_Pro)
			{
				SetBarCodeBuffLine(mTempcount);
			}
			else
			{
				g_BarCodeHigh_Pro = 0;
				g_BarCode_enable = 0;
			}
		}
	}

#endif
//-------------½ûÖ¹ÖÐ¶Ï------------------------------------------
/* 2. Reset Serial Output Counters & Mark Data In-Send */
	SendBuffer_counter = 0;
	g_lineBitCount = 0;
	g_SendFlag = DATA_IN_SEND;
	SendBuffer_counter = 0;
	m_SSCdata.m_data = 0;
	m_SSCdata.m_len = 0;

	
	if(s_PrinterMOTOPar.m_PrintLSbit == ENABLE_VAR)
	{
		for (dummy = BUFFER_SHORT_LENGTH;dummy > 0;dummy--)
		{
			SendDICLK(0);
		}
		g_SendFlag = DATA_SEND_OK;
		return;
	}

	if(g_BufferSwitch != 0)
		p_PrParmeters = &s_ListParmeter_A;
	else
		p_PrParmeters = &s_ListParmeter_B;

//---------------------ÏÈ´òÓ¡¿ªÍ·µÄ¿Õ°×µã------------------
//==========================µÚÒ»²½========================================


	if(p_PrParmeters->m_PrintDir == PRINT_DIRECT_NOR)
	{
		m_SSCdata.m_len = p_PrParmeters->m_LeftFreeDot + g_dotSwitch;
		addpar = 1;
	}
	else
	{
		m_SSCdata.m_len = p_PrParmeters->m_RightFreeDot + g_dotSwitch;
		addpar = 0xffff;
	}
	while(m_SSCdata.m_len >= 32)
	{
		SendDICLK(0);
		SendDICLK(0);
		m_SSCdata.m_len -= 32;
	}
//==================================================================
	m_IndexCounter = p_PrParmeters->m_PrintStartPos;
	m_GOrder = p_PrParmeters->m_ListIndex[m_IndexCounter];
	while (m_GOrder != LIST_END)
	{
		//===========¿ªÊ¼´òÓ¡Êý¾Ý
		p_PrinterData = p_PrParmeters->m_ListBuffer + m_GOrder;//µÃµ½Êý¾ÝÆðÊ¼µØÖ·
		m_GOrder = *p_PrinterData++;
		if (m_GOrder == PRINT_DATA )
		{
			m_CharWide = ((*p_PrinterData) >> 4) & 0xff;
			m_WideZoom = (*p_PrinterData++) & 0xf;
			m_CharHigh = ((*p_PrinterData) >> 4) & 0xff;
			m_HighZoom = (*p_PrinterData++) & 0xf;
			m_alignSpace = *p_PrinterData++;
			m_calZe = m_CharHigh * m_HighZoom;


			//---------------ÅÐ¶Ï¿É´òÓ¡ÐÐÊý-------------------------
			if ((m_alignSpace & 0x8000) != 0)
			{
				//-----------ÏÂ±ê---------------------
				dummy = g_PrintLineCount + m_calZe - p_PrParmeters->m_PrintLineHigh;//´òÓ¡µÄÐÐ¸ß
			}
			else
			{
				//-----------ÉÏ±ê---------------------
				if(g_PrintLineCount < m_calZe)
					dummy = g_PrintLineCount;
				else
					dummy = -1;
			}
			m_alignSpace &= 0x7fff;//È¥µô×î¸ßÎ»µÄÉÏÏÂ¶ÔÆë±êÖ¾
			//---------------´òÓ¡¿Õ°×µã
			m_calZe = m_CharWide * m_WideZoom;
			if (dummy < 0)
			{

				if ((m_SSCdata.m_len + m_calZe) >= 32)
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_SSCdata.m_len = m_calZe + m_SSCdata.m_len - 32;

					SendDICLK(m_SSCdata.m_data >> 16);
					SendDICLK(m_SSCdata.m_data );
					m_SSCdata.m_data = 0;
					while(m_SSCdata.m_len >= 32)
					{
						SendDICLK(0);
						SendDICLK(0);
						m_SSCdata.m_len -= 32;
					}
				}
				else
				{
					m_SSCdata.m_len += m_calZe;
					m_SSCdata.m_data = m_SSCdata.m_data << m_calZe;
				}
				if ((m_SSCdata.m_len + m_alignSpace) >= 32)
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_SSCdata.m_len = m_alignSpace + m_SSCdata.m_len - 32;

					SendDICLK(m_SSCdata.m_data >> 16);
					SendDICLK(m_SSCdata.m_data );

					m_SSCdata.m_data = 0;
					while(m_SSCdata.m_len >= 32)
					{
						SendDICLK(0);
						SendDICLK(0);
						m_SSCdata.m_len -= 32;
					}
				}
				else
				{
					m_SSCdata.m_len += m_alignSpace;
					m_SSCdata.m_data = m_SSCdata.m_data << m_alignSpace;
				}
				m_IndexCounter += addpar;
				m_GOrder = p_PrParmeters->m_ListIndex[m_IndexCounter];
				continue;
			}
			//=--------------´òÓ¡Êµ¼ÊÊý¾Ý------------------------------------------

			m_calR0 = (dummy * DiV_config[m_HighZoom]) >> 16;//µÃµ½ÐÐÊý
			if (m_CharWide > 16)//15)
			{
				p_PrinterData  +=  (m_calR0 << 1);//µÃµ½µãÕóÊý¾Ý´æ·ÅµÄÆðÊ¼Î»ÖÃ
				m_Dotdata = *p_PrinterData++;
				m_Dotdata = (m_Dotdata << 16) | (*p_PrinterData++);

			}
			else
			{
				p_PrinterData  +=  m_calR0;
				m_Dotdata = *p_PrinterData++;
			}
			if (m_WideZoom == 1)
			{
				if((m_SSCdata.m_len + m_calZe) <= 32)
				{
					m_SSCdata.m_len =  m_calZe + m_SSCdata.m_len;
					m_SSCdata.m_data = (m_SSCdata.m_data << m_calZe) | m_Dotdata;
					if (m_SSCdata.m_len == 32)
					{
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );

						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
					if (m_SSCdata.m_len == 16)
					{
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
				}
				else
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_calR0 = m_calZe + m_SSCdata.m_len -32;
					m_Tempdata = m_Dotdata >> m_calR0;
					m_SSCdata.m_data |= m_Tempdata;

					SendDICLK(m_SSCdata.m_data >> 16);
					SendDICLK(m_SSCdata.m_data );

					m_SSCdata.m_data = m_Dotdata;
					m_SSCdata.m_len = m_calR0;
				}
				if((m_SSCdata.m_len + m_alignSpace) <= 32)
				{
					m_SSCdata.m_len =  m_alignSpace + m_SSCdata.m_len;
					m_SSCdata.m_data = (m_SSCdata.m_data << m_alignSpace) ;
					if (m_SSCdata.m_len == 32)
					{
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );

						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
					if (m_SSCdata.m_len == 16)
					{
						SendDICLK(m_SSCdata.m_data );

						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
				}
				else
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_calR0 = m_alignSpace + m_SSCdata.m_len -32;
					SendDICLK(m_SSCdata.m_data >> 16);
					SendDICLK(m_SSCdata.m_data );

					m_SSCdata.m_data = 0;
					m_SSCdata.m_len = 0;
					while(m_calR0 >= 32)
					{
						SendDICLK(0);
						SendDICLK(0);

						m_calR0 -= 32;
					}
					m_SSCdata.m_len = m_calR0;
				}
			}
			else
			{

				dummy = m_CharWide;
				while (dummy > 4)
				{
					m_Tempdata = (m_Dotdata >> (dummy - 4)) & 0xf;
					m_calR0 = m_WideZoom << 2;
					m_Tempdata = m_Tempdata + ((m_WideZoom -1)<< 4);
					m_Tempdata = Config_zoom[m_Tempdata];//µÃµ½·Å´óºóµÄÊý¾Ý
					if((m_SSCdata.m_len + m_calR0) <= 32)
					{
						m_SSCdata.m_len =  m_calR0 + m_SSCdata.m_len;
						m_SSCdata.m_data = (m_SSCdata.m_data << m_calR0) | m_Tempdata;
						if (m_SSCdata.m_len == 32)
						{
							SendDICLK(m_SSCdata.m_data >> 16);
							SendDICLK(m_SSCdata.m_data );

							m_SSCdata.m_data = 0;
							m_SSCdata.m_len = 0;
						}
						if (m_SSCdata.m_len == 16)
						{
							SendDICLK(m_SSCdata.m_data );
							m_SSCdata.m_data = 0;
							m_SSCdata.m_len = 0;
						}
					}
					else
					{
						m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
						m_calR0 = m_calR0 + m_SSCdata.m_len -32;
						m_SSCdata.m_data |= m_Tempdata >> m_calR0;
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );

						m_SSCdata.m_data = m_Tempdata;
						m_SSCdata.m_len = m_calR0;
					}
					dummy -= 4;
				}
				if (dummy != 0)
				{
					m_Tempdata = m_Dotdata  & 0xf;
					m_calR0 = m_WideZoom * dummy;//µÃµ½ÒÆÎ»´óÐ¡
					m_Tempdata = m_Tempdata + ((m_WideZoom -1)<< 4);
					m_Tempdata = Config_zoom[m_Tempdata];//µÃµ½·Å´óºóµÄÊý¾Ý
					if((m_SSCdata.m_len + m_calR0) <= 32)
					{
						m_SSCdata.m_len =  m_calR0 + m_SSCdata.m_len;
						m_SSCdata.m_data = (m_SSCdata.m_data << m_calR0) | m_Tempdata;
						if (m_SSCdata.m_len == 32)
						{
							SendDICLK(m_SSCdata.m_data >> 16);
							SendDICLK(m_SSCdata.m_data );

							m_SSCdata.m_data = 0;
							m_SSCdata.m_len = 0;
						}
						if (m_SSCdata.m_len == 16)
						{
							SendDICLK(m_SSCdata.m_data );
							m_SSCdata.m_data = 0;
							m_SSCdata.m_len = 0;
						}
					}
					else
					{
						m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
						m_calR0 = m_calR0 + m_SSCdata.m_len -32;
						m_SSCdata.m_data |= m_Tempdata >> m_calR0;
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );

						m_SSCdata.m_data = m_Tempdata;
						m_SSCdata.m_len = m_calR0;
					}
				}
				if((m_SSCdata.m_len + m_alignSpace) <= 32)
				{
					m_SSCdata.m_len =  m_alignSpace + m_SSCdata.m_len;
					m_SSCdata.m_data = (m_SSCdata.m_data << m_alignSpace) ;
					if (m_SSCdata.m_len == 32)
					{
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );

						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
					if (m_SSCdata.m_len == 16)
					{
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
				}
				else
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_calR0 = m_alignSpace + m_SSCdata.m_len -32;

					SendDICLK(m_SSCdata.m_data >> 16);
					SendDICLK(m_SSCdata.m_data );

					m_SSCdata.m_data = 0;
					m_SSCdata.m_len = 0;
					while(m_calR0 >= 32)
					{
						SendDICLK(0);
						SendDICLK(0);
						m_calR0 -= 32;
					}
					m_SSCdata.m_len = m_calR0;
				}

			}
		}
		else
		{
			//´¦Àí ¿Õµã ºÍÍ¼ÐÎ ÇúÏß
			//========================================

			if (m_GOrder == PRINT_CURVE )
			{
				p_PrinterData++; //µÃµ½´òÓ¡µÄ·½Ïò

				dummy = BUFFER_SHORT_LENGTH;//print_head_length >> 4;

				for (;dummy > 0;dummy--)
				{
					SendDICLK(*p_PrinterData++ );
				}
				m_SSCdata.m_len =0;
				m_calR0 = 0;

			}
			if (m_GOrder == PRINT_SPACE )
			{
				m_CharWide = *p_PrinterData;//µÃµ½´òÓ¡¿ÕµãµÄ¿í¶È

				if((m_SSCdata.m_len + m_CharWide) <= 32)
				{
					m_SSCdata.m_len =  m_CharWide + m_SSCdata.m_len;
					m_SSCdata.m_data = (m_SSCdata.m_data << m_CharWide) ;
					if (m_SSCdata.m_len == 32)
					{
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
					if (m_SSCdata.m_len == 16)
					{
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
				}
				else
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_calR0 = m_CharWide + m_SSCdata.m_len -32;
					SendDICLK(m_SSCdata.m_data >> 16);
					SendDICLK(m_SSCdata.m_data );
					m_SSCdata.m_data = 0;
					m_SSCdata.m_len = 0;
					while(m_calR0 >= 32)
					{
						SendDICLK(0);
						SendDICLK(0 );
						m_calR0 -= 32;
					}
					m_SSCdata.m_len = m_calR0;
				}
			}
			if (m_GOrder == PRINT_MAP )
			{
				g_SpeedEnstu = 0xA123;
				m_CharWide = ((*p_PrinterData) >> 4) & 0x3ff;
				m_WideZoom = (*p_PrinterData++) & 0xf;
				m_CharHigh = ((*p_PrinterData) >> 4) & 0x3ff;
				m_HighZoom = (*p_PrinterData++) & 0xf;
				m_alignSpace = *p_PrinterData++;
				m_calZe = m_CharHigh * m_HighZoom;
				//---------------ÅÐ¶Ï¿É´òÓ¡ÐÐÊý-------------------------
				if ((m_alignSpace & 0x8000) != 0)
				{
					//-----------ÏÂ±ê---------------------
					dummy = g_PrintLineCount + m_calZe - p_PrParmeters->m_PrintLineHigh;//´òÓ¡µÄÐÐ¸ß
				}
				else
				{
					//-----------ÉÏ±ê---------------------
					if(g_PrintLineCount < m_calZe)
						dummy = g_PrintLineCount;
					else
						dummy = -1;
				}
				m_alignSpace &= 0x7fff;//È¥µô×î¸ßÎ»µÄÉÏÏÂ¶ÔÆë±êÖ¾
				//---------------´òÓ¡¿Õ°×µã
				m_calZe = m_CharWide * m_WideZoom;
				if (dummy < 0)
				{
					if ((m_SSCdata.m_len + m_calZe) >= 32)
					{
						m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
						m_SSCdata.m_len = m_calZe + m_SSCdata.m_len - 32;

						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						while(m_SSCdata.m_len >= 32)
						{
							SendDICLK(0);
							SendDICLK(0);
							m_SSCdata.m_len -= 32;
						}
					}
					else
					{
						m_SSCdata.m_len += m_calZe;
						m_SSCdata.m_data = m_SSCdata.m_data << m_calZe;
					}
					if ((m_SSCdata.m_len + m_alignSpace) >= 32)
					{
						m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
						m_SSCdata.m_len = m_alignSpace + m_SSCdata.m_len - 32;

						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );

						while(m_SSCdata.m_len >= 32)
						{
							SendDICLK(0);
							SendDICLK(0);
							m_SSCdata.m_len -= 32;
						}
					}
					else
					{
						m_SSCdata.m_len += m_alignSpace;
						m_SSCdata.m_data = m_SSCdata.m_data << m_alignSpace;
					}

					m_IndexCounter += addpar;
					m_GOrder = p_PrParmeters->m_ListIndex[m_IndexCounter];
					continue;
				}
				//=--------------´òÓ¡Êµ¼ÊÊý¾Ý------------------------------------------

				m_calR0 = (dummy * DiV_config[m_HighZoom]) >> 16;//µÃµ½ÐÐÊý
				//---------µÃµ½Í¼ÐÎ¿í¶È×Ö½Ú
				m_calZe = m_CharWide >> 4;
				if ((m_CharWide & 0xf) != 0)
				{
					m_calZe++;
				}
				//Êý¾ÝÆðÊ¼µØÖ·
				//--------------------
				p_PrinterData  +=  m_calR0 * m_calZe;
				//--------------------
				dummy = m_CharWide;
				while (m_CharWide > 16)
				{
					m_Dotdata = *p_PrinterData++;
					dummy = 16;
					while(dummy!=0)
					{
						m_Tempdata = (m_Dotdata >> (dummy - 4)) & 0xf;
						m_calR0 = m_WideZoom << 2;
						m_Tempdata = m_Tempdata + ((m_WideZoom -1)<< 4);//
						m_Tempdata = Config_zoom[m_Tempdata];//µÃµ½·Å´óºóµÄÊý¾Ý
						//-------------·¢ËÍÊý¾Ý
						if((m_SSCdata.m_len + m_calR0) <= 32)
						{
							m_SSCdata.m_len =  m_calR0 + m_SSCdata.m_len;
							m_SSCdata.m_data = (m_SSCdata.m_data << m_calR0) | m_Tempdata;
							if (m_SSCdata.m_len == 32)
							{
								SendDICLK(m_SSCdata.m_data >> 16);
								SendDICLK(m_SSCdata.m_data );

								m_SSCdata.m_data = 0;
								m_SSCdata.m_len = 0;
							}
							if (m_SSCdata.m_len == 16)
							{
								SendDICLK(m_SSCdata.m_data );
								m_SSCdata.m_data = 0;
								m_SSCdata.m_len = 0;
							}
						}
						else
						{
							m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
							m_calR0 = m_calR0 + m_SSCdata.m_len -32;
							m_SSCdata.m_data |= m_Tempdata >> m_calR0;
							SendDICLK(m_SSCdata.m_data >> 16);
							SendDICLK(m_SSCdata.m_data );
							m_SSCdata.m_data = m_Tempdata;
							m_SSCdata.m_len = m_calR0;
						}

						dummy -= 4;
					}
					m_CharWide -= 16;
				}
				if (m_CharWide > 0)
				{
					//´¦ÀíÊ£ÓàÊý¾Ý

					m_Dotdata = *p_PrinterData++;
					dummy = m_CharWide;
					while(dummy!=0)
					{
						if (dummy > 4)
						{
							m_Tempdata = (m_Dotdata >> (dummy - 4)) & 0xf;
							m_calR0 = m_WideZoom << 2;
							dummy -= 4;
						}
						else
						{
							m_Tempdata = m_Dotdata & 0xf;
							m_calR0 = m_WideZoom * dummy;
							dummy = 0;
						}
						m_Tempdata = m_Tempdata + ((m_WideZoom -1)<< 4);//
						m_Tempdata = Config_zoom[m_Tempdata];//µÃµ½·Å´óºóµÄÊý¾Ý
						//-------------·¢ËÍÊý¾Ý
						if((m_SSCdata.m_len + m_calR0) <= 32)
						{
							m_SSCdata.m_len =  m_calR0 + m_SSCdata.m_len;
							m_SSCdata.m_data = (m_SSCdata.m_data << m_calR0) | m_Tempdata;
							if (m_SSCdata.m_len == 32)
							{
								SendDICLK(m_SSCdata.m_data >> 16);
								SendDICLK(m_SSCdata.m_data );
								m_SSCdata.m_data = 0;
								m_SSCdata.m_len = 0;
							}
							if (m_SSCdata.m_len == 16)
							{
								SendDICLK(m_SSCdata.m_data );
								m_SSCdata.m_data = 0;
								m_SSCdata.m_len = 0;
							}
						}
						else
						{
							m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
							m_calR0 = m_calR0 + m_SSCdata.m_len -32;
							m_SSCdata.m_data |= m_Tempdata >> m_calR0;
							SendDICLK(m_SSCdata.m_data >> 16);
							SendDICLK(m_SSCdata.m_data );
							m_SSCdata.m_data = m_Tempdata;
							m_SSCdata.m_len = m_calR0;
						}

					}
				}
				if((m_SSCdata.m_len + m_alignSpace) <= 32)
				{
					m_SSCdata.m_len =  m_alignSpace + m_SSCdata.m_len;
					m_SSCdata.m_data = (m_SSCdata.m_data << m_alignSpace) ;
					if (m_SSCdata.m_len == 32)
					{
						SendDICLK(m_SSCdata.m_data >> 16);
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
					if (m_SSCdata.m_len == 16)
					{
						SendDICLK(m_SSCdata.m_data );
						m_SSCdata.m_data = 0;
						m_SSCdata.m_len = 0;
					}
				}
				else
				{
					m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
					m_calR0 = m_alignSpace + m_SSCdata.m_len -32;
					SendDICLK(m_SSCdata.m_data );
					m_SSCdata.m_data = 0;
					m_SSCdata.m_len = 0;
					while(m_calR0 >= 32)
					{
						SendDICLK(0);
						SendDICLK(0);
						m_calR0 -= 32;
					}
					m_SSCdata.m_len = m_calR0;
				}
			}
		}
		m_IndexCounter += addpar;
		m_GOrder = p_PrParmeters->m_ListIndex[m_IndexCounter];
	}
	//*********************½áÊø******************************************
	if(p_PrParmeters->m_PrintDir == PRINT_DIRECT_NOR)
	{
		m_calR0 = p_PrParmeters->m_RightFreeDot + g_dotSwitch ;
	}
	else
	{
		m_calR0 = p_PrParmeters->m_LeftFreeDot + g_dotSwitch;
	}
	if((m_SSCdata.m_len + m_calR0) == 0)
	{
		g_SendFlag = DATA_SEND_OK;
		return;
	}
	if((m_SSCdata.m_len + m_calR0) <= 32)
	{
		m_SSCdata.m_len =  m_calR0 + m_SSCdata.m_len;
		m_SSCdata.m_data = (m_SSCdata.m_data << m_calR0) ;
		if (m_SSCdata.m_len == 32)
		{
			SendDICLK(m_SSCdata.m_data >> 16);
			SendDICLK(m_SSCdata.m_data );
			m_SSCdata.m_data = 0;
			m_SSCdata.m_len = 0;
		}
		if (m_SSCdata.m_len == 16)
		{
			SendDICLK(m_SSCdata.m_data );
			m_SSCdata.m_data = 0;
			m_SSCdata.m_len = 0;
		}
	}
	else
	{
		m_SSCdata.m_data = m_SSCdata.m_data << (32 - m_SSCdata.m_len);
		m_calR0 = m_calR0 + m_SSCdata.m_len -32;
		SendDICLK(m_SSCdata.m_data >> 16);
		SendDICLK(m_SSCdata.m_data );
		m_SSCdata.m_data = 0;
		m_SSCdata.m_len = 0;
		while(m_calR0 >= 16)
		{
			SendDICLK(0);
			m_calR0 -= 16;
		}
		m_SSCdata.m_len = m_calR0;
	}
	//**************************************************************
	g_SendFlag = DATA_SEND_OK;
}


extern unsigned char bt_name[15];

#ifdef INTELLIGENT_SELFTEST_RECEIPT
/**
	* @FuncName  SetSelftestData
	* @param     void
	* @retval    void
	* @LocalVar  unsigned short m_type      - Diagnostic test page layout type identifier
	*            unsigned short i           - Buffer copy iterator index
	*            unsigned short com         - Communication interface parameter temporary
variable
	*            unsigned short m_DaSpave   - Saved buffer index offset for serial number
injection
	*            unsigned int m_judgeyear   - Encoded 24-bit compilation month string for
serial number formatting
	*            char checksum_str[32]      - Formatted Flash Golden Checksum string buffer
(under CHECKSUM build)
	* @GlobalVar g_SelfTestStartFlag, g_Receive_count, Receive_count, g_Order_count,
	*            g_SelfRunFlag, g_DumpReceiveFlage, s_PrintParameters, g_PaperType,
	*            g_label_size_final, s_PrintrConfig, receive_bufer, p_REAndSAcount,
	*            CompileDate, CompileTime, SelfTestSerial, SelfTest_S, SelfTest_COM,
golden_checksum
	* @Description Generates and populates the printer receive buffer (`receive_bufer`) with
	*              diagnostic self-test report data and triggers the internal print engine.
	*
*/				
void SetSelftestData(void)
{
#ifdef PSF20_R002_TSPL_PROTOCOL
    TsplFirmware_RequestSelfTest();
#else // legacy flow

	g_SelfTestStartFlag = 1;
	unsigned short m_type;
	unsigned short i,com,m_DaSpave;
	unsigned int m_judgeyear;
	char checksum_str[32];
		__SETPRIMASK();
	SysTickFlag = 0;
	g_TimeOutFlag = 0;
	m_ProKeyValue = 0;
	PrinterParameterInt();

 
	g_Receive_count = 0;//ï¿½ï¿½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	Receive_count = 0;//ï¿½ï¿½ï¿½ï¿½Õ¼ï¿½ï¿½ï¿½ï¿½ï¿½
	g_Order_count = 0;//ï¿½ï¿½ï¿½È¡ï¿½ï¿½ï¿½Ä´ï¿½ï¿½ï¿½	
	__RESETPRIMASK();
	g_SelfRunFlag = 0xAA;
	g_DumpReceiveFlage = 0xAAAA;	
	PRINTER_PDL.SYS._POWER_EN();
	s_PrintParameters.m_LineSpace = 6;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	if(g_PaperType == 0)
	{
		unsigned short align_steps = s_PrintrConfig.m_HotLineTOOUT * 32 + 44;
		PRINTER_PDL.SYS._Drive_Power_EN();
		PRINTER_PDL.MOTO._MotoRun_FX(&g_MotoAll, align_steps);
		PRINTER_PDL.SYS._Drive_Power_DIS();
		for(i=0; i<32; i++)
		{
			Delay1ms(2);
			SetFreValue(PRINTER_PDL.SYS._PaperGapDet());
		}
	}

	//50mm label
	if(g_label_size_final == 50)
	{
				INJECT_STR_TO_PRINTER(STR_SELFTEST_HEADER);
				INJECT_STR_TO_PRINTER(STR_PRINT_MODEL);
				INJECT_STR_TO_PRINTER(STR_PRINT_VERSION_LABEL);
			// Flash checksum to self test
#ifdef CHECKSUM		
      sprintf(checksum_str,"CHECKSUM: 0x%04X\r\n", golden_checksum);		
			INJECT_STR_TO_PRINTER(checksum_str);
#endif		

				i =0;
				while(CompileDate[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileDate[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x20;
				i =0;
				while(CompileTime[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileTime[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i = 0;
				m_DaSpave = (*p_REAndSAcount);//ï¿½Ãµï¿½ï¿½ï¿½ï¿½Ðºï¿½ï¿½ï¿½Ê¼Î»ï¿½ï¿½
				while(SelfTestSerial[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestSerial[i++];
				}
 
				m_judgeyear = 0;
 
				m_judgeyear = CompileDate[0]<<16;
				m_judgeyear |= CompileDate[1] << 8;
				m_judgeyear |= CompileDate[2];
			/*
			"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
 
			const char SelfTestSerial[]={"SN:32F06430081605\r"};
 
			*/
				receive_bufer[m_DaSpave+9] = '2';
				switch(m_judgeyear)
				{
					case 0x4A616E:
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x466562:
						receive_bufer[m_DaSpave+10] = '2';
						break;
					case 0x4D6172:
						receive_bufer[m_DaSpave+10] = '3';
						break;
					case 0x417072:
						receive_bufer[m_DaSpave+10] = '4';
						break;
					case 0x4D6179:
						receive_bufer[m_DaSpave+10] = '5';
						break;
					case 0x4A756E:
						receive_bufer[m_DaSpave+10] = '6';
						break;
					case 0x4A756C:
						receive_bufer[m_DaSpave+10] = '7';
						break;
					case 0x417567:
						receive_bufer[m_DaSpave+10] = '8';
						break;
					case 0x536570:
						receive_bufer[m_DaSpave+10] = '9';
						break;
					case 0x4F6374:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '0';
						break;
					case 0x4E6F76:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x446563:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '2';
						break;
				}
 
				receive_bufer[m_DaSpave+11] = CompileDate[4];
				if(CompileDate[4] == ' ')
				{
					receive_bufer[m_DaSpave+11] = '0'	  ;
				}
				receive_bufer[m_DaSpave+12] = CompileDate[5];
 
 
				receive_bufer[m_DaSpave+7] = CompileDate[9] + 5;
				receive_bufer[m_DaSpave+8] = CompileDate[10];
 
				receive_bufer[m_DaSpave+13] = CompileTime[0];
				receive_bufer[m_DaSpave+14] = CompileTime[1];
				receive_bufer[m_DaSpave+15] = CompileTime[3];
				receive_bufer[m_DaSpave+16] = CompileTime[4];
 
				i =0;
				while(SelfTest_S[i] != 0xff)
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_S[i++];
				}
			/*	for (i=0x20;i<0x100;i++)
				{
					receive_bufer[(*p_REAndSAcount)++] = i;
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;*/
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i =0;
				while(SelfTest_COM[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_COM[i++];
				}
				// Remove in Self Test
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				//receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
	}
	else if(g_label_size_final == 30)   //30mm Label
	{
	
				INJECT_STR_TO_PRINTER(STR_SELFTEST_HEADER);
				INJECT_STR_TO_PRINTER(STR_PRINT_MODEL);
				INJECT_STR_TO_PRINTER(STR_PRINT_VERSION_LABEL);
			// Flash checksum to self test
#ifdef CHECKSUM			
      sprintf(checksum_str,"CHECKSUM: 0x%04X\r\n", golden_checksum);		
			INJECT_STR_TO_PRINTER(checksum_str);
#endif	  
	
 
				i =0;
				while(CompileDate[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileDate[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x20;
				i =0;
				while(CompileTime[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileTime[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i = 0;
				m_DaSpave = (*p_REAndSAcount);//ï¿½Ãµï¿½ï¿½ï¿½ï¿½Ðºï¿½ï¿½ï¿½Ê¼Î»ï¿½ï¿½
				while(SelfTestSerial[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestSerial[i++];
				}
 
				m_judgeyear = 0;
 
				m_judgeyear = CompileDate[0]<<16;
				m_judgeyear |= CompileDate[1] << 8;
				m_judgeyear |= CompileDate[2];
			/*
			"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
 
			const char SelfTestSerial[]={"SN:32F06430081605\r"};
 
			*/
				receive_bufer[m_DaSpave+9] = '2';
				switch(m_judgeyear)
				{
					case 0x4A616E:
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x466562:
						receive_bufer[m_DaSpave+10] = '2';
						break;
					case 0x4D6172:
						receive_bufer[m_DaSpave+10] = '3';
						break;
					case 0x417072:
						receive_bufer[m_DaSpave+10] = '4';
						break;
					case 0x4D6179:
						receive_bufer[m_DaSpave+10] = '5';
						break;
					case 0x4A756E:
						receive_bufer[m_DaSpave+10] = '6';
						break;
					case 0x4A756C:
						receive_bufer[m_DaSpave+10] = '7';
						break;
					case 0x417567:
						receive_bufer[m_DaSpave+10] = '8';
						break;
					case 0x536570:
						receive_bufer[m_DaSpave+10] = '9';
						break;
					case 0x4F6374:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '0';
						break;
					case 0x4E6F76:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x446563:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '2';
						break;
				}
 
				receive_bufer[m_DaSpave+11] = CompileDate[4];
				if(CompileDate[4] == ' ')
				{
					receive_bufer[m_DaSpave+11] = '0'	  ;
				}
				receive_bufer[m_DaSpave+12] = CompileDate[5];
 
 
				receive_bufer[m_DaSpave+7] = CompileDate[9] + 5;
				receive_bufer[m_DaSpave+8] = CompileDate[10];
 
				receive_bufer[m_DaSpave+13] = CompileTime[0];
				receive_bufer[m_DaSpave+14] = CompileTime[1];
				receive_bufer[m_DaSpave+15] = CompileTime[3];
				receive_bufer[m_DaSpave+16] = CompileTime[4];
 
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
		    receive_bufer[(*p_REAndSAcount)++] = 0x0D;
			//	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				i =0;
				while(SelfTest_S[i] != 0xff)
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_S[i++];
				}
			/*	for (i=0x20;i<0x100;i++)
				{
					receive_bufer[(*p_REAndSAcount)++] = i;
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;*/
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i =0;
				while(SelfTest_COM[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_COM[i++];
				}
		   receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
			//	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
	}
	else if(g_label_size_final == 25)   //25mm Label
	{
		
				INJECT_STR_TO_PRINTER(STR_SELFTEST_HEADER);
				INJECT_STR_TO_PRINTER(STR_PRINT_MODEL);
				INJECT_STR_TO_PRINTER(STR_PRINT_VERSION_LABEL);
			// Flash checksum to self test
#ifdef CHECKSUM		
      sprintf(checksum_str,"CHECKSUM: 0x%04X\r\n", golden_checksum);		
			INJECT_STR_TO_PRINTER(checksum_str);
#endif	  
		
 
				i =0;
				while(CompileDate[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileDate[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x20;
				i =0;
				while(CompileTime[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileTime[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
		    receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i = 0;
				m_DaSpave = (*p_REAndSAcount);//ï¿½Ãµï¿½ï¿½ï¿½ï¿½Ðºï¿½ï¿½ï¿½Ê¼Î»ï¿½ï¿½
				while(SelfTestSerial[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestSerial[i++];
				}
 
				m_judgeyear = 0;
 
				m_judgeyear = CompileDate[0]<<16;
				m_judgeyear |= CompileDate[1] << 8;
				m_judgeyear |= CompileDate[2];
			/*
			"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
 
			const char SelfTestSerial[]={"SN:32F06430081605\r"};
 
			*/
				receive_bufer[m_DaSpave+9] = '2';
				switch(m_judgeyear)
				{
					case 0x4A616E:
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x466562:
						receive_bufer[m_DaSpave+10] = '2';
						break;
					case 0x4D6172:
						receive_bufer[m_DaSpave+10] = '3';
						break;
					case 0x417072:
						receive_bufer[m_DaSpave+10] = '4';
						break;
					case 0x4D6179:
						receive_bufer[m_DaSpave+10] = '5';
						break;
					case 0x4A756E:
						receive_bufer[m_DaSpave+10] = '6';
						break;
					case 0x4A756C:
						receive_bufer[m_DaSpave+10] = '7';
						break;
					case 0x417567:
						receive_bufer[m_DaSpave+10] = '8';
						break;
					case 0x536570:
						receive_bufer[m_DaSpave+10] = '9';
						break;
					case 0x4F6374:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '0';
						break;
					case 0x4E6F76:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x446563:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '2';
						break;
				}
 
				receive_bufer[m_DaSpave+11] = CompileDate[4];
				if(CompileDate[4] == ' ')
				{
					receive_bufer[m_DaSpave+11] = '0'	  ;
				}
				receive_bufer[m_DaSpave+12] = CompileDate[5];
 
 
				receive_bufer[m_DaSpave+7] = CompileDate[9] + 5;
				receive_bufer[m_DaSpave+8] = CompileDate[10];
 
				receive_bufer[m_DaSpave+13] = CompileTime[0];
				receive_bufer[m_DaSpave+14] = CompileTime[1];
				receive_bufer[m_DaSpave+15] = CompileTime[3];
				receive_bufer[m_DaSpave+16] = CompileTime[4];
				i =0;
				while(SelfTest_S[i] != 0xff)
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_S[i++];
				}
			/*	for (i=0x20;i<0x100;i++)
				{
					receive_bufer[(*p_REAndSAcount)++] = i;
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;*/
			//	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i =0;
				while(SelfTest_COM[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_COM[i++];
				}
		   receive_bufer[(*p_REAndSAcount)++] = 0x0D;
			 receive_bufer[(*p_REAndSAcount)++] = 0x0D;
			 receive_bufer[(*p_REAndSAcount)++] = 0x0D;
	}
	else
	{
		
				INJECT_STR_TO_PRINTER(STR_SELFTEST_HEADER);
				INJECT_STR_TO_PRINTER(STR_PRINT_MODEL);
				INJECT_STR_TO_PRINTER(STR_PRINT_VERSION_LABEL);
			
#ifdef CHECKSUM		
      sprintf(checksum_str,"CHECKSUM: 0x%04X\r\n", golden_checksum);		
			INJECT_STR_TO_PRINTER(checksum_str);
#endif		
		


				i =0;
				while(CompileDate[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileDate[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x20;
				i =0;
				while(CompileTime[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = CompileTime[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i = 0;
				m_DaSpave = (*p_REAndSAcount);//ï¿½Ãµï¿½ï¿½ï¿½ï¿½Ðºï¿½ï¿½ï¿½Ê¼Î»ï¿½ï¿½
				while(SelfTestSerial[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestSerial[i++];
				}
 
				m_judgeyear = 0;
 
				m_judgeyear = CompileDate[0]<<16;
				m_judgeyear |= CompileDate[1] << 8;
				m_judgeyear |= CompileDate[2];
			/*
			"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
 
			const char SelfTestSerial[]={"SN:32F06430081605\r"};
 
			*/
				receive_bufer[m_DaSpave+9] = '2';
				switch(m_judgeyear)
				{
					case 0x4A616E:
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x466562:
						receive_bufer[m_DaSpave+10] = '2';
						break;
					case 0x4D6172:
						receive_bufer[m_DaSpave+10] = '3';
						break;
					case 0x417072:
						receive_bufer[m_DaSpave+10] = '4';
						break;
					case 0x4D6179:
						receive_bufer[m_DaSpave+10] = '5';
						break;
					case 0x4A756E:
						receive_bufer[m_DaSpave+10] = '6';
						break;
					case 0x4A756C:
						receive_bufer[m_DaSpave+10] = '7';
						break;
					case 0x417567:
						receive_bufer[m_DaSpave+10] = '8';
						break;
					case 0x536570:
						receive_bufer[m_DaSpave+10] = '9';
						break;
					case 0x4F6374:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '0';
						break;
					case 0x4E6F76:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '1';
						break;
					case 0x446563:
						receive_bufer[m_DaSpave+9] = '3';
						receive_bufer[m_DaSpave+10] = '2';
						break;
				}
 
				receive_bufer[m_DaSpave+11] = CompileDate[4];
				if(CompileDate[4] == ' ')
				{
					receive_bufer[m_DaSpave+11] = '0'	  ;
				}
				receive_bufer[m_DaSpave+12] = CompileDate[5];
 
 
				receive_bufer[m_DaSpave+7] = CompileDate[9] + 5;
				receive_bufer[m_DaSpave+8] = CompileDate[10];
 
				receive_bufer[m_DaSpave+13] = CompileTime[0];
				receive_bufer[m_DaSpave+14] = CompileTime[1];
				receive_bufer[m_DaSpave+15] = CompileTime[3];
				receive_bufer[m_DaSpave+16] = CompileTime[4];
 
				i =0;
				while(SelfTest_S[i] != 0xff)
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_S[i++];
				}
			/*	for (i=0x20;i<0x100;i++)
				{
					receive_bufer[(*p_REAndSAcount)++] = i;
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;*/
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
 
				i =0;
				while(SelfTest_COM[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTest_COM[i++];
				}
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
				receive_bufer[(*p_REAndSAcount)++] = 0x0D;
	}
//	receive_bufer[(*p_REAndSAcount)++] = 0x12;
//	receive_bufer[(*p_REAndSAcount)++] = 0xAD;//ï¿½ï¿½ï¿½ï¿½DUMPÄ£Ê½
//	receive_bufer[(*p_REAndSAcount)++] = 0x1B;
//	receive_bufer[(*p_REAndSAcount)++] = 0x40;
//	receive_bufer[(*p_REAndSAcount)++] = 0x1B;
//	receive_bufer[(*p_REAndSAcount)++] = 0x31;
//	receive_bufer[(*p_REAndSAcount)++] = 0x06;
 
//	receive_bufer[(*p_REAndSAcount)++] = 0x1B;//ï¿½ï¿½ï¿½ï¿½ï¿½Ç©Öµ
//	receive_bufer[(*p_REAndSAcount)++] = 0xBC;
//	receive_bufer[(*p_REAndSAcount)++] = 0x53;
//	receive_bufer[(*p_REAndSAcount)++] = 0xAA;
//	receive_bufer[(*p_REAndSAcount)++] = 0x12;
//	receive_bufer[(*p_REAndSAcount)++] = 0x11;
 
	(*p_SaveDrREcount) = (*p_REAndSAcount);

 
	PRINTER_PDL.SYS._WDG_ReloadCounter();
 

#endif
}

#else
void SetSelftestData(void)
{
#ifdef PSF20_R002_TSPL_PROTOCOL
    TsplFirmware_RequestSelfTest();
#else // legacy flow

	unsigned short m_type;
	unsigned short i,com,m_DaSpave;
	unsigned int m_judgeyear;

		__SETPRIMASK();
	PrinterParameterInt();
	g_Receive_count = 0;//Çå½ÓÊÕ»º´æ¼ÆÊýÆ÷
	Receive_count = 0;//Çå½ÓÊÕ¼ÆÊýÆ÷
	g_Order_count = 0;//Çå½ÓÈ¡Êý¼Ä´æÆ÷	
	__RESETPRIMASK();
	g_SelfRunFlag = 0xAA;
	g_DumpReceiveFlage = 0xAAAA;	
PRINTER_PDL.SYS._POWER_EN();
	s_PrintParameters.m_LineSpace = 6;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	i =0;
	while(CompileDate[i])
	{
		receive_bufer[(*p_REAndSAcount)++] = CompileDate[i++];
	}
	receive_bufer[(*p_REAndSAcount)++] = 0x20;
	i =0;
	while(CompileTime[i])
	{
		receive_bufer[(*p_REAndSAcount)++] = CompileTime[i++];
	}
	receive_bufer[(*p_REAndSAcount)++] = 0x0D;

	i = 0;
	m_DaSpave = (*p_REAndSAcount);//µÃµ½ÐòÁÐºÅÆðÊ¼Î»ÖÃ
	while(SelfTestSerial[i])
	{
		receive_bufer[(*p_REAndSAcount)++] = SelfTestSerial[i++];
	}

	m_judgeyear = 0;

	m_judgeyear = CompileDate[0]<<16;
	m_judgeyear |= CompileDate[1] << 8;
	m_judgeyear |= CompileDate[2];
/*
"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"

const char SelfTestSerial[]={"SN:32F06430081605\r"};

*/
	receive_bufer[m_DaSpave+9] = '2';
	switch(m_judgeyear)
	{
		case 0x4A616E:
			receive_bufer[m_DaSpave+10] = '1';
			break;
		case 0x466562:
			receive_bufer[m_DaSpave+10] = '2';
			break;
		case 0x4D6172:
			receive_bufer[m_DaSpave+10] = '3';
			break;
		case 0x417072:
			receive_bufer[m_DaSpave+10] = '4';
			break;
		case 0x4D6179:
			receive_bufer[m_DaSpave+10] = '5';
			break;
		case 0x4A756E:
			receive_bufer[m_DaSpave+10] = '6';
			break;
		case 0x4A756C:
			receive_bufer[m_DaSpave+10] = '7';
			break;
		case 0x417567:
			receive_bufer[m_DaSpave+10] = '8';
			break;
		case 0x536570:
			receive_bufer[m_DaSpave+10] = '9';
			break;
		case 0x4F6374:
			receive_bufer[m_DaSpave+9] = '3';
			receive_bufer[m_DaSpave+10] = '0';
			break;
		case 0x4E6F76:
			receive_bufer[m_DaSpave+9] = '3';
			receive_bufer[m_DaSpave+10] = '1';
			break;
		case 0x446563:
			receive_bufer[m_DaSpave+9] = '3';
			receive_bufer[m_DaSpave+10] = '2';
			break;
	}

	receive_bufer[m_DaSpave+11] = CompileDate[4];
	if(CompileDate[4] == ' ')
	{
		receive_bufer[m_DaSpave+11] = '0'	  ;
	}
	receive_bufer[m_DaSpave+12] = CompileDate[5];


	receive_bufer[m_DaSpave+7] = CompileDate[9] + 5;
	receive_bufer[m_DaSpave+8] = CompileDate[10];

	receive_bufer[m_DaSpave+13] = CompileTime[0];
	receive_bufer[m_DaSpave+14] = CompileTime[1];
	receive_bufer[m_DaSpave+15] = CompileTime[3];
	receive_bufer[m_DaSpave+16] = CompileTime[4];

	i =0;
	while(SelfTest_S[i] != 0xff)
	{
		receive_bufer[(*p_REAndSAcount)++] = SelfTest_S[i++];
	}
	for (i=0x20;i<0x100;i++)
	{
		receive_bufer[(*p_REAndSAcount)++] = i;
	}
	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
	receive_bufer[(*p_REAndSAcount)++] = 0x0D;

	i =0;
	while(SelfTest_COM[i])
	{
		receive_bufer[(*p_REAndSAcount)++] = SelfTest_COM[i++];
	}
	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
	receive_bufer[(*p_REAndSAcount)++] = 0x12;
	receive_bufer[(*p_REAndSAcount)++] = 0xAD;//ÉèÖÃDUMPÄ£Ê½
	receive_bufer[(*p_REAndSAcount)++] = 0x1B;
	receive_bufer[(*p_REAndSAcount)++] = 0x40;
	receive_bufer[(*p_REAndSAcount)++] = 0x1B;
	receive_bufer[(*p_REAndSAcount)++] = 0x31;
	receive_bufer[(*p_REAndSAcount)++] = 0x06;

	receive_bufer[(*p_REAndSAcount)++] = 0x1B;//¼ÆËã±êÇ©Öµ
	receive_bufer[(*p_REAndSAcount)++] = 0xBC;
	receive_bufer[(*p_REAndSAcount)++] = 0x53;
	receive_bufer[(*p_REAndSAcount)++] = 0xAA;
	receive_bufer[(*p_REAndSAcount)++] = 0x12;
	receive_bufer[(*p_REAndSAcount)++] = 0x11;

	(*p_SaveDrREcount) = (*p_REAndSAcount);

	PRINTER_PDL.SYS._WDG_ReloadCounter();

#endif
}
#endif


void SetSerialContrl(unsigned char baudvalue)
{
	const char*  p_SetDataParity;
	unsigned char i;

	switch(baudvalue&0xF)
	{
		case 0x0:
			p_SetDataParity = SelfTestD_Ctrl;
			break;
		case 0x1:
			p_SetDataParity = SelfTestD_XFCtrl;
			break;
	}
	i =0;
	while(p_SetDataParity[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}


}
void DefaultSet(void)
{

	unsigned char i;
	i =0;
	while(Setresort[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =Setresort[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}


}

void SetSoftwarePart(unsigned char baudvalue)
{
	const char*  p_SetDataParity;
	unsigned char i;

	i =0;
	p_SetDataParity = Setenergy;
	while(p_SetDataParity[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}

	(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
	receive_bufer[(*p_SaveDrREcount)++] = (6 - baudvalue) + 0x30;
	(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}


}

void SetPrintFont(unsigned char baudvalue)
{
	const char*  p_SetDataParity;
	unsigned char i;

	switch(baudvalue&0xF)
	{
		case 0x0:
			p_SetDataParity = GB2312_24x24;
			break;
		case 0x1:
			p_SetDataParity = GB18030_24x24;
			break;
		case 0x2:
			p_SetDataParity = GB2312_16x16;
			break;
	}

	i =0;
	while(p_SetDataParity[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}


}


void SetPrtintDir(unsigned char baudvalue)
{
	const char*  p_SetDataParity;
	unsigned char i;

	switch(baudvalue&0xF)
	{
		case 0x0:
			p_SetDataParity = Self_Forward;
			break;
		case 0x1:
			p_SetDataParity = Self_Reverse;
			break;
	}

	i =0;
	while(p_SetDataParity[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}


}

void SetPrtintDataParity(unsigned char baudvalue)
{
	const char*  p_SetDataParity;
	unsigned char i;

	SetDataBaud(g_cruve_arry[1]);
	switch(baudvalue&0x3)
	{
		case 0x0:
			p_SetDataParity = SelfTestD_8N1;
			break;
		case 0x1:
			p_SetDataParity = SelfTestD_8O1;
			break;
		case 0x2:
			p_SetDataParity = SelfTestD_8E1;
			break;
		case 0x3:
			p_SetDataParity = SelfTestD_7E1;
			break;
		case 0x4:
			p_SetDataParity = SelfTestD_8N1;
	}
	i =0;
	while(p_SetDataParity[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
}
void SetDataBaud(unsigned char baudvalue)
{
	const char*  p_SetDataBaude;
	unsigned char i;

	switch(baudvalue&0xF)
	{
		case 0x0:
	//150
			p_SetDataBaude = SelfTestD_150;
			break;
		case 0x1:
	//300
			p_SetDataBaude = SelfTestD_300;
			break;
		case 0x2:
	//600
			p_SetDataBaude = SelfTestD_600;
			break;
		case 0x3:
	//1200
			p_SetDataBaude = SelfTestD_1200;
			break;
		case 0x4:
	//2400
			p_SetDataBaude = SelfTestD_2400;
			break;
		case 0x5:
	//4800
			p_SetDataBaude = SelfTestD_4800;
			break;
		case 0x6:
	//9600
			p_SetDataBaude = SelfTestD_9600;
			break;
		case 0x7:
	//19200  0000
			p_SetDataBaude = SelfTestD_19200;
			break;
		case 0x8:
	//19200  0000
			p_SetDataBaude = SelfTestD_38400;
			break;
		case 0x9:
	//19200  0000
			p_SetDataBaude = SelfTestD_57600;
			break;
		case 0xA:
	//19200  0000
			p_SetDataBaude = SelfTestD_115200;
			break;
	}
	i =0;
	while(p_SetDataBaude[i])
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataBaude[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
}

void SetPrtintDataBaud(unsigned char baudvalue)
{

	unsigned char i;

	SetDataBaud(baudvalue);
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//»º³åÇøµÃµ½µÄÊý¾Ý¸öÊý
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
}
//¶ÔÉä¹âµç
unsigned short GetDETPaper(void)
{
	unsigned short dat;
	#ifdef AD_PAPER_GAP_FX
	dat = 1200 - g_PaperGapAD;
	#endif
	#ifndef AD_PAPER_GAP_FX
	dat = g_PaperGapAD;
	#endif
	return dat;

}

//·´Éä¹âµç

unsigned short GetPaper(void)
{
	unsigned short dat;
	#ifdef AD_PAPER_FX
	//dat = 1200 - PRINTER_PDL.ADC._GetRefPhotoelectric();
	dat = 1200 - g_PaperOutAD;
	#endif
	#ifndef AD_PAPER_FX
	//dat = PRINTER_PDL.ADC._GetRefPhotoelectric();
	dat = g_PaperOutAD;
	#endif
	if(PRINTER_PDL.SYS._GetHSStatus())
	{
		//Èç¹ûÃ»ÓÐºÐ¸ÇÒ²ÊÇ0
		return 0;
	}
	
	return dat;

}
 /****************************************************************************
 * º¯ÊýÃû£º PrinterLPS()
 * ¹¦  ÄÜ£º¼ì²âÈ±Ö½

 ****************************************************************************/

#ifdef NEW_GARBAGE_PRINT_AVOID_LOGIC
void PrinterLPS(void)
{
    unsigned int count;
    unsigned int m_sysTime = 0;
    unsigned int DelayCount = 0;
    unsigned int drain_timer = 0;
    unsigned char data;
    unsigned char fault_detected = 0;

#ifdef AUDIO_BEEP_SEQUENCE
    unsigned int current_paper_out = 0;
    unsigned int current_lid_open = 0;

    if (g_LPSStatus < g_PE_Standard)
        current_paper_out = 1;

    if (PRINTER_PDL.SYS._GetHSStatus())
        current_lid_open = 1;

    BuzzerManager_Init(current_paper_out, current_lid_open);
#endif

    g_LPSStatus = GetPaper();
    PRINTER_PDL.SYS._WDG_ReloadCounter();

#ifdef SAFE_PRINTING

    /*
     * Enter recovery if the lid is open or media is absent.
     * The interrupted print job will be discarded.
     */
    if ((g_LPSStatus < g_PE_Standard) || PRINTER_PDL.SYS._GetHSStatus())
    {
        fault_detected = 1;
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
        BatteryLowCharge_LabelAborted();
#endif			
#ifdef FIXED_LATE_BATTERY_DISPLAY
				g_BatteryBarPrintState = BATTERY_BAR_IDLE;
#endif
        /*
         * Stop the active print mechanism.
         */
        s_PrinterMOTOPar.m_PrintEnd    = ENABLE_VAR;
        s_PrinterMOTOPar.m_StopPrint   = ENABLE_VAR;
        s_PrinterMOTOPar.m_MotoRun     = DISABLE_VAR;
        s_PrinterMOTOPar.m_PrintLSbit  = DISABLE_VAR;
        s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;

        PRINTER_PDL.MOTO._MotoDisable();
        PRINTER_PDL.SYS._Drive_Power_DIS();
        PRINTER_PDL.SYS._STB_ALL_CLEAR();

        /*
         * Discard the interrupted job and reset its parser state.
         * Protect the shared receive-buffer counters from the ISR.
         */
        __SETPRIMASK();

        Receive_count   = 0;
        g_Order_count   = 0;
        g_Receive_count = 0;

        __RESETPRIMASK();

        s_PrintOrderstr.m_GetdataPos = 0;
        s_PrintOrderstr.m_DataFlag   = 0;
        g_BufferSwitch               = 0;
        g_dotSwitch                  = 0;
        g_0D0AFlag                   = 0;
        g_PrintStatusReg             = 0;

        PrinterParameterInt();

        g_DiscardPrinting = 1;

        /*
         * Wait for lid closure and media availability.
         * Discard any data received while waiting.
         */
				 
				 
				 
        while ((GetPaper() < g_PE_Standard) || PRINTER_PDL.SYS._GetHSStatus())
        {
            g_LPSStatus = GetPaper();
            g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();

            if (PRINTER_PDL.SYS._GetHSStatus())
            {
                mediaType = "DOOR_OPEN";
								Get_Media_Detect_BT_Response(mediaType);
								SetCoverStatus(DIS_COV_X,DIS_COV_Y,0);
								DisplayStr8x16char("                ", 0, 14, 1);
								unsigned int m_w = GetStr8x16Wide((char*)"Lid Open", 1);
								DisplayStr8x16char("Lid Open", (128-m_w)/2, 14, 1);
            }
            else
            {
                SetCoverStatus(DIS_COV_X, DIS_COV_Y, 1);
            }

            if (g_LPSStatus < g_PE_Standard)
            {
                SetPrinterStatus(DIS_PE_X,DIS_PE_Y,0);
								if(!PRINTER_PDL.SYS._GetHSStatus())
								{
									mediaType = "NO_PAPER";
									Get_Media_Detect_BT_Response(mediaType);
									DisplayStr8x16char("                ", 0, 14, 1);
									unsigned int m_w = GetStr8x16Wide((char*)"Media Absent", 1);
									DisplayStr8x16char("Media Absent", (128-m_w)/2, 14, 1);
									}
						}
						//            else   //Added below due to modification in the garbage printing logic in this function (Modify By: Kajal ---> 10 Oct 2026)
//						{ 
//							mediaType = "PAPER_READY";
//							Get_Media_Detect_BT_Response(mediaType);
//							SetPrinterStatus(DIS_PE_X,DIS_PE_Y,1);	
//							mediaType = "READY";
//							Get_Media_Detect_BT_Response(mediaType);		
//							
//						}

            DisplayBTPro();
            DisplayBufEn();
            DisplayBatty(DIS_BAT_X, DIS_BAT_Y);

#ifdef AUDIO_BEEP_SEQUENCE
            current_paper_out =
                (g_LPSStatus < g_PE_Standard) ? 1 : 0;

            current_lid_open =
                PRINTER_PDL.SYS._GetHSStatus() ? 1 : 0;

            if ((current_paper_out != g_BuzzerCtx.prev_paper_status) ||
                (current_lid_open != g_BuzzerCtx.prev_lid_status))
            {
                BuzzerManager_Init(current_paper_out,
                                   current_lid_open);
            }

            Process_Buzzer_State_Machine();
#endif

            /*
             * Never parse print bytes while the job is discarded.
             * Clear newly received bytes atomically.
             */
            __SETPRIMASK();

            Receive_count   = 0;
            g_Order_count   = 0;
            g_Receive_count = 0;

            __RESETPRIMASK();

            PRINTER_PDL.SYS._WDG_ReloadCounter();
            Delay1ms(10);
        }

        /*
         * Lid is closed and media is present.
         * Allow trailing communication bytes to arrive, and discard them.
         * The timer restarts whenever data is received.
         */
        drain_timer = 0;

        while (drain_timer < 400)
        {
            Delay1ms(1);

            __SETPRIMASK();

            if (Receive_count != 0)
            {
                Receive_count   = 0;
                g_Order_count   = 0;
                g_Receive_count = 0;

                __RESETPRIMASK();

                drain_timer = 0;
            }
            else
            {
                __RESETPRIMASK();

                drain_timer++;
            }

            PRINTER_PDL.SYS._WDG_ReloadCounter();
        }

        /*
         * Reset parser state again after the receive buffer is drained.
         */
        __SETPRIMASK();

        Receive_count   = 0;
        g_Order_count   = 0;
        g_Receive_count = 0;

        __RESETPRIMASK();

        s_PrintOrderstr.m_GetdataPos = 0;
        s_PrintOrderstr.m_DataFlag   = 0;
        g_BufferSwitch               = 0;
        g_dotSwitch                  = 0;
        g_0D0AFlag                   = 0;
        g_PrintStatusReg             = 0;

        PrinterParameterInt();
				
		/****Before Calibration send command to application that paper is ready ******/
				mediaType = "PAPER_READY";
				Get_Media_Detect_BT_Response(mediaType);
				SetPrinterStatus(DIS_PE_X,DIS_PE_Y,1);	
				mediaType = "READY";
				Get_Media_Detect_BT_Response(mediaType);

        /*
         * Calibrate once after recovery.
         */
#ifdef PRINTER_LABEL
#ifdef POWER_ON_CAL
        lid_mediacal();
				g_Lab_head_Feed = 0xA537;   //Set flag to pull the paper back for safe print the next label
#else
        if (g_PaperType == 0)
        {
            Delay1ms(500);
            RunFeedLabPaper();
        }
#endif
#endif

        /*
         * The interrupted job has been discarded.
         * Allow subsequent, newly submitted jobs.
         */
        g_DetPaperValueFlag = 0xA5EF;
        g_DetPV_Count = 0;
        g_ProFlagVelue = 0xAA00;
        g_PaperEnd = 0;
#ifndef FIXED_LATE_BATTERY_DISPLAY
        SetCoverStatus(DIS_COV_X, DIS_COV_Y, 1);
        SetPrinterStatus(DIS_PE_X, DIS_PE_Y, 1);
        DisplayBufEn();
#endif
        g_LedPFlag = 0xA100;

        PRINTER_PDL.SYS._LED_Stauts_OFF();
#ifdef FIXED_LATE_BATTERY_DISPLAY
				g_BatteryBarPrintState = BATTERY_BAR_IDLE; // Ensure clean idle state before returning
#endif
        g_DiscardPrinting = 0;

        /*
         * Do not feed or parse the discarded print job again.
         */
        return;
    }

#else

    /*
     * Keep the existing non-SAFE_PRINTING implementation here
     * if this function is built for legacy configurations.
     */

#endif

    /*
     * No lid/media fault: preserve the existing normal print path.
     * This replacement must be merged with the remainder of your
     * original PrinterLPS() function for normal operation.
     */
}

#else
void PrinterLPS(void)
{
 	unsigned int count,m_sysTime;
	 	unsigned int DelayCount;
	 unsigned char data;
	 
#ifdef AUDIO_BEEP_SEQUENCE
    unsigned int current_paper_out = 0;
    unsigned int current_lid_open = 0;
    
    // Initial evaluation to kickstart the state machine
    if(g_LPSStatus < g_PE_Standard) current_paper_out = 1;
    if(PRINTER_PDL.SYS._GetHSStatus()) current_lid_open = 1;
    BuzzerManager_Init(current_paper_out, current_lid_open);
#endif	 
		 
  g_LPSStatus = GetPaper();//AD_Value[1]>>4;
//	MyPrintf("*X ");
 	PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
  if(g_LPSStatus < g_PE_Standard)
 	{
//		MyPrintf("*1 ");
 		for(count = 0;count<0xA50;count++);
		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
			
  		g_LPSStatus = GetPaper();// AD_Value[1]>>4;
		if(g_LPSStatus > g_PE_Standard)
 		{
 			return;
 		}
 		for(count = 0;count<0x200;count++);
		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
			
 		g_LPSStatus = GetPaper();// AD_Value[1]>>4;
 		if(g_LPSStatus > g_PE_Standard)
 		{
 			return;
 		}
 	//	g_PaperEnd = 0xAA55;

		count= 0;
		g_sysTime = 0;
		g_sysTime = 0;
		// printf("PE:%d\r\n",g_PaperOutAD);
		
 lps_feed:
		DelayCount = 0;
 		while(g_LPSStatus < g_PE_Standard)
 		{
			
			if((m_sysTime + 200) < g_sysTime)
			{
				m_sysTime = g_sysTime;
				
#ifdef LID_MEDIA_DISPLAY_SETTING
				if(PRINTER_PDL.SYS._GetHSStatus())
				{
					mediaType = "DOOR_OPEN";
				  Get_Media_Detect_BT_Response(mediaType);
					SetCoverStatus(DIS_COV_X,DIS_COV_Y,0);
					DisplayStr8x16char("                ", 0, 14, 1);
					unsigned int m_w = GetStr8x16Wide((char*)"Lid Open", 1);
					DisplayStr8x16char("Lid Open", (128-m_w)/2, 14, 1);
	
					#ifdef LID_OPEN_MEDIA_ABSENT_AVOID_GARBAGE_PRINT	
          LID_Detection_Flag = 1;					
						if(Receive_count)
						{
							MyPrintf("*LO\r\n");
							__SETPRIMASK();   // Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï
							Receive_count = 0;
							g_Order_count = 0;
							data = 0;
							__RESETPRIMASK();//½ûÖ¹´®¿Ú1ÖÐ¶Ï
						}
					#endif
					
				}
				else
				{
					SetCoverStatus(DIS_COV_X,DIS_COV_Y,1);			
				}					
				if(g_PaperOutAD < g_PE_Standard  )
				{
					SetPrinterStatus(DIS_PE_X,DIS_PE_Y,0);
					if(!PRINTER_PDL.SYS._GetHSStatus())
					{
						mediaType = "NO_PAPER";
						Get_Media_Detect_BT_Response(mediaType);
						DisplayStr8x16char("                ", 0, 14, 1);
						unsigned int m_w = GetStr8x16Wide((char*)"Media Absent", 1);
						DisplayStr8x16char("Media Absent", (128-m_w)/2, 14, 1);
						
						#ifdef LID_OPEN_MEDIA_ABSENT_AVOID_GARBAGE_PRINT	
							LID_Detection_Flag = 1;
								if(Receive_count)
								{
									MyPrintf("*MA\r\n");
									__SETPRIMASK();   // Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï
									Receive_count = 0;
									g_Order_count = 0;
									data = 0;
									__RESETPRIMASK();//½ûÖ¹´®¿Ú1ÖÐ¶Ï
								}
						#endif
						
					}
				}
				else
				{ 
					mediaType = "PAPER_READY";
					Get_Media_Detect_BT_Response(mediaType);
					SetPrinterStatus(DIS_PE_X,DIS_PE_Y,1);	
					mediaType = "READY";
					Get_Media_Detect_BT_Response(mediaType);		
					
				}
#else				
				if(PRINTER_PDL.SYS._GetHSStatus())
				{
					SetCoverStatus(DIS_COV_X,DIS_COV_Y,0);			
				}
				else
				{
					SetCoverStatus(DIS_COV_X,DIS_COV_Y,1);			
				}					
				if(g_PaperOutAD < g_PE_Standard  )
				{
					SetPrinterStatus(DIS_PE_X,DIS_PE_Y,0);			
				}
				else
				{
					SetPrinterStatus(DIS_PE_X,DIS_PE_Y,1);			
				}
#endif				
				DisplayBTPro();
				DisplayBufEn();
			}					
			DisplayBatty(DIS_BAT_X,DIS_BAT_Y);	
			/* Refresh the on-screen head temperature while printer status is refreshed. */
			#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
			DisplayTemp(DIS_TEMP_X, DIS_TEMP_Y);
			#endif

#ifdef AUDIO_BEEP_SEQUENCE			
        current_paper_out = (g_LPSStatus < g_PE_Standard) ? 1 : 0; // if true set 1 (ternary operator) else set 0
        current_lid_open = PRINTER_PDL.SYS._GetHSStatus() ? 1 : 0;
        
        
        if ((current_paper_out != g_BuzzerCtx.prev_paper_status) || 
					(current_lid_open != g_BuzzerCtx.prev_lid_status)) {
            BuzzerManager_Init(current_paper_out, current_lid_open);
        }
        
        // Execute non-blocking audio logic
        Process_Buzzer_State_Machine();
#else			
			if((g_sysTime == 1000)||(g_sysTime == 2000)||(g_sysTime == 3000))
			{
				PRINTER_PDL.SYS._BEEP_EN();
				Delay1ms(100);			
				PRINTER_PDL.SYS._BEEP_DIS();
				Delay1ms(100);	
				g_sysTime++;
				g_sysTime++;
			}
#endif
			
			
			g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
				
 			g_LPSStatus = GetPaper();//AD_Value[1]>>4;
 			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
			
 			while(PRINTER_PDL.SYS._GetKey_LF() == 0)
 			{
				Delay1ms(10);
				DelayCount++;
				
				if(DelayCount > 500)
				{
						//»Ö¸´Ä¬ÈÏÖµ
					g_LedPFlag = 0;
					PRINTER_PDL.SYS._LED_Stauts_ON();
					PRINTER_PDL.SYS._LED_Stauts_ON();
					PRINTER_PDL.SYS._LED_Stauts_ON();
					PrinterConfigInt();
					PRINTER_PDL.SYS._WDG_ReloadCounter();
#ifndef PAPER_FEED_SOLUTION					
					SavePrinterConfig();
					PRINTER_PDL.SYS._WDG_ReloadCounter();
					ReadPrinterConfig();
#endif					
					Delay1ms(2000);
					DelayCount = 0;
					g_LedPFlag = 0xA100;
				}
 			}
			
			if(PRINTER_PDL.SYS._GetKey_LF())
			{
				DelayCount = 0;				
			}
			g_LedPFlag = 0xA100;
	
#ifdef LID_OPEN_MEDIA_ABSENT_AVOID_GARBAGE_PRINT			
			if(Receive_count)
			{
				MyPrintf("*112\r\n");
				__SETPRIMASK();   // Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï
				Receive_count = 0;
				g_Order_count = 0;
				data = 0;
				__RESETPRIMASK();//½ûÖ¹´®¿Ú1ÖÐ¶Ï
			}
#else			
			if(Receive_count)
			{
				__SETPRIMASK();   // Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï
				Receive_count--;
				__RESETPRIMASK();//½ûÖ¹´®¿Ú1ÖÐ¶Ï

				data = receive_bufer[g_Order_count++];
				g_Order_count &= MASK_DATA;

#ifdef PSF20_R002_ESC_POS_PROTOCOL
				NpReturnStatus(data);
#else // legacy flow is selected only for ESC/POS
                TsplFirmware_ReceiveError();
#endif
				
			}
#endif


 		}
	
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();

		//	printf("PE:%d\r\n",g_PaperOutAD);
		
 		for(count = 0;count<50;count++);
 		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
 		for(count = 0;count<0xA00;count++);
 		g_LPSStatus = GetPaper();//AD_Value[1]>>4;
 		if(g_LPSStatus < g_PE_Standard)
 		{
 			goto lps_feed;
 		}
 			
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
	
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
 		for(count = 0;count<0x150;count++);
 		g_LPSStatus = GetPaper();//AD_Value[1]>>4;
 		if(g_LPSStatus < g_PE_Standard)
 		{
 			goto lps_feed;
 		}
		#ifdef AUTO_PAPERFEED
		for(count = 0;count<0x200000;count++)
			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
			AutoMOTORUN();
		#endif
		#ifdef PRINTER_AND_USB
		for(count = 0;count<0x800;count++)
			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
		#endif
		#ifndef PRINTER_AND_USB
		for(count = 0;count<0x200000;count++)
			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
		#endif
			SetCoverStatus(DIS_COV_X,DIS_COV_Y,1);
			SetPrinterStatus(DIS_PE_X,DIS_PE_Y,1);	
			DisplayBufEn();
		g_DetPaperValueFlag = 0xA5EF;
		g_DetPV_Count = 0;


		#ifdef PRINTER_LABEL
#ifdef POWER_ON_CAL
		lid_mediacal();
#else
		if(g_PaperType == 0)
		{
			Delay1ms(500);
			//RunFeedLabPaperLPS();
			RunFeedLabPaper();
		}
#endif		
		#endif
		
		while(Receive_count)
		{
#ifdef LID_OPEN_MEDIA_ABSENT_AVOID_GARBAGE_PRINT	
						MyPrintf("*113\r\n");
			__SETPRIMASK();   // Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï
					Receive_count = 0;
					g_Order_count = 0;
					data = 0;
		  __RESETPRIMASK();//½ûÖ¹´®¿Ú1ÖÐ¶Ï
#else			
			//MyPrintf("*2 ");
				__SETPRIMASK();   // Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï
				Receive_count--;
				__RESETPRIMASK();//½ûÖ¹´®¿Ú1ÖÐ¶Ï

				data = receive_bufer[g_Order_count++];
				g_Order_count &= MASK_DATA;

#ifdef PSF20_R002_ESC_POS_PROTOCOL
				NpReturnStatus(data);
#else // legacy flow is selected only for ESC/POS
                TsplFirmware_ReceiveError();
#endif
#endif
			
		}

		s_PrintOrderstr.m_GetdataPos = 0;

		//if(s_PrintParameters.m_InterFace == INTERFACE_SERIAL)
		//{
		//	SET_L_BUSY_S;
		//}
		//else
		//{
		//	SET_L_BUSY_P;
		//}
			g_ProFlagVelue = 0xaa00;
	//	PrinterIdle();//ÉèÖÃ´òÓ¡»ú¿ÕÏÐ
		#ifdef RD_TYPE_PARALLEL
		#ifdef RD_PARALLEL_PE
		SET_L_PE_P;
		#endif
		#endif

		PRINTER_PDL.SYS._LED_Stauts_OFF();


 		g_PaperEnd = 0;
		#ifdef RD_TYPE_PARALLEL
		PRINTER_PDL.PARALLEL._ClearPE();
		#endif
 	}
}
#endif




void GetLabPaperSta(void)
{
	unsigned short i,j;
	unsigned int count;
		unsigned short RunstarStepTime,StepTime;
	unsigned short Feedcont;
	double a,valu_a;
	PRINTER_PDL.SYS._Drive_Power_EN();
	g_CJCalfalg = 0;
	for(i=0;i<30;i++)
	{
		Delay1ms(2);
		PRINTER_PDL.SYS._PaperGapDet();	
		SetFreValue(PRINTER_PDL.SYS._PaperGapDet());
		PRINTER_PDL.SYS._Drive_Power_Trig();
	}

	
	count = 3200;
	valu_a = 0;
	RunstarStepTime = 3000;
	j=0;
	Feedcont =0;
	while(count--)
	{
		
		PRINTER_PDL.MOTO._MotoDisable();
		PRINTER_PDL.SYS._Drive_Power_Trig();
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog ÖØ×°ÔØ¿´ÃÅ¹·¶¨Ê±Æ÷
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],400);
		g_Tim3ProFlag = 0;
		
	//	GetNoPaperStatus();	
		if(RunstarStepTime > (j * 100))
		{
			StepTime = RunstarStepTime - j * 100;
		}
		else
		{
			StepTime = 0;
		}		
		if(StepTime < 500)
		{
			StepTime = 500;
		}			
		j++;
		
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0);


		g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		
//printf("%d ",g_PaperGapAD);
		if(g_CJCalfalg == 0xA565)
		{
			linearRegression_0_30();
			g_CJCalfalg = 0;
		}
		if((g_MotoAll & 1) == 0)
		{
			Feedcont++;
			SetFreValue(GetDETPaper());//ÉèÖÃ²ÉÑù¹âµçÖµ
			if(Feedcont > 32 )
			{
				g_CJCalfalg = 0xA565;
				linearRegression_32(&a);
				if(valu_a < a)
				{
					valu_a = a;
				}
#ifdef MY_DEBUG_ENABLE				
				printf("%2.3f ",a);
#endif				
			}
			else
			{
				linearRegression_0_30();
			}
		}
		if(valu_a > 4)		
		{
			if(count > 32*5)
			{
				count = 32*5;
			}
		}


	}	
		
//	PRINTER_PDL.RS232._SendChar(0xAA);
//	PRINTER_PDL.RS232._SendChar(0);
//	
//	PRINTER_PDL.RS232._SendChar(data_L);
//	PRINTER_PDL.RS232._SendChar(data_L>>8);

//	PRINTER_PDL.RS232._SendChar(data_H);
//	PRINTER_PDL.RS232._SendChar(data_H>>8);
	
PRINTER_PDL.MOTO._MotoDisable();

	s_PrintrConfig.m_PE_LabValue_H =  ((unsigned short)valu_a /2 );//data_H- (i *5);//5

//	printf("s_PrintrConfig.m_PE_LabValue_H:%d \r\n",s_PrintrConfig.m_PE_LabValue_H);

	if(s_PrintrConfig.m_PE_LabValue_H < 4 )
	{
		//Á¬ÐøÖ½
		g_PaperType = 1;
		s_PrintrConfig.m_InterFace = 1;
	}
	else
	{
		g_PaperType = 0;
		s_PrintrConfig.m_InterFace = 0;
	}	
#ifndef PAPER_FEED_SOLUTION	
	SavePrinterConfig();
#endif	
//	printf("s_PrintrConfig.m_PE_LabValue_H: %04X  g_PaperType= %d\r\n",s_PrintrConfig.m_PE_LabValue_H ,g_PaperType);
	
	if(g_PaperType == 0)
	{
#ifdef POWER_ON_CAL		
		if (g_CAL_Mode_statusFlag == 1)
		{
			//we need to measure label size, so find the next gap
			RunFeedLabPaper();
		}
		else
		{
			//lid closed ( we already found a gap)
			//just align to tear bar
			unsigned int runcount = g_HotLineTOPD * 32 + g_HotLineTOOUT * 32;
			if (runcount > 160) runcount -= 160; else runcount = 0; // Deduct the 160 steps 
			
			StepTime = 950;
			for(;runcount>0;runcount--)
			{
				PRINTER_PDL.SYS._Drive_Power_Trig();
				PRINTER_PDL.SYS._WDG_ReloadCounter();
				g_MotoAll++;
				SetMotoPWM(PrintMotoHalfStep[g_MotoAll  & 7],300);
				g_Tim3ProFlag = 0;
				PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
				while(g_Tim3ProFlag == 0) {}
				if((g_MotoAll & 1) == 0)
				{
					g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();
					SetFreValue(GetDETPaper());
				}
				PRINTER_PDL.MOTO._MotoDisable();
				PRINTER_PDL.SYS._Drive_Power_DIS();
			}
		}
#else				
		RunFeedLabPaper();
#endif			
	}
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();
}
void RunFeedLabPaper(void)
{	
	unsigned short j,k,Feedcont,count,runcount,m_SpaceStatus;
	unsigned short StepTime;
	unsigned short RunstarStepTime;
	double a;

//	SET_H_VCC_CON;//¹Ø±Õ³äµç
	g_DetPaperValueFlag = 0xA5EF;
	g_DetPV_Count = 0;

	m_SpaceStatus = 0;
	PRINTER_PDL.SYS._Drive_Power_EN();

	RunstarStepTime = 3000;
	PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],500);
	g_Tim3ProFlag = 0;
	PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(RunstarStepTime);
	while(g_Tim3ProFlag == 0)
	{
	}

	RunstarStepTime = 3000;
	for(j=0;j<10;j++)
	{
		Delay1ms(2);
		PRINTER_PDL.SYS._Drive_Power_Trig();
	}
	j = 0;
	k = 0;
	Feedcont = 0;
	count = 4800;
	while(count--)
	{

		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog ÖØ×°ÔØ¿´ÃÅ¹·¶¨Ê±Æ÷
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],400);
		PRINTER_PDL.SYS._Drive_Power_Trig();
		if(RunstarStepTime > (j * 100))
		{
			StepTime = RunstarStepTime - j * 100;
		}
		else
		{
			StepTime = 0;
		}		
		if(StepTime < 500)
		{
			StepTime = 500;
		}
		g_Tim3ProFlag = 0;
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
	
		}
		k++;
		j++;

		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
		g_LPSStatus = GetPaper();//AD_Value[1]>>4;
		g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		
		if(g_LPSStatus < g_PE_Standard)
		{
				
			PRINTER_PDL.MOTO._MotoDisable();
			PRINTER_PDL.SYS._Drive_Power_DIS();			
			return;				
		}


		if(g_CJCalfalg == 0xA565)
		{
			linearRegression_0_30();
			g_CJCalfalg = 0;
		}
		if((g_MotoAll & 1) == 0)
		{
			Feedcont++;
			SetFreValue(GetDETPaper());//ÉèÖÃ²ÉÑù¹âµçÖµ

			if(Feedcont > 32 )
			{
				g_CJCalfalg = 0xA565;
				linearRegression_32(&a);
				if(a > s_PrintrConfig.m_PE_LabValue_H)
				{
					m_SpaceStatus = 0xA501;
				}
				//printf("%2.3f ",a);
			}
			else			
			{
				linearRegression_0_30();
			}
		}	



//		
//		if((g_MotoAll & 1) == 0)
//		{
//			if(Feedcont < 32)
//			{
//				Feedcont ++;
//			}
//			SetFreValue(g_PaperOutAD);//ÉèÖÃ²ÉÑù¹âµçÖµ
//			if(Feedcont > 31)
//			{
//				if(sw == 0)
//				{
//					linearRegression_31();
//					sw = 1;
//				}
//				else
//				{
//					sw = 0;
//					linearRegression_32(&a);
//					if(a > s_PrintrConfig.m_PE_LabValue_H)
//					{
//						m_SpaceStatus = 0xA501;
//					}
//				}
//			}				
//		}
		if(m_SpaceStatus == 0xA501)
		{
			m_SpaceStatus = 0;
		
			runcount = g_HotLineTOPD * 32  + g_HotLineTOOUT*32;//  - 8- 4;//¼à²âµ½Ö½·ìÖØÐÂÐÞÕý×ßµ½¼ÓÈÈÏßµÄ²½Êý¡¢
	
			
		//	PrintLineTest();

			break;
		}
	}
/*		g_Tim3ProFlag = 0;
		PRINTER_PDL.SYS._STB_A_EN();
		PRINTER_PDL.SYS._STB_B_EN();	
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(10000);
		while(g_Tim3ProFlag == 0)
		{		
			PRINTER_PDL.SYS._Drive_Power_Trig();
			Delay1ms(2);
		}		
		//--Ö½·ìÔÙ´ÎÊ¶±ð
		PRINTER_PDL.SYS._STB_A_DIS();
		PRINTER_PDL.SYS._STB_B_DIS();		
	*/
	StepTime = 950;
	for(;runcount>0;runcount--)
	{
		PRINTER_PDL.SYS._Drive_Power_Trig();
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog ÖØ×°ÔØ¿´ÃÅ¹·¶¨Ê±Æ÷
		g_MotoAll++;
		SetMotoPWM(PrintMotoHalfStep[g_MotoAll  & 7],300);
		g_Tim3ProFlag = 0;
	//	GetNoPaperStatus();
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
			
		}		
		if((g_MotoAll & 1) == 0)
		{
					g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
			SetFreValue(GetDETPaper());//ÉèÖÃ²ÉÑù¹âµçÖµ
		}
		
		
	}
//		SavePrinterConfig();
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();		

		PRINTER_PDL.SYS._STB_A_DIS();
		PRINTER_PDL.SYS._STB_B_DIS();	
}

/**
	* @FuncName  DisplayBTPro
	* @param     void
	* @retval    void
	* @LocalVar  None
	* @GlobalVar g_BT1msCount, g_BTStatus, PRINTER_PDL
	* @Description Periodically polls Bluetooth connection state and updates OLED status display.
	*              Execution Breakdown:
	*              - Enforces 400ms sampling interval (`g_BT1msCount < 400`). 
	*              - Queries active link connection status via `PRINTER_PDL.BT._GetConnect()`.
	*              - If connection state changed (`g_BTStatus != PRINTER_PDL.BT._GetConnect()`):
	*                - Updates cached state `g_BTStatus`.
	*                - Calls `DisplayBT(0, 0, 1)` on link establish -> renders "Paired " text & connected icon.
	*                - Calls `DisplayBT(0, 0, 0)` on link loss      -> erases text & renders disconnected icon.
	*/
unsigned short g_BTStatus = 0xFFFF;
void DisplayBTPro(void)
{
	if(g_BT1msCount < 400)
	{
		return;
	}
	g_BT1msCount = 0;
	g_BT1msCount = 0;
	g_BT1msCount = 0;
	if(g_BTStatus != PRINTER_PDL.BT._GetConnect())
	{
		g_BTStatus = PRINTER_PDL.BT._GetConnect();
		if(PRINTER_PDL.BT._GetConnect())
		{
			//Îª¸ß±íÊ¾Á¬ÉÏÄ£¿é
			DisplayBT(0,0,1);
		}
		else
		{
			DisplayBT(0,0,0);
		}	
	}
}
unsigned short			g_BattyCount;//
unsigned short g_StaBat=0xA100;
unsigned short g_Battystatus;
unsigned short g_Battystatus_bak=10;

extern void DisplayStr6x8(char * pstr,unsigned char x,unsigned char y);
extern void DisplayBufEn(void);

/* Render the thermistor-derived head temperature only in the protected firmware image. */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
void DisplayTemp(unsigned char x, unsigned char y)
{
	static unsigned int last_temp_time = 0;
	static unsigned char last_temp_val_int = 0xFF;
	unsigned short current_temp;
	char temp_str[10];
	unsigned char temp_val_int;
	unsigned short adc_VAL;
	// mukesh - updated_23_09: sample the actual thermistor every 100 ms so cooling is displayed as it occurs.
	if (last_temp_time == 0 || (g_sysTime - last_temp_time) >= 100)
	{
		last_temp_time = g_sysTime;
		adc_VAL = ADC_Quick_GetVoltage(ADC_CHANNEL_0,ADC_GPIO);	// mukesh - direct reading the adc value 
#ifndef TEMP_SET_ADC_VALUE_DEFAULT80_ISSUE
		MyPrintf("*3_TMP_:%d\r\n",adc_VAL);
#endif		
		current_temp = Read_Thermistor_Temperature(adc_VAL);
#ifndef TEMP_SET_ADC_VALUE_DEFAULT80_ISSUE
		MyPrintf("*4_CurrTMP:%d\r\n",current_temp);
#endif		
		temp_val_int = current_temp / 10;
		
		if (temp_val_int != last_temp_val_int)
		{
			last_temp_val_int = temp_val_int;
			sprintf(temp_str, "%dC ", temp_val_int);
			DisplayStr6x8(temp_str, x, y);
#ifdef USB_CHANGING_DATALOSS_ISSUE_SOLUTION	
			if ((s_PrinterMOTOPar.m_In_printing != ENABLE_VAR) && (s_PrinterMOTOPar.m_StopPrint == ENABLE_VAR))
			{
				DisplayBufEn();
			}
#else			
			DisplayBufEn();
#endif
		}
		else
		{
			sprintf(temp_str, "%dC ", temp_val_int);
			DisplayStr6x8(temp_str, x, y);
		}
	}
	else if (last_temp_val_int != 0xFF)
	{
		sprintf(temp_str, "%dC ", last_temp_val_int);
		DisplayStr6x8(temp_str, x, y);
	}
}
#endif


#ifdef BATTERY_INDICATION_SOLUTION
void BatteryBar_PrintStarted(void)
{
	if (PRINTER_PDL.USB._GetUSBDet() != 0)
	{
		g_BatteryBarPrintState = BATTERY_BAR_IDLE;
		return;
	}

	if (g_BatteryBarPrintState != BATTERY_BAR_PRINTING)
	{
		//g_BarLevelAtPrintStart = GetBattyPer();
		g_BarLevelAtPrintStart = g_Battystatus;
		g_Battystatus = g_BarLevelAtPrintStart;
		g_Battystatus_bak = g_BarLevelAtPrintStart;
		g_BatteryBarPrintState = BATTERY_BAR_PRINTING;
#ifdef DEBUG_BATTERY_INDICATION_FIX
		printf("BAT_ICON: print_start locked_level=%u\r\n", g_BarLevelAtPrintStart);
#endif
		DisplayBatteryL(DIS_BAT_X, DIS_BAT_Y, g_BarLevelAtPrintStart);
	}
}
#endif



void DisplayBatty(unsigned char x,unsigned char y)
{
#ifdef BATTERY_INDICATION_SOLUTION	
  unsigned char measured_level;
	unsigned char initial_level_1;
	unsigned char initial_level_2;
	unsigned char initial_level_3;
	unsigned char swap_level;	
	
#ifdef USB_CHANGING_DATALOSS_ISSUE_SOLUTION	
/* FIX: Never execute 200-250ms bit-banged I2C OLED transfers while the motor or printhead is
active.
* Bypassing display refresh during active printing prevents buffer starvation, motor
stutter,
* and mid-label cuts in charging state. Charging animation resumes automatically once idle.
*/
		if ((s_PrinterMOTOPar.m_In_printing == ENABLE_VAR) || (s_PrinterMOTOPar.m_StopPrint ==
		DISABLE_VAR))
		{
			return;
		}
#endif	
	
		if (PRINTER_PDL.USB._GetUSBDet() != 0) /* USB charging behavior is unchanged. */
		{
			/* Cancel any battery print-session state when USB is connected. */
			g_BatteryBarPrintState = BATTERY_BAR_IDLE;
			if (g_DlinkCount < 500)
			{
				return;
			}
			g_DlinkCount = 0;
			g_DlinkCount = 0;
			g_DlinkCount = 0;
			if (PRINTER_PDL.SYS._GetChargeNCHRG())
			{
				DisplayBatteryL(x, y, 4);
				return;
			}

			g_BattyCount++;
			if (g_BattyCount > 4)
			{
				g_BattyCount = GetBattyPer();		/// returns 0 ,1,2,3,4
				if (g_BattyCount)
				{
					g_BattyCount--;
				}
			}
			DisplayBatteryL(x, y, g_BattyCount);
			g_StaBat = 0xA100;
			return;
		}
		
#ifdef FIXED_LATE_BATTERY_DISPLAY
		 /*
         * FIX 1: If an immediate initial draw is requested (e.g. OLED_RestoreHomeScreen),
         * sample 3 times, pick the median, draw immediately, and cancel any stale print lock!
         */
        if (g_StaBat == 0xA100)
        {
            initial_level_1 = GetBattyPer();
            initial_level_2 = GetBattyPer();
            initial_level_3 = GetBattyPer();

            if (initial_level_1 > initial_level_2) 
						{ 
							swap_level = initial_level_1; 
							initial_level_1 = initial_level_2; initial_level_2 = swap_level; 
						}
            if (initial_level_2 > initial_level_3) 
						{ 
							swap_level = initial_level_2; 
							initial_level_2 = initial_level_3; 
						  initial_level_3 = swap_level; 
						}
            if (initial_level_1 > initial_level_2) 
						{ 
							swap_level = initial_level_1; 
							initial_level_1 = initial_level_2; 
						  initial_level_2 = swap_level; 
						}

            measured_level = initial_level_2;
            g_Battystatus = measured_level;
            g_Battystatus_bak = measured_level;
            g_StaBat = 0;
            g_BatteryBarPrintState = BATTERY_BAR_IDLE; // Reset print state to IDLE
            g_BatteryBarSampleTime = g_sysTime;
            DisplayBatteryL(x, y, measured_level);
            return;
        }

#endif		
	
	  if (g_BatteryBarPrintState == BATTERY_BAR_PRINTING)
		{
			if ((s_PrinterMOTOPar.m_In_printing == DISABLE_VAR) && (s_PrinterMOTOPar.m_StopPrint == ENABLE_VAR))
			{
				g_BatteryBarPrintState = BATTERY_BAR_RECOVERING;
				g_BatteryBarRecoveryTime = g_sysTime;
#ifdef DEBUG_BATTERY_INDICATION_FIX
		printf("BAT_ICON: print_end recovery_started locked_level=%u\r\n", g_BarLevelAtPrintStart);
#endif
			}
			return;
		}
		
		if (g_BatteryBarPrintState == BATTERY_BAR_RECOVERING)
		{
			if ((g_sysTime - g_BatteryBarRecoveryTime) < BATTERY_BAR_POST_PRINT_DELAY_MS) return;
			g_BatteryBarCandidateLevel = GetBattyPer();
			g_BatteryBarPrintState = BATTERY_BAR_CONFIRMING;
			g_BatteryBarRecoveryTime = g_sysTime;
#ifdef DEBUG_BATTERY_INDICATION_FIX
			printf("BAT_ICON: post_print candidate=%u current=%u confirmation_wait_started\r\n",
				g_BatteryBarCandidateLevel, g_Battystatus);
#endif
			return;
		}
		
		if (g_BatteryBarPrintState == BATTERY_BAR_CONFIRMING)
		{
			if ((g_sysTime - g_BatteryBarRecoveryTime) < BATTERY_BAR_POST_PRINT_DELAY_MS) return;
			measured_level = GetBattyPer();
			/*
			 * A direction match is not sufficient: for example, candidate 2 then
			 * measured 1 while the displayed bar is 3 is still moving.  Update
			 * only after two identical samples prove a stable new bar level.
			 */
			if ((g_BatteryBarCandidateLevel == measured_level) &&
				(g_BatteryBarCandidateLevel != g_Battystatus))
			{
#ifdef DEBUG_BATTERY_INDICATION_FIX
				printf("BAT_ICON: confirmation_result current=%u candidate=%u measured=%u action=stable_update\r\n",
					g_Battystatus, g_BatteryBarCandidateLevel, measured_level);
#endif
				g_BarLevelAtPrintStart = measured_level;
				g_Battystatus = measured_level;
				g_Battystatus_bak = measured_level;
				DisplayBatteryL(x, y, measured_level);
			}
#ifdef DEBUG_BATTERY_INDICATION_FIX
		else printf("BAT_ICON: confirmation_result current=%u candidate=%u measured=%u action=retain_not_stable\r\n",
				g_Battystatus, g_BatteryBarCandidateLevel, measured_level);
#endif
			g_BatteryBarPrintState = BATTERY_BAR_IDLE;
			g_BatteryBarSampleTime = g_sysTime;
			return;
		}

#ifndef FIXED_LATE_BATTERY_DISPLAY		
		if (g_StaBat == 0xA100)
		{
			/*
			 * The home screen is reached after the normal power-on delay, but one
			 * conversion can still be optimistic.  Take three immediate bar
			 * samples and use their median without delaying the first icon draw.
			 */
			initial_level_1 = GetBattyPer();
			initial_level_2 = GetBattyPer();
			initial_level_3 = GetBattyPer();

			if (initial_level_1 > initial_level_2)
			{
				swap_level = initial_level_1;
				initial_level_1 = initial_level_2;
				initial_level_2 = swap_level;
			}
			if (initial_level_2 > initial_level_3)
			{
				swap_level = initial_level_2;
				initial_level_2 = initial_level_3;
				initial_level_3 = swap_level;
			}
			if (initial_level_1 > initial_level_2)
			{
				swap_level = initial_level_1;
				initial_level_1 = initial_level_2;
				initial_level_2 = swap_level;
			}

			measured_level = initial_level_2;
			g_Battystatus = measured_level;
			g_Battystatus_bak = measured_level;
			g_StaBat = 0;
			g_BatteryBarSampleTime = g_sysTime;
#ifdef DEBUG_BATTERY_INDICATION_FIX
			printf("BAT_ICON: initial_draw sorted=%u,%u,%u selected=%u\r\n",
				initial_level_1, initial_level_2, initial_level_3, measured_level);
#endif
			DisplayBatteryL(x, y, measured_level);
			return;
		}
#endif		
	
	  if ((g_sysTime - g_BatteryBarSampleTime) < BATTERY_BAR_POST_PRINT_DELAY_MS)
		{
			return;
		}

		g_BatteryBarCandidateLevel = GetBattyPer();
		g_BatteryBarSampleTime = g_sysTime;
#ifdef DEBUG_BATTERY_INDICATION_FIX		
		//DEBUG_for_exactbar
		printf("BAT_ICON_IDLE: adc_mv=%u calculated_bar=%u should_display=%u displayed_bar=%u\r\n",
			g_Battycal, g_BatteryBarCandidateLevel,
			g_BatteryBarCandidateLevel, g_Battystatus);
#endif		
		if (g_BatteryBarCandidateLevel == g_Battystatus)
		{
#ifdef DEBUG_BATTERY_INDICATION_FIX
			printf("BAT_ICON: idle_sample time=%lu current=%u candidate=%u action=retain_same\r\n",
				g_sysTime, g_Battystatus, g_BatteryBarCandidateLevel);
#endif
			return;
		}
		
		g_BatteryBarPrintState = BATTERY_BAR_CONFIRMING;
		g_BatteryBarRecoveryTime = g_sysTime;
#ifdef DEBUG_BATTERY_INDICATION_FIX
		printf("BAT_ICON: idle_sample time=%lu current=%u candidate=%u action=confirmation_wait_started\r\n",
		g_sysTime, g_Battystatus, g_BatteryBarCandidateLevel);
#endif
	
#else
	if(PRINTER_PDL.USB._GetUSBDet() != 0)
	{
		//³äµç	// Battery charging initialization
		if(g_DlinkCount < 500)
		{
			return;
		}
		g_DlinkCount = 0;
		g_DlinkCount = 0;
		g_DlinkCount = 0;
		if(PRINTER_PDL.SYS._GetChargeNCHRG())
		{//³äÂú // Battery fully charged
			DisplayBatteryL(x,y,4);
			return;
		}

		g_BattyCount++;
		
		
		if(g_BattyCount > 4)
		{
			g_BattyCount =  GetBattyPer();
			if(g_BattyCount)
			{
				g_BattyCount--;
			}
	
		}		
		DisplayBatteryL(x,y,g_BattyCount);
		g_StaBat = 0xA100;
	}
	else
	{
		if(g_DlinkCount < 2000)
		{
			g_Battystatus_bak=10;
			if(g_StaBat == 0xA100)
			{
				g_Battystatus = 0xFF;
				g_StaBat = 0;
			
				if(g_Battystatus != GetBattyPer())
				{
					g_Battystatus =  GetBattyPer();
					DisplayBatteryL(x,y,g_Battystatus);
				}	
			}						
			return;
		}		
		
		if(g_StaBat == 0xA100)
		{
			g_Battystatus = 0xFF;
			g_StaBat = 0;
		}	
		if(g_Battystatus != GetBattyPer())
		{
			g_Battystatus =  GetBattyPer();
			if(g_Battystatus < g_Battystatus_bak)
			{
				g_Battystatus_bak = g_Battystatus;
				DisplayBatteryL(x,y,g_Battystatus);
			}
			
		}
	}
#endif	
	
}


#ifdef POWER_ON_CAL
/**
		* @FuncName  PowerON_Calibration
		* @param     void
		* @retval    unsigned char - Status code:
		*                            1 = Success (Media calibrated)
		*                            3 = Failure (Media absent)
		*                            4 = Failure (Lid open)
		* @LocalVar  unsigned int m_w          - String width calculation variable for OLED display centering
		*            unsigned char result      - Operation result accumulator
		*            unsigned short ad         - Raw ADC gap sensor reading (under GAP_STATUS debug mode)
		*            char gap_debug[32]        - Formatted gap debug string buffer
		* @GlobalVar g_LID_Status, g_Media_Status, PRINTER_PDL, g_PaperType,
		*            g_boundary_detection_flag, g_POC_PullbackPending
		* @Description Senses printer cover state and paper presence, then executes smart optical
		*              sensor calibration for media boundary/gap detection.
		*
		*              Execution Breakdown:
		*              1. Hardware Cover Sensing:
		*                 Queries cover microswitch (`g_LID_Status = PRINTER_PDL.SYS._GetHSStatus()`).
		*                 If cover is OPEN (`g_LID_Status == LID_DETECTION`), returns status code 4 (Lid Open Failure).
		*              2. Paper Sensor Check:
		*                 Queries paper sensor (`g_Media_Status = PRINTER_PDL.SYS._PaperOutDet()`).
		*                 If media is ABSENT (`g_Media_Status < MEDIA_ABSENT`), returns status code 3 (Media Absent Failure).
		*              3. Sensor Calibration Execution:
		*                 Displays "CAL Started" on OLED display.
		*                 Invokes smart calibration sequence (`GetMediaCAl()`).
		*              4. Media Boundary Flag Setting:
		*                 - If `g_PaperType == 0` (Label Media): Sets `g_boundary_detection_flag = 1`
		*                   and arms pullback recovery (`g_POC_PullbackPending = 1`).
		*                 - If `g_PaperType == 1` (Paper Media): Sets `g_boundary_detection_flag = 0`.
		*                 Returns status code 1 (Success).
		*/
unsigned char PowerON_Calibration(void)
{
	unsigned int m_w;
	unsigned char result;
	
	g_LID_Status =  PRINTER_PDL.SYS._GetHSStatus();
#ifdef MY_DEBUG_ENABLE	
	MyPrintf("LID_Status:%d\r\n",g_LID_Status);
#endif	
	if(LID_DETECTION == g_LID_Status)   //LID Open
	{
		g_Media_Status = PRINTER_PDL.SYS._PaperOutDet();
#ifdef MY_DEBUG_ENABLE		
		MyPrintf("Media_Status:%d\r\n",g_Media_Status);
#endif		
		// 4 = Lid Open Failure (regardless of media state)
		return 4; 
	}
	else //LID Closed
	{
		g_Media_Status = PRINTER_PDL.SYS._PaperOutDet();
		if(g_Media_Status < MEDIA_ABSENT) //LID Closed - Media Absent
		{
			// 3 = Media Absent Failure
			return 3; 
		}
		else //LID Closed - Media Present
	    {
#ifdef GAP_STATUS
			PRINTER_PDL.SYS._Drive_Power_EN();
			while(PRINTER_PDL.SYS._GetKey_LF()) // Stay here until Feed button is pressed
			{
				unsigned short ad = PRINTER_PDL.SYS._PaperGapDet();
				ClearDisplayBuf();
				
				// Display raw AD and 'G' if in gap (Gap is ~440-460 according to user)
				if(ad < 600) 
					DisplayStr6x8("G", 110, 0); 
				
				char gap_debug[32];
				sprintf(gap_debug, "ADC:%04d", ad);
				DisplayStr8x16char(gap_debug, 32, 14, 0); 
				
				DisplayStr6x8("Press Feed->Cal", 20, 0);
				
				DisplayBufEn();
				Delay1ms(50);
				PRINTER_PDL.SYS._WDG_ReloadCounter();
			}
			// Wait for button release to prevent accidental extra feeds
			while(PRINTER_PDL.SYS._GetKey_LF() == 0) PRINTER_PDL.SYS._WDG_ReloadCounter();
#endif

#ifdef AUTO_CAL_DISCARD_OPTION
	    ClearDisplayBuf();
			m_w = GetStr8x16Wide((char*)"Calibrating...",1);
			DisplayStr8x16char("Calibrating...",(128-m_w)/2,14,1);
			DisplayBufEn();
#else
			ClearDisplayBuf();
			m_w = GetStr8x16Wide((char*)"CAL Started",1);
			DisplayStr8x16char("CAL Started",(128-m_w)/2,14,1);
			DisplayBufEn();
#endif			
			GetMediaCAl();//Media_Calibration();  // smart calibration 
			
			if (g_PaperType == 0) // Label
			{
				g_boundary_detection_flag = 1;
#ifdef POC_LABEL_RECOVERY
				g_POC_PullbackPending = 1;
				MyPrintf("POC: Label recovery armed\r\n");
#endif
			}
			else // Paper
			{
				g_boundary_detection_flag = 0;
			}

			return 1; // Success
			
		}
	}
}
/**
	* @FuncName  Label_Size_Calculation
	* @param     uint16_t Boundary_Count - Cumulative motor step count logged between consecutive label gaps
	* @retval    void
	* @LocalVar  uint16_t gap_steps      - Motor step count corresponding to physical label gap
width (m_BarPaGap * 32)
	*            uint32_t net_label_steps- Net step count calculated by subtracting gap steps
from total boundary steps
	*            uint16_t label_length_mm- Calculated net label height expressed in millimeters
	* @GlobalVar s_PrintrConfig, g_label_size_final
	* @Description Converts raw motor step counts logged during gap sensing into physical label length in millimeters.
	*
	*              Calculation Formula:
	*              - Each motor step equals 1/32 mm (32 steps per mm).
	*              - Subtracts inter-label gap steps (`gap_steps = s_PrintrConfig.m_BarPaGap * 32`).
	*              - Net steps = `Boundary_Count - gap_steps`.
	*              - Height (mm) = `net_label_steps / 32`.
	*              - Saves final dimension into `g_label_size_final` and `s_PrintrConfig.m_LabelLength`.
	*/
void Label_Size_Calculation(uint16_t Boundary_Count)
{
#ifdef PSF20_R002_TSPL_PROTOCOL
    g_label_size_final = 0U;
#endif

	// Each motor step (g_MotoAll increment) is 1/32 mm.
	//  scaling factor to compensate for mechanical steps
	
//	uint32_t corrected_boundary = (Boundary_Count * 97) /100;
	
	
	uint16_t gap_steps = s_PrintrConfig.m_BarPaGap * 32;
	
	if (Boundary_Count > gap_steps) {
		label_length_mm = (Boundary_Count - gap_steps + 16) / 32;
	} else {
		label_length_mm = (Boundary_Count + 16) / 32;
	}
	
	if(label_length_mm >= 48 && label_length_mm <= 54) 
	{
		g_label_size_final = 50;
	} 
	else if(label_length_mm >= 27 && label_length_mm <= 33) 
	{
		g_label_size_final = 30;
	} 
	else if(label_length_mm >= 23 && label_length_mm <= 26) 
	{
		g_label_size_final = 25;
	} 
	else if(label_length_mm >= 11 && label_length_mm <= 13) 
	{
		g_label_size_final = 12;
	} 
	else if(label_length_mm >= 14 && label_length_mm <= 16) 
	{
		g_label_size_final = 15;
	} 
	else if(label_length_mm >= 36 && label_length_mm <= 40) 
	{
		g_label_size_final = 38;
	}	
	else if(label_length_mm >= 73 && label_length_mm <= 79) 
	{
		g_label_size_final = 75;
	}

/*	
	if(label_length_mm >= 48 && label_length_mm <= 54) {
		g_label_size_final = 50;
	} else if (label_length_mm >= 27 && label_length_mm <= 33) {
		g_label_size_final = 30;
	} else if (label_length_mm >= 23 && label_length_mm <= 26) {
		g_label_size_final = 25;
	}
	*/
//	g_label_size_final = label_length_mm;
	
	//MyPrintf("\r\nCAL DEBUG: Steps = %u, Label Size = %u mm\r\n", Boundary_Count, label_length_mm);

#ifdef PSF20_R002_TSPL_PROTOCOL
    TsplNative_Calibrated();
#endif
}

/**
	* @FuncName  lid_mediacal
	* @param     void
	* @retval    void
	* @LocalVar  unsigned int m_w    - Calculated text string pixel width for UI horizontal centering
	*            char size_msg[32]   - Formatted label size message string buffer
	* @GlobalVar g_PaperType, g_boundary_detection_flag, g_boundary_gap_count_val,
	*            s_PrintrConfig, g_label_size_final, label_length_mm
	* @Description Performs  smart media calibration following cover lid close/open events.
	*
	*              Detailed Execution Flow:
	*              1. UI Feedback:
	*                 Displays "Media Checking" on the OLED display screen.
	*              2. Core Calibration Execution:
	*                 Invokes low-level motor step and optical sensing function `GetMediaCAl()`.
	*              3. Result Evaluation & System Configuration:
	*                 - If Label Media Detected (`g_PaperType == 0`):
	*                   - Sets `g_boundary_detection_flag = 1`.
	*                   - Invokes `Label_Size_Calculation(g_boundary_gap_count_val)` to derive height.
	*                   - Updates persistent parameters: `s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN`,
	*                     `m_BarPaGap = 3`, and `m_LabelLength = g_label_size_final`.
	*                   - Displays `"Media: Label"` and `"Size: @XX mm"` on OLED.
	*                   - Transmits Bluetooth size response if `BT_CMD_REQ` is active.
	*                 - If Continuous Paper Detected (`g_PaperType == 1`):
	*                   - Sets `g_boundary_detection_flag = 0` and `s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS`.
	*                   - Displays `"Media: Paper"` on OLED.
	*              4. Screen Restoration:
	*                 Holds result message on screen for 2 seconds and restores main home UI (`OLED_RestoreHomeScreen()`).
*/
void lid_mediacal(void)
{
	unsigned int m_w;
	char size_msg[32];
	
#ifdef AUTO_CAL_DISCARD_OPTION   //Provide calibration option after lid open/close event
   
	 uint8_t user_cal_selection = 1; // 1 = YES, 0 = NO
   uint32_t selection_timer = 0;
	
      // --- 1. Prompt User: "Calibrate: YES / NO" (matching AUTO_CAL_SELECTION pattern) ---
        ClearDisplayBuf();
        m_w = GetStr8x16Wide((char*)"Calibrate: YES", 0);
        DisplayStr8x16char("Calibrate: YES", (128 - m_w) / 2, 14, 0);
        DisplayBufEn();

        while(selection_timer < CAL_MODE_SELECTION_TIME) // Selection timeout (3 seconds)
        {
            if(PRINTER_PDL.SYS._GetKey_LF() == 0) // Feed button pressed
            {
                ClearDisplayBuf();
                if(user_cal_selection == 1)
                {
                    user_cal_selection = 0; // Toggle to NO
                    m_w = GetStr8x16Wide((char*)"Calibrate: NO", 0);
                    DisplayStr8x16char("Calibrate: NO", (128 - m_w) / 2, 14, 0);
                }
                else
                {
                    user_cal_selection = 1; // Toggle to YES
                    m_w = GetStr8x16Wide((char*)"Calibrate: YES", 0);
                    DisplayStr8x16char("Calibrate: YES", (128 - m_w) / 2, 14, 0);
                }
                DisplayBufEn();

                selection_timer = 0; // Reset countdown after user interaction

                // Wait for Feed button release (debounce)
                while(PRINTER_PDL.SYS._GetKey_LF() == 0)
                {
                    PRINTER_PDL.SYS._WDG_ReloadCounter();
                }
            }
            Delay1ms(1);
            selection_timer++;
            PRINTER_PDL.SYS._WDG_ReloadCounter();
        }

        // --- 2. If NO: Abort calibration, restore display, and return to printing ---
        if(user_cal_selection == 0)
        {
#ifdef MEDIA_UNKNOWN_DETECTION_IN_SETTING_MENU					
					  g_boundary_detection_flag = 0;
					  User_calibration_discarded_flag = 1;
#endif					
						ClearDisplayBuf();
						m_w = GetStr8x16Wide((char*)"Calibration", 0);
						DisplayStr8x16char("Calibration", (128-m_w)/2, 0, 0);
						m_w = GetStr8x16Wide((char*)"Discarded", 0);
						DisplayStr8x16char("Discarded", (128-m_w)/2, 16, 0);
						
						DisplayBufEn();		
						
						PRINTER_PDL.SYS._WDG_ReloadCounter();	
						Delay1ms(2000);
						PRINTER_PDL.SYS._WDG_ReloadCounter();
						
						ClearDisplayBuf();
						m_w = GetStr8x16Wide((char*)"Media: Unknown", 0);
						DisplayStr8x16char("Media: Unknown", (128-m_w)/2, 0, 0);
						m_w = GetStr8x16Wide((char*)"Size: Unknown", 0);
						DisplayStr8x16char("Size: Unknown", (128-m_w)/2, 16, 0);
						DisplayBufEn();	
						
						PRINTER_PDL.SYS._WDG_ReloadCounter();	
						Delay1ms(2000);
						PRINTER_PDL.SYS._WDG_ReloadCounter();	
						
						ClearDisplayBuf();					
            OLED_RestoreHomeScreen();
            return;
        }

        // --- 3. If YES: Discard all active and printing data ---
        __SETPRIMASK();
        Receive_count = 0;
        g_Receive_count = 0;
        g_Order_count = 0;
        s_PrintOrderstr.m_GetdataPos = 0;
        s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;
        __RESETPRIMASK();
#endif   
	
	// 1. Initial UI Feedback
	ClearDisplayBuf();
	m_w = GetStr8x16Wide((char*)"Calibrating...",1);
	DisplayStr8x16char("Calibrating...",(128-m_w)/2,14,1);			
//	m_w = GetStr8x16Wide((char*)"Media Checking", 1);
//	DisplayStr8x16char("Media Checking", (128 - m_w) / 2, 14, 1);
	DisplayBufEn();
	Delay1ms(500);

	// 2. Perform Smart Calibration Logic
	GetMediaCAl();

	// 3. Process Results and Update System State
	ClearDisplayBuf();
	if (g_PaperType == 0) // Label detected
	{
		// Success: Update state and measure size
		g_boundary_detection_flag = 1;

		Label_Size_Calculation(g_boundary_gap_count_val);

		// Update Persistent Settings
		s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN; 
		s_PrintrConfig.m_BarPaGap = 3; // Standard Gap 3mm
		s_PrintrConfig.m_LabelLength = g_label_size_final;

		// Display Label Info
		m_w = GetStr8x16Wide((char*)"Media: Label", 1);
		DisplayStr8x16char("Media: Label", (128 - m_w) / 2, 0, 1);

		sprintf(size_msg, "Size: @%d mm", label_length_mm);  //label_length_mm
		m_w = GetStr8x16Wide(size_msg, 1);
		DisplayStr8x16char(size_msg, (128 - m_w) / 2, 16, 1);
#ifdef BT_CMD_REQ		
		Get_Label_Size_BT_Response(label_length_mm);
#endif		
		
#ifdef MY_DEBUG_ENABLE
		MyPrintf("LidCal: Label Found. Size: %dmm\r\n", g_label_size_final);
#endif	
	}
	else // Paper detected
	{
		g_boundary_detection_flag = 0;
#ifndef DISCARDING_CAL_SET_PREV_SR20_FUNCTIONALITY		   //[Modify by: Kajal ---> 10 Oct 2026] ---> see macro definition		
		s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS; 
#endif		
#ifdef BT_CMD_REQ		
		Get_Label_Size_BT_Response(0);
#endif
		// Display Paper Info
		m_w = GetStr8x16Wide((char*)"Media: Paper", 1);
		DisplayStr8x16char("Media: Paper", (128 - m_w) / 2, 14, 1);
#ifdef MY_DEBUG_ENABLE
		MyPrintf("LidCal: No Gap Found. Defaulted to Continuous.\r\n");
#endif	
	}

	// 4. Save Settings to Flash if needed
/*  // No Need to save this inflash
#ifndef PF_ISSUE_SOLUTION
	SavePrinterConfig();
#endif
*/
	// 5. Finalize Display
	DisplayBufEn();
	Delay1ms(2000);

	// Restore home screen
	OLED_RestoreHomeScreen();
}
/**
	* @FuncName  GetMediaCAl
	* @param     void
	* @retval    void
	* @LocalVar  unsigned short i, j              - Loop index and timing calculation variables
	*            unsigned short Feedcont          - Step counter tracking linear regression evaluation window
	*            unsigned short count             - Timeout step counter (max 4600 half-steps / ~60mm limit)
	*            unsigned short runcount          - Motor step loop iteration variable
	*            unsigned short StepTime          - Dynamically calculated inter-step delay duration (µs)
	*            unsigned short RunstarStepTime   - Initial motor start step timing value (3000µs)
	*            double a, valu_a                 - Calculated slope parameter from linear regression analysis
	*            unsigned short start_adc         - Initial ambient optical sensor ADC value
	*            unsigned char started_in_gap     - Flag indicating if media started directly over a gap (ADC < 600)
	* @GlobalVar PRINTER_PDL, g_CJCalfalg, g_boundary_detection_flag, g_step_counter,
	*            g_MotoAll, g_Tim3ProFlag, g_PaperGapAD, g_boundary_gap_count_val, g_LPSStatus, g_PE_Standard
	* @Description  Drives the stepper motor step-by-step
	*              while analyzing paper gap sensor ADC values using real-time linear regression slope
	*              calculations to locate gap boundaries and compute exact label pitch.
	*
	*/
void GetMediaCAl(void)
{
	unsigned short i, j, Feedcont, count, runcount;
	unsigned short StepTime, RunstarStepTime;
	double a, valu_a;
	unsigned short start_adc;
	unsigned char started_in_gap = 0;
	
	PRINTER_PDL.SYS._Drive_Power_EN();
	g_CJCalfalg = 0;
	
	// Initial sensor stabilization
	for(i=0; i<30; i++)
	{
		Delay1ms(2);
		PRINTER_PDL.SYS._PaperGapDet();
		SetFreValue(PRINTER_PDL.SYS._PaperGapDet());
		PRINTER_PDL.SYS._Drive_Power_Trig();
	}
	
	start_adc = PRINTER_PDL.SYS._PaperGapDet();
	
	// Check starting position using absolute ADC values
	// User params: Gap = 440~460, Label Surface = 800+, Paper = ~700
	if(start_adc < 600) 
	{
		g_boundary_detection_flag = 4; // Started in GAP, wait to reach Label Surface
		started_in_gap = 1;
#ifdef MY_DEBUG_ENABLE		
		MyPrintf("SmartCal: Started in GAP (ADC: %d)\r\n", start_adc);
#endif	
	}
	else
	{
		g_boundary_detection_flag = 0; // Started on LABEL/PAPER, find Gap 1 first
		started_in_gap = 0;
#ifdef MY_DEBUG_ENABLE		
		MyPrintf("SmartCal: Started on LABEL (ADC: %d)\r\n", start_adc);
#endif	
	}

	count = 4800;//4600;//3200; // Maximum timeout (~60mm)
	valu_a = 0;
	RunstarStepTime = 3000;
	j = 0;
	Feedcont = 0;
	g_step_counter = 0;
	
	while(count--)
	{
		PRINTER_PDL.MOTO._MotoDisable();
		PRINTER_PDL.SYS._Drive_Power_Trig();
		PRINTER_PDL.SYS._WDG_ReloadCounter();
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7], 400);		
		g_Tim3ProFlag = 0;
		
		if(RunstarStepTime > (j * 100)) StepTime = RunstarStepTime - j * 100;
		else StepTime = 0;		
		if(StepTime < 500) StepTime = 500;			
		j++;
		
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0);

		g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		
#ifdef MY_DEBUG_ENABLE_O
		MyPrintf("WHERE_AM_I:%d\r\n",PRINTER_PDL.SYS._PaperGapDet());
#endif		
		if(g_CJCalfalg == 0xA565)
		{
			linearRegression_0_30();
			g_CJCalfalg = 0;
		}
		
		if((g_MotoAll & 1) == 0)
		{
			Feedcont++;
			SetFreValue(GetDETPaper());
			if(Feedcont > 32)
			{
				g_CJCalfalg = 0xA565;
				linearRegression_32(&a);
				if(valu_a < a) valu_a = a;
				
				// --- SMART CALIBRATION STATE MACHINE ---
				if (g_boundary_detection_flag == 4) 
				{
					// We are in a gap, waiting to reach the label surface
					if (g_PaperGapAD >= 750) 
					{
						g_boundary_detection_flag = 5; // Reached surface, start measuring Label Length
						g_step_counter = 0;
					}
				}
				else if (g_boundary_detection_flag == 0) 
				{
					// We are on a label, waiting for the first gap
					if (a > 4.0) // Vendor logic for Label -> Gap transition
					{
						g_boundary_detection_flag = 6; // Found first gap, now measure Pitch to the next gap
						g_step_counter = 0;
					}
				}
				else if (g_boundary_detection_flag == 5 || g_boundary_detection_flag == 6) 
				{
					// Measuring label size, waiting for the NEXT gap
					// Small debounce (g_step_counter > 100) to ensure we've fully transitioned onto the label
					if (a > 4.0 && g_step_counter > 100) 
					{
						g_boundary_gap_count_val = g_step_counter;
						g_boundary_detection_flag = 1; // Success! Measurement complete.
						break; // Stop motor exactly at the gap
					}
				}
			}
			else
			{
				linearRegression_0_30();
			}
		}
		
		// Increment the step counter for EVERY motor step if we are in a measuring state
		if (g_boundary_detection_flag == 5 || g_boundary_detection_flag == 6)
		{
			g_step_counter++;
		}

		// Safety: Out of paper
		g_LPSStatus = GetPaper();
		if(g_LPSStatus < g_PE_Standard)
		{
			PRINTER_PDL.MOTO._MotoDisable();
			PRINTER_PDL.SYS._Drive_Power_DIS();			
			return;				
		}
	}
	
#ifdef FOR_PSF20_PAPER_FEED_SOLUTION	   //Modify by: Kajal (09 Sep 2026)
//@exception management of g_HotLineTOPD & g_HotLineTOOUT start -->
	//Added to prevent paper feed during run time
		if(9 != g_HotLineTOPD )
		{
			g_HotLineTOPD = 9;     //if not 9 then set it to 9 bydefault
		}
		if(8 != g_HotLineTOOUT)
		{
			g_HotLineTOOUT = 8;   //if not 8 then set it to 8 bydefault
		}
//@exception management end <-- 	
#endif	
		
	PRINTER_PDL.MOTO._MotoDisable();

	s_PrintrConfig.m_PE_LabValue_H = ((unsigned short)valu_a / 2);

	if(s_PrintrConfig.m_PE_LabValue_H < 4)
	{
		g_PaperType = 1; // Continuous Paper
		s_PrintrConfig.m_InterFace = 1;
	}
	else
	{
		g_PaperType = 0; // Label
		s_PrintrConfig.m_InterFace = 0;
	}		
	SavePrinterConfig();
	
	if(g_PaperType == 0 && g_boundary_detection_flag == 1)
	{
		g_CAL_started_in_gap = started_in_gap;
		
		Label_Size_Calculation(g_boundary_gap_count_val);

		// Align to tear bar
		runcount = g_HotLineTOPD * 32 + g_HotLineTOOUT * 32;
		StepTime = 950;
		for(; runcount > 0; runcount--)
		{
			PRINTER_PDL.SYS._Drive_Power_Trig();
			PRINTER_PDL.SYS._WDG_ReloadCounter();
			g_MotoAll++;
			SetMotoPWM(PrintMotoHalfStep[g_MotoAll & 7], 300);
			g_Tim3ProFlag = 0;
			PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
			while(g_Tim3ProFlag == 0);
			if((g_MotoAll & 1) == 0)
			{
				g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();
				SetFreValue(GetDETPaper());
			}
		}
	}		
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();
}

void OLED_RestoreHomeScreen(void)
{
	unsigned int m_w;
	
	// Reset menu state to ensure auto-return timeout works correctly
	// and we are officially "out of menu" mode.
	g_OptionStatus = 0;
	g_optionCount = 0;
	gHomeMenuRefreshFlag = 0xFF;
	g_waitflag = 0;
#ifdef BT_NACK
	g_DiscardPrinting = 0;
#endif

#ifdef FIXED_LATE_BATTERY_DISPLAY
/* 1. LOCK screen transmission: suppress premature intermediate flushes */
        g_DisplayHoldOff = 1;
#endif	
	
	ClearDisplayBuf();
	
	SetPrinterStatus(DIS_PE_X, DIS_PE_Y, 1);
	SetCoverStatus(DIS_COV_X, DIS_COV_Y, 1);
	
	// Force redraw by resetting cache/timers
	g_BTStatus = 0xFFFF;
	g_BT1msCount = 400;
	g_DlinkCount = 2000;
	g_StaBat = 0xA100;
	g_Battystatus_bak = 10;

      /* 5. Force Battery icon to render immediately (on time) */
#ifdef FIXED_LATE_BATTERY_DISPLAY
        g_BatteryBarPrintState = BATTERY_BAR_IDLE;
#endif

	DisplayBTPro();
	DisplayBatty(DIS_BAT_X, DIS_BAT_Y);

	/* Restore the temperature readout with the rest of the home screen only in the protected build. */
	#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
	DisplayTemp(DIS_TEMP_X, DIS_TEMP_Y);
	#endif
	
#ifdef POSIFLOW_LOGO_AND_NAME
	m_w = GetStr8x16Wide((char*)PRODUCT_NAME,1);
	DisplayStr8x16char((char*)PRODUCT_NAME,(128-m_w)/2,14,1);
#else
	m_w = GetStr8x16Wide((char*)"SR20",1);
	DisplayStr8x16char("SR20",(128-m_w)/2,14,1);
#endif

#ifdef FIXED_LATE_BATTERY_DISPLAY
        /* UNLOCK transmission to send the completed screen to hardware in 1 atomic write */
        g_DisplayHoldOff = 0;
#endif
	DisplayBufEn();
}
#endif

static unsigned char g_BuzzerRecursionFlag = 0;

#ifdef AUDIO_BEEP_SEQUENCE
/**
* @FuncName  BuzzerManager_Init
* @param     uint8_t is_paper_out - Current paper sensor status (1 = Paper Out / Absent, 0 =
Paper Present)
*            uint8_t is_lid_open  - Current lid cover status (1 = Lid Open, 0 = Lid Closed)
* @retval    void
* @LocalVar  uint8_t user_ui_index - Encoded user selection index retrieved from non-
volatile memory
* @GlobalVar g_BuzzerCtx, s_PrintrConfig, g_sysTime
* @Description Initializes the buzzer state machine for a new error notification session.

*              Execution Breakdown:
*              1. Context Reset:
*                 - Stores current fault states (`prev_paper_status`, `prev_lid_status`).
*                 - Resets `reminders_played = 0` counter for new error session.
*                 - Sets `next_action_timestamp = g_sysTime` to force immediate trigger.
*              2. User Parameter Translation:
*                 Reads option index from `s_PrintrConfig.m_SaveConPar[MENU_PAR_INDEX_REMINDER]`:
*                 - Index 0: `target_reminders = 0xFF` (Continuous mode)
*                 - Index 1: `target_reminders = 3`    (3 Times)
*                 - Index 2: `target_reminders = 5`    (5 Times)
*                 - Index 3: `target_reminders = 9`    (9 Times)
*              3. State Transition:
*                 Sets `current_state = BUZZ_STATE_CALC_INTERVAL`.
*/
void BuzzerManager_Init(uint8_t is_paper_out, uint8_t is_lid_open)
{
    g_BuzzerCtx.prev_paper_status = is_paper_out;
    g_BuzzerCtx.prev_lid_status   = is_lid_open;
    g_BuzzerCtx.reminders_played  = 0;		//new session
    
    
    // Force the state machine to trigger immediately upon entry, 
    // ignoring any leftover delays from the previous error session.
    g_BuzzerCtx.next_action_timestamp = g_sysTime; 
    
    // Read user configuration from non-volatile memory 
    uint8_t user_ui_index = s_PrintrConfig.m_SaveConPar[MENU_PAR_INDEX_REMINDER] & 0xFF;
    
    // Translate the UI index into our actual state machine targets
    switch(user_ui_index) {
        case 0: g_BuzzerCtx.target_reminders = 0xFF; break; // Index 0 = Continuous
        case 1: g_BuzzerCtx.target_reminders = 3;    break; // Index 1 = 3 Times
        case 2: g_BuzzerCtx.target_reminders = 5;    break; // Index 2 = 5 Times
        case 3: g_BuzzerCtx.target_reminders = 9;    break; // Index 3 = 9 Times
        default: g_BuzzerCtx.target_reminders = 0xFF; break; // Failsafe
    }

    g_BuzzerCtx.current_state = BUZZ_STATE_CALC_INTERVAL;
		
    
}
/**
* @FuncName  Process_Buzzer_State_Machine
* @param     void
* @retval    void
* @LocalVar  uint32_t play_time - Computed total execution time of the active beep pulse sequence
* @GlobalVar g_BuzzerRecursionFlag, g_BuzzerCtx, g_sysTime, g_System_Sound_Flag
* @Description Periodic non-blocking background task that executes error buzzer sequences
*              based on active state timestamps without blocking main loop execution.
*
*              State Machine Processing:
*              - Re-entrancy Protection: Returns immediately if `g_BuzzerRecursionFlag` is set.
*              - Idle / Exhausted Guard: Exits if state is `BUZZ_STATE_IDLE` or `BUZZ_STATE_EXHAUSTED`.
*              - Timestamp Delay Check: Exits if `g_sysTime < next_action_timestamp`.
*              - State Actions:
*                - `BUZZ_STATE_CALC_INTERVAL`: Calculates `interval_ms` gap and advances to `BUZZ_STATE_PLAY_MEDIA`.
*                - `BUZZ_STATE_PLAY_MEDIA`: Generates 3 paper-out beeps via `Audio_Beep_Custom()` if paper missing; moves to `BUZZ_STATE_GAP_WAIT`.
*                - `BUZZ_STATE_GAP_WAIT`: Pauses 500ms gap, moves to `BUZZ_STATE_PLAY_LID`.
*                - `BUZZ_STATE_PLAY_LID`: Generates 3 lid-open beeps via `Audio_Beep_Custom()` if cover open; moves to `BUZZ_STATE_INTERVAL_WAIT`.
*                - `BUZZ_STATE_INTERVAL_WAIT`: Increments `reminders_played`. If target count reached, enters `BUZZ_STATE_EXHAUSTED`,
*                  else schedules next interval timestamp (`next_action_timestamp = g_sysTime + interval_ms`).
*/
void Process_Buzzer_State_Machine(void)
{
	if (g_BuzzerRecursionFlag) return;
	
    if (g_BuzzerCtx.current_state == BUZZ_STATE_IDLE || 
        g_BuzzerCtx.current_state == BUZZ_STATE_EXHAUSTED) {
        return; 
    }

    if (g_sysTime < g_BuzzerCtx.next_action_timestamp) {
        return; 
    }
		
		g_BuzzerRecursionFlag = 1;
    switch (g_BuzzerCtx.current_state)
    {
        case BUZZ_STATE_CALC_INTERVAL:
            if (g_BuzzerCtx.target_reminders == 0xFF) {
                g_BuzzerCtx.interval_ms = 5000; // 5-second gap for continuous mode
            } else {
                if (g_BuzzerCtx.target_reminders == 0) g_BuzzerCtx.target_reminders = 3; // Failsafe
                g_BuzzerCtx.interval_ms = 60000 / g_BuzzerCtx.target_reminders; // Distribute over 1 min
            }
            g_BuzzerCtx.current_state = BUZZ_STATE_PLAY_MEDIA;
            break;

        case BUZZ_STATE_PLAY_MEDIA:
            if (g_BuzzerCtx.prev_paper_status) {
#ifdef SYSTEM_SOUND_SETTING
                if (g_System_Sound_Flag) {
                    Audio_Beep_Custom(MEDIA_BEEP_COUNT, MEDIA_ABSENT_BEEP_ON_TIME_MS, MEDIA_ABSENT_BEEP_OFF_TIME_MS);
                }                
#else                
								Audio_Beep_Custom(MEDIA_BEEP_COUNT, MEDIA_ABSENT_BEEP_ON_TIME_MS, MEDIA_ABSENT_BEEP_OFF_TIME_MS);
#endif                
								uint32_t play_time = MEDIA_BEEP_COUNT * (MEDIA_ABSENT_BEEP_ON_TIME_MS + MEDIA_ABSENT_BEEP_OFF_TIME_MS);
                g_BuzzerCtx.next_action_timestamp = g_sysTime + play_time;
            } else {
                g_BuzzerCtx.next_action_timestamp = g_sysTime; 
            }
            g_BuzzerCtx.current_state = BUZZ_STATE_GAP_WAIT;
            break;

        case BUZZ_STATE_GAP_WAIT:
            g_BuzzerCtx.next_action_timestamp = g_sysTime + 500; // 500ms gap between sequences
            g_BuzzerCtx.current_state = BUZZ_STATE_PLAY_LID;
            break;

        case BUZZ_STATE_PLAY_LID:
            if (g_BuzzerCtx.prev_lid_status) {
#ifdef SYSTEM_SOUND_SETTING
                if (g_System_Sound_Flag) {
                    Audio_Beep_Custom(LID_BEEP_COUNT, LID_OPEN_BEEP_ON_TIME_MS, LID_OPEN_BEEP_OFF_TIME_MS);
                }							
#else							
                Audio_Beep_Custom(LID_BEEP_COUNT, LID_OPEN_BEEP_ON_TIME_MS, LID_OPEN_BEEP_OFF_TIME_MS);
#endif                
								uint32_t play_time = LID_BEEP_COUNT * (LID_OPEN_BEEP_ON_TIME_MS + LID_OPEN_BEEP_OFF_TIME_MS);
                g_BuzzerCtx.next_action_timestamp = g_sysTime + play_time;
            } else {
                g_BuzzerCtx.next_action_timestamp = g_sysTime;
            }
            g_BuzzerCtx.current_state = BUZZ_STATE_INTERVAL_WAIT;
            break;

        case BUZZ_STATE_INTERVAL_WAIT:
            g_BuzzerCtx.reminders_played++;
            if (g_BuzzerCtx.target_reminders != 0xFF && g_BuzzerCtx.reminders_played >= g_BuzzerCtx.target_reminders) {
                g_BuzzerCtx.current_state = BUZZ_STATE_EXHAUSTED; // Go silent							
						} 
						else {
                g_BuzzerCtx.next_action_timestamp = g_sysTime + g_BuzzerCtx.interval_ms;
                g_BuzzerCtx.current_state = BUZZ_STATE_PLAY_MEDIA;
            }
            break;
            
        default:
            g_BuzzerCtx.current_state = BUZZ_STATE_IDLE;
            break;
    }
		g_BuzzerRecursionFlag = 0;
}

/**
* @FuncName  BuzzerManager_UpdateUI
* @param     void
* @retval    void
* @LocalVar  uint8_t user_ui_index - Menu selection index from non-volatile config
*            uint8_t new_target   - Newly mapped target reminder count
* @GlobalVar s_PrintrConfig, g_BuzzerCtx, g_sysTime
* @Description swaps the active alert reminder target count in real-time when the user
*              modifies the Alert setting within the OLED menu.
*
*              Execution Flow:
*              - Maps new UI menu selection to numeric target (`0xFF`, `3`, `5`, or `9`).
*              - If target changed and an error state is currently active (`current_state!= BUZZ_STATE_IDLE`),
*                resets `reminders_played = 0`, sets `current_state = BUZZ_STATE_CALC_INTERVAL`,
*                and forces `next_action_timestamp = g_sysTime` to immediately play the new  sequence.
*/
void BuzzerManager_UpdateUI(void)
{
    uint8_t user_ui_index = s_PrintrConfig.m_SaveConPar[MENU_PAR_INDEX_REMINDER] & 0xFF;
    uint8_t new_target;
    
    switch(user_ui_index) {
        case 0: new_target = 0xFF; break; 
        case 1: new_target = 3;    break; 
        case 2: new_target = 5;    break; 
        case 3: new_target = 9;    break; 
        default: new_target = 0xFF; break; 
    }

    // Only force a reset if the setting actually changed
    if (new_target != g_BuzzerCtx.target_reminders) {
        g_BuzzerCtx.target_reminders = new_target;
        
        // If an error alert is currently active, restart the sequence so the user hears the change immediately
        if (g_BuzzerCtx.current_state != BUZZ_STATE_IDLE) {
            g_BuzzerCtx.reminders_played = 0;
            g_BuzzerCtx.current_state = BUZZ_STATE_CALC_INTERVAL;
            g_BuzzerCtx.next_action_timestamp = g_sysTime; // Force immediate trigger
        }
    }
}

#endif

#ifdef BT_NACK
unsigned char g_DiscardPrinting = 0;


void UpdateOLEDStatus(char* msg) {
	unsigned int m_w;
	extern unsigned char GetStr8x16Wide(char * pstr,unsigned char m_Type);
	
	extern void DisplayBufEn(void);

	DisplayStr8x16char("          ",20,14,0);
	m_w = GetStr8x16Wide(msg, 1);
	DisplayStr8x16char(msg, (128-m_w)/2, 14, 1);
	DisplayBufEn();
}
/**
* @FuncName  ShowSizeMismatchPrompt
* @param     void
* @retval    unsigned char - Decision result:
*                            1 = User pressed Line Feed (LF) button to override overflow and force print
*                            0 = User timed out (5s) or canceled the print job
* @LocalVar  unsigned int m_w       - Calculated text string pixel width for OLED display centering
*            uint32_t prompt_timer  - Millisecond countdown timer tracking user input window (5000ms limit)
*            unsigned char result   - Return decision flag
* @GlobalVar PRINTER_PDL
* @Description Interactive user prompt handler for label height overflow conditions (`BT_NACK_SIZE_MISMATCH`).
*
*              Detailed Execution Flow:
*              1. Bluetooth Protocol Alert: Calls `BT_SendNACK(BT_NACK_SIZE_MISMATCH)` to send telemetry
*                 error response `"BT_NACK: Label Size Mismatch\r\n"` to the connected Bluetooth host application.
*              2. OLED Warning Render: Clears display buffer and renders `"Size Overflow"` on line 1
*                 and `"Print? Feed=Y"` on line 2.
*              3. Key-Press Input Loop: Enters a 5-second polling loop inspecting the physical Line Feed (`LF`) button:
*                 - If LF button is pressed (`_GetKey_LF() == 0`), sets `result = 1` (Override),
*                   waits for button release while feeding watchdog, and breaks loop.
*                 - If 5 seconds elapse without key press, defaults to `result = 0` (Cancel).
*              4. Screen Restoration & Status Render: If confirmed, displays `"printing"` status string
*                 on screen; otherwise clears screen. Returns `result`.
*/
unsigned char ShowSizeMismatchPrompt(void)
{
	unsigned int m_w;
	uint32_t prompt_timer = 0;
	unsigned char result = 0; 
	extern void BT_SendNACK(BT_NACK_Reason reason);
	extern unsigned char GetStr8x16Wide(char * pstr,unsigned char m_Type);
	extern void ClearDisplayBuf(void);
	extern void DisplayBufEn(void);
	extern void Delay1ms(unsigned short mdelay);

	/* 1. Transmit Bluetooth NACK Error Response to Host */
	BT_SendNACK(BT_NACK_SIZE_MISMATCH);
	
	//Display Error msg on OLED
	ClearDisplayBuf();
	m_w = GetStr8x16Wide((char*)"Size Overflow", 0);
	DisplayStr8x16char("Size Overflow", (128-m_w)/2, 0, 0);
	m_w = GetStr8x16Wide((char*)"Print? Feed=Y", 0);
	DisplayStr8x16char("Print? Feed=Y", (128-m_w)/2, 16, 0);
	DisplayBufEn();
	
	while(prompt_timer < 5000) 	//5 sec window for user
	{
		if(PRINTER_PDL.SYS._GetKey_LF() == 0) 
		{
			result = 1; 
			while(PRINTER_PDL.SYS._GetKey_LF() == 0) PRINTER_PDL.SYS._WDG_ReloadCounter();			// Check if Line Feed (LF) button is pressed
			break;
		}
		else
		{
			result = 0; 
		}
		Delay1ms(1);
		prompt_timer++;
		PRINTER_PDL.SYS._WDG_ReloadCounter();
	}
	//update oled based on user decision
	if (result == 1) {
		OLED_RestoreHomeScreen();
		DisplayStr8x16char("                ",20,14,0);
		m_w = GetStr8x16Wide((char*)"printing", 1);
		DisplayStr8x16char("printing", (128-m_w)/2, 14, 1);
		DisplayBufEn();
	} else {
		ClearDisplayBuf();
	}
	
	return result;
}


/**
* @FuncName  BT_NACK_Response_Media_And_Size
* @param     char *Media_Type        - Media type string identifier ("L" for Label, "P" for Paper)
*            uint16_t Get_Media_Size - Calibrated physical media length expressed in millimeters
* @retval    void
* @LocalVar  char TxBuffer_NACK[64] - Formatted output response text buffer
* @GlobalVar PRINTER_PDL
* @Description Formats and transmits current media status and calibrated length back to
*              the paired Bluetooth host application.
*
*              Execution Breakdown:
*              - Formats string: `"MediaType:<Media_Type>,Size:<Get_Media_Size>"`.
*              - Outputs debug text via `MyPrintf()`.
*              - Transmits payload over Bluetooth connection via `PRINTER_PDL.BT._ReturnNBytes()`.
*/
void BT_NACK_Response_Media_And_Size(char *Media_Type,uint16_t Get_Media_Size)
{
	char TxBuffer_NACK[64];
	
	sprintf(TxBuffer_NACK,"MediaType:%s,Size:%d",Media_Type,Get_Media_Size);
	MyPrintf("%s\n", TxBuffer_NACK);
	PRINTER_PDL.BT._ReturnNBytes((unsigned char *)TxBuffer_NACK, strlen(TxBuffer_NACK));

}

#endif


//Added to check paper feed solution
void Before_PrinterCOnfig(void)
{
	MyPrintf("m_CRC=%X\r\n",s_PrintrConfig.m_CRC);
	
	MyPrintf("m_InterFace=%X\r\n",s_PrintrConfig.m_InterFace);//´òÓ¡»ú½øÈë´òÓ¡Èë¿Ú±êÖ¾0

	MyPrintf("m_CharSpace=%X\r\n",s_PrintrConfig.m_CharSpace);//×Ö¼ä¾à1
	MyPrintf("m_ChineseLib=%X\r\n",s_PrintrConfig.m_ChineseLib);//ºº×Ö×Ö¿âÀàÐÍ GBK GB2312  2
	MyPrintf("m_ChineseFont=%X\r\n",s_PrintrConfig.m_ChineseFont);//ºº×Ö×ÖÌåÀàÐÍ3
	MyPrintf("m_DefaultChar=%X\r\n",s_PrintrConfig.m_DefaultChar);;//Ä¬ÈÏ´òÓ¡×Ö·û4
	MyPrintf("m_DefaultChinese=%X\r\n",s_PrintrConfig.m_DefaultChinese);//Ä¬ÈÏºº×Ö5
	MyPrintf("m_PrintDir=%X\r\n",s_PrintrConfig.m_PrintDir);//´òÓ¡·½Ïò6
	MyPrintf("m_HoriZoomOut=%X\r\n",s_PrintrConfig.m_HoriZoomOut);//ºáÏò·Å´ó±¶Êý7
	MyPrintf("m_VertZoomOut=%X\r\n",s_PrintrConfig.m_VertZoomOut);//×ÝÏò·Å´ó±¶Êý8
	MyPrintf("m_Liftpos=%X\r\n",s_PrintrConfig.m_Liftpos);//´òÓ¡»ú×óÏÞ9
	MyPrintf("m_Righttpos=%X\r\n",s_PrintrConfig.m_Righttpos);//´òÓ¡»úÓÒÏÞ10 A
	MyPrintf("m_BaudRate=%X\r\n",s_PrintrConfig.m_BaudRate);//´òÓ¡»ú´®¿Ú»úÐÍÄ¬ÈÏ²¨ÌØÂÊ11 B
	MyPrintf("m_SoftwarePart=%X\r\n",s_PrintrConfig.m_SoftwarePart);//Èí¼þ·ÖÇø12 C
	MyPrintf("m_HotDeep=%X\r\n",s_PrintrConfig.m_HotDeep);//¼ÓÈÈÉî¶È13 D
	MyPrintf("m_PrintSub=%X\r\n",s_PrintrConfig.m_PrintSub);//´òÓ¡Ï¸·Ö¶È14 E

	MyPrintf("m_HotLineTOPD=%X\r\n",s_PrintrConfig.m_HotLineTOPD);//15 F ¼ÓÈÈÏßµ½È±Ö½¼ì²âµÄ¾àÀë ÒÔmmÎªµ¥Î» 8mm
	MyPrintf("m_HotLineTOOUT=%X\r\n",s_PrintrConfig.m_HotLineTOOUT);//16 10 ¼ÓÈÈÏßµ½³öÖ½¿ÚµÄ¾àÀë ÒÔmmÎªµ¥Î» 5mm
	MyPrintf("m_PrintLabFlage=%X\r\n",s_PrintrConfig.m_PrintLabFlage);//17 11 ±êÇ©´òÓ¡±êÖ¾
	MyPrintf("m_LabelWide=%X\r\n",s_PrintrConfig.m_LabelWide);//18 12±êÇ©Ö½¿í¶È ÒÔmmÎªµ¥Î» 56mm
	MyPrintf("m_LabelLength=%X\r\n",s_PrintrConfig.m_LabelLength);//19 13±êÇ©Ö½³¤¶È ÒÔmmÎªµ¥Î» 30mm
	MyPrintf("m_BarPaGap=%X\r\n",s_PrintrConfig.m_BarPaGap);//20 14 ±êÇ©Ö½·ì
	MyPrintf("m_PE_LabValue_L=%X\r\n",s_PrintrConfig.m_PE_LabValue_L);//21 15ÇúÏßÉÏÉýÇ÷ÊÆ·§Öµ
	MyPrintf("m_PE_LabValue_H=%X\r\n",s_PrintrConfig.m_PE_LabValue_H);//22 16ÇúÏßÏÂ½µÇ÷ÊÆ·§Öµ
	MyPrintf("m_ReceiveBuffrWor=%X\r\n",s_PrintrConfig.m_ReceiveBuffrWor);//23 17ÉèÖÃ´òÓ¡»ú»º³åÇø±êÖ¾
	MyPrintf("m_LineSpace=%X\r\n",s_PrintrConfig.m_LineSpace);//24 18ÉèÖÃ´òÓ¡Ä¬ÈÏÐÐ¼ä¾à
	MyPrintf("m_CharType=%X\r\n",s_PrintrConfig.m_CharType);//25 19 »úÆ÷´òÓ¡ÀàÐÍ,ºº×Ö»úÐÍ»ò Ó¢ÎÄ»úÐÍ
	MyPrintf("m_PrintEndFeed=%X\r\n",s_PrintrConfig.m_PrintEndFeed);//26 1A ´òÓ¡Íê³ÉºóÏòÇ°×ßÖ½
	MyPrintf("m_PrintEnCode=%X\r\n",s_PrintrConfig.m_PrintEnCode);//27 ¼ÓÃÜ
	MyPrintf("m_Delayms_Head=%X\r\n",s_PrintrConfig.m_Delayms_Head);//28 1CÑÓÊ±µÈ´ý
	MyPrintf("m_Delayms_End=%X\r\n",s_PrintrConfig.m_Delayms_End);

}










#ifdef PSF20_R002_TSPL_PROTOCOL
/* Function name: TsplCore_Readiness; Description: Snapshot existing protection gates.
 * Parameters: None; Returns: TSPL readiness; Global vars: thermal/battery state.
 * Remarks: An active label retains the established thermal boundary policy. */
TsplResult_t TsplCore_Readiness(void)
{
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
    if ((g_ThermalState == THERMAL_STATE_STOP) && !g_ThermalRasterLabelActive) return TSPL_NOT_READY;
#endif
#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING
    if (g_BatteryLowChargeShutdownRequest) return TSPL_NOT_READY;
#endif
    return TSPL_OK;
}

/* Function name: TsplCore_LabelStarted; Description: Reuse label protection accounting.
 * Parameters: None; Returns: None; Global vars: existing thermal/battery label flags.
 * Remarks: Called once per accepted native copy. */
void TsplCore_LabelStarted(void)
{
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    BatteryLowCharge_LabelStarted();
#endif
#ifdef BATTERY_INDICATION_SOLUTION
    BatteryBar_PrintStarted();
#endif
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
    g_ThermalRasterLabelActive = 1U;
    g_ThermalRasterFinalChunkQueued = 0U;
    g_ThermalRasterFinalBuffer = 0U;
#endif
}

/* Function name: TsplCore_LabelFinal; Description: Mark the native final list buffer.
 * Parameters: existing buffer id 1 or 2; Returns: None; Global vars: boundary flags.
 * Remarks: The existing motor ISR performs the actual safe stop. */
void TsplCore_LabelFinal(uint8_t buffer)
{
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    BatteryLowCharge_LabelFinalQueued(buffer);
#endif
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
    g_ThermalRasterFinalBuffer = buffer;
    g_ThermalRasterFinalChunkQueued = 1U;
#endif
}

/* Function name: TsplCore_LabelFinished; Description: Reuse physical completion handling.
 * Parameters: None; Returns: None; Global vars: boundary/accounting flags.
 * Remarks: Called only after the final list was claimed and the motor stopped. */
void TsplCore_LabelFinished(void)
{
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    BatteryLowCharge_LabelFinished();
#endif
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
    g_ThermalRasterLabelActive = 0U;
    g_ThermalRasterFinalChunkQueued = 0U;
    g_ThermalRasterFinalBuffer = 0U;
#endif
}

/* Function name: TsplCore_LabelAborted; Description: Cancel existing label accounting.
 * Parameters: None; Returns: None; Global vars: existing thermal/battery flags.
 * Remarks: Aborted copies never count as successfully printed low-battery labels. */
void TsplCore_LabelAborted(void)
{
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    BatteryLowCharge_LabelAborted();
#endif
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
    g_ThermalRasterLabelActive = 0U;
    g_ThermalRasterFinalChunkQueued = 0U;
    g_ThermalRasterFinalBuffer = 0U;
#endif
}

/* Function name: TsplCore_Service; Description: Service existing foreground facilities.
 * Parameters: None; Returns: None; Global vars: existing UI/protection state.
 * Remarks: Prompt waits never call blocking receive, media recovery or cooling loops.
 * Ordinary media recovery and manual feeding keep the existing hardware routines. */
void TsplCore_Service(void)
{
    static uint32_t s_lastDisplay;
    static uint8_t s_feedWasDown;
    uint8_t feedDown = (uint8_t)(PRINTER_PDL.SYS._GetKey_LF() == 0U);
    TsplInputs_t inputs;
    PRINTER_PDL.SYS._WDG_ReloadCounter();
#ifdef BATTERY_LOW_CHARGE_LABEL_SHUTDOWN
    BatteryLowCharge_Service();
#endif
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
    Thermal_State_Machine_Update();
    if ((g_ThermalState != THERMAL_STATE_STOP) && g_ThermalBoundaryPause) {
        g_ThermalBoundaryPause = 0U;
        g_ThermalStopPending = 0U;
        g_ThermalPrintTrack.thermal_pause_active = 0U;
    }
#endif
    TsplNative_Read(&inputs);
    if (TsplFirmware_Busy() || TsplFirmware_OwnsFeed()) {
        s_feedWasDown = feedDown;
        g_PowerOffCount = 0U;
        return;
    }
    if (inputs.readiness == TSPL_MEDIA) {
        TsplFirmware_ReceiveError();
        PrinterLPS();
        s_feedWasDown = feedDown;
        return;
    }
    ReadKeyMenu();
    if (!g_OptionStatus && s_feedWasDown && !feedDown) RunFeedLabPaper();
    s_feedWasDown = feedDown;
    if ((uint32_t)(g_tsplMilliseconds - s_lastDisplay) >= 500U) {
        s_lastDisplay = g_tsplMilliseconds;
        g_pinrtStb = CalStbTime();
        g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();
        DisplayBTPro(); DisplayBatty(DIS_BAT_X, DIS_BAT_Y);
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
        DisplayTemp(DIS_TEMP_X, DIS_TEMP_Y);
#endif
    }
}
#endif
