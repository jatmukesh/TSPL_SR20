#include "PDL.h"
#include "LabelPro.h"
#include "flash.h"
#include "ParseESC.h"
#include <stdio.h>
#include <string.h>
//软件延时程序，不是精确的延时
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
#define EraseTimeout          ((unsigned int)0x000B0000)
//*****************************************************************************
extern _PDL		PRINTER_PDL;
extern void MyPrintf(char *format, ...);
//***************计算参数表**************************************************************

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

//8 无校验
const char SelfTestD_8N1[]={",N,8,1\r"};
//8 奇校验
const char SelfTestD_8O1[]={",O,8,1\r"};
//8 偶校验
const char SelfTestD_8E1[]={",E,8,1\r"};
//7 偶校验
const char SelfTestD_7E1[]={",E,7,1\r"};

const char Self_Reverse[]={"Reverse printing\r"};
const char Self_Forward[]={"Forward printing\r"};

const char SelfTestD_Ctrl[]={"flow ctrl:hardware\r"};
const char SelfTestD_XFCtrl[]={"flow ctrl:Xon-Xoff\r"};

const char SelfTestD_2[]={"Installed Fonts:\r\
0:ANK 6X8\r\
1:SYSTEM 12X24\r\
"};
const char SelfTestD_2_1[]={"Installed Fonts:\r\
0:ANK 6X8\r\
1:SYSTEM 8X16\r\
"};

const char SelfTestD_24_GB2312[]={"2:24X24 国标一二级汉字库\r"}	 ;
const char SelfTestD_16_GB2312[]={"2:16X16 国标一二级汉字库\r"}	  ;
const char SelfTestD_24_GBK[]={"2:24X24 GB18030汉字库\r"};
const char SelfTestD_16_GBK[]={"2:16X16 GB18030汉字库\r"};

const char GB2312_24x24[]={"GB2312_24x24"};
const char GB18030_24x24[]={"GB18030_24x24"};
const char GB2312_16x16[]={"GB2312_16x16"};




const U8 SelfTest_S[]={'F','u','l','l',' ','B','a','r','c','o','d','e',' ','S','u','p','p','o','r','t','.',0x0D,
0x1D,0x6B,0x49,14,0x7B,0x41,'A','B','0','1','2','3','4','5','6','7','8','9',0x0D,
0x43,0x68,0x61,0x72,0x61,0x63,0x74,0x65,0x72,0x20,0x53,0x65,0x74,0x3A,0x28,0x6D,0x6F,0x64,0x69,0x66,0x69,0x65,0x64,0x29,0x1C,0x38,0x0D,0xff};

#ifndef NONAME_SELF

const U8 SelfTest_COM[]={"\
Tel:+86-10-62105070\r\
Web:www.rd-cn.com\r\
\r\
\r\
\r\
Selftest Finished!\r\r\r"};
#endif

#ifdef NONAME_SELF

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

//***************电机参数加热参数**************************************************************

extern const U32 PrintMotoHalfStep[8];

const unsigned short VolRUNTime[]=  {4500,4000,3500,3000,2500,2000,1500,1431,1362,1293,1224,1155,1086,1017,948,879,810,741,672,672,672,672};

#ifdef NEW_JX
const unsigned short VolTHSTB[]=    {4500,4000,3500,3000,2500,2000,1400,1300,1200,1150,1090,1000, 832, 834,746,650,570,530,512,512,512,512};
#endif

#ifndef NEW_JX
const unsigned short VolTHSTB[]=    {4500,4000,3500,3000,2300,1900,1700,1500,1300,1250,1190,1100, 932, 934,846,750,570,530,512,512,512,512};
#endif




const unsigned short VolRUNPWM[]=   { 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304,304,304,304,304,304,304,304,304};
const unsigned short VolFreeTime[]= {1000,1000,1000,1000,1000,1000,1000, 950, 900, 850, 800, 750, 700, 650,600,550,500,450,400,400,400,400};

const unsigned short VolTemAdd[]= 	{ 300, 300, 300, 300, 300, 250, 200, 192, 184, 176, 166, 158, 150, 142,134,126,118,108,100,100,100,100};
const unsigned short VolTemSubb[]= 	{ 200, 200, 200, 200, 200, 150, 100,  96,  92,  88,  84,  80,  76,  72, 68, 64, 60, 56, 50, 50, 50, 50};

	
	
	//const unsigned short VolTHSTB_2[]=    {4500,4000,3500,3000,2500,2000,1500,1400,1300,1250,1190,1100, 932, 932,846, 800,750,700,650,650,650,650};

const unsigned short VolTHSTB_2[]=    {4500,4000,3500,3000,2500,2000,1500,1400,1300,1250,1190,1100, 550, 550,550, 550,550,550,550,550,550,550,550,550,550,400};

//************全局变量*****************************************************************
extern unsigned int   Getdata_ok;//判断ISP数据是否接收完成

extern	unsigned short g_printChar;

//------------数据缓冲区------------------------------------------------------------------------
unsigned char  receive_bufer[BUFFERSIZE]={0};

extern unsigned int Receive_count;
extern unsigned int	g_Receive_count;//数据接收
extern unsigned int	g_Order_count;//取命令计数器
unsigned short g_ProFlagVelue;
extern unsigned int *p_OcAndGDAdr;//得到数据的指针

extern  unsigned int *p_REAndSAcount;//保存数据的指针
extern unsigned int *p_SaveDrREcount;//存储数据到缓冲区的偏移量
//------------------------------------------------------------------------------------
unsigned short	g_CalcPrintPos;//打印数据当前的位置
unsigned short	g_PrintLineCount;//打印行数计数器
unsigned short	g_MotoAll;//电机走步索引
//unsigned short	g_BufferLength;//接受数据长度
//unsigned short	g_MotoSTBStatus;//加热使能标志
unsigned short	g_BufferSwitch;//链表缓冲区切换标志

#define DATA_IN_SEND				0x5A67
#define DATA_SEND_OK				0x57A9
unsigned short	g_SendFlag;//链表缓冲区切换标志

unsigned short	g_mapCount;//图形数据计数器
unsigned char 	CodeBarStr[40];
unsigned char	HTValue[20];//保存水平造表值
unsigned char	HTCount;//水平造表计数器
unsigned short	g_0D0AFlag;//0D0A打印标志
extern unsigned char	g_0D0AHigh;//回车走纸高度
//-----------------------------------------------------------------------------
unsigned short	g_PrintStatusReg;//命令链表解析完成状态
//****************************************************************************
GetOrderPoint	s_PrintOrderstr;//取命令结构
PrinterStruct	s_PrintParameters;//打印机参数
PrintControl	s_PrinterMOTOPar;//打印机步进电机参数
CodeBarCon		s_CodeBarCon;//条码控制参数

PrintrConfig	s_PrintrConfig;//打印机配置参数

unsigned short g_DetPaperGapSpace;
unsigned short g_DetPaperNoprint;

unsigned short g_NotStbFlag;

extern unsigned short g_PaperType;

unsigned int	m_OrderSave;//保留取数指针

//	unsigned short	m_flag;//
extern unsigned int   Getdata_ok;//判断ISP数据是否接收完成

PrintListPar		s_ListParmeter_A;//缓冲区A
PrintListPar		s_ListParmeter_B;//缓冲区B

unsigned short	Buffer_Dataflash[36];
unsigned short	Buffer_CodeBar[BUFFER_SHORT_LENGTH];//条码缓冲区

unsigned short	g_alignType;//左右中 对齐方式
extern unsigned short g_ReturnEp;
unsigned short	g_SpeedChoice;//设置打印机运行速度
unsigned short	g_DriverAFlag;//驱动打印标志
//	unsigned short	g_CutterErr;//
//	unsigned int	g_BUSYFlag;//Busy 标志
extern unsigned short*	Buffer_writer_I;//[10];//条码缓冲区 可以公用
p_PrintListPar			p_ListParmeters	;//链表结构指针
//int dd;
unsigned short*	p_ListDataDot;//链表中字符图形的存放位置
unsigned int FlowFlag;//波特率

unsigned short	g_dotSwitch;//预留点数切换
unsigned int 	print_head_length;//			384

unsigned short g_StandbyPar;

unsigned short g_CutterErr;

unsigned short g_Char24_17;
unsigned short	m_charType;//字符类型
unsigned short*	p_MapCurListDataDot;//图形曲线指针位置
extern unsigned short g_CurveCountFlag;


unsigned short	m_high;//字符高
unsigned short	m_Wide;//字符宽

unsigned short	m_CharWide;//字符宽
unsigned short	m_SaverightPos;//右宽度
unsigned short	m_UPDown;//上下对齐

unsigned short	m_len;//点阵数据长度
unsigned short	g_SavePrintPos;//保存左限
unsigned short*	p_ListSave;//链表保存指针
unsigned short	m_GetData;
p_PrintListPar	p_ListPrease;//链表结构指针
unsigned short*	p_IndexListData;//链表单元索引地址
unsigned short	m_ProcessFlag;//命令处理标志
unsigned int	m_ProcessStatus;

unsigned int	g_THDeep;//增加加热值

unsigned short	g_PaperEnd;//缺纸
unsigned short g_PaperOutAD,g_PaperGapAD;
//****************************************************************************
extern unsigned short AD_Value[3];
/****************************************************************************/
unsigned short g_TimeOutFlag;
unsigned int			g_lineBitCount,g_S_lineBitCount;//计算点行中加热点数
unsigned short			g_RunFlag;//加热运行标志
unsigned short			SendBufferToSPI[BUFFER_SHORT_LENGTH];
unsigned short			SaveToBuffer[BUFFER_SHORT_LENGTH];
unsigned int			SendBuffer_counter;
unsigned int			SendToSPI_counter;
unsigned short			g_LPSStatus,g_PE_Standard;//缺纸检测器状态

unsigned short			g_RunLTime;

extern  unsigned short g_CodePage;//国际字符代码页
extern unsigned short g_InternationChar;//国际字符集

 //****************************************************************************
unsigned short g_ConfigRCC;
extern unsigned short	g_unicodeType;//unicode计数器
extern  unsigned int	Erase_Sector;
extern  unsigned int	Order_Erase_Sector;

unsigned short g_BarCharHigh;
extern unsigned int		g_SaveDataToFlashAddr;//存储数据的当前地址
extern unsigned int		g_GetDataFromFlashAddr;//从flash中取数据的地址

extern unsigned int		g_SaveDataOffset;//存储数据的当前偏移量
extern unsigned int		g_GetDataOffset;//得到储数据的当前偏移量

extern unsigned short	g_InterFaceBufferCount;//端口接收数据缓冲区接收到的数据个数
extern unsigned short	g_ThType;//加热类型
extern unsigned short	g_cruve_arry[20];
extern unsigned short	g_cruvefalg;
extern unsigned short	g_PrinterDip;//打印DIP
extern unsigned char	g_HTCharSpace;//水平造表计算值

unsigned short			g_ParTypeValue;//分区类型值
unsigned short* p_PrCon;//打印机配置指针

extern unsigned short g_ListControlFor;//字符控制字格式
extern unsigned short 	g_ReturnStatusFlag;
extern unsigned short g_ListPrintFinish;//是否打印完成
extern unsigned short g_PaperRunToOut;//出纸步数
extern unsigned char g_ListSaveToFlashCount;//数据存储到FLash位置偏移量计数
extern unsigned short	g_PrintListOrNorm;//是否是打印表格或者正常数据
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
void SetSoftwarePart(unsigned char baudvalue);
void SetPrintFont(unsigned char baudvalue);
void RunFeedLabPaper(void);
 void PrinterLPS(void);
extern void NpReturnStatus(unsigned char mdatOr);
#ifdef PRINTER_AND_USB
extern void returnstatus(unsigned char flag);
#endif
extern unsigned short g_ListTable[BUFFER_SHORT_LENGTH];//
extern unsigned char(*PrinterCore)(void);//外部定义的函数指针
extern unsigned short	g_receiveFlage;
unsigned short	g_DumpReceiveFlage;
unsigned short g_ProFuncValue;
extern unsigned short g_PE_UP_Count;//曲线下降趋势计数器
extern unsigned short g_PE_DN_Count;//曲线上升趋势计数器
extern unsigned short g_PEDataS;//采样值预存器


extern unsigned short g_PE_Staue;//整型后波形当前状态
extern unsigned char * p_LabData;

unsigned char g_SelfRunFlag;//自检运行标志
unsigned char Trg,Cont;

unsigned short GetPaper(void);
unsigned short GetDETPaper(void);
unsigned int gPLow1msCount;

unsigned short g_DetPaperValueFlag;
unsigned short g_DetPV_H,g_DetPV_L;
unsigned short g_DetPV_Count;

unsigned short  g_RunToGapFlag;

unsigned short g_CurrentVol,g_TMIndex;
extern unsigned short g_LabPrintEndF;


void RunFeedLabPaperLPS(void);




void PrintLineTest(void)
{
		unsigned char m_bufer[48];
		memset(m_bufer,0x33,48);
	
		PRINTER_PDL.SDTH._SendToHead_u8(m_bufer,48);
		PRINTER_PDL.SYS._LAT_H();
		//*********************************************
		PRINTER_PDL.SYS._LAT_L();
		PRINTER_PDL.SYS._LAT_H();				
		PRINTER_PDL.SYS._STB_A_EN();
		PRINTER_PDL.SYS._STB_B_EN();	
}






unsigned short g_ChineseEnco = 0;

/****************************************************************************
* 函数名：ProcessCode128()
* 功  能：主程序
*
* 说  明：

****************************************************************************/
unsigned short g_DotProlen,g_DotBuf,g_Dotcount;
unsigned short g_QRCodeBuffer[48];
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
		g_QRCodeBuffer[g_Dotcount++]= g_DotBuf;
		g_DotBuf = m_InData;
	}
}

extern QRcode *qrcode;
void SetQRCodeBuffLine(unsigned short mLine)
{
	unsigned short k;
	unsigned short mDat,mlen,m_line;
	
	mDat = 0xFFFF;
	mDat = mDat>>(16 - s_PrintParameters.m_QRZoom);
	


	if(s_PrintParameters.m_PrintDir == PRINT_DIRECT_NOR)
	{
		//台序
		
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
		g_QRCodeBuffer[g_Dotcount++]= g_DotBuf;	
	}
	else
	{
		g_Dotcount = (383 - g_QR_x - qrcode->width*s_PrintParameters.m_QRZoom)/16;
		g_DotProlen = (383 - g_QR_x - qrcode->width*s_PrintParameters.m_QRZoom)%16;
		
		g_DotBuf = 0;			
		
		m_line = qrcode->width - 1 - mLine;
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
		g_QRCodeBuffer[g_Dotcount++]= g_DotBuf;			
		
		
		
	}
}


unsigned short g_BarProlen,g_BarBuf,g_Barcount;
unsigned short g_BarCodeBuffer[48];
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
		g_BarCodeBuffer[g_Barcount++]= g_BarBuf;
		g_BarBuf = m_InData;
	}
}


extern unsigned short g_BarLen;
unsigned short g_BarCodeLen,g_BarCodeHigh,g_BarCodeHigh_Pro;
//一维码设置数据
void SetBarCodeBuffLine(unsigned short mLine)
{
	unsigned short k,i;
	unsigned short mDat,mlen,m_line;
	unsigned short m_barx;

	if(s_PrintParameters.m_PrintDir == PRINT_DIRECT_NOR)
	{
		//台序
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
			g_BarCodeBuffer[g_Barcount++]= g_BarBuf;					
		}			
						
			
			
	}
	else
	{
		//面板
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
};

void ProTraehdat(unsigned short rule)
{
	if(rule == 0)
	{
		//配置数据到数组
		//配置波特率
		g_cruve_arry[1] = s_PrintrConfig.m_BaudRate &  0x0F;

		if((s_PrintrConfig.m_BaudRate&0x30) == 0x00)
		{
			//无校验
			g_cruve_arry[2] = 0;
		}
		if((s_PrintrConfig.m_BaudRate&0x30) == 0x20)
		{	//Odd 奇效验
			g_cruve_arry[2] = 1;
		}
		if((s_PrintrConfig.m_BaudRate&0x30) == 0x10)
		{	//Even 偶效验
			g_cruve_arry[2] = 2;
		}
		if((s_PrintrConfig.m_BaudRate&0x30) == 0x30)
		{//7位偶效验
			g_cruve_arry[2] = 3;
		}
		if((s_PrintrConfig.m_BaudRate & 0x40) == 0)
		{
			g_cruve_arry[3] = 0;//硬件流控
		}
		else
		{
			g_cruve_arry[3] = 1;//软件流控
		}
		if(s_PrintrConfig.m_PrintDir == PRINT_DIRECT_NOR)
		{
			g_cruve_arry[4] = 0;//正向为0

		}
		else
		{
			g_cruve_arry[4] = 1;//反向为1
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
		//数组到配置数据
		s_PrintrConfig.m_BaudRate = g_cruve_arry[1];

		if(g_cruve_arry[2] == 0x00)
		{
			//无校验
			s_PrintrConfig.m_BaudRate &= 0xCF;
		}
		if(g_cruve_arry[2] == 0x01)
		{	//Odd 奇效验
			s_PrintrConfig.m_BaudRate &= 0xCF;
			s_PrintrConfig.m_BaudRate |= 0x20;
		}
		if(g_cruve_arry[2] == 0x2)
		{	//Even 偶效验
			s_PrintrConfig.m_BaudRate &= 0xCF;
			s_PrintrConfig.m_BaudRate |= 0x10;
		}
		if(g_cruve_arry[2] == 3)
		{//7位偶效验
			s_PrintrConfig.m_BaudRate |= 0x30;
		}
		if(g_cruve_arry[3] == 0)
		{
			//硬件流控
			s_PrintrConfig.m_BaudRate &= 0x3F;
		}
		else
		{
			s_PrintrConfig.m_BaudRate |= 0x40;//软件流控
		}
		if(g_cruve_arry[4] == 0)
		{
			//正向为0
			s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;

		}
		else
		{
			//反向为1
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
				s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//默认国标一二级
			}
			else
			{
				s_PrintrConfig.m_ChineseLib = TYPE_GBK;//默认国标一二级
			}
			s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//默认24点阵机型
			s_PrintrConfig.m_DefaultChar = TYPE_ASC12X24;//默认打印字符
			s_PrintrConfig.m_DefaultChinese = TYPE_CHIN24X24;//默认打印汉字
			s_PrintrConfig.m_HoriZoomOut = 1;//默认横向放大倍数
			s_PrintrConfig.m_VertZoomOut = 1;//默认纵向放大倍数
		}
		if(g_cruve_arry[6] == 2)
		{
			s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//默认国标一二级
			s_PrintrConfig.m_ChineseFont = PRINTTYPE_16;//默认24点阵机型
			s_PrintrConfig.m_DefaultChar = TYPE_ASC8X16;//默认打印字符
			s_PrintrConfig.m_DefaultChinese = TYPE_CHIN16X16;//默认打印汉字
			s_PrintrConfig.m_HoriZoomOut = 2;//默认横向放大倍数
			s_PrintrConfig.m_VertZoomOut = 2;//默认纵向放大倍数
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
}

unsigned short gTimer_100ms_Head,gTimer_100ms_End;
 unsigned short  g_KeyTimeOutFlag;
extern unsigned int gTimer_Print;

unsigned short g_LedPFlag = 0;
unsigned short g_LedCount;
unsigned short g_PowerOnFlag;
unsigned short g_SetModelFlag;
unsigned int g_SetModel1ms;
extern unsigned int gDelay_D_1ms;
void SysTickProFun(void)
{
	int  i;
	g_KeyTimeOutFlag++;
	gPLow1msCount++;
	g_PowerTimerCount++;
	gTimer_100ms_Head++;
	gTimer_100ms_End++;
	g_LedCount++;
	g_SetModel1ms++;
	gTimer_Print++;
	gDelay_D_1ms++;
	if(g_LedPFlag == 0xA100)
	{
		if(g_LedCount > 500)
		{
			g_LedCount = 0;
			PRINTER_PDL.SYS._LED_Stauts_Trig();
			
		}
	}
	

	#ifdef POWER_KEY

		if(PRINTER_PDL.SYS._GetKey_POW())
		{
			g_PowerTimerCount = 0;
		}
		else
		{
			if(g_PowerOnFlag == 0xA546)
			{
				if(g_PowerTimerCount > 200)
				{
					PRINTER_PDL.SYS._LED_Stauts_OFF();
					PRINTER_PDL.SYS._LED_Power_OFF();
					
					i = 3000;
					while(i--)
					{
						while( PRINTER_PDL.SYS._GetKey_POW() == 0)
						{
								PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
						}
					}
					i = 3000;
					while(i--)
					{
						while( PRINTER_PDL.SYS._GetKey_POW() == 0)
						{
								PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
						}
					}
					i = 3000;
					while(i--)
					{
						while( PRINTER_PDL.SYS._GetKey_POW() == 0)
						{
								PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
						}
					}
					i = 3000;
					while(i--)
					{
						while( PRINTER_PDL.SYS._GetKey_POW() == 0)
						{
								PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
						}
					}
					i= 3000;
					while(i--)
					{
							PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
					}					
					
					
					PRINTER_PDL.SYS._POWER_DIS();
				//	PRINTER_PDL.SYS._Drive_Power_EN();
				//	powerMOTORUN();
					PRINTER_PDL.SYS._Drive_Power_DIS();					
					
					
					while( 1)
					{
							PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
					}		
					
				}
			}
			
		}
	#endif
	
	gDelay1ms++;
	g_sysTime++;
	if(SysTickFlag == 0xAAAA)
	{
    	gTimer_1ms++;

    	if(gTimer_1ms > 600)
    	{
    		g_TimeOutFlag = 0xaa;
    	}
    }
		gTimer_cutter1ms++;
		
		
		
		
		
		
		
} // end SysTick_Handler
//第一项表示总共的项目数
//const unsigned char ParSetlist[]={2,4,6,8,10,12,16};
const unsigned char ItemIndexCount[]={6,11,4,2,2,7,3};
unsigned short m_ProKeyValue;
void ProKeySetModel(void)
{
	unsigned short m_itemMax,i,j;

	
	if(PRINTER_PDL.SYS._Get_Plus_LF())
	{
		m_ProKeyValue++;
		__SETPRIMASK();
		SysTickFlag = 0xAAAA;
		gTimer_1ms = 0;
		g_TimeOutFlag = 0;
		__RESETPRIMASK();
	//	TIM_DISABLE(TIM6);
	//	TIM_ENABLE(TIM6,6000);//演示400ms
	}
	if(g_TimeOutFlag != 0)
	{
		//SendChar(m_ProKeyValue);
		g_TimeOutFlag = 0;//清定时标志
		SysTickFlag = 0;
		if(g_ProFuncValue == 0)
		{
			s_PrintParameters.m_HoriZoomOut = 2;//横向放大倍数为1
			s_PrintParameters.m_VertZoomOut = 2;//纵向打印放大为1
			s_PrintParameters.m_CharType = TYPE_LITTLECHAR;//默认为0表示打印的是汉字,小于80的表示打印的是字符
			s_PrintParameters.m_Charlist = 1;//默认打印字符集1

			ProTraehdat(0);
		//第一次进入判断按键值是否大于等于9,是表示进入设置状态
			if(m_ProKeyValue > 9)
			{
				g_ProFuncValue = 0x0100 ;//第一项默认是波特率
				#ifdef RD_TYPE_PARALLEL
				g_ProFuncValue = 0x0400 ;//第一项默认是波特率
				#endif

				i = (g_ProFuncValue >> 8) & 0xFF;//得到项目值

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

			i = (g_ProFuncValue >> 8) & 0xFF;//得到项目值
			m_itemMax = ItemIndexCount[i];
			j =  g_ProFuncValue & 0xFF;
			if(j >= m_itemMax)
			{
				g_ProFuncValue &= 0xFF00;
			}
			g_cruve_arry[i] = g_ProFuncValue & 0xFF;
		}
		if(m_ProKeyValue == 2)
		{
			g_ProFuncValue += 0x0100;//项目累加
			i = (g_ProFuncValue >> 8) & 0xFF;//得到项目值
			if(i> ItemIndexCount[0])
			{
				g_ProFuncValue = 0x0100;//大于最大项会到第一项
				#ifdef RD_TYPE_PARALLEL
				g_ProFuncValue = 0x0400 ;//第一项默认是波特率
				#endif
			}
			g_ProFuncValue &= 0xFF00;
			i = (g_ProFuncValue >> 8) & 0xFF;//得到项目值
			g_ProFuncValue |= g_cruve_arry[i];

		}
		if(m_ProKeyValue > 5)
		{
			//回复默认值
			PrinterConfigInt();
			SavePrinterConfig();
			DefaultSet();
			g_ProFuncValue = 0;
		}
		if(m_ProKeyValue < 3)
		{
			i = (g_ProFuncValue >> 8) & 0xFF;//得到项目值
			j = g_ProFuncValue & 0xFF;
			ProTraehdat(1);
			SavePrinterConfig();
			(*StandardSetPro[i])(j);


		}
		m_ProKeyValue = 0;
	}



}


void  Delay1ms(unsigned short mdelay)
{
	__SETPRIMASK();
	gDelay1ms = 0;
	__RESETPRIMASK();
	while(gDelay1ms <mdelay )
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器

	}


}




/****************************************************************************
* 函数名：PrinterConfigInt()
* 功  能：打印机参数初始化
*
* 说  明：
****************************************************************************/
void PrinterConfigInt(void)
{
		s_PrintrConfig.m_InterFace = 1;//连续纸
		#ifndef PRINTER_LABEL
		 s_PrintrConfig.m_InterFace = 1;//软件固定接口方式; 
		#endif
		#ifdef PRINTER_LABEL
			s_PrintrConfig.m_InterFace = 0;//软件固定接口方式;
			#ifdef PAPER_LABEL
			 s_PrintrConfig.m_InterFace = 0;//软件固定接口方式; 
			#endif
			#ifdef PAPER_CONTINUE
			 s_PrintrConfig.m_InterFace = 1;//软件固定接口方式; 
			#endif	
		#endif	
	
//	s_PrintrConfig.m_InterFace = 0;//软件固定接口方式
	s_PrintrConfig.m_CharSpace = 0;//字间距默认为0

	s_PrintrConfig.m_LineSpace = 3;//默认行间距0

#ifdef PRINTER_GB2312
	s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//默认国标一二级
#endif
#ifdef PRINTER_GBK
	s_PrintrConfig.m_ChineseLib = TYPE_GBK;//默认GBK
#endif

#ifdef PRINTER_CHINESE_24
	s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//默认24点阵机型
	s_PrintrConfig.m_DefaultChar = TYPE_ASC12X24;//默认打印字符
	s_PrintrConfig.m_DefaultChinese = TYPE_CHIN24X24;//默认打印汉字
#endif
#ifdef PRINTER_CHINESE_16
	s_PrintrConfig.m_ChineseFont = PRINTTYPE_16;//默认24点阵机型
	s_PrintrConfig.m_DefaultChar = TYPE_ASC8X16;//默认打印字符
	s_PrintrConfig.m_DefaultChinese = TYPE_CHIN16X16;//默认打印汉字
#endif



	s_PrintrConfig.m_PrintDir = PRINT_DIRECT_FX;//打印机打印方向 台式 面板

#ifdef PRINTER_DIRECT_NOR

	s_PrintrConfig.m_PrintDir = PRINT_DIRECT_NOR;//打印机打印方向 台式 面板

#endif


	s_PrintrConfig.m_HoriZoomOut = 1;//默认横向放大倍数
	s_PrintrConfig.m_VertZoomOut = 1;//默认纵向放大倍数

	g_0D0AHigh = 24;//
	g_HTCharSpace = 12;//根据打印的字符确定空格宽度

	if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_16)
	{
		s_PrintrConfig.m_HoriZoomOut = 2;//默认横向放大倍数
		s_PrintrConfig.m_VertZoomOut = 2;//默认纵向放大倍数
		g_0D0AHigh = 16;//
 	}



	#ifdef	PRINT_TYPE_48
		s_PrintrConfig.m_HoriZoomOut = 1;//默认横向放大倍数
		s_PrintrConfig.m_VertZoomOut = 2;//默认纵向放大倍数

	#endif


	s_PrintrConfig.m_Liftpos = 0;//默认打印机左限
	s_PrintrConfig.m_Righttpos = PRINT_HEAD_LENGTH;//默认打印机右限
	s_PrintrConfig.m_BaudRate = 0;//串口机型波特率

	s_PrintrConfig.m_BaudRate = 6;//串口机型波特率
	#ifdef IF_TYPE_BL
	s_PrintrConfig.m_BaudRate = 10;//串口机型波特率115200
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



	//Odd 奇效验
	#ifdef PRINT_SERIAL_ODD
		s_PrintrConfig.m_BaudRate |= 0x20;
	#endif
	//Even 偶效验
	#ifdef PRINT_SERIAL_EVEN
		s_PrintrConfig.m_BaudRate |= 0x10;
	#endif
//7位偶效验
	#ifdef PRINT_SERIAL_7_EVEN
		s_PrintrConfig.m_BaudRate |= 0x30;
	#endif




	s_PrintrConfig.m_SoftwarePart = 4;//软件最大分区
	#ifdef PRINTER_LOW_16
	s_PrintrConfig.m_SoftwarePart = 16;//软件最大分区
	#endif
	#ifdef PRINTER_LOW_12
	s_PrintrConfig.m_SoftwarePart = 12;//软件最大分区
	#endif
	#ifdef PRINTER_LOW_10
	s_PrintrConfig.m_SoftwarePart = 10;//软件最大分区
	#endif
	#ifdef PRINTER_LOW_8
	s_PrintrConfig.m_SoftwarePart = 8;//软件最大分区
	#endif
	#ifdef PRINTER_LOW_6
	s_PrintrConfig.m_SoftwarePart = 6;//软件最大分区
	#endif
	#ifdef PRINTER_LOW_4
	s_PrintrConfig.m_SoftwarePart = 4;//软件最大分区
	#endif
	#ifdef PRINTER_LOW_2
	s_PrintrConfig.m_SoftwarePart = 2;//软件最大分区
	#endif



	s_PrintrConfig.m_HotDeep = 0;//额外附加加热深度
	s_PrintrConfig.m_PrintSub = 3;//3;//打印机细分度

	s_PrintrConfig.m_PrintLabFlage = LABEL_PR_DIS;//设置为0表示无标签打印
	s_PrintrConfig.m_LabelWide = 50;//表示打印的纸宽,要根据普通纸58mm纸宽打印宽度48mm 58-50 = 8mm * 8 空64点
	s_PrintrConfig.m_LabelLength = 200;//30mm * 32 1mm=32步
	s_PrintrConfig.m_BarPaGap = 2;////3mm * 32 1mm=32步
	//s_PrintrConfig.m_PE_LabValue_L 纸缝判断值 根据自检测试自动计算
	//s_PrintrConfig.m_PE_LabValue_H
	#ifdef PRINTER_LABEL
	s_PrintrConfig.m_PrintLabFlage = LABEL_PR_EN;//设置为标签打印
	#endif
	
	s_PrintrConfig.m_HotLineTOPD = 6 ; //加热线到传感器的位置
	s_PrintrConfig.m_HotLineTOOUT = 9;//6;//7 ;//加热线到出纸口的位置	
	
	#ifdef RD_TH
	s_PrintrConfig.m_HotLineTOOUT = 20;//加热线到出纸口的位置
	#endif


	#ifdef RD_D20
	s_PrintrConfig.m_HotLineTOPD = 13 ; //加热线到传感器的位置
	s_PrintrConfig.m_HotLineTOOUT = 9;//6;//7 ;//加热线到出纸口的位置	
	#endif


	#ifdef RD_TYPE_D30
	s_PrintrConfig.m_HotLineTOPD = 7 ; //加热线到传感器的位置
	s_PrintrConfig.m_HotLineTOOUT = 7;//6;//7 ;//加热线到出纸口的位置	
	#endif

	#ifdef PRINTER_BUFF_RAM
	s_PrintrConfig.m_ReceiveBuffrWor = RECEIVE_RAM;//RECEIVE_FLASH;
	#endif

	#ifdef PRINTER_BUFF_FLASH
	s_PrintrConfig.m_ReceiveBuffrWor = RECEIVE_FLASH;//RECEIVE_FLASH;
	#endif
	s_PrintrConfig.m_CharType = TYPE_CHINESE;//默认中文机型
	#ifdef PRINTER_ENGLISH
	s_PrintrConfig.m_CharType = TYPE_ENGLISH;//默认英文机型
	#endif
	s_PrintrConfig.m_PrintEndFeed = 0;//默认不走纸
	s_PrintrConfig.m_PrintEnCode = 0;//默认不加密
	
	
	s_PrintrConfig.m_Delayms_End = 2;
	s_PrintrConfig.m_Delayms_Head = 10;	

}
/****************************************************************************
* 函数名：SavePrinterConfig()
* 功  能：打印机参数初始化
*
* 说  明：
****************************************************************************/
#if 0
void SavePrinterConfig(void)
{
	short i;
	Buffer_writer_I = &(s_ListParmeter_B.m_ListBuffer[700]);//设置指针位置
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
	AT91F_DataFlashPageWrite_NE(DL_PRINTER_PAR_ADDR,Buffer_writer_I);//写入数据


}
#endif
void SavePrinterConfig(void)
{
	short i,j;
	unsigned char mbuf[128];
	unsigned char *p_con;
	
	
	p_con = (unsigned char*)&s_PrintrConfig;
	j = 0;
	
	mbuf[j++] = CompileTime[0];
	mbuf[j++] = CompileTime[1];
	mbuf[j++] = CompileTime[3];
	mbuf[j++] = CompileTime[4];
//	MyPrintf("-- %d\r\n",sizeof(PrintrConfig));
	for(i=0;i<sizeof(PrintrConfig);i++)
	{
		mbuf[j++] = p_con[i];
//		MyPrintf("%02x ",p_con[i]);
	}
//MyPrintf("\r\n");
	PRINTER_PDL.SYS._SavePrinterConfig(mbuf,j);


}

void ReadPrinterConfig(void)
{
	short i,j;
	unsigned char mbuf[128];
	unsigned char *p_con;
	
	
	PRINTER_PDL.SYS._ReadPrinterConfig(mbuf,sizeof(PrintrConfig)+4);
	
	
	p_con = (unsigned char*)&s_PrintrConfig;
	j = 4;
//	MyPrintf("-- %d\r\n",sizeof(PrintrConfig));
	for(i=0;i<sizeof(PrintrConfig);i++)
	{
		p_con[i] = mbuf[j++];
//		MyPrintf("%02x ",p_con[i]);
	}
//	MyPrintf("\r\n");



}


/****************************************************************************
* 函数名：ConfigToPrinter()
* 功  能：配置打印机
*
* 说  明：
****************************************************************************/
void ConfigToPrinter(void)
{

	//s_PrintrConfig.m_InterFace = 0;//软件固定接口方式

	s_PrintParameters.m_CharSpace =	s_PrintrConfig.m_CharSpace;//字间距默认为0
	//s_PrintrConfig.m_ChineseLib = TYPE_GB2312;//默认国标一二级

	//s_PrintrConfig.m_ChineseFont = PRINTTYPE_24;//默认24点阵机型

	s_PrintParameters.m_CharRepertoire = s_PrintrConfig.m_DefaultChar;// = TYPE_ASC12X24;//默认打印字符
	s_PrintParameters.m_ChineseCharType = s_PrintrConfig.m_DefaultChinese;// = TYPE_CHIN24X24;//默认打印汉字


	s_PrintParameters.m_PrintDir = s_PrintrConfig.m_PrintDir;// = PRINT_DIRECT_NOR;//打印机打印方向 台式 面板

	s_PrintParameters.m_HoriZoomOut = s_PrintrConfig.m_HoriZoomOut;// = 1;//默认横向放大倍数
	s_PrintParameters.m_VertZoomOut = s_PrintrConfig.m_VertZoomOut;// = 1;//默认纵向放大倍数


	s_PrintParameters.m_Liftpos = s_PrintrConfig.m_Liftpos;//  = 0;//默认打印机左限
	s_PrintParameters.m_Righttpos = s_PrintrConfig.m_Righttpos;// = 384;//默认打印机右限

	//s_PrintrConfig.m_BaudRate = 0;//串口机型波特率



	g_ParTypeValue = PRINT_HEAD_LENGTH/s_PrintrConfig.m_SoftwarePart;//s_PrintrConfig.m_SoftwarePart = 4;//软件最大分区


	g_THDeep = s_PrintrConfig.m_HotDeep;// = 0;//额外附加加热深度

	g_PrinterDip = s_PrintrConfig.m_PrintSub;// = 3;//打印机细分度



	s_PrintParameters.m_LineSpace = s_PrintrConfig.m_LineSpace;

	s_PrintParameters.m_CharType = s_PrintrConfig.m_CharType;

}

/****************************************************************************
* 函数名：PrinterParameterInt()
* 功  能：打印机参数初始化
*
* 说  明：
****************************************************************************/
 void  PrinterParameterInt(void)
 {
	int i;
	s_PrintParameters.m_AlignUPDown = 0x8000;//=1表示下对齐
	g_Char24_17 = 0;
	print_head_length = PRINT_HEAD_LENGTH;
	s_PrintParameters.m_CharSpace = 0;//字间距默认为0

	s_PrintParameters.m_CharType = TYPE_CHINESE;//默认为0表示打印的是汉字,小于80的表示打印的是字符
	s_PrintParameters.m_ChineseCharType = TYPE_CHIN24X24;
	s_PrintParameters.m_CharRepertoire = TYPE_ASC12X24;
	s_PrintParameters.m_Charlist = 1;//如果打印ANK字符,默认打印字符集1

	s_PrintParameters.m_HoriZoomOut = 1;//横向放大倍数为1
	s_PrintParameters.m_VertZoomOut = 1;//纵向打印放大为1

	s_PrintParameters.m_boldface = VALUE_DISABLE;//取消加粗模式

	s_PrintParameters.m_Inverse = 0;//反白为0不反白

	s_PrintParameters.m_Liftpos = 0;//左限的位置默认为起始位置0
	s_PrintParameters.m_Righttpos = print_head_length;//打印机右限

	s_PrintParameters.m_LineSpace = 3;//行间距默认为3

	s_PrintParameters.m_PageLength = 255;//默认为255
	s_PrintParameters.m_Pagecount = 0;//打印页长计数器为0

	s_PrintParameters.m_positions = print_head_length;//打印机能打印的位置为打印头能够打印的点数


	s_PrintParameters.m_PrintDir = PRINT_DIRECT_NOR;//正常台式打印机打印方向

	s_PrintParameters.m_Prstate = 0;//打印机当前状态

	s_PrintParameters.m_RotDegree = 0;//旋转角度默认为0度
	s_PrintParameters.m_alignType = 0;//左对齐
	for (i=0;i<20;i++)
	{
		s_PrintParameters.m_TabPos[i] = 0;//清空水平造表位置最小值为1
	}
	s_PrintParameters.m_TabCount = 0;//水平造表计数器为0

	s_CodeBarCon.m_BarHigh = 50;//条码高度默认50
	s_CodeBarCon.m_Barwide = 2;//宽度默认2
	s_CodeBarCon.m_CharSpace = 2;//条码字符在上面
	s_CodeBarCon.m_BarSpace = 0;
	g_DriverAFlag = 0;

	

	
	
	
	g_THDeep = 0;//默认增加加热深度0
	g_ThType = 0;//加热类型
	g_SpeedChoice = 0;//取消设定速度

	g_CodePage = 255;//0;
	g_InternationChar = 0;

	s_PrintParameters.m_QRZoom = 0x04;

	//得到配置表
	ConfigToPrinter();
	g_0D0AHigh = 24;//
	g_HTCharSpace = 12;//根据打印的字符确定空格宽度

	if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_16)
	{
		g_0D0AHigh = 16;//
	}

	g_unicodeType = 0;//asc编码
 }
extern unsigned short g_PowerVol;
/****************************************************************************
* 函数名：main()
* 功  能：主程序
*
* 说  明：
RD_USART1_EN
****************************************************************************/

int Handler_main()
{
	unsigned char mbuf[8];
	unsigned int i;
	 RCC_ClocksTypeDef clocks;
	g_PowerFlag = 0;
	#ifdef LED_OFF
	PRINTER_PDL.SYS._LED_Stauts_OFF();
	#endif
	#ifndef LED_OFF
	PRINTER_PDL.SYS._LED_Stauts_ON();
	#endif

	PRINTER_PDL.SYS._LED_Power_ON();
		PRINTER_PDL.SYS._WDG_ReloadCounter();
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
	while( PRINTER_PDL.SYS._GetKey_POW() == 0)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器		
	}
		PRINTER_PDL.SYS._WDG_ReloadCounter();
	g_PowerOnFlag = 0xA546;


	g_PowerFlag = 0xA5E3;
//***********系统初始化**********************
	g_ConfigRCC = 8;
	g_CutterErr = 0;
g_LabPrintEndF = 0;
	g_BarCodeDegree = 0;
	g_BarCharHigh = 24;
#ifdef RD_TYPE_SERIAL


	MX_Resume();
	//************************************************
	//p_PrCon = &s_PrintrConfig.m_InterFace;
	//得到整个配置结构的所有数据
	//AT91F_DataFlashADDRRead(DL_PRINTER_PAR_ADDR+4,CONFIG_PAR_LENGTH,p_PrCon);//得到数据

	//g_ConfigRCC = s_PrintrConfig.m_BaudRate & 0xF;
#endif
//MyPrintf("Star1..\r\n");

	for(i=0;i<0xfff;i++);

//	#ifdef RD_TYPE_SERIAL
//	s_PrintParameters.m_InterFace = INTERFACE_SERIAL;
//	#endif

//	#ifdef RD_TYPE_PARALLEL
//	s_PrintParameters.m_InterFace = INTERFACE_PARALLEL;
//	#endif

	//初始化SPI

	MX_Resume();
	//************************************************
		PRINTER_PDL.SYS._ReadPrinterConfig(mbuf,4);
//	Buffer_writer_I = &(s_ListParmeter_B.m_ListBuffer[700]);//设置指针位置

//	AT91F_DataFlashADDRRead(DL_PRINTER_PAR_ADDR,2,Buffer_Dataflash);//得到数据

//	g_ProFuncValue = CompileTime[0] << 8;
//	g_ProFuncValue |= CompileTime[1];
//	g_CurveCountFlag = CompileTime[3] << 8;
//	g_CurveCountFlag |= CompileTime[4];
//	MyPrintf("Star1111..\r\n");
//	if((Buffer_Dataflash[0] != g_ProFuncValue) || (Buffer_Dataflash[1] != g_CurveCountFlag))
//	{
//		MyPrintf("Star666666..\r\n");
//		//无参数配置需要初始化配置
//		PrinterConfigInt();
//		SavePrinterConfig();
//	}
//	p_PrCon = &s_PrintrConfig.m_InterFace;
//	//得到整个配置结构的所有数据
//	AT91F_DataFlashADDRRead(DL_PRINTER_PAR_ADDR+4,CONFIG_PAR_LENGTH,p_PrCon);//得到数据
	if((CompileTime[0] != mbuf[0])||(CompileTime[1] != mbuf[1])||(CompileTime[3] != mbuf[2])||(CompileTime[4] != mbuf[3])    )
	{
			PRINTER_PDL.SYS._WDG_ReloadCounter();
		//无参数配置需要初始化配置
//		MyPrintf("Star1\r\n");
		PrinterConfigInt();
		PRINTER_PDL.SYS._WDG_ReloadCounter();
		SavePrinterConfig();
	}
		PRINTER_PDL.SYS._WDG_ReloadCounter();

	ReadPrinterConfig();

	//*****************系统配置*******************************

	g_PaperType = s_PrintrConfig.m_InterFace;
	g_ProFuncValue = 0;
	g_CurveCountFlag = 0;

	g_DetPaperNoprint = 0;
	g_NotStbFlag = 0;
	g_DetPaperGapSpace = 0;

	g_ProFuncValue = 0;
	g_CurveCountFlag = 0;
	g_ProFlagVelue = 0xaa00;



	g_receiveFlage = 0;


	#ifdef RD_TYPE_SERIAL

	FlowFlag = s_PrintrConfig.m_BaudRate;

	#ifdef  SWITCH_BAUD
		ReadSwStatus();
	#endif
	FlowFlag = 0x0A;
	//初始化串口
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

	RCC_GetClocksFreq(&clocks);
	printf("\n");
	printf("USB Printer\n");
	printf("SYSCLK: %3.1fMhz, HCLK: %3.1fMhz, PCLK1: %3.1fMhz, PCLK2: %3.1fMhz, ADCCLK: %3.1fMhz\n", \
	(float)clocks.SYSCLK_Frequency/1000000, (float)clocks.HCLK_Frequency/1000000, \
	(float)clocks.PCLK1_Frequency/1000000, (float)clocks.PCLK2_Frequency / 1000000, (float)clocks.ADCCLK_Frequency / 1000000);
	#ifdef PRINTER_AND_USB
	//USB 功能模块
	PRINTER_PDL.USB._Init_Fun();	
	#endif

//***************************************

 g_printChar = 0;

//***************************************


gTimer_Print = 60000;

	PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器



	PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器

	WRDI();

 	g_PrinterDip = 3;//默认4步一个点
 //	g_PrinterDip = 1;//默认2步一个点


 	g_cruvefalg = 0xAAAA;
 	for(i=0;i<20;i++)
 	{
 		g_cruve_arry[i] = 0;
	}

	Erase_Sector = 0x300000; // 设置图形下载扇区比对初始值

	g_DetPaperValueFlag = 0xA5EF;
	g_DetPV_Count = 0;
	g_DetPV_H = 0;

//******************************************
	g_Receive_count = 0;//清接收缓存计数器
	Receive_count = 0;//清接收计数器
	g_Order_count = 0;//清接取数寄存器

	g_SaveDataToFlashAddr = ADDRPRO_OR;
	g_GetDataFromFlashAddr = ADDRPRO_OR;
	g_SaveDataOffset = 0;
	g_GetDataOffset = 0;
	g_InterFaceBufferCount = 0;
	Order_Erase_Sector = 0x300000;// 设置打印命令存储扇区比对初始值

	g_DumpReceiveFlage = 0;

//	g_DumpReceiveFlage = 0xAAAA;

	g_BufferSwitch = 0;//初始化打印缓冲区切换标志
	g_0D0AFlag =0;//清0d 0a标志
	g_dotSwitch = 0;//预处理
	s_PrintOrderstr.m_DataFlag = 0;//清空手动打印标志
	s_PrintOrderstr.m_GetdataPos = 0;//清空手动打印计数器
	//=====打印参数初始化
		g_TMIndex = PRINTER_PDL.SYS._GetCurTemperatureLevel();		

		g_CurrentVol = PRINTER_PDL.SYS._GetCurVoltageLevel();	
	PrinterParameterInt();
	//缓冲区的配置只在上电配置一次
	s_PrintParameters.m_ReceiveBuffrWor =  s_PrintrConfig.m_ReceiveBuffrWor;//设置打印缓冲区标志

	s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//打印机停机
	s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;//取消切换标志
	s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;//停止打印禁止

	g_PaperRunToOut = 8000;

	g_PE_Standard = 300;//设置无纸时AD采样最低值

	
	//设置打印机空闲
	PRINTER_PDL.SYS._PeripheralVcc_EN();
	PRINTER_PDL.SERVICE._PrinterIdle();
	#ifdef LED_OFF
	PRINTER_PDL.SYS._LED_Stauts_OFF();
	#endif
	#ifndef LED_OFF
	PRINTER_PDL.SYS._LED_Stauts_ON();
	#endif

	PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
	//======设置打印机缓冲区=================================
//	s_PrintParameters.m_ReceiveBuffrWor = RECEIVE_RAM;//RECEIVE_FLASH;

//	if(s_PrintParameters.m_ReceiveBuffrWor == RECEIVE_FLASH)
//	{
//		PrinterCore = PrinterCore_Flash;//得到取命令函数指针
//		p_OcAndGDAdr = &g_GetDataFromFlashAddr ;//g_Order_count
//		p_REAndSAcount = &g_InterFaceBufferCount; //Receive_count
//		p_SaveDrREcount = &g_SaveDataOffset;//保存数据的偏移量 g_Receive_count
//	}
//	else
	{
		PrinterCore = PrinterCore_RAM;
		p_OcAndGDAdr = &g_Order_count ;//数据缓冲区buffer 位置偏移量 g_GetDataFromFlashAddr
		p_REAndSAcount = &Receive_count; //接收数据寄存器g_InterFaceBufferCount
		p_SaveDrREcount = &g_Receive_count;//保存数据的偏移量 g_SaveDataOffset
	}

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

		s_PrintParameters.m_CurMotoRunTime = VolRUNTime[g_CurrentVol] - 100;//得到当前电压电机运行时间
		s_PrintParameters.m_PrHeadHottime = VolTHSTB[g_CurrentVol] - 100;//得到加热值
		if(s_PrintrConfig.m_SoftwarePart == 2)
		{
			s_PrintParameters.m_PrHeadHottime = VolTHSTB_2[g_CurrentVol];//得到加热值
		}				
		s_PrintParameters.m_CurMotoPWM = VolRUNPWM[g_CurrentVol] - 100;//得到当前电压电机PWM值
		s_PrintParameters.m_CurFreeTime = VolFreeTime[g_CurrentVol];//得到当前电压空走纸步进时间	
		
		
//			MyPrintf("Vol:%d\r\n",g_CurrentVol);	
//MyPrintf("ss---%d\r\n",g_TMIndex);

	//======表格打印预处理=================================
	for(i=0;i<BUFFER_SHORT_LENGTH;i++)
	{
		g_ListTable[i] = 0;
	}
	for(i=0;i<1000;i++);
	g_ListSaveToFlashCount = 0;
	g_ListPrintFinish = 0;
	g_ListControlFor = 0;
	g_PrintListOrNorm = 0;//不打印表格
	//=======================================
	g_SelfRunFlag = 0;//自检运行标志清零
	g_PE_DN_Count = 0;//计数器清零
	g_PE_UP_Count = 0;//计数器清零

	//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
	
	g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();
	
	
	g_PEDataS = GetDETPaper();// (AD_Value[1] >> 4);
	if(g_PEDataS > s_PrintrConfig.m_PE_LabValue_H)
	{
		g_PE_Staue = 1;
	}
	else
	{
		g_PE_Staue = 0;
	}

    Trg = 0;
    Cont = 0;


	
	#ifdef RD_TYPE_PARALLEL
	//初始化并口
	//置高忙信号,以防外部接口连接到客户设备后,外部设备发送数据误触发IRQ信号
	PRINTER_PDL.PARALLEL._SetBusy();
	PRINTER_PDL.PARALLEL._ClearOE();
	PRINTER_PDL.PARALLEL._ClearACK();
	PRINTER_PDL.PARALLEL._ClearOE();
	PRINTER_PDL.PARALLEL._ClearACK();
	PRINTER_PDL.PARALLEL._ClearOE();
	PRINTER_PDL.PARALLEL._ClearACK();
	PRINTER_PDL.PARALLEL._ClearOE();
	PRINTER_PDL.PARALLEL._ClearACK();
	
	
	PRINTER_PDL.PARALLEL._SetOE();
	PRINTER_PDL.PARALLEL._SetACK();
	PRINTER_PDL.PARALLEL._Init_Fun();
	PRINTER_PDL.PARALLEL._ClearPE();//设置当前打印机有纸
	PRINTER_PDL.PARALLEL._ClearBusy();
	#endif	
	
	


#ifdef	WIN10_DRIVE
	g_DriverAFlag = 0xaaaa; 

#endif

    g_PaperRunToOut = 8000;//s_PrintrConfig.m_LabelLength + (s_PrintrConfig.m_BarPaGap /2 );
	//=======================================

	
//	while(1)
//	{
//		PRINTER_PDL.SYS._WDG_ReloadCounter();
		
//		while(PRINTER_PDL.SYS._GetKey_LF() == 0)
//		{
//			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
//		

//			{//普通打印走纸模式
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

						
	if(PRINTER_PDL.SYS._GetKey_LF() == 0)
	{
		Delay1ms(100);
		if(PRINTER_PDL.SYS._GetKey_LF() == 0)
		{
			
			
			#ifdef PRINTER_AND_USB
			PRINTER_PDL.USB._ReceiveNBytes = PUSB_Dump_ReceiveNBytes;
			#endif
			
			#ifdef RD_TYPE_PARALLEL
			PRINTER_PDL.PARALLEL._EXTInterruptIRQHandler = PARALLEL_DUMP_ReceiveNBytes;
			#endif			
			
			//打印自检使用FLASH存储,dunmp模式存储数据量比较大
//				MyPrintf("Star.....Selftest.....\r\n");
			//g_PaperType = 1;//自检处理为连续纸
		//	SysTick_Init();
			//g_PaperType = 1;//自检处理为连续纸

			g_SelfRunFlag = 0xAA;
				g_DumpReceiveFlage = 0xAAAA;
			SetSelftestData();

			g_DetPV_H = 0;
			g_DetPV_L = 0xFFFF;			
		}
	}

	
	

	
	
	//=======================================
	while (1)
	{
ProcessStart:
		__SETPRIMASK();
		g_PrintStatusReg = PRASE_PROCESS;
		__RESETPRIMASK();
		if(g_BufferSwitch != 0)
			p_ListPrease = &s_ListParmeter_B;
		else
			p_ListPrease = &s_ListParmeter_A;
		HTCount = 0;
		p_ListPrease->m_PrintLineSpace = s_PrintParameters.m_LineSpace;
		p_ListPrease->m_HotTime = s_PrintParameters.m_PrHeadHottime;//得到打印头的加热时间
		p_ListPrease->m_PrintDir = s_PrintParameters.m_PrintDir;//得到打印的方向
		p_ListPrease->m_PrintLineHigh = 8;//默认一行的高度为8;
		
		
		p_ListPrease->m_PrinterDip	= g_PrinterDip;
		p_ListPrease->m_LableStopFlag = 0;
		p_ListPrease->m_ThType = g_ThType;
		p_ListPrease->m_MotoRunTime = g_SpeedChoice;//设置打印机运行速度
		p_ListDataDot = &(p_ListPrease->m_ListBuffer[0]);//得到数据链表的首地址
		p_IndexListData = &(p_ListPrease->m_ListIndex[0]);//得到链表索引的首地址
		if (p_ListPrease->m_PrintDir != PRINT_DIRECT_NOR)
		{
			*p_IndexListData++= LIST_END;//如果为面板方向链表起始位为整个链表的末尾
		}
		p_ListPrease->m_MotoRunDir = MOTO_CLOCKWISE;//设置打印机正转
		p_ListPrease->m_LableFeedFlag = 0;
		p_ListPrease->m_LeftFreeDot = s_PrintParameters.m_Liftpos;
		g_CalcPrintPos = s_PrintParameters.m_Liftpos;//打印数据的起始位置
		g_SavePrintPos = g_CalcPrintPos;
		m_SaverightPos = s_PrintParameters.m_Righttpos;//得到右限
		g_alignType = s_PrintParameters.m_alignType;//得到对齐方式
		for(i=0;i<BUFFER_SHORT_LENGTH;i++)
		{
			p_ListPrease->m_LineProArry[i] = g_ListTable[i];
		}
		__SETPRIMASK();
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
		__RESETPRIMASK();			
		PrinterLPS();			
ProcessLoop:
		if (p_ListPrease->m_PrintDir != PRINT_DIRECT_NOR)
		{
			if (s_PrintParameters.m_AlignUPDown == 0)
			{
				m_UPDown = 0x8000;
			}
			else
			{
				m_UPDown = 0;
			}
		}
		else
		{
			m_UPDown = s_PrintParameters.m_AlignUPDown;
		}
		//m_OrderSave = g_Order_count;
		m_OrderSave = (*p_OcAndGDAdr);
	//增加 unicode码的处理
	ProUnicodeChar();
		m_GetData = PrinterCore();//得到打印命令
#ifdef PRINTER_PROTOCOL_9A9A
		if((m_GetData == 0x9A)||(m_GetData == 0x9D))
		{
			m_GetData = 0;
		}
#endif
		if(g_DumpReceiveFlage == 0xAAAA)
		{
			if(m_GetData == 0x20)
			{
				if(g_SavePrintPos == g_CalcPrintPos)
				{
					m_GetData = 0;
				}
			}
		}
Process_In_Loop:
		m_ProcessFlag = PROCESS_CONTINUE;//默认为继续取命令
		if(m_GetData > 0x1F)
		{
			m_ProcessStatus = __PrcessCharCh();
		}
		else
		{
			switch(m_GetData)
			{
				//case 0x03:{ m_ProcessStatus = __03_Order(); }break; //协议
				case 0x09:{ m_ProcessStatus = __HT_Order(); }break; //水平造表
				case 0x0A:{ m_ProcessStatus = __OAOD_Order(); }break; //回车换行
				case 0x0D:{ m_ProcessStatus = __OAOD_Order(); }break; //回车换行
				case 0x0E:{ m_ProcessStatus = __SO_Order(); }break; //双倍宽
				case 0x0C:{ m_ProcessStatus = __FF_Order(); }break; //黑标
				case 0x10:{ m_ProcessStatus = __DC0_Order(); }break; //
				case 0x12:{ m_ProcessStatus = __DC2_Order(); }break; //
				case 0x14:{ m_ProcessStatus = __DC4_Order(); }break; //恢复倍宽
				case 0x18:{ m_ProcessStatus = __18_Order(); }break; //删除一行
				case 0x1B:{ m_ProcessStatus = __ESC_Order(); }break; //解析ESC指令
				case 0x1C:{ m_ProcessStatus = __FS_Order(); }break; //解析FS指令
				case 0x1D:{ m_ProcessStatus = __GS_Order(); }break; //解析GS指令
				case 0x1F:{ m_ProcessStatus = __US_Order(); }break; //解析US指令
				default:
				m_ProcessStatus = __OrderReceive();
			}
		}
		if(m_ProcessStatus == PROCESS_START)
		{
			goto ProcessStart;
		}
		else if(m_ProcessStatus == PROCESS_IN_LOOP)
		{
			goto Process_In_Loop;
		}
		else if(m_ProcessStatus == PROCESS_LOOP)
		{
			goto ProcessLoop;
		}
		else if (m_ProcessStatus == PROCESS_CONTINUE)
		{
			goto ProcessLoop;
		}
		else if (m_ProcessStatus == PROCESS_FINISH)
		{
			//=========计算需要空出的空点=================================



			if((g_THDeep & 0x8000) == 0)
			{
				p_ListPrease->m_HotTime = s_PrintParameters.m_PrHeadHottime + g_THDeep;//得到打印头的加热时间
			}
			else
			{
				p_ListPrease->m_HotTime = s_PrintParameters.m_PrHeadHottime - (g_THDeep & 0x7FFF);//得到打印头的加热时间
			}

		if(g_PaperType == 0)
		{
			
			p_ListPrease->m_HotTime = p_ListPrease->m_HotTime * 2;
			
		}			
			
			
			p_ListPrease->m_RightFreeDot = print_head_length - g_CalcPrintPos;

			if (g_alignType  == 1)//居中
			{
				i = p_ListPrease->m_RightFreeDot + p_ListPrease->m_LeftFreeDot;
				p_ListPrease->m_RightFreeDot = i >> 1;
				p_ListPrease->m_LeftFreeDot = i - p_ListPrease->m_RightFreeDot;
			}
			if (g_alignType  == 2)//居中
			{
				p_ListPrease->m_LeftFreeDot = p_ListPrease->m_RightFreeDot + p_ListPrease->m_LeftFreeDot;
				p_ListPrease->m_RightFreeDot = 0;
			}

			if (p_ListPrease->m_PrintDir == PRINT_DIRECT_NOR)
			{
				p_ListPrease->m_PrintStartPos = 0;//正常台序方向从0开始
				*p_IndexListData++= LIST_END;//给结尾标志
			}
			else
			{
				p_ListPrease->m_PrintStartPos = p_IndexListData - p_ListPrease->m_ListIndex - 1;//得到起始位置,为p_IndexListData++ 所以要减一
			}
			p_ListPrease->m_FeedTime = 0;
			#ifdef TM_ENABLE
			
			__SETPRIMASK();
			g_TMIndex = PRINTER_PDL.SYS._GetCurTemperatureLevel();	
			__RESETPRIMASK();		
			
			
			
			s_PrintParameters.m_VolTemAdd = 130;//占9%
			s_PrintParameters.m_VolTemSubb = 90;//占6%
			
			
			s_PrintParameters.m_VolTemAdd = (s_PrintParameters.m_PrHeadHottime * 9)/100;
			s_PrintParameters.m_VolTemSubb = (s_PrintParameters.m_PrHeadHottime * 6)/100;
			
			
			
			p_ListPrease->m_TMIndex = g_TMIndex ;//PRINTER_PDL.SYS._GetCurTemperatureLevel();//得到当前的温度

			
			
		
			
			if(p_ListPrease->m_TMIndex > 9)
			{
				p_ListPrease->m_HotTime += ( p_ListPrease->m_TMIndex - 9) * s_PrintParameters.m_VolTemAdd*2;//加热时间
			}
			else
			{
				p_ListPrease->m_FeedTime = (9 - (p_ListPrease->m_TMIndex)  ) * s_PrintParameters.m_VolTemSubb;//加热时间
				p_ListPrease->m_HotTime -= p_ListPrease->m_FeedTime;
			}
			
			#endif
			
			if(g_PrintListOrNorm == 0xAA55)
			{
				p_ListPrease->m_PrintLineSpace = 0;
			}
			

			//=============================================================
			//---处理打印机数据
			//与字符混打时，如果在此处不清掉标志，容易出现
			//曲线预先处理了两行，而后打印别的字符，
			//但是标志还在，再打印曲线时会出现错误
			g_CurveCountFlag = 0;//清曲线打印标志，
			#ifdef PRINTER_AND_USB
			returnstatus(0);
			
			#endif
			__SETPRIMASK();
			g_PrintStatusReg = FINISH_PROCESS;
			__RESETPRIMASK();
			#ifdef RD_OEMPRINTER
				SET_H_POWER;//打开驱动电源
			#endif
			
				#ifdef RD_INDEP_CONTROL

					SET_H_POWER;//打开驱动电源

				#endif			
			if (s_PrinterMOTOPar.m_PrintEnd == DISABLE_VAR)//表示打印机正在工作
			{
				//等待当前行打印完成后可以继续打印
				while(s_PrinterMOTOPar.m_MutualFlag != ENABLE_VAR)//判断是否切换数据
				{
					SaveDataToFlash();
				}
				s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;//取消切换标志
			}
			else
			{
				//打印机停机
				//需要重新开始打印
				while(s_PrinterMOTOPar.m_StopPrint == DISABLE_VAR)//
				{
					SaveDataToFlash();
				}
				if(g_PrintStatusReg == FINISH_PROCESS )
				{
					s_PrinterMOTOPar.m_In_printing = ENABLE_VAR;
					s_PrinterMOTOPar.m_InFirstPrint = ENABLE_VAR;

					g_RunFlag = 0;
					//--------------打开步进电机中断
					//TIM_ENABLE(TIM_Run,10);//开始第一次的ARR//重新开启定时器1

					PRINTER_PDL.SYS._SystemPrintIRQ_EN(10);
					s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;//取消切换标志
					while(s_PrinterMOTOPar.m_MutualFlag != ENABLE_VAR)//判断是否切换数据
					{
						SaveDataToFlash();
					}
					s_PrinterMOTOPar.m_MutualFlag = DISABLE_VAR;//取消切换标志

				}
			}
			goto	ProcessStart;
		}
		else
		{
			goto ProcessLoop;
		}
	}//* End
}

//2分区
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
0,
0,
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
小于90 2分区
大于90 小于 180 4分区
大于 180 小于 270  6分区
大于270 8分区

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
		return 2;//2分区
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
unsigned short g_RunCuStep,g_runLstep;//当前运行的步数
const unsigned short * p_GetParSTB;//
/****************************************************************************
* 函数名：Timer0ControlMoto
* 功  能：控制电机走步，控制打印行数
*
* 说  明：切换打印缓冲区，处理打印链表头，控制步进电机走步
*			Moto_Step 电机走步计数器，在外边赋值


void KeyRead( void )
{
    unsigned char ReadData = PINB^0xff;// 1
    Trg = ReadData & (ReadData ^ Cont);// 2
    Cont = ReadData;// 3


****************************************************************************/
unsigned short g_SpeedEnstu;
unsigned short g_lindotnu;
unsigned short g_SpaceStatus;
unsigned short g_falg;
unsigned short g_FeedStepCount = 0;
extern unsigned short g_PowerVol;
#ifdef  HIGH_SPEED
void ProcessMotoLineData(void)
{
	  unsigned short temp,ADtemp;

	PRINTER_PDL.SYS._STB_ALL_CLEAR();
PRINTER_PDL.SYS._WDG_ReloadCounter();
	//ClearMoto();
//	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_Trig();
	if(g_runLstep)
	{

		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],5);
		}
		else
		{
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],5);
		}
		//TIM_ENABLE(TIMx,g_runLstep);
		PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_runLstep);
		g_runLstep = 0;
		return;
	}
	ADtemp =  GetDETPaper();//得到光电采样值
	if((g_RunFlag & 0xaa00) == 0xaa00)
	{
		if(g_RunFlag & 0xF)
		{
			SendParData(p_GetParSTB[g_RunCuStep++]);
			PRINTER_PDL.SYS._LAT_L();
			//SET_H_STB1;SET_H_STB2;
			PRINTER_PDL.SYS._STB_A_EN();
			PRINTER_PDL.SYS._STB_B_EN();
			PRINTER_PDL.SYS._LAT_H();
			g_RunLTime = 550;
			g_runLstep = 0;
			g_FeedStepCount = 0;
			g_RunLTime += g_lindotnu * 2;
			
			
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],5);

			//TIM_ENABLE(TIMx,g_RunLTime);
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime);
			//TIM_ENABLE(TIM6,g_RunLTime>>1);//控制加热深度
	
			g_RunFlag--;
			if(g_RunFlag < 0xAA01)
			{
				g_RunFlag = 0;
			}
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
			return ;
		}
		g_RunFlag = 0;
	}
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		if(g_SendFlag == DATA_IN_SEND)
		{
			//TIM_ENABLE(TIMx,40);//等待一行数据处理完成
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
			return;
		}
	}
// =============================================================================
	#ifdef PRINTER_LABEL
	
	if(g_SelfRunFlag == 0xAA)
	{
		if(ADtemp != 0)
		{
			if(ADtemp < g_DetPV_L)
			{
				g_DetPV_L = ADtemp;
			}
			if(ADtemp > g_DetPV_H)
			{
				g_DetPV_H = ADtemp;
			}
		}		
	}
	
	
//	if(s_PrintrConfig.m_InterFace == 0)
	{//0表示标签机
//		if(g_DetPaperValueFlag == 0xA5EF)
//		{
//			temp =  GetDETPaper();//得到光电采样值
//			g_DetPV_Count++;
//			if(g_DetPV_H < temp)
//			{
//				g_DetPV_H = temp;
//			}
//			if(g_DetPV_Count > 32*10)
//			{
//				s_PrintrConfig.m_PE_LabValue_H = g_DetPV_H - (g_DetPV_H /6);
//				g_DetPaperValueFlag = 0;
//				g_DetPV_Count = 0;
//				g_DetPV_H = 0;
//			}
//			
//		}	

// =============================================================================

		FitWave();
		//-------------------------------处理标签位置----------------------------------------------------------
		temp = g_PE_Staue^0xff;// 1
		Trg = temp & (temp ^ Cont);// 2
		Cont = temp;// 3
		//if((Trg&1) == 1)
		if(g_PE_Staue == 0)
		{
			//设置检测到纸缝标志
			g_SpaceStatus = 0xA500;
		}
		g_SpaceStatus |= g_PE_Staue;
		if((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
		{


			//标签打印
			//在每一步的开头计算
			if(g_PaperRunToOut)
			{
				g_PaperRunToOut--;
			}

			if(g_SpaceStatus == 0xA501)
			{
				//检测到标签的起始打印位置

				g_falg = 0x0303;

				g_SpaceStatus = 0;//清空监测状态
				g_DetPaperGapSpace = 0xaaa;//检测到标签头
				//监测到纸缝后往前所走的步数
				//只走到加热线处，出纸部分由超时控制
				#ifdef RD_TYPE_EBP
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + 6*8 - 8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
				#ifndef RD_TYPE_EBP
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + 6*8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
				#ifdef RD_D20
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + s_PrintrConfig.m_HotLineTOOUT*8 -8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
				#ifdef RD_TYPE_D30
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + s_PrintrConfig.m_HotLineTOOUT*8 -8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
				
				
			}


			if(g_PaperRunToOut == 0)
			{
				//检测到纸缝并且走到出纸位置
				if(g_DetPaperGapSpace == 0xAAA)
				{
					if(p_ListParmeters->m_LableStopFlag == 0xA5A1)
					{
						//走到0点，设置长度为值的长度+纸缝的长度
						g_PaperRunToOut = 8000;//8000步 1000mm 设置默认走1米长度

						if(g_PrintStatusReg == FINISH_PROCESS)
						{
							goto first_Pro_printDataList;
						}
						else
						{
							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//打印机停机;
							p_ListParmeters->m_LableStopFlag = 0;
						}
					}
					else
					{
//						if(g_SelfRunFlag != 0xAA)
//						{
//							g_RunToGapFlag = 0xA55A;
//							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//打印机停机;
//							p_ListParmeters->m_LableStopFlag = 0;
//						}
					}
					
					
					
					//重新从一个表单开始打印记数
					g_DetPaperGapSpace = 0;


				}
			}
	
		}
		
	
	}	
	#endif
	
	
	
	
//	g_MotoSTBStatus = 0;
	if (s_PrinterMOTOPar.m_In_printing == ENABLE_VAR)//等于ENABLE_VAR 表示第一次进入打印程序需要预处理很多东西
	{

first_Pro_printDataList:
		//得到打印数据链表
		if(g_PrintStatusReg != FINISH_PROCESS)
		{
			goto Pro_printDataList_EX;
		}
		g_PrintStatusReg = 0;
		g_BufferSwitch = ~g_BufferSwitch;//取反打印链表切换标志
		if(g_BufferSwitch != 0)
		{
			p_ListParmeters = &s_ListParmeter_A;
		}
		else
		{
			p_ListParmeters = &s_ListParmeter_B;
		}
		g_SpeedEnstu = 0;
		s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//手动标志使能
		g_PrintLineCount = 0; //打印第一行的数据
		s_PrinterMOTOPar.m_PrintLSbit_f = DISABLE_VAR;//行间距标志清零
		s_PrinterMOTOPar.m_PrintLSbit = DISABLE_VAR;//无行间距 标志
		s_PrinterMOTOPar.m_MotoRun = ENABLE_VAR;//电机运行标志使能
		s_PrinterMOTOPar.m_StopPrint = DISABLE_VAR;//停止打印禁止
		s_PrinterMOTOPar.m_PrintEnd = DISABLE_VAR;//打印结束禁止
		s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//等于 DISABLE_VAR 表示打印机正在打印
		if(s_PrinterMOTOPar.m_InFirstPrint == ENABLE_VAR)
		{
			//第一次进入打印预处理
			g_FeedStepCount = 0;
			s_PrinterMOTOPar.m_InFirstPrint = DISABLE_VAR;//清除第一次打印标志
			s_PrinterMOTOPar.m_MotoStep = 0;//开始打印moto运行步数清空
		//	TIM_ENABLE(TIMx,20000);//20ms启动时序
		//	TIM_ENABLE(TIM3,20);//启动定时器3处理一行数据
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(20000);
			PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],s_PrintParameters.m_CurMotoPWM);
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
			return;
		}
		//TIM_ENABLE(TIM3,10);//启动定时器3处理一行数据
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
		goto timer_run;
	}
Pro_printDataList_EX:
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		g_QR_SpaceCount++;
		g_BarCode_SpaceCount++;
		//*********************************************
		//判断停止打印
		if(s_PrinterMOTOPar.m_PrintEnd == ENABLE_VAR)
		{
			s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//;;;;;没有可以打印的数据
			//清除加热线和电机驱动信号线
		//	SET_L_STB1;SET_L_STB2;//清加热线
				PRINTER_PDL.SYS._STB_ALL_CLEAR();
		//	ClearMoto();
				PRINTER_PDL.MOTO._MotoDisable();
			s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//翻转
			s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;//打印已经停止
			//TIM_DISABLE(TIMx);//关闭定时器//停止计数器时钟
			PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
			return;
		}
		//*********************************************
		#if 0
		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}
			g_StandbyPar = GetPaerStb(g_lineBitCount);//得到分区标准
		}
		else
		{
			#ifndef  RD_HIGH_PRINTER
			PRINTER_PDL.SYS._LAT_H();
			PRINTER_PDL.SDTH._IS_Send_Empty();
			//*********************************************
			PRINTER_PDL.SYS._LAT_L();
			PRINTER_PDL.SYS._LAT_H();			
			g_lindotnu = g_lineBitCount;
			
			
			#endif
			#ifdef  RD_HIGH_PRINTER
			g_lindotnu = g_lineBitCount;
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}
			#endif

		}
		#endif
		
		if(g_lineBitCount > 256)
		{
		
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}
			g_StandbyPar = 4;
		}
		else
		{
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}	
//			PRINTER_PDL.SYS._LAT_H();
//			PRINTER_PDL.SDTH._IS_Send_Empty();
//			//*********************************************
//			PRINTER_PDL.SYS._LAT_L();
//			PRINTER_PDL.SYS._LAT_H();	
		}
		
		g_lindotnu = g_lineBitCount;
		//*********************************************
		s_PrinterMOTOPar.m_PrintLSbit_f = s_PrinterMOTOPar.m_PrintLSbit;
		//*********************************************
		//行计数器加一
		g_PrintLineCount++;//准备处理下一行的数据，提前加一

		// *********************************************
		//打印到最后一行,此行开始打印字间距
		if(g_PrintLineCount >= p_ListParmeters->m_PrintLineHigh)//
		{
			if(p_ListParmeters->m_PrintLineSpace != 0)
			{
				p_ListParmeters->m_PrintLineHigh = p_ListParmeters->m_PrintLineSpace;
				p_ListParmeters->m_PrintLineSpace = 0;
				g_PrintLineCount = 0;
				s_PrinterMOTOPar.m_PrintLSbit = ENABLE_VAR;//使能间距 标志
			}
			else
			{
				if(g_PrintStatusReg == FINISH_PROCESS)
				{
					//添加缺纸和按键标志

					//*********************************************
					goto first_Pro_printDataList;

				}
				else
				{
					s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//停机;
					goto timer_run;
				}
			}
		}
		//****************使能数据发送*****************************
		 //启动发送程序
		//定时器3时钟使能发送数据
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
	}//if((Moto_Step & 0x3) == 0)
timer_run:
	
	g_RunFlag = 0;
	g_runLstep = 0;
	g_RunLTime =  s_PrintParameters.m_CurFreeTime;
//	if(s_PrintrConfig.m_SoftwarePart == 2)
	{
		g_RunLTime = 600;//380 430;
		//D11 380
		#ifdef SPEED_80MM
		g_RunLTime = 380;
		#endif
		
		
	}
	if(g_FeedStepCount < 90)
	{
		g_runLstep += 20 * (90 - g_FeedStepCount);
		
		g_RunLTime += 14 * (90 - g_FeedStepCount);
		
		g_FeedStepCount++;
	}
	
	
//	if(g_lindotnu > 156)
//	{
//		g_RunLTime = 450;
//		g_runLstep = 0;
//		g_FeedStepCount = 0;
//		g_RunLTime += g_lindotnu * 3;
//	}
	p_ListParmeters->m_HotTime = g_RunLTime;
//	if(s_PrinterMOTOPar.m_PrintLSbit_f != ENABLE_VAR)
	{
		
		if(g_lindotnu > 256)
		{
			g_RunCuStep = s_PrinterMOTOPar.m_MotoStep & 1;
			
			g_RunLTime = 450;
			g_runLstep = 0;
			g_FeedStepCount = 0;
			g_RunLTime += g_lindotnu * 2;
			
			g_RunFlag = g_RunFlagArry[g_StandbyPar];
			temp = g_ForArry[g_StandbyPar];//得到偏移倍数
			g_RunCuStep = g_RunCuStep * temp;//
			p_GetParSTB = bySTBExp[g_StandbyPar];
			SendParData(p_GetParSTB[g_RunCuStep++]);			
			PRINTER_PDL.SYS._STB_A_EN();
			PRINTER_PDL.SYS._STB_B_EN();		
			PRINTER_PDL.SYS._LAT_H();
			PRINTER_PDL.SDTH._IS_Send_Empty();
			//*********************************************
			PRINTER_PDL.SYS._LAT_L();
			PRINTER_PDL.SYS._LAT_H();				
			
			
		}
		else
		{
			// **********快速打印****************************************************************
			g_StandbyPar = 3;
			if(g_lindotnu == 0)
			{
				PRINTER_PDL.SYS._STB_ALL_CLEAR();//清加热线
				//g_RunLTime = p_ListParmeters->m_HotTime/4;//加热时间 5
				g_StandbyPar = 0;
			}
			else
			{
				//g_RunLTime = p_ListParmeters->m_HotTime/3;//加热时间 4
				//g_RunLTime = (p_ListParmeters->m_HotTime * 3)/4 ;//加热时间
				SendParData(0xaaaa);
				if(s_PrinterMOTOPar.m_MotoStep & 1)
				{
//					if(g_lindotnu > 156)
//					{
//						PRINTER_PDL.SYS._STB_A_EN();
//					}
//					else
//					{
						PRINTER_PDL.SYS._STB_A_EN();
						PRINTER_PDL.SYS._STB_B_EN();
//					}
					//	PRINTER_PDL.SYS._STB_A_EN();
					
				}
				else
				{
					SendParData(0x5555);
//					if(g_lindotnu > 156)
//					{
//						PRINTER_PDL.SYS._STB_B_EN();
//					}
//					else
//					{
						PRINTER_PDL.SYS._STB_A_EN();
						PRINTER_PDL.SYS._STB_B_EN();
//					}
//						PRINTER_PDL.SYS._STB_B_EN();
				}
				PRINTER_PDL.SYS._LAT_H();
				PRINTER_PDL.SYS._LAT_L();
				PRINTER_PDL.SYS._LAT_H();
			}
		
		}

	}
	//********************************************
	if(g_StandbyPar == 0)
	{
		PRINTER_PDL.SYS._STB_ALL_CLEAR();//清加热线
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],5);
	}
	else
	{

			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],5);
		
	}


	if(g_ReturnEp == 0xA002)
	{
		if(g_SpeedEnstu == 0xA123)
		{
			g_runLstep += 400;
			g_RunLTime += 100;
		}
	}

	PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime);
	s_PrinterMOTOPar.m_MotoStep++;
	
g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
//	g_MotoAll++;

}


/****************************************************************************
* 文件名：Timer1PrintDataSend
* 功  能：向打印机头发送数据
* 入口参数：
* 说  明：在进入timer1中断后发送下一行需要打印的数据，PrintLineCount 打印行数计数器
*为了能不在向打印头串行发送数据的时候耽误CPU的时间，使用SSC这个单元发送数据
****************************************************************************/

void SendDICLK(unsigned short u16_data)
{
	unsigned short mdat,temp;
//	SaveToBuffer[SendBuffer_counter] = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter));
//	SendBuffer_counter++;
//	g_lineBitCount += CalintCount((u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter)));
	mdat = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter))|g_QRCodeBuffer[SendBuffer_counter]|g_BarCodeBuffer[SendBuffer_counter];
	SaveToBuffer[SendBuffer_counter] = mdat;
	g_lineBitCount += CalintCount(mdat);

//	#ifndef RD_HIGH_PRINTER
//	if(s_PrintrConfig.m_SoftwarePart == 2)
//	{
//	//	while((SPI1->SR & SPI_FLAG_TXE) == 0);
//	//	SPI1->DR = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter));
//		mdat = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter));
//		PRINTER_PDL.SDTH._SendToHead_u16(&mdat,1);
//		
//	}
//	#endif
		SendBuffer_counter++;
		if(SendBuffer_counter == 24)
		{
			
		//	if(s_PrintrConfig.m_SoftwarePart == 2)
			{
				for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
				{
					SendBufferToSPI[temp] = SaveToBuffer[temp];
				}				
			//	SendParData(0xFFFF);
			}
			
		}
	
	
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
	PRINTER_PDL.SDTH._SendToHead_u8(buuff,BUFFER_SHORT_LENGTH*2);
	
	
	
	//PRINTER_PDL.SDTH._IS_Send_Empty();

}

#endif

#ifndef  HIGH_SPEED
void ProcessMotoLineData(void)
{
	unsigned short temp,ADtemp;
	float m_sum,m_reg;
	PRINTER_PDL.SYS._STB_ALL_CLEAR();
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	//ClearMoto();
	//PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_Trig();
	if(g_runLstep)
	{

		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],s_PrintParameters.m_CurMotoPWM);
		}
		else
		{
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],10);
		}
		//TIM_ENABLE(TIMx,g_runLstep);
		PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_runLstep);
		g_runLstep = 0;
		return;
	}
	ADtemp =  GetDETPaper();//得到光电采样值
	if((g_RunFlag & 0xaa00) == 0xaa00)
	{
		if(g_RunFlag & 0xF)
		{
			SendParData(p_GetParSTB[g_RunCuStep++]);
			PRINTER_PDL.SYS._LAT_L();
			//SET_H_STB1;SET_H_STB2;
			PRINTER_PDL.SYS._STB_A_EN();
			PRINTER_PDL.SYS._STB_B_EN();
			PRINTER_PDL.SYS._LAT_H();
			g_RunLTime = p_ListParmeters->m_HotTime;//加热时间
			g_RunLTime += (g_RunLTime >> 2) * g_StandbyPar;//
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],s_PrintParameters.m_CurMotoPWM);

			if(s_PrintrConfig.m_SoftwarePart == 2)
			{
				g_RunLTime += 300;
			}
			
			//TIM_ENABLE(TIMx,g_RunLTime);
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime);
			//TIM_ENABLE(TIM6,g_RunLTime>>1);//控制加热深度
			if(s_PrintParameters.m_CurMotoRunTime > g_RunLTime)
			{
				g_runLstep = s_PrintParameters.m_CurMotoRunTime - g_RunLTime;
			}
			g_RunFlag--;
			if(g_RunFlag < 0xAA01)
			{
				g_RunFlag = 0;
			}
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			return ;
		}
		g_RunFlag = 0;
	}
	if ((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
	{
		if(g_SendFlag == DATA_IN_SEND)
		{
			//TIM_ENABLE(TIMx,40);//等待一行数据处理完成
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(40);
			return;
		}
	}
// =============================================================================
	#ifdef PRINTER_LABEL
	
	if(g_SelfRunFlag == 0xAA)
	{
		if(ADtemp != 0)
		{
			if(ADtemp < g_DetPV_L)
			{
				g_DetPV_L = ADtemp;
			}
			if(ADtemp > g_DetPV_H)
			{
				g_DetPV_H = ADtemp;
			}
		}		
	}
	
	
//	if(s_PrintrConfig.m_InterFace == 0)
	{//0表示标签机
//		if(g_DetPaperValueFlag == 0xA5EF)
//		{
//			temp =  GetDETPaper();//得到光电采样值
//			g_DetPV_Count++;
//			if(g_DetPV_H < temp)
//			{
//				g_DetPV_H = temp;
//			}
//			if(g_DetPV_Count > 32*10)
//			{
//				s_PrintrConfig.m_PE_LabValue_H = g_DetPV_H - (g_DetPV_H /6);
//				g_DetPaperValueFlag = 0;
//				g_DetPV_Count = 0;
//				g_DetPV_H = 0;
//			}
//			
//		}	

// =============================================================================

		FitWave();
		//-------------------------------处理标签位置----------------------------------------------------------
		temp = g_PE_Staue^0xff;// 1
		Trg = temp & (temp ^ Cont);// 2
		Cont = temp;// 3
		//if((Trg&1) == 1)
		if(g_PE_Staue == 0)
		{
			//设置检测到纸缝标志
			g_SpaceStatus = 0xA500;
		}
		g_SpaceStatus |= g_PE_Staue;
		if((s_PrinterMOTOPar.m_MotoStep & p_ListParmeters->m_PrinterDip) == 0)
		{


			//标签打印
			//在每一步的开头计算
			if(g_PaperRunToOut)
			{
				g_PaperRunToOut--;
			}

			if(g_SpaceStatus == 0xA501)
			{
				//检测到标签的起始打印位置

				g_falg = 0x0303;

				g_SpaceStatus = 0;//清空监测状态
				g_DetPaperGapSpace = 0xaaa;//检测到标签头
				//监测到纸缝后往前所走的步数
				//只走到加热线处，出纸部分由超时控制
				#ifdef RD_TYPE_EBP
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + 6*8 - 8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
				#ifndef RD_TYPE_EBP
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + 6*8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
				#ifdef RD_D20
				g_PaperRunToOut = s_PrintrConfig.m_HotLineTOPD * 8  + 6*8 - 8;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
				#endif
			}


			if(g_PaperRunToOut == 0)
			{
				//检测到纸缝并且走到出纸位置
				if(g_DetPaperGapSpace == 0xAAA)
				{
					if(p_ListParmeters->m_LableStopFlag == 0xA5A1)
					{
						//走到0点，设置长度为值的长度+纸缝的长度
						g_PaperRunToOut = 8000;//8000步 1000mm 设置默认走1米长度

						if(g_PrintStatusReg == FINISH_PROCESS)
						{
							goto first_Pro_printDataList;
						}
						else
						{
							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//打印机停机;
							p_ListParmeters->m_LableStopFlag = 0;
						}
					}
					else
					{
//						if(g_SelfRunFlag != 0xAA)
//						{
//							g_RunToGapFlag = 0xA55A;
//							s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//打印机停机;
//							p_ListParmeters->m_LableStopFlag = 0;
//						}
					}
					
					
					
					//重新从一个表单开始打印记数
					g_DetPaperGapSpace = 0;


				}
			}
	
		}
		
	
	}	
	#endif
	
	
	
	
//	g_MotoSTBStatus = 0;
	if (s_PrinterMOTOPar.m_In_printing == ENABLE_VAR)//等于ENABLE_VAR 表示第一次进入打印程序需要预处理很多东西
	{

first_Pro_printDataList:
		//得到打印数据链表
		if(g_PrintStatusReg != FINISH_PROCESS)
		{
			goto Pro_printDataList_EX;
		}
		g_PrintStatusReg = 0;
		g_BufferSwitch = ~g_BufferSwitch;//取反打印链表切换标志
		if(g_BufferSwitch != 0)
		{
			p_ListParmeters = &s_ListParmeter_A;
		}
		else
		{
			p_ListParmeters = &s_ListParmeter_B;
		}

		s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//手动标志使能
		g_PrintLineCount = 0; //打印第一行的数据
		s_PrinterMOTOPar.m_PrintLSbit_f = DISABLE_VAR;//行间距标志清零
		s_PrinterMOTOPar.m_PrintLSbit = DISABLE_VAR;//无行间距 标志
		s_PrinterMOTOPar.m_MotoRun = ENABLE_VAR;//电机运行标志使能
		s_PrinterMOTOPar.m_StopPrint = DISABLE_VAR;//停止打印禁止
		s_PrinterMOTOPar.m_PrintEnd = DISABLE_VAR;//打印结束禁止
		s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//等于 DISABLE_VAR 表示打印机正在打印
		if(s_PrinterMOTOPar.m_InFirstPrint == ENABLE_VAR)
		{
			//第一次进入打印预处理
			s_PrinterMOTOPar.m_InFirstPrint = DISABLE_VAR;//清除第一次打印标志
			s_PrinterMOTOPar.m_MotoStep = 0;//开始打印moto运行步数清空
		//	TIM_ENABLE(TIMx,20000);//20ms启动时序
		//	TIM_ENABLE(TIM3,20);//启动定时器3处理一行数据
			PRINTER_PDL.SYS._SystemPrintIRQ_EN(20000);
			PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[(g_MotoAll ) & 7],s_PrintParameters.m_CurMotoPWM);
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			
			return;
		}
		//TIM_ENABLE(TIM3,10);//启动定时器3处理一行数据
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
		//判断停止打印
		if(s_PrinterMOTOPar.m_PrintEnd == ENABLE_VAR)
		{
			s_PrinterMOTOPar.m_In_printing = DISABLE_VAR;//;;;;;没有可以打印的数据
			//清除加热线和电机驱动信号线
		//	SET_L_STB1;SET_L_STB2;//清加热线
				PRINTER_PDL.SYS._STB_ALL_CLEAR();
		//	ClearMoto();
				PRINTER_PDL.MOTO._MotoDisable();
			s_PrinterMOTOPar.m_MutualFlag = ENABLE_VAR;//翻转
			s_PrinterMOTOPar.m_StopPrint = ENABLE_VAR;//打印已经停止
			//TIM_DISABLE(TIMx);//关闭定时器//停止计数器时钟
			PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();			
			g_FeedStepCount = 0;
			return;
		}
		//*********************************************
		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
			{
				SendBufferToSPI[temp] = SaveToBuffer[temp];
			}
			g_StandbyPar = GetPaerStb(g_lineBitCount);//得到分区标准
		}
		else
		{
			if(g_lineBitCount > 256)
			{
			
				for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
				{
					SendBufferToSPI[temp] = SaveToBuffer[temp];
				}
				g_StandbyPar = 4;
			}
			else
			{
			
//				PRINTER_PDL.SYS._LAT_H();
//				PRINTER_PDL.SDTH._IS_Send_Empty();
//				//*********************************************
//				PRINTER_PDL.SYS._LAT_L();
//				PRINTER_PDL.SYS._LAT_H();	
			}
			
			

		}
		g_lindotnu = g_lineBitCount;
		//*********************************************
		s_PrinterMOTOPar.m_PrintLSbit_f = s_PrinterMOTOPar.m_PrintLSbit;
		//*********************************************
		//行计数器加一
		g_PrintLineCount++;//准备处理下一行的数据，提前加一

		// *********************************************
		//打印到最后一行,此行开始打印字间距
		if(g_PrintLineCount >= p_ListParmeters->m_PrintLineHigh)//
		{
			if(p_ListParmeters->m_PrintLineSpace != 0)
			{
				p_ListParmeters->m_PrintLineHigh = p_ListParmeters->m_PrintLineSpace;
				p_ListParmeters->m_PrintLineSpace = 0;
				g_PrintLineCount = 0;
				s_PrinterMOTOPar.m_PrintLSbit = ENABLE_VAR;//使能间距 标志
			}
			else
			{
				if(g_PrintStatusReg == FINISH_PROCESS)
				{
					//添加缺纸和按键标志

					//*********************************************
					goto first_Pro_printDataList;

				}
				else
				{
					s_PrinterMOTOPar.m_PrintEnd = ENABLE_VAR;//停机;
					goto timer_run;
				}
			}
		}
		//****************使能数据发送*****************************
		 //启动发送程序
		//定时器3时钟使能发送数据
		PRINTER_PDL.SYS._SystemPrintLineDat_EN(0);
	}//if((Moto_Step & 0x3) == 0)
timer_run:
	
	g_RunFlag = 0;
	g_runLstep = 0;
	g_RunLTime =  s_PrintParameters.m_CurFreeTime;
	if(s_PrintrConfig.m_SoftwarePart == 2)
	{
	
		g_RunLTime =  750;//550;
		if((g_THDeep & 0x8000) == 0)
		{
			g_RunLTime += g_THDeep;//得到打印头的加热时间
		}
		else
		{
			if(g_RunLTime > (g_THDeep & 0x7FFF))
			{
				g_RunLTime -= (g_THDeep & 0x7FFF);//得到打印头的加热时间
			}
		}			



		m_sum	= (float)g_PowerVol;
		m_sum	= m_sum/1000;
		m_sum = m_sum * m_sum; 
		m_reg = 36;//360000;
		m_reg = m_reg * g_RunLTime;
		

		
		
		g_RunLTime = m_reg/m_sum;
		
		m_reg = 9;
		m_sum =  (float)g_PowerVol/1000;
		
		g_RunLTime = g_RunLTime * m_reg/m_sum;
		
		
		if(g_RunLTime < 550)
		{
			g_runLstep += 550 - g_RunLTime;
		}
	
		if(g_PaperType == 0)
		{
			
			g_RunLTime = (g_RunLTime *13)/10;
			
		}
		if(g_FeedStepCount < 40)
		{
			g_runLstep += 15 * (40 - g_FeedStepCount);
			
			g_RunLTime += 5 * (40 - g_FeedStepCount);
			
			g_FeedStepCount++;
		}		
		
	}

//	if(s_PrinterMOTOPar.m_PrintLSbit_f != ENABLE_VAR)
	{
		if(s_PrintrConfig.m_SoftwarePart != 2)
		{
			g_RunCuStep = s_PrinterMOTOPar.m_MotoStep & 1;


			if(g_StandbyPar == 0)
			{
				g_RunLTime = s_PrintParameters.m_CurFreeTime;//空走纸速度
				SendParData(0);
			}
			else
			{
				g_RunLTime = p_ListParmeters->m_HotTime;//加热时间

				if(p_ListParmeters->m_ThType == 0)
				{	//正常处理流程


					g_RunLTime += (g_RunLTime >> 2) * g_StandbyPar;//
					g_RunFlag = g_RunFlagArry[g_StandbyPar];
					temp = g_ForArry[g_StandbyPar];//得到偏移倍数
					g_RunCuStep = g_RunCuStep * temp;//
					p_GetParSTB = bySTBExp[g_StandbyPar];
					SendParData(p_GetParSTB[g_RunCuStep++]);
					if(s_PrintParameters.m_CurMotoRunTime > g_RunLTime)
					{
						g_runLstep = s_PrintParameters.m_CurMotoRunTime - g_RunLTime;
					}
				}
				else
				{
					SendParData(0xFFFF);
					if(p_ListParmeters->m_MotoRunTime != 0)
					{
						if(p_ListParmeters->m_MotoRunTime > g_RunLTime)
						{
							g_runLstep = p_ListParmeters->m_MotoRunTime - g_RunLTime;
						}
					}
				}

			}
			PRINTER_PDL.SYS._LAT_L();
			//SET_H_STB1;SET_H_STB2;
			PRINTER_PDL.SYS._STB_A_EN();
			PRINTER_PDL.SYS._STB_B_EN();
			PRINTER_PDL.SYS._LAT_H();			
		}
#if 1		
		else
		{
			
			if(g_lindotnu > 256)
			{
				g_RunLTime = p_ListParmeters->m_HotTime;//加热时间
				g_RunLTime += (g_RunLTime >> 2) * g_StandbyPar;//
				g_RunFlag = g_RunFlagArry[g_StandbyPar];
				temp = g_ForArry[g_StandbyPar];//得到偏移倍数
				g_RunCuStep = g_RunCuStep * temp;//
				p_GetParSTB = bySTBExp[g_StandbyPar];
				SendParData(p_GetParSTB[g_RunCuStep++]);
				if(s_PrintParameters.m_CurMotoRunTime > g_RunLTime)
				{
					g_runLstep = s_PrintParameters.m_CurMotoRunTime - g_RunLTime;
				}		
				PRINTER_PDL.SYS._STB_A_EN();
				PRINTER_PDL.SYS._STB_B_EN();		
				PRINTER_PDL.SYS._LAT_H();
				PRINTER_PDL.SDTH._IS_Send_Empty();
				//*********************************************
				PRINTER_PDL.SYS._LAT_L();
				PRINTER_PDL.SYS._LAT_H();				
				
				
			}
			else
			{
					
				//g_RunLTime = p_ListParmeters->m_HotTime/3;//加热时间 4
				//g_RunLTime = (p_ListParmeters->m_HotTime * 3)/4 ;//加热时间
				g_StandbyPar = 4;
				SendParData(0xaaaa);
				if(s_PrinterMOTOPar.m_MotoStep & 1)
				{
//					if(g_lindotnu > 156)
//					{
//						PRINTER_PDL.SYS._STB_A_EN();
//					}
//					else
//					{
						PRINTER_PDL.SYS._STB_A_EN();
						PRINTER_PDL.SYS._STB_B_EN();
//					}
					//	PRINTER_PDL.SYS._STB_A_EN();
					
				}
				else
				{
					SendParData(0x5555);
//					if(g_lindotnu > 156)
//					{
//						PRINTER_PDL.SYS._STB_B_EN();
//					}
//					else
//					{
						PRINTER_PDL.SYS._STB_A_EN();
						PRINTER_PDL.SYS._STB_B_EN();
//					}
//						PRINTER_PDL.SYS._STB_B_EN();
				}
				PRINTER_PDL.SYS._LAT_H();
				PRINTER_PDL.SYS._LAT_L();
				PRINTER_PDL.SYS._LAT_H();
			
			}

		}
#endif	
		//	TIM_ENABLE(TIM6,g_RunLTime >> 1);//控制加热深度
	}
	//********************************************
	if(g_StandbyPar == 0)
	{
		PRINTER_PDL.SYS._STB_ALL_CLEAR();//清加热线
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],10);
	}
	else
	{
		#ifdef RD_W2C1PRINTER
		s_PrintParameters.m_CurMotoPWM = 20;
		#endif
		if(s_PrintrConfig.m_SoftwarePart == 2)
		{
			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],10);
		}
		else
		{
			g_MotoAll++;
			PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll & 7],s_PrintParameters.m_CurMotoPWM);
		}
	}


	if((g_SelfRunFlag == 0xAA) || (g_PaperType != 0))
	{
		PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime);
		s_PrinterMOTOPar.m_MotoStep++;
		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
		g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		
		return;
	}

	PRINTER_PDL.SYS._SystemPrintIRQ_EN(g_RunLTime);
	s_PrinterMOTOPar.m_MotoStep++;
//	if(s_PrintrConfig.m_SoftwarePart != 2)
//	{
//		g_runLstep += p_ListParmeters->m_FeedTime ;
//	
//		g_runLstep += (p_ListParmeters->m_FeedTime >> 2) * g_StandbyPar ;
//	}
	
//	g_MotoAll++;
	//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
	g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();	
	
}


/****************************************************************************
* 文件名：Timer1PrintDataSend
* 功  能：向打印机头发送数据
* 入口参数：
* 说  明：在进入timer1中断后发送下一行需要打印的数据，PrintLineCount 打印行数计数器
*为了能不在向打印头串行发送数据的时候耽误CPU的时间，使用SSC这个单元发送数据
****************************************************************************/

void SendDICLK(unsigned short u16_data)
{
	unsigned short mdat,temp;
//	SaveToBuffer[SendBuffer_counter] = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter));
//	SendBuffer_counter++;
//	g_lineBitCount += CalintCount((u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter)));
	mdat = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter))|g_QRCodeBuffer[SendBuffer_counter]|g_BarCodeBuffer[SendBuffer_counter];
	SaveToBuffer[SendBuffer_counter] = mdat;
	g_lineBitCount += CalintCount(mdat);

//	#ifndef RD_HIGH_PRINTER
//	if(s_PrintrConfig.m_SoftwarePart == 2)
//	{
//	//	while((SPI1->SR & SPI_FLAG_TXE) == 0);
//	//	SPI1->DR = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter));
//		mdat = (u16_data | *(p_ListParmeters->m_LineProArry + SendBuffer_counter));
//		PRINTER_PDL.SDTH._SendToHead_u16(&mdat,1);
//		
//	}
//	#endif
		SendBuffer_counter++;
		if(SendBuffer_counter == 24)
		{
			
			if(s_PrintrConfig.m_SoftwarePart == 2)
			{
				for(temp=0;temp<BUFFER_SHORT_LENGTH;temp++)
				{
					SendBufferToSPI[temp] = SaveToBuffer[temp];
				}			
				if(g_lineBitCount < 201)
				{					
					SendParData(0xFFFF);
				}
			}
			
		}
	
	
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
	PRINTER_PDL.SDTH._SendToHead_u8(buuff,BUFFER_SHORT_LENGTH*2);
	
	
	
	//PRINTER_PDL.SDTH._IS_Send_Empty();

}
#endif
/****************************************************************************
* 文件名：Timer1PrintDataSend
* 功  能：向打印机头发送数据
* 入口参数：
* 说  明：在进入timer1中断后发送下一行需要打印的数据，PrintLineCount 打印行数计数器
*为了能不在向打印头串行发送数据的时候耽误CPU的时间，使用SSC这个单元发送数据
****************************************************************************/
void OneLinePrintDataSend(void)
{
	p_PrintListPar			p_PrParmeters	;//链表结构指针
	DataCon					m_SSCdata;//SSC数据发送结构
	int 					dummy;						// temporary
	unsigned short mTempcount,i;
	unsigned int			m_Dotdata;
	unsigned int			m_Tempdata;
//	int						m_xount;
	unsigned short			addpar;
	unsigned short			m_IndexCounter;//索引表
	unsigned short			*p_PrinterData;//数据指针
	unsigned short			m_GOrder;//命令

	unsigned short			m_CharWide;//字符的宽度
	unsigned short			m_WideZoom;//宽度放大
	unsigned short			m_CharHigh;//字符的高度
	unsigned short			m_HighZoom;//高度放大
	unsigned short			m_calZe;//计算器
	unsigned short			m_calR0;//计算器
	unsigned short			m_alignSpace;//上下对齐和字间距


//---------------清除中断标志----------------------------------------

	for(i=0;i<36;i++)
	{
		g_QRCodeBuffer[i] = 0;
		g_BarCodeBuffer[i] = 0;
	}
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

//-------------禁止中断------------------------------------------

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

//---------------------先打印开头的空白点------------------
//==========================第一步========================================


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
		//===========开始打印数据
		p_PrinterData = p_PrParmeters->m_ListBuffer + m_GOrder;//得到数据起始地址
		m_GOrder = *p_PrinterData++;
		if (m_GOrder == PRINT_DATA )
		{
			m_CharWide = ((*p_PrinterData) >> 4) & 0xff;
			m_WideZoom = (*p_PrinterData++) & 0xf;
			m_CharHigh = ((*p_PrinterData) >> 4) & 0xff;
			m_HighZoom = (*p_PrinterData++) & 0xf;
			m_alignSpace = *p_PrinterData++;
			m_calZe = m_CharHigh * m_HighZoom;


			//---------------判断可打印行数-------------------------
			if ((m_alignSpace & 0x8000) != 0)
			{
				//-----------下标---------------------
				dummy = g_PrintLineCount + m_calZe - p_PrParmeters->m_PrintLineHigh;//打印的行高
			}
			else
			{
				//-----------上标---------------------
				if(g_PrintLineCount < m_calZe)
					dummy = g_PrintLineCount;
				else
					dummy = -1;
			}
			m_alignSpace &= 0x7fff;//去掉最高位的上下对齐标志
			//---------------打印空白点
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
			//=--------------打印实际数据------------------------------------------

			m_calR0 = (dummy * DiV_config[m_HighZoom]) >> 16;//得到行数
			if (m_CharWide > 16)//15)
			{
				p_PrinterData  +=  (m_calR0 << 1);//得到点阵数据存放的起始位置
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
					m_Tempdata = Config_zoom[m_Tempdata];//得到放大后的数据
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
					m_calR0 = m_WideZoom * dummy;//得到移位大小
					m_Tempdata = m_Tempdata + ((m_WideZoom -1)<< 4);
					m_Tempdata = Config_zoom[m_Tempdata];//得到放大后的数据
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
			//处理 空点 和图形 曲线
			//========================================

			if (m_GOrder == PRINT_CURVE )
			{
				p_PrinterData++; //得到打印的方向

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
				m_CharWide = *p_PrinterData;//得到打印空点的宽度

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
				//---------------判断可打印行数-------------------------
				if ((m_alignSpace & 0x8000) != 0)
				{
					//-----------下标---------------------
					dummy = g_PrintLineCount + m_calZe - p_PrParmeters->m_PrintLineHigh;//打印的行高
				}
				else
				{
					//-----------上标---------------------
					if(g_PrintLineCount < m_calZe)
						dummy = g_PrintLineCount;
					else
						dummy = -1;
				}
				m_alignSpace &= 0x7fff;//去掉最高位的上下对齐标志
				//---------------打印空白点
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
				//=--------------打印实际数据------------------------------------------

				m_calR0 = (dummy * DiV_config[m_HighZoom]) >> 16;//得到行数
				//---------得到图形宽度字节
				m_calZe = m_CharWide >> 4;
				if ((m_CharWide & 0xf) != 0)
				{
					m_calZe++;
				}
				//数据起始地址
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
						m_Tempdata = Config_zoom[m_Tempdata];//得到放大后的数据
						//-------------发送数据
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
					//处理剩余数据

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
						m_Tempdata = Config_zoom[m_Tempdata];//得到放大后的数据
						//-------------发送数据
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
	//*********************结束******************************************
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

void SetSelftestData(void)
{
	unsigned short m_type;
	unsigned short i,com,m_DaSpave;
	unsigned int m_judgeyear;
	unsigned char name_bt[32];
	char mbuf[128];

	PrinterParameterInt();
	
	g_Receive_count = 0;//清接收缓存计数器
	Receive_count = 0;//清接收计数器
	g_Order_count = 0;//清接取数寄存器	
	
	
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
	m_DaSpave = (*p_REAndSAcount);//得到序列号起始位置
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

#ifdef SET_BT_NAME

	i = 0;
	while(bt_name[i])
	{
		receive_bufer[(*p_REAndSAcount)++] = bt_name[i++];
	}
	receive_bufer[(*p_REAndSAcount)++] = 0x0D;
#endif
//---------------------------------------------------------------

	#ifdef RD_TYPE_SERIAL
	m_type = INTERFACE_SERIAL;
	#endif
//---------------------------------------------------------------
 
// 	RCC_GetClocksFreq(&clocks);

//	sprintf(mbuf,"SYSCLK: %3.1fMhz, HCLK: %3.1fMhz, PCLK1: %3.1fMhz, PCLK2: %3.1fMhz, ADCCLK: %3.1fMhz\n", \
//	(float)clocks.SYSCLK_Frequency/1000000, (float)clocks.HCLK_Frequency/1000000, \
//	(float)clocks.PCLK1_Frequency/1000000, (float)clocks.PCLK2_Frequency / 1000000, (float)clocks.ADCCLK_Frequency / 1000000);
// 		i = 0;
//		while(mbuf[i])
//		{
//			receive_bufer[(*p_REAndSAcount)++] = mbuf[i++];
//		}
//---------------------------------------------------------------
	if(m_type == INTERFACE_SERIAL)
	{
		i = 0;
		while(SelfTestD_1[i])
		{
			receive_bufer[(*p_REAndSAcount)++] = SelfTestD_1[i++];
		}
		com = FlowFlag & 0x0F;
		switch(com)
		{
			case 0x0:
		//150
				i =0;
				while(SelfTestD_150[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_150[i++];
				}
				break;
			case 0x1:
		//300
				i =0;
				while(SelfTestD_300[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_300[i++];
				}
				break;
			case 0x2:
		//600
				i =0;
				while(SelfTestD_600[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_600[i++];
				}
				break;
			case 0x3:
		//1200
				i =0;
				while(SelfTestD_1200[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_1200[i++];
				}
				break;
			case 0x4:
		//2400
				i =0;
				while(SelfTestD_2400[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_2400[i++];
				}
				break;
			case 0x5:
		//4800
				i =0;
				while(SelfTestD_4800[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_4800[i++];
				}
				break;
			case 0x6:
		//9600
				i =0;
				while(SelfTestD_9600[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_9600[i++];
				}
				break;
			case 0x7:
		//19200  0000
				i =0;
				while(SelfTestD_19200[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_19200[i++];
				}
				break;
			case 0x8:
		//4800
				i =0;
				while(SelfTestD_38400[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_38400[i++];
				}
				break;
			case 0x9:
		//9600
				i =0;
				while(SelfTestD_57600[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_57600[i++];
				}
				break;
			case 0xA:
		//19200  0000
				i =0;
				while(SelfTestD_115200[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_115200[i++];
				}
			case 0xB:
		//19200  0000
				i =0;
				while(SelfTestD_230400[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_230400[i++];
				}				
				break;

		}
		switch(FlowFlag & 0x30)
		{
			case 0x00:
				i =0;
				while(SelfTestD_8N1[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_8N1[i++];
				}
				break;
			case 0x20:
				i =0;
				while(SelfTestD_8O1[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_8O1[i++];
				}
				break;
			case 0x10:
				i =0;
				while(SelfTestD_8E1[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_8E1[i++];
				}
				break;
			case 0x30:
				i =0;
				while(SelfTestD_7E1[i])
				{
					receive_bufer[(*p_REAndSAcount)++] = SelfTestD_7E1[i++];
				}
				break;
		}


		if((FlowFlag & 0x40) == 0)
		{
			i =0;
			while(SelfTestD_Ctrl[i])
			{
				receive_bufer[(*p_REAndSAcount)++] = SelfTestD_Ctrl[i++];
			}
		}
		else
		{
			i =0;
			while(SelfTestD_XFCtrl[i])
			{
				receive_bufer[(*p_REAndSAcount)++] = SelfTestD_XFCtrl[i++];
			}
		}

	}
	
		#ifdef RD_TYPE_PARALLEL
	m_type = INTERFACE_PARALLEL;
	#endif
	if(m_type == INTERFACE_PARALLEL)
	{
		i = 0;
		while(SelfTestD_P[i])
		{
			receive_bufer[(*p_REAndSAcount)++] = SelfTestD_P[i++];
		}
	}

	if(s_PrintrConfig.m_ChineseFont == PRINTTYPE_24)
	{
		i =0;
		while(SelfTestD_2[i])
		{
			receive_bufer[(*p_REAndSAcount)++] = SelfTestD_2[i++];
		}
		if(s_PrintrConfig.m_ChineseLib == TYPE_GB2312)
		{
			i =0;
			while(SelfTestD_24_GB2312[i])
			{
				receive_bufer[(*p_REAndSAcount)++] = SelfTestD_24_GB2312[i++];
			}
		}
		else
		{
			i =0;
			while(SelfTestD_24_GBK[i])
			{
				receive_bufer[(*p_REAndSAcount)++] = SelfTestD_24_GBK[i++];
			}

		}
	}
	else
	{
		i =0;
		while(SelfTestD_2_1[i])
		{
			receive_bufer[(*p_REAndSAcount)++] = SelfTestD_2_1[i++];
		}
		if(s_PrintrConfig.m_ChineseLib == TYPE_GB2312)
		{
			i =0;
			while(SelfTestD_16_GB2312[i])
			{
				receive_bufer[(*p_REAndSAcount)++] = SelfTestD_16_GB2312[i++];
			}
		}
		else
		{
			i =0;
			while(SelfTestD_16_GBK[i])
			{
				receive_bufer[(*p_REAndSAcount)++] = SelfTestD_16_GBK[i++];
			}

		}
	}

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
	receive_bufer[(*p_REAndSAcount)++] = 0xAD;//设置DUMP模式
	receive_bufer[(*p_REAndSAcount)++] = 0x1B;
	receive_bufer[(*p_REAndSAcount)++] = 0x40;
	receive_bufer[(*p_REAndSAcount)++] = 0x1B;
	receive_bufer[(*p_REAndSAcount)++] = 0x31;
	receive_bufer[(*p_REAndSAcount)++] = 0x06;

	receive_bufer[(*p_REAndSAcount)++] = 0x1B;//计算标签值
	receive_bufer[(*p_REAndSAcount)++] = 0xBC;
	receive_bufer[(*p_REAndSAcount)++] = 0x53;
	receive_bufer[(*p_REAndSAcount)++] = 0xAA;
	receive_bufer[(*p_REAndSAcount)++] = 0x12;
	receive_bufer[(*p_REAndSAcount)++] = 0x11;

	(*p_SaveDrREcount) = (*p_REAndSAcount);

	PRINTER_PDL.SYS._WDG_ReloadCounter();
}
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] =Setresort[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}

	(*p_REAndSAcount)++;//缓冲区得到的数据个数
	receive_bufer[(*p_SaveDrREcount)++] = (6 - baudvalue) + 0x30;
	(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] =p_SetDataParity[i++];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
	for(i=0;i<4;i++)
	{
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
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
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		receive_bufer[(*p_SaveDrREcount)++] = 0x0D;
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;
	}
}
//对射光电
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

//反射光电

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
	
	
	return dat;

}
 /****************************************************************************
 * 函数名： PrinterLPS()
 * 功  能：检测缺纸

 ****************************************************************************/
 void PrinterLPS(void)
 {
 	unsigned int count,i;
	 	unsigned int DelayCount;
	 unsigned char data;
 	g_LPSStatus = GetPaper();//AD_Value[1]>>4;
 	PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
  	if(g_LPSStatus < g_PE_Standard)
 	{
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
		#ifdef RD_TYPE_PARALLEL
		PRINTER_PDL.PARALLEL._SetPE();
		#endif
 lps_feed:
		DelayCount = 0;
 		while(g_LPSStatus < g_PE_Standard)
 		{
			#ifdef RD_TYPE_PARALLEL
			#ifdef RD_PARALLEL_PE
			SET_H_PE_P;
			#endif
			#endif
			//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
				
 			g_LPSStatus = GetPaper();//AD_Value[1]>>4;
 			PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog
 			while(PRINTER_PDL.SYS._GetKey_LF() == 0)
 			{
				Delay1ms(10);
				DelayCount++;
				
				if(DelayCount > 500)
				{
						//恢复默认值
					g_LedPFlag = 0;
					PRINTER_PDL.SYS._LED_Stauts_ON();
					PRINTER_PDL.SYS._LED_Stauts_ON();
					PRINTER_PDL.SYS._LED_Stauts_ON();
					PrinterConfigInt();
					PRINTER_PDL.SYS._WDG_ReloadCounter();
					SavePrinterConfig();
					PRINTER_PDL.SYS._WDG_ReloadCounter();
					ReadPrinterConfig();				
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
			if(Receive_count)
			{
				__SETPRIMASK();   // 使能串口1中断
				Receive_count--;
				__RESETPRIMASK();//禁止串口1中断

				data = receive_bufer[g_Order_count++];
				g_Order_count &= MASK_DATA;
				NpReturnStatus(data);
				
			}




 		}
		g_LedPFlag = 0;
		g_LedPFlag = 0;
		g_LedPFlag = 0;
		//SET_L_LED;		  // turn LED  on
		#ifdef LED_OFF
		PRINTER_PDL.SYS._LED_Stauts_OFF();
		#endif
		#ifndef LED_OFF
		PRINTER_PDL.SYS._LED_Stauts_ON();
		#endif

	
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
			
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


		g_DetPaperValueFlag = 0xA5EF;
		g_DetPV_Count = 0;
		g_DetPV_H = 0;

		#ifdef PRINTER_LABEL
		if(g_PaperType == 0)
		{
			Delay1ms(4000);
			RunFeedLabPaperLPS();
		}
		#endif


		while(Receive_count)
		{
				__SETPRIMASK();   // 使能串口1中断
				Receive_count--;
				__RESETPRIMASK();//禁止串口1中断

				data = receive_bufer[g_Order_count++];
				g_Order_count &= MASK_DATA;
				NpReturnStatus(data);
				
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
	//	PrinterIdle();//设置打印机空闲
		#ifdef RD_TYPE_PARALLEL
		#ifdef RD_PARALLEL_PE
		SET_L_PE_P;
		#endif
		#endif
		#ifdef LED_OFF
		PRINTER_PDL.SYS._LED_Stauts_OFF();
		#endif
		#ifndef LED_OFF
		PRINTER_PDL.SYS._LED_Stauts_ON();
		#endif

 		g_PaperEnd = 0;
		#ifdef RD_TYPE_PARALLEL
		PRINTER_PDL.PARALLEL._ClearPE();
		#endif
 	}
}




void GetLabPaperSta(void)
{
	unsigned short i,j,temp,sw;
	unsigned int count;
		unsigned short RunstarStepTime,StepTime;
	unsigned short data_L,data_H;
	double a,valu_a;
	PRINTER_PDL.SYS._Drive_Power_EN();

	data_L = 0xFFFF;
	data_H = 0;
	for(i=0;i<30;i++)
	{
		Delay1ms(2);
		PRINTER_PDL.SYS._PaperGapDet();	
		SetFreValue(PRINTER_PDL.SYS._PaperGapDet());
	}
	sw = 0;
	
	count = 3200;
	valu_a = 0;
	RunstarStepTime = 3000;
	j=0;
	while(count--)
	{
		
		PRINTER_PDL.MOTO._MotoDisable();
		
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
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
		if(StepTime < 900)
		{
			StepTime = 900;
		}			
		j++;
		
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0);

		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		
		temp =  GetDETPaper();////得到光电采样值
		
		if((g_MotoAll & 1) == 0)
		{

			SetFreValue(temp);//设置采样光电值
			if(j > 31)
			{
				if(sw == 0)
				{
					linearRegression_31();
					sw = 1;
				}
				else
				{
					sw = 0;
					linearRegression_32(&a);
					if(valu_a < a)
					{
						valu_a = a;
					}
				}
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
	


	s_PrintrConfig.m_PE_LabValue_H =  ((unsigned short)valu_a * 2 )/3;//data_H- (i *5);//5

	printf("s_PrintrConfig.m_PE_LabValue_H:%d \r\n",s_PrintrConfig.m_PE_LabValue_H);

	if(s_PrintrConfig.m_PE_LabValue_H < 10 )
	{
		//连续纸
		g_PaperType = 1;
		s_PrintrConfig.m_InterFace = 1;
	}
	else
	{
		g_PaperType = 0;
		s_PrintrConfig.m_InterFace = 0;
	}		
	SavePrinterConfig();
//	MyPrintf("s_PrintrConfig.m_PE_LabValue_H: %04X  g_PaperType= %d\r\n",s_PrintrConfig.m_PE_LabValue_H ,g_PaperType);
	
	if(g_PaperType == 0)
	{
		RunFeedLabPaper();
	}
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();
}
void RunFeedLabPaper(void)
{	
	unsigned short temp,i,j,k,count,runcount,m_SpaceStatus,tempAD;
	unsigned short StepTime;
	unsigned short RunstarStepTime;
//	SET_H_VCC_CON;//关闭充电
	g_DetPaperValueFlag = 0xA5EF;
	g_DetPV_Count = 0;
	g_DetPV_H = 0;
	m_SpaceStatus = 0;
	PRINTER_PDL.SYS._Drive_Power_EN();

	RunstarStepTime = 3000;
	PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],500);
	g_Tim3ProFlag = 0;
	PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(RunstarStepTime);
	while(g_Tim3ProFlag == 0)
	{
	}
	g_PE_Staue = 1;
	RunstarStepTime = 3000;
	g_PE_DN_Count = 0;
	j = 0;
	k = 0;

	count = 4800;
	while(count--)
	{

		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],400);
		
		if(RunstarStepTime > (j * 100))
		{
			StepTime = RunstarStepTime - j * 100;
		}
		else
		{
			StepTime = 0;
		}		
		if(StepTime < 950)
		{
			StepTime = 950;
		}
		g_Tim3ProFlag = 0;
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
	
		}
		k++;
		j++;
		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();
		
		
			tempAD =  GetDETPaper();//得到光电采样值

//		PRINTER_PDL.TTL._SendChar(tempAD&0xFF);
//		PRINTER_PDL.TTL._SendChar(tempAD >> 8);
		if(g_DetPaperValueFlag == 0xA5EF)
		{

			g_DetPV_Count++;
			if(g_DetPV_H < tempAD)
			{
				g_DetPV_H = tempAD;
			}
			if(g_DetPV_Count > 32*10)
			{
				//s_PrintrConfig.m_PE_LabValue_H = g_DetPV_H - (g_DetPV_H /10);
				g_DetPaperValueFlag = 0;
				g_DetPV_Count = 0;
			}
			g_PE_UP_Count = 0;
			g_PE_DN_Count = 0;
			g_PE_Staue = 1;					
		}			
	
		
		FitWave();
		//-------------------------------处理标签位置----------------------------------------------------------
//		temp = g_PE_Staue^0xff;// 1
//		Trg = temp & (temp ^ Cont);// 2
//		Cont = temp;// 3
//		
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
		g_LPSStatus = GetPaper();//AD_Value[1]>>4;

		if(g_LPSStatus < g_PE_Standard)
		{
				
			PRINTER_PDL.MOTO._MotoDisable();
			PRINTER_PDL.SYS._Drive_Power_DIS();			
			return;				
		}		
//		MyPrintf("g_PE_Staue: %02X\r\n",g_PE_Staue );
		if(g_PE_Staue == 0)
		{
			//设置检测到纸缝标志
			m_SpaceStatus = 0xA500;
		}
		m_SpaceStatus |= g_PE_Staue;	
		
		//SendChar(g_PE_Staue);
		runcount = 0;
		if(m_SpaceStatus == 0xA501)
		{
			m_SpaceStatus = 0;
			//runcount = (s_PrintrConfig.m_HotLineTOPD + s_PrintrConfig.m_HotLineTOOUT )*32;
			runcount = (s_PrintrConfig.m_HotLineTOPD + 6 )*32;
			#ifdef RD_D20
			runcount = s_PrintrConfig.m_HotLineTOPD * 32  + s_PrintrConfig.m_HotLineTOOUT*32 -32;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
			#endif
			
			#ifdef RD_TYPE_D30
			runcount = s_PrintrConfig.m_HotLineTOPD * 32  + s_PrintrConfig.m_HotLineTOOUT*32 -32;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
			#endif
			
			
			//PrintLineTest();

			break;
		}
	}
		g_Tim3ProFlag = 0;
	//	GetNoPaperStatus();
			PRINTER_PDL.SYS._STB_A_EN();
		PRINTER_PDL.SYS._STB_B_EN();	
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(10000);
		while(g_Tim3ProFlag == 0)
		{
			
		}		
		//--纸缝再次识别
		PRINTER_PDL.SYS._STB_A_DIS();
		PRINTER_PDL.SYS._STB_B_DIS();		
	
	StepTime = 950;
	for(;runcount>0;runcount--)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		g_MotoAll++;
		SetMotoPWM(PrintMotoHalfStep[g_MotoAll  & 7],300);
		g_Tim3ProFlag = 0;
	//	GetNoPaperStatus();
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
			
		}		

		
		
	}
//		SavePrinterConfig();
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();		

		PRINTER_PDL.SYS._STB_A_DIS();
		PRINTER_PDL.SYS._STB_B_DIS();	
}


void RunFeedLabPaperLPS(void)
{	
	unsigned short temp,i,j,k,count,runcount,m_SpaceStatus,tempAD;
	unsigned short StepTime,m_PE_LabValue_H,m_PE_LabValue_L;
	unsigned short RunstarStepTime;
//	SET_H_VCC_CON;//关闭充电
	g_DetPaperValueFlag = 0xA5EF;
	g_DetPV_Count = 0;
	g_DetPV_H = 0;
	g_DetPV_L = 0xFFFF;
	m_SpaceStatus = 0;
	PRINTER_PDL.SYS._Drive_Power_EN();

	RunstarStepTime = 3000;
	PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],500);
	g_Tim3ProFlag = 0;
	PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(RunstarStepTime);
	while(g_Tim3ProFlag == 0)
	{
	}
	g_PE_Staue = 1;
	RunstarStepTime = 3000;
	g_PE_DN_Count = 0;
	j = 0;
	k = 0;

	count = 4800;
	while(count--)
	{

		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],400);
		
		if(RunstarStepTime > (j * 100))
		{
			StepTime = RunstarStepTime - j * 100;
		}
		else
		{
			StepTime = 0;
		}		
		if(StepTime < 950)
		{
			StepTime = 950;
		}
		g_Tim3ProFlag = 0;
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
	
		}
		k++;
		j++;
		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
	
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();		
		
		
			tempAD =  GetDETPaper();//得到光电采样值
		

		
		
//		PRINTER_PDL.TTL._SendChar(tempAD&0xFF);
//		PRINTER_PDL.TTL._SendChar(tempAD >> 8);
		if(g_DetPaperValueFlag == 0xA5EF)
		{

			g_DetPV_Count++;
			if(g_DetPV_Count > 32*10)
			{
				//s_PrintrConfig.m_PE_LabValue_H = g_DetPV_H - (g_DetPV_H /10);
				g_DetPaperValueFlag = 0;
				g_DetPV_Count = 0;


			}
			g_PE_UP_Count = 0;
			g_PE_DN_Count = 0;
			g_PE_Staue = 1;					
		}	
		else
		{
			if(g_DetPV_H < tempAD)
			{
				g_DetPV_H = tempAD;
			}
			if(g_DetPV_L > tempAD )
			{
				g_DetPV_L = tempAD;
			}					
		}
	
	
		
		FitWave();
		//-------------------------------处理标签位置----------------------------------------------------------
//		temp = g_PE_Staue^0xff;// 1
//		Trg = temp & (temp ^ Cont);// 2
//		Cont = temp;// 3
//		
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
		g_LPSStatus = GetPaper();//AD_Value[1]>>4;

		if(g_LPSStatus < g_PE_Standard)
		{
				
			PRINTER_PDL.MOTO._MotoDisable();
			PRINTER_PDL.SYS._Drive_Power_DIS();			
			return;				
		}		
//		MyPrintf("g_PE_Staue: %02X\r\n",g_PE_Staue );
		if(g_PE_Staue == 0)
		{
			//设置检测到纸缝标志
			m_SpaceStatus = 0xA500;
		}
		m_SpaceStatus |= g_PE_Staue;	
		
		//SendChar(g_PE_Staue);
		runcount = 0;
		if(m_SpaceStatus == 0xA501)
		{
			m_SpaceStatus = 0;
			//runcount = (s_PrintrConfig.m_HotLineTOPD + s_PrintrConfig.m_HotLineTOOUT )*32;
			runcount = (s_PrintrConfig.m_HotLineTOPD + 6 )*32;
			#ifdef RD_D20
			runcount = s_PrintrConfig.m_HotLineTOPD * 32  + s_PrintrConfig.m_HotLineTOOUT*32 -32;//  - 8- 4;//监测到纸缝重新修正走到加热线的步数、
			#endif
			break;
		}
	}
	
	i = g_DetPV_H - g_DetPV_L;

	i = i/10;
	m_PE_LabValue_H =  g_DetPV_L + (i * 5);//data_H- (i *5);//5

	m_PE_LabValue_L = g_DetPV_L;//

	if(m_PE_LabValue_H < (m_PE_LabValue_L+30))
	{
		//连续纸
		g_PaperType = 1;
		s_PrintrConfig.m_InterFace = 1;
		SavePrinterConfig();	
	}
	else
	{
		g_PaperType = 0;
		s_PrintrConfig.m_InterFace = 0;
	}		
	
	
	
	
	
	
	StepTime = 950;
	for(;runcount>0;runcount--)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		g_MotoAll++;
		SetMotoPWM(PrintMotoHalfStep[g_MotoAll  & 7],300);
		g_Tim3ProFlag = 0;
	//	GetNoPaperStatus();
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
			
		}		
		//--纸缝再次识别

		
		
	}
//		SavePrinterConfig();
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();		


}








void LocationLabPaper(unsigned short mdettolin,unsigned short mlintoout)
{	
	unsigned short temp,i,j,k,count,runcount,m_SpaceStatus,tempAD;
	unsigned short StepTime;
	unsigned short RunstarStepTime;
	unsigned char m_bufer[48];
//	SET_H_VCC_CON;//关闭充电
	g_DetPaperValueFlag = 0xA5EF;
	g_DetPV_Count = 0;
	g_DetPV_H = 0;
	m_SpaceStatus = 0;
	PRINTER_PDL.SYS._Drive_Power_EN();

	RunstarStepTime = 3000;
	PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],500);
	g_Tim3ProFlag = 0;
	PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(RunstarStepTime);
	while(g_Tim3ProFlag == 0)
	{
	}
	g_PE_Staue = 1;
	RunstarStepTime = 3000;
	g_PE_DN_Count = 0;
	j = 0;
	k = 0;
	
	
	memset(m_bufer,0x33,48);

	count = 4800;
	while(count--)
	{

		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		g_MotoAll++;
		PRINTER_PDL.MOTO._SetMotoOutputPWM(PrintMotoHalfStep[g_MotoAll  & 7],400);
		
		if(RunstarStepTime > (j * 100))
		{
			StepTime = RunstarStepTime - j * 100;
		}
		else
		{
			StepTime = 0;
		}		
		if(StepTime < 550)
		{
			StepTime = 550;
		}
		g_Tim3ProFlag = 0;
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
	
		}
		k++;
		j++;
		//g_PaperOutAD = PRINTER_PDL.ADC._GetRefPhotoelectric();
			g_PaperGapAD = PRINTER_PDL.SYS._PaperGapDet();
		
		
			tempAD =  GetDETPaper();//得到光电采样值
		
		
//		PRINTER_PDL.TTL._SendChar(tempAD&0xFF);
//		PRINTER_PDL.TTL._SendChar(tempAD >> 8);
		if(g_DetPaperValueFlag == 0xA5EF)
		{

			g_DetPV_Count++;
			if(g_DetPV_H < tempAD)
			{
				g_DetPV_H = tempAD;
			}
			if(g_DetPV_Count > 32*10)
			{
				//s_PrintrConfig.m_PE_LabValue_H = g_DetPV_H - (g_DetPV_H /10);
				g_DetPaperValueFlag = 0;
				g_DetPV_Count = 0;
			}
			g_PE_UP_Count = 0;
			g_PE_DN_Count = 0;
			g_PE_Staue = 1;					
		}			
	
		
		FitWave();
		//-------------------------------处理标签位置----------------------------------------------------------
//		temp = g_PE_Staue^0xff;// 1
//		Trg = temp & (temp ^ Cont);// 2
//		Cont = temp;// 3
//		
		g_PaperOutAD = PRINTER_PDL.SYS._PaperOutDet();
		g_LPSStatus = GetPaper();//AD_Value[1]>>4;

		if(g_LPSStatus < g_PE_Standard)
		{
				
			PRINTER_PDL.MOTO._MotoDisable();
			PRINTER_PDL.SYS._Drive_Power_DIS();			
			return;				
		}		
//		MyPrintf("g_PE_Staue: %02X\r\n",g_PE_Staue );
		if(g_PE_Staue == 0)
		{
			//设置检测到纸缝标志
			m_SpaceStatus = 0xA500;
		}
		m_SpaceStatus |= g_PE_Staue;	
		
		//SendChar(g_PE_Staue);
		runcount = 0;
		if(m_SpaceStatus == 0xA501)
		{
			m_SpaceStatus = 0;
			
			PRINTER_PDL.SDTH._SendToHead_u8(m_bufer,48);
			PRINTER_PDL.SYS._LAT_H();
			//*********************************************
			PRINTER_PDL.SYS._LAT_L();
			PRINTER_PDL.SYS._LAT_H();				
			PRINTER_PDL.SYS._STB_A_EN();
	//		PRINTER_PDL.SYS._STB_B_EN();		
			count=2;
		}
	}
	PRINTER_PDL.SYS._STB_A_DIS();
	PRINTER_PDL.SYS._STB_B_DIS();			
	StepTime = 550;
	runcount = mlintoout;
	for(;runcount>0;runcount--)
	{
		PRINTER_PDL.SYS._WDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		g_MotoAll++;
		SetMotoPWM(PrintMotoHalfStep[g_MotoAll  & 7],300);
		g_Tim3ProFlag = 0;
	//	GetNoPaperStatus();
		PRINTER_PDL.SERVICE._ServiceIRQ_A_EN(StepTime);
		while(g_Tim3ProFlag == 0)
		{
			
		}		
		//--纸缝再次识别

		
		
	}
//		SavePrinterConfig();
	PRINTER_PDL.MOTO._MotoDisable();
	PRINTER_PDL.SYS._Drive_Power_DIS();		


}






















