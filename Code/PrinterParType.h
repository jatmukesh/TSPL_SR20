#ifndef PrinterParType_h
#define PrinterParType_h

#include "macros.h"
#include "yc3121.h"

#define U8	unsigned  char
#define U16	unsigned  short
#define U32	unsigned  int
//***********打印头参数结构******************************************************************
#ifdef PRINTER_58MM
#define PRINT_HEAD_LENGTH			384
#define BUFFER_SHORT_LENGTH			24
#endif

#ifdef PRINTER_80MM
#define PRINT_HEAD_LENGTH			576
#define BUFFER_SHORT_LENGTH			36
#endif

	
#define  BAR_CHAR_SPACE  4

/*
//8v 以上100%
//7.5v 以上75%
//7.3v y以上50%
//7.1v y以上25%

3.95  7.9 以上 百分之66% -- 100% 显示3格
3.74  7.48 以上  百分之33% -- 66%  显示 2格
3.62  7.24 以上 百分之0% ---- 33%  显示 1格
3.62  7.24 以下 闪烁提示
3.44  6.88 以下 自动关机

*/
#define		BATTERY_POWER_100		8000
#define		BATTERY_POWER_75		7500
#define		BATTERY_POWER_50		7300
#define		BATTERY_POWER_25		7100
//#define		BATTERY_POWER_00		6900
#define		BATTERY_POWER_00		6500

typedef struct _StbProDataCon
{
	unsigned short	m_SoftwarePart;//当前系统的分区方式
	unsigned short	m_StandbyPar;//得到当前数据的分区方式
	//---
	unsigned short	m_ProCurPar;//处理第几分区
	unsigned short	m_RunStatus;//当前的处理状态
	unsigned short	m_StatusFlag;//状态处理标志
	unsigned short	m_MarkOffset;//掩码位置偏移量
	unsigned short	m_StbHot;//加热时长
	const unsigned short * p_GetParSTB;//
}StbProDataCon;

//***********SSC发送数据结构******************************************************************
typedef struct _DataCon
{
	unsigned int	m_data;//余留的数据
	unsigned int	m_len;//余留数据的长度
}DataCon,*p_DataCon;

//***********条码参数结构******************************************************************
typedef struct _CodeBarCon
{
	unsigned short	m_BarHigh;//条码高度
	unsigned short	m_Barwide;//条码宽度
	unsigned short	m_CharSpace;//打印字符位置
	unsigned short	m_BarSpace;//条码位置 = 0不打印条 =1打印条码在上面 =2 打印条码在下面

}CodeBarCon,*p_CodeBarCon;
//***********打印机配置结构******************************************************************
/*
切换为 24点阵机器 GBK
1B 81 06 02 59 98 03 AA AA 04 5F 3A 05 50 09 07 00 01 08 00 01 0D
切换为 24点阵机器
1B 81 05 03 AA AA 04 5F 3A 05 50 09 07 00 01 08 00 01 0D
切换为 16点阵机器
1B 81 06 03 55 55 04 50 06 05 50 00 07 00 02 08 00 02 02 58 97 0D
设置为正向
1B BB 53 AA 0D 1B 81 01 06 55 12 0D
设置为反向
1B 81 01 06 12 55 0D
设置加热深度
1B 81 01 0D 80 60 0D
恢复打印机默认值
1B BB 53 AA 0D
//设置接收缓冲区在flash中
1B 81 01 17 AA AA 0D
//设置接收缓冲区在RAM中
1B 81 01 17 55 55 0D


1B 81 01 0C 00 04 0D

0D的作用主要是用来表示命令处理完成
RD_W2C1PRINTER -- W2C1
RD_MICROPRINTER -- 通用系列机型 包含 DP DG
RD_OEMPRINTER  -- OEM系列机型 包含 EH OEM57V6
RD_EXPRINTER -- EX系列机型 EF ET

RD_TYPE_SERIAL -- 串口系列机型
RD_TYPE_PARALLEL -- 并口系列机型

PRINTER_GB2312 ----- 定义打印机字库为国标一二级
PRINTER_GBK    ----- 定义打印机字库为GBK
PRINTER_CHINESE_24-- 定义打印机为24点阵机型
PRINTER_CHINESE_16-- 定义打印机为16点阵机型

PRINTER_CHINESE -- 定义打印机为中文文机型  打印机默认此种机型
PRINTER_ENGLISH -- 定义打印机为英文机型


PRINTER_LABEL -- 标签打印机型

PRINTER_BUFF_RAM	-- 打印机接收缓冲区在RAM中
PRINTER_BUFF_FLASH	-- 打印机接收缓冲区在FLASH中


*/
#define CONFIG_PAR_LENGTH	30
typedef struct _PrintrConfig
{
	
	unsigned short	m_CRC;//
	unsigned short	m_InterFace;//打印机进入打印入口标志0

	unsigned short	m_CharSpace;//字间距1
	unsigned short	m_ChineseLib;//汉字字库类型 GBK GB2312  2
	unsigned short	m_ChineseFont;//汉字字体类型3
	unsigned short	m_DefaultChar;//默认打印字符4
	unsigned short	m_DefaultChinese;//默认汉字5
	unsigned short	m_PrintDir;//打印方向6
	unsigned short	m_HoriZoomOut;//横向放大倍数7
	unsigned short	m_VertZoomOut;//纵向放大倍数8
	unsigned short	m_Liftpos;//打印机左限9
	unsigned short	m_Righttpos;//打印机右限10 A
	unsigned short	m_BaudRate;//打印机串口机型默认波特率11 B


	unsigned short	m_SoftwarePart;//软件分区12 C
	unsigned short	m_HotDeep;//加热深度13 D
	unsigned short	m_PrintSub;//打印细分度14 E

	//走4部为一个点行,8点为1mm,走1毫米需要32步
	unsigned short 	m_HotLineTOPD;//15 F 加热线到缺纸检测的距离 以mm为单位 8mm
	unsigned short 	m_HotLineTOOUT;//16 10 加热线到出纸口的距离 以mm为单位 5mm
	unsigned short	m_PrintLabFlage;//17 11 标签打印标志
	unsigned short	m_LabelWide;//18 12标签纸宽度 以mm为单位 56mm
	unsigned short	m_LabelLength;//19 13标签纸长度 以mm为单位 30mm
	unsigned short	m_BarPaGap;//20 14 标签纸缝
	unsigned short	m_PE_LabValue_L;//21 15曲线上升趋势阀值
	unsigned short	m_PE_LabValue_H;//22 16曲线下降趋势阀值
	unsigned short	m_ReceiveBuffrWor;//23 17设置打印机缓冲区标志
	unsigned short	m_LineSpace;//24 18设置打印默认行间距
	unsigned short	m_CharType;//25 19 机器打印类型,汉字机型或 英文机型
	unsigned short	m_PrintEndFeed;//26 1A 打印完成后向前走纸
	unsigned short	m_PrintEnCode;//27 加密
	unsigned short	m_Delayms_Head;//28 1C延时等待
	unsigned short	m_Delayms_End;//29 1D 延时等待	
	unsigned short	m_Density;//浓度
	unsigned short	m_SaveConPar[10];//菜单配置
//1B 81 02 13 01 E0 11 A5 A5 0D

//	unsigned short	m_PrinterAddr;//打印机地址
//	unsigned short	m_ProtocolCount;//协议个数
//	unsigned short	m_Protocoldata[20];//协议个数

//	unsigned short	m_SelfTestCount;//自检打印个数
//	unsigned short	m_SelfTestAddr_H;//自检起始地址高16位
//	unsigned short	m_SelfTestAddr_L;//自检起始地址低16位
#ifdef AUTO_SHUTDOWN_INDICATION_HANDLING
	/* Flash-persisted only after an automatic low-battery shutdown. */
	unsigned short	 m_LowChargeShutdownLatch;
#endif

}PrintrConfig,*p_PrintrConfig;

//***********电机运行参数结构******************************************************************

typedef struct _PrintControl
{
	unsigned short	m_InFirstPrint;//打印机进入打印入口标志
	unsigned short	m_In_printing;//正在打印
	unsigned short	m_MutualFlag;//链表切换完成标志
	unsigned short	m_PrintLSbit;//行间距空走纸标志
	unsigned short	m_PrintLSbit_f;//行间距空走纸标志
	unsigned short	m_StopPrint;//停止打印标志
	unsigned short	m_MotoRun;//电机运行标志
	unsigned short	m_PrintEnd;//打印结束标志
	unsigned short	m_MotoStep;//电机走步计数器

}PrintControl,*p_PrintControl;

//**********打印机参数结构******************************************************************
typedef struct _PrinterStruct
{
	unsigned short	m_ReceiveBuffrWor;//接收数据buffer 判断标志
	unsigned short	m_InterFace;//打印机进入打印入口标志0
	unsigned short	m_positions;//打印头点数
	unsigned short	m_Score_NOR;//上下侧划线
	unsigned short	m_ChiRepertoire;//选择16x16还是12x12汉字集
	unsigned short	m_PrHeadHottime;//打印头加热时间
	unsigned short	m_CurMotoRunTime;//根据电压得到当前电机运行时间
	unsigned short	m_CurMotoPWM;//得到当前电压电机PWM值
	unsigned short	m_CurFreeTime;//得到当前电压空走纸步进时间
	unsigned short	m_VolTemAdd;
	unsigned short	m_VolTemSubb;

	unsigned short	m_Prstate;//打印机状态 在打印中/打印完成/打印机缺纸/打印机异常
	unsigned short	m_PageLength;//打印页长
	unsigned short	m_Pagecount;//打印页长计数器
	unsigned short	m_PrintDir;//打印方向,横向打印纵向打印
	unsigned short	m_Liftpos;//打印机左限
	unsigned short	m_Righttpos;//打印机右限
	unsigned short	m_LineSpace;//打印机行间距
	unsigned short	m_CharSpace;//打印机字间距
	unsigned short	m_HoriZoomOut;//横向放大倍数
	unsigned short	m_VertZoomOut;//纵向放大倍数
	unsigned short	m_RotDegree;//字符旋转角度
	unsigned short	m_CharRepertoire;//字符集选项 0 - 15 字符集选择
	unsigned short	m_CharType;//字符类型汉字 字符选择
	unsigned short	m_ChineseCharType;//字符类型汉字 字符选择
	unsigned short	m_boldface;//加粗

	unsigned short	m_Charlist;//小字符集表
	unsigned short	m_AlignUPDown;//上下对齐 最高位表示上下对齐.1下对齐 0上对齐
	unsigned short	m_alignType;//左右中 对齐方式
	unsigned short	m_Inverse;//反白
	unsigned short	m_TabCount;//水平造表计数器
	unsigned short	m_TabPos[20];//水平造表值表
	unsigned short	m_QRZoom;

} PrinterStruct, *p_PrinterStruct;

//***********命令结构******************************************************************
typedef struct _GetOrderPoint
{
	unsigned char m_GetdataPos;//得到命令位置标志
	unsigned char m_DataFlag;
	unsigned char *pSpecialposition;//得到命令位置

}GetOrderPoint,*p_GetOrderPoint;

//***********数据链表参数结构******************************************************************
#ifdef PRINTER_58MM
#ifndef ENCODE_ENABLE
typedef struct _PrintListPar
{
	unsigned short m_TMIndex;
	unsigned short m_PrinterDip;//打印解析度
	unsigned short m_PrintLineHigh;//打印一行的最大高度
	unsigned short m_PrintLineSpace;//打印一行的行间距
	unsigned short m_PrintDir;//打印方向
	unsigned short m_MotoRunDir;//电机运行方向
	unsigned short m_LableFeedFlag;//标签送纸
	unsigned short m_LableStopFlag;//标签停止标志
	unsigned short m_PrintStartPos;//打印取数据起始位置
	unsigned short m_HotTime;//打印加热时间
	unsigned short m_FeedTime;//调节走步
	unsigned short m_MotoRunTime;//打印步进电机运行时间
	unsigned short m_LeftFreeDot;//左边空点
	unsigned short m_RightFreeDot;//右边空点
	unsigned short m_ThType;//加热类型
	unsigned short m_ListIndex[55];//打印位置索引
	unsigned short m_ListBuffer[970];//数据存放位置[970]
	unsigned short m_LineCharCount;//每行字符计数器
	unsigned short m_LineProArry[BUFFER_SHORT_LENGTH];
}PrintListPar,*p_PrintListPar;
#endif
#ifdef ENCODE_ENABLE
typedef struct _PrintListPar
{
	unsigned short m_TMIndex;
	unsigned short m_PrinterDip;//打印解析度
	unsigned short m_PrintLineHigh;//打印一行的最大高度
	unsigned short m_PrintLineSpace;//打印一行的行间距
	unsigned short m_PrintDir;//打印方向
	unsigned short m_MotoRunDir;//电机运行方向
	unsigned short m_LableFeedFlag;//标签送纸
	unsigned short m_LableStopFlag;//标签停止标志
	unsigned short m_PrintStartPos;//打印取数据起始位置
	unsigned short m_HotTime;//打印加热时间
	unsigned short m_FeedTime;//调节走步
	unsigned short m_MotoRunTime;//打印步进电机运行时间
	unsigned short m_LeftFreeDot;//左边空点
	unsigned short m_RightFreeDot;//右边空点
	unsigned short m_ThType;//加热类型
	unsigned short m_ListIndex[100];//打印位置索引
	unsigned short m_ListBuffer[2100];//数据存放位置[970]
	unsigned short m_LineCharCount;//每行字符计数器
	unsigned short m_LineProArry[BUFFER_SHORT_LENGTH];
}PrintListPar,*p_PrintListPar;
#endif

#endif

#ifdef PRINTER_80MM
typedef struct _PrintListPar
{
	unsigned short m_TMIndex;
	unsigned short m_PrinterDip;//打印解析度
	unsigned short m_PrintLineHigh;//打印一行的最大高度
	unsigned short m_PrintLineSpace;//打印一行的行间距
	unsigned short m_PrintDir;//打印方向
	unsigned short m_MotoRunDir;//电机运行方向
	unsigned short m_LableFeedFlag;//标签送纸
	unsigned short m_LableStopFlag;//标签停止标志
	unsigned short m_PrintStartPos;//打印取数据起始位置
	unsigned short m_HotTime;//打印加热时间
	unsigned short m_MotoRunTime;//打印步进电机运行时间
	unsigned short m_LeftFreeDot;//左边空点
	unsigned short m_RightFreeDot;//右边空点
	unsigned short m_ThType;//加热类型
	unsigned short m_ListIndex[100];//打印位置索引
	unsigned short m_ListBuffer[2100];//数据存放位置[970]
	unsigned short m_LineCharCount;//每行字符计数器
	unsigned short m_LineProArry[BUFFER_SHORT_LENGTH];
}PrintListPar,*p_PrintListPar;
#endif

//************全局常量*****************************************************************

#define TYPE_UNICODE_ENGLISH		0x5145//打印汉字
#define TYPE_ENGLISH		0x5155//打印汉字
#define TYPE_LITTLECHAR		0x55A1//打印汉字

#define TYPE_CHINESE		0x5A55//打印字符


#define TYPE_GB2312			0x5897//打印字符
#define TYPE_GBK			0x5998//打印字符
//-----------------------------------------

#define PRINT_CHAR		0xA123//打印字符和汉字
#define PRINT_MAP		0x1A23//打印图形
#define PRINT_CURVE		0x12A3//打印曲线

#define PRINT_DATA		0x123A//打印数据
#define PRINT_SPACE		0x3A12//打印数据


#define RECEIVE_FLASH		0xAAAA//
#define RECEIVE_RAM 		0x5555//

#define	LABEL_PR_EN			0xA5A5
#define	LABEL_PR_DIS		0
#define MOTO_CLOCKWISE		0xAAAA
#define MOTO_REVERSE		0x5555


#define PRINT_START		0xAAAA//打印开始

#define PRINT_END		0xFFFF//打印结束

#define LIST_END		0xFFFF//链表结束

#define VALUE_ENABLE			0xAAAA//
#define VALUE_DISABLE			0//
//---------------------------------------------
#define DEFAULT_PRINT_STB_TIME		4300//3800默认的打印头加热时间
#define FIRST_PRINT_STB_TIME		1000//
#define PRINT_DIRECT_NOR			0x5512
#define PRINT_DIRECT_FX				0x1255
#define ENABLE_VAR					0x55AA
#define DISABLE_VAR					0xAA55
#define FINISH_PROCESS				0x5555
#define PRASE_PROCESS				0xA515
#define DATA_IN_SEND				0x5A67
#define DATA_SEND_OK				0x57A9

#define	PROCESS_FINISH			0xAA3A//打印一行
#define	PROCESS_CONTINUE		0xAAA3//接续解析命令
#define	PROCESS_WAITORDER		0xA3AA//等待数据
#define	PROCESS_LOOP			0x3AAA//等待数据
#define	PROCESS_START			0x55A3
#define	PROCESS_IN_LOOP			0x55A5


#define	INTERFACE_SERIAL		0xAA34//串行接口
#define	INTERFACE_PARALLEL		0xAA01//并行接口

//************全局变量*****************************************************************
#define SETHEAD_ADDR		0
#define SETEND_ADDR			0x2000

#define Chi16X16_ADDR		0x10000//打印字符
#define Chi12X12_ADDR		0x52600//打印字符

#define Chi24X24_ADDR		0x90000//打印字符

#define ASC8X16_ADDR		0x51800//打印字符

#define ASC8X12_ADDR		0x76600//打印字符
#define ASC12X24_ADDR		0x10D940//打印字符


#define TYPE_ASC12X24		0x5F3A//打印字符

#define DL_PRINTER_PAR_ADDR		0x1AD000//存储打印机参数


#define TYPE_CHIN16X16		0x5000//打印字符

#define TYPE_CHIN8X16		0x5001//打印字符
#define TYPE_CHIN16X8		0x5002//打印字符
#define TYPE_CHIN8X8		0x5003//打印字符

#define TYPE_CHIN12X12		0x5004//打印字符
#define TYPE_CHIN6X12		0x5005//打印字符

#define TYPE_ASC8X16		0x5006//打印字符
#define TYPE_ASC8X12		0x5007//打印字符

#define TYPE_ASC9X17		0x500A//打印字符
#define TYPE_CHIN24X24		0x5009//打印字符


#define PRINTTYPE_24		0xAAAA//24点阵汉字机型
#define PRINTTYPE_16		0x5555//16点阵汉字机型




#define	PROCESS_FINISH			0xAA3A//打印一行
#define	PROCESS_CONTINUE		0xAAA3//接续解析命令
#define	PROCESS_WAITORDER		0xA3AA//等待数据

//*****************************************************************************
#define LF_D			0x00A//1	*打印并换行
#define CR_D			0x00D//1	*打印并换行
#define HT			0x09//1	*水平造表
#define SO			0x0E//1	*双宽度打印
#define DC4			0x14//1	*删除双宽度打印
#define FF			0x0C//1	*换页

//***************计算参数表**************************************************************

#define PRINTER_RECEIVE_L		1024*7

#define BUFFERSIZE				1024*8
#define BUUFER_MAX_LEN	BUFFERSIZE-1






#define MASK_DATA	BUUFER_MAX_LEN//2K数据缓冲区
#define ADDRPRO_OR	0x1B0000
#define ADDRPRO_AND	  0xFFFF
void FeedLabPaper(void);


#ifdef POWER_ON_CAL

typedef enum{
    Cal_Mode_Auto = 0,
		Cal_Mode_Manual = 1
}Cal_Mode_t;


extern unsigned char Power_ON_CAL_Status;
extern Cal_Mode_t Calibration_Mode;
extern unsigned int g_CAL_Mode_statusFlag;
extern unsigned int g_boundary_gap_counter;
extern unsigned int g_label_size_counter;

unsigned char PowerON_Calibration(void);

#define LID_DETECTION    256
#define MEDIA_ABSENT     200

void Label_Size_Calculation(uint16_t Boundary_Count);


#endif



#endif
