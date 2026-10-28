/****************************************************************************************
* 文件名：PDL.C
* 功能：打印机驱动链路层。主要用于连接底层BSP和打印机应用程序。
*       打印机所有调用的硬件方法均出于此处，所有硬件的操作不允许在应用程序中调用
* 日期：2021.06.13
* 软件版本：V1.0
****************************************************************************************/
#ifndef __PDL__H__123
#define __PDL__H__123

#include "RD_V2_DeviceBSP.h"
#include <stdio.h>

#include "cortexm3_macro.h"

#ifdef MOTO_STEP_FX

#define MOTO_RUN_HALF_8_CODR			(O_MOTO1_PIN|O_MOTO4_PIN)
#define MOTO_RUN_HALF_7_CODR			(O_MOTO4_PIN)
#define MOTO_RUN_HALF_6_CODR			(O_MOTO2_PIN|O_MOTO4_PIN)
#define MOTO_RUN_HALF_5_CODR			(O_MOTO2_PIN)
#define MOTO_RUN_HALF_4_CODR			(O_MOTO2_PIN|O_MOTO3_PIN)
#define MOTO_RUN_HALF_3_CODR			(O_MOTO3_PIN)
#define MOTO_RUN_HALF_2_CODR			(O_MOTO1_PIN|O_MOTO3_PIN)
#define MOTO_RUN_HALF_1_CODR			(O_MOTO1_PIN)

#endif

#ifndef MOTO_STEP_FX

#define MOTO_RUN_HALF_1_CODR			(O_MOTO1_PIN|O_MOTO4_PIN)
#define MOTO_RUN_HALF_2_CODR			(O_MOTO4_PIN)
#define MOTO_RUN_HALF_3_CODR			(O_MOTO2_PIN|O_MOTO4_PIN)
#define MOTO_RUN_HALF_4_CODR			(O_MOTO2_PIN)
#define MOTO_RUN_HALF_5_CODR			(O_MOTO2_PIN|O_MOTO3_PIN)
#define MOTO_RUN_HALF_6_CODR			(O_MOTO3_PIN)
#define MOTO_RUN_HALF_7_CODR			(O_MOTO1_PIN|O_MOTO3_PIN)
#define MOTO_RUN_HALF_8_CODR			(O_MOTO1_PIN)

#endif

typedef struct __PDL
{
		struct _SYS
		{
			void (*_SystemInit)  (void);// 系统初始化,看门狗 ，中断，内核功能
			void (*_SystemIOInit)  (void);// 系统io初始化

			void (*_SystemPrintIRQInit)  (void);//打印中断初始化
			void (*_SystemPrintLineDatInit)  (void);//处理一行打印数据中断初始化
			void (*_SystemSysTickInit)  (void);//系统定时器初始化

			void (*_SystemPrintIRQ_EN)  (unsigned short mTimeLen);//打印中断初使能
			void (*_SystemPrintIRQ_DIS)  (void);//打印中断禁止
			
			void (*_SystemPrintLineDat_EN)  (unsigned short mTimeLen);//打印一行数据中断使能
			void (*_SystemPrintLineDat_DIS)  (void);//打印一行数据中断禁止
	
			void (*_SystemPrintIRQHandler)  (void);//打印中断处理
			void (*_SystemPrintLineDatIRQHandler)  (void);//打印一行数据中断处理
			void (*_SystemSysTickIRQHandler)  (void);//系统定时器处理
			
			//----------------------- 
			void (*_LAT_L) (void);//锁存信号低 
			void (*_LAT_H) (void);//锁存信号高
			//----------------------- 
			void (*_STB_ALL_CLEAR) (void);//所有加热线清除加热状态 
			void (*_STB_ALL_SET) (void); //所有加热线设置加热状态 

			void (*_STB_A_EN) (void);// 
			void (*_STB_A_DIS) (void);// 

			void (*_STB_B_EN) (void);// 
			void (*_STB_B_DIS) (void);// 

			void (*_Charge_LED1_ON) (void);// 
			void (*_Charge_LED1_OFF) (void);// 

			void (*_Charge_LED2_ON) (void);// 
			void (*_Charge_LED2_OFF) (void);// 

			void (*_Charge_LED3_ON) (void);// 
			void (*_Charge_LED3_OFF) (void);// 

			void (*_BEEP_EN) (void);// 
			void (*_BEEP_DIS) (void);//  

			void (*_LED_Power_ON) (void);// 状态指示灯
			void (*_LED_Power_OFF) (void);//  
			void (*_LED_Power_Trig) (void);// 
			unsigned int (*_GET_LED_Power) (void);// 


			void (*_LED_Stauts_ON) (void);// 状态指示灯
			void (*_LED_Stauts_OFF) (void);//  
			void (*_LED_Stauts_Trig) (void);// 
			unsigned int (*_GET_LED_Stauts) (void);// 
			
			
			void (*_LED_Charge_ON) (void);// 充电指示灯
			void (*_LED_Charge_OFF) (void);//  
			void (*_LED_Charge_Trig) (void);//  
			unsigned int (*_GET_Charge_LED1) (void);// 
			
			void (*_LED_WIFI_Link_ON) (void);// 
			void (*_LED_WIFI_Link_OFF) (void);//  
			void (*_LED_WIFI_Link_Trig) (void);//  
			unsigned int (*_GET_LED_WIFI_Link) (void);// 

			void (*_LED_BT_Link_ON) (void);// 
			void (*_LED_BT_Link_OFF) (void);//  
			void (*_LED_BT_Link_Trig) (void);//  
			unsigned int (*_GET_LED_BT_Link) (void);// 
			
			//-------------------
	
			// 拨码开关状态需要做定义
			
			unsigned int (*_GetDIPswitchStatus) (void);// 得到拨码开关状态
			
			void (*_POWER_EN)  (void);//总电源使能
			void (*_POWER_DIS) (void);//总电源不使能			
			
			void (*_PeripheralVcc_EN)  (void);//外围电源使能
			void (*_PeripheralVcc_DIS) (void);//外围电源不使能			
			
			
			
			unsigned int (*_GetKey_POW) (void);//电源开关 
			unsigned int (*_GetKey_LF) (void);// 走纸开关
			unsigned int (*_GetKey_SEL) (void);// 自检开关

			unsigned int (*_Get_Plus_POW) (void);//得到按键次数脉冲
			unsigned int (*_Get_Plus_LF) (void);// 得到按键次数脉冲
			unsigned int (*_Get_Plus_SEL) (void);//得到按键次数脉冲



			void (*_Drive_Power_EN) (void);// 驱动电源输出
			void (*_Drive_Power_DIS) (void);// 驱动禁止输出
			void (*_Drive_Power_Trig) (void);// 驱动控制输出脉冲

			void (*_WDG_ReloadCounter) (void);// 看门狗复位
			//----------------------- 
			unsigned int (*_GetHSStatus) (void);// 得到关门开关状态
			unsigned int (*_GetChargeNCHRG) (void);// 得到充电状态
			
			unsigned int (*_GetChargeNSTDBY) (void);// 得到脉冲充电状态
			
			void (*_Charge_EN) (void);// 充电使能
			void (*_Charge_DIS) (void);//充电禁止

			//----------------------- 
			unsigned int (*_GetCurVoltageLevel) (void);// 得当前电压索引等级
			unsigned int (*_GetCurTemperatureLevel) (void);// 得到当前温度索引等级

			void (*_SavePrinterConfig) (unsigned char * pbuf,unsigned short mplen);// 保存打印机参数
			void (*_ReadPrinterConfig) (unsigned char * pbuf,unsigned short mplen);// 读取打印机参数


			void (*_DelayNus) (unsigned short mTimeLen);// 
			
			
			//----------------------- 
			unsigned int (*_PaperOutDet) (void);// 缺纸检测
			unsigned int (*_PaperGapDet) (void);// 纸缝检测
			
		}SYS;
		struct _MOTO
		{
			void (*_Init_Fun) (void);//电机功能初始化 
			void (*_MotoDisable) (void);//关电机输出 
			
			void (*_SetMotoOutputPWM) (unsigned short mMOtoIndex,unsigned short mPwmValue);//PWM电机输出 
			void (*_SetMotoOutputIO) (unsigned short mMOtoIndex);//IO电机输出 
			
			void (*_MotoRun_FX) (unsigned short * p_g_MotoAll,unsigned short mRunStep);//电机反转
			void (*_MotoRun_RW) (unsigned short * p_g_MotoAll,unsigned short mRunStep);//电机正转
			
		}MOTO;
		
		struct _PARALLEL
		{	
			//-------BUSY---------------- 
			void (*_Init_Fun) (void);// 
			void (*_SetBusy) (void);// 
			void (*_ClearBusy) (void);// 
			
			void (*_SetACK) (void);// 
			void (*_ClearACK) (void);// 	
			
			void (*_SetPE) (void);// 
			void (*_ClearPE) (void);// 	
			
			unsigned char (*_ReadChar) (void);//得到一个字节数据
			unsigned short (*_EXTInterruptStatus) (void);//是否产生有效接收中断，有中断发生，返回中断状态，并清掉中断
			void (*_EXTInterruptIRQHandler)  (void);//外部中断处理函数
		}PARALLEL;			
		struct _TTL
		{	
			//-------BUSY---------------- 
			void (*_Init_Fun) (unsigned short mSerPar);// 
			unsigned int (*_GetBusy) (void);// 
			void (*_SetBusy) (void);// 
			void (*_ClearBusy) (void);// 
			void (*_WriteChar) (unsigned char mSdat);//只写一个字节数据到缓冲区，不判断状态
			void (*_SendChar) (unsigned char mSdat);//发送一个字节数据
			void (*_SendBuf) (unsigned char *mSBuf,unsigned short mLength);//发送缓冲区
			unsigned char (*_ReceivedChar) (void);//得到一个字节数据
			unsigned int (*_RxInterruptStatus) (void);//是否产生有效接收中断
			unsigned int (*_IsRXFIFONotEmpty ) (void);//中断处理函数
			void (*_RxInterruptIRQHandler)  (void);//中断处理函数
			
		}TTL;			
		struct _RS232
		{	
			//-------BUSY---------------- 
			void (*_Init_Fun) (unsigned short mSerPar);// 
			unsigned int (*_GetBusy) (void);// 
			void (*_SetBusy) (void);// 
			void (*_ClearBusy) (void);// 
			void (*_WriteChar) (unsigned char mSdat);//只写一个字节数据到缓冲区，不判断状态
			void (*_SendChar) (unsigned char mSdat);//发送一个字节数据
			void (*_SendBuf) (unsigned char *mSBuf,unsigned short mLength);//发送缓冲区
			unsigned char (*_ReceivedChar) (void);//得到一个字节数据	
			unsigned int (*_RxInterruptStatus) (void);//是否产生有效接收中断
			unsigned int (*_IsRXFIFONotEmpty ) (void);//中断处理函数
			void (*_RxInterruptIRQHandler)  (void);//中断处理函数
			
		}RS232;		 
		struct _BT
		{	
			void (*_Init_Fun) (unsigned short mSerPar);// 
			void (*_SetBTName) (unsigned char* mNameBuf);// 
			void (*_SetBTKEY) (unsigned char* mNameBuf);// 
			unsigned int (*_GetConnect) (void);// 
			//-------BUSY---------------- 
			unsigned int (*_GetBusy) (void);// 
			void (*_SetBusy) (void);// 
			void (*_ClearBusy) (void);// 
			void (*_WriteChar) (unsigned char mSdat);//只写一个字节数据到缓冲区，不判断状态
			void (*_SendChar) (unsigned char mSdat);//发送一个字节数据
			unsigned char (*_ReceivedChar) (void);//得到一个字节数据		
			unsigned int (*_RxInterruptStatus) (void);//是否产生有效接收中断
			void (*_RxInterruptIRQHandler)  (void);//中断处理函数
			unsigned short (*_ReceiveNBytes)(unsigned char *dataBuffer,unsigned short len);//接收N字节数据到打印机缓冲区
			void (*_ReturnNBytes)(unsigned char *dataBuffer,unsigned short len);//接收N字节数据到打印机缓冲区
			void (*_DisConnect) (void);// 
			
		}BT;
		struct _WIFI
		{	
			void (*_Init_Fun) (unsigned short mSerPar);// 
			//-------BUSY---------------- 
			unsigned int (*_GetBusy) (void);// 
			void (*_SetBusy) (void);// 
			void (*_ClearBusy) (void);//
			void (*_SetRest) (void);// 
			void (*_ClearRest) (void);//
			unsigned int (*_GetLink) (void);// 
			void (*_WriteChar) (unsigned char mSdat);//只写一个字节数据到缓冲区，不判断状态
			void (*_SendChar) (unsigned char mSdat);//发送一个字节数据			
			unsigned char (*_ReceivedChar) (void);//得到一个字节数据		
			unsigned int (*_RxInterruptStatus) (void);//是否产生有效接收中断
			void (*_RxInterruptIRQHandler)  (void);//中断处理函数
		}WIFI;	 
		 
		 
		//ADC采样
		struct _ADC
		{
			void (*_Init_Fun)  (void); //初始化ADC 模块功能
			void (*_HVIN_Init_Fun)  (void); //初始化ADC 模块功能
			void (*_GPIO_Init_Fun)  (unsigned short num) ; //初始化ADC 模块功能
			unsigned short (*_GetDriveVoltage) (void);// 得到驱动电压
			unsigned short (*_GetBattyPer) (void);// 得到电池百分比
			unsigned short (*_GetRefPhotoelectric) (void);// 反射光电值
			unsigned short (*_GetCorPhotoelectric) (void);// 对射光电值
			unsigned short (*_GetPTemperature) (void);// 得到打印头温度
			
			
		}ADC;
		//打印头数据发送
		struct _SDTH
		{
			void (*_Init_Fun)  (void); //初始化打印头数据发送 模块功能
			unsigned short (*_SendToHead_u16) (unsigned short* mSDBuffer,unsigned short mLen);//发送数据到打印头
			unsigned short (*_SendToHead_u32) (unsigned int* mSDBuffer,unsigned short mLen);//发送数据到打印头

			unsigned short (*_DMA_SendToHead_u16) (unsigned short* mSDBuffer,unsigned short mLen);//发送数据到打印头
			unsigned short (*_DMA_SendToHead_u32) (unsigned int* mSDBuffer,unsigned short mLen);//发送数据到打印头
			
			
			unsigned short (*_IS_Send_Empty)  (void); //发送是否空，用于判断数据是否发送完成
			
			void (*_DMA_SendToHead_u8) (unsigned char* mSDBuffer,unsigned short mLen);//发送数据到打印头
			
		}SDTH;

		struct _RFID
		{
			void (*_Init_Fun)  (void); //初始化打印头数据发送 模块功能
			void (*_VCC_EN)  (void);//电源选使能
			void (*_VCC_DIS) (void);//电源不使能

			void (*_CS_EN)  (void);//片选使能
			void (*_CS_DIS) (void);//片选不使能
			
			void (*_SetRest) (void);// 
			void (*_ClearRest) (void);//
			
			unsigned char (*_WriteByte)(unsigned char mRdat);//写入一个字节
			unsigned char (*_ReadByte)(void);//读入一个字节
			
		}RFID;	

		struct _NET
		{
			void (*_Init_Fun)  (void); //初始化打印头数据发送 模块功能
			void (*_CS_EN)  (void);//片选使能
			void (*_CS_DIS) (void);//片选不使能
			
			void (*_SetRest) (void);// 
			void (*_ClearRest) (void);//
			
		
			unsigned char (*_WriteByte)(unsigned char mRdat);//写入一个字节
			unsigned char (*_ReadByte)(void);//读入一个字节
			
		}NET;		
		struct _SPI
		{
			void (*_Init_Fun)  (void); //初始化打印头数据发送 模块功能
			
			void (*_CS_EN)  (void);//片选使能
			void (*_CS_DIS) (void);//片选不使能

			void (*_CS_E1_EN)  (void);//片选使能
			void (*_CS_E1_DIS) (void);//片选不使能
			
			unsigned int (*_SendDat_u32)(unsigned int mSdat);//发送一个int
			unsigned short (*_SendDat_u16)(unsigned short mSdat);//发送一个short
			unsigned char (*_SendDat_u8)(unsigned char mSdat);//发送一个字char
			void (*_SendData)(unsigned char data);
			void (*_SendBuff)(unsigned char *buff, int len);
			void (*_SendAndReceiveData)( unsigned char *TxBuff, unsigned short TxLen, unsigned char *RxBuff, unsigned short RxLen);
			
			
		}SPI;
		struct _USB
		{
			void (*_Init_Fun)  (void); //初始化USB 模块功能
			
			void (*_POWER_EN)  (void); //电源使能OTG HOST
			void (*_POWER_DIS)  (void); //电源不使能OTG HOST
			
			void (*_Connect)  (void); //连接使能
			void (*_DisConnect)  (void); //不连接

			unsigned short (*_GetUSBDet)  (void); //判断是否连接
			
			unsigned short (*_ReceiveNBytes)(unsigned char *dataBuffer,unsigned short len);//接收N字节数据到打印机缓冲区
			unsigned short (*_ReturnNBytes)(unsigned char *dataBuffer,unsigned short len);//返回N字节数据到主机
			void (*_ReceiveEPValid)  (void); //接收端点有效用于使能继续接收数据
			
			void (*_Judge_EN_EPValid)  (void); //用于有效使能，内部判断流控标志
			
		}USB;	
		struct _SERVICE
		{
			void (*_Init_Fun) (void);//服务性中断初始化，基本就是定时器

			void (*_ServiceIRQ_A_EN) (unsigned short TimeCouValue);
			void (*_ServiceIRQ_A_DIS) (void);
			void (*_ServiceIRQ_A_Handler)  (void);//服务中断A处理

			void (*_ServiceIRQ_B_EN) (unsigned short TimeCouValue);
			void (*_ServiceIRQ_B_DIS) (void);
			void (*_ServiceIRQ_B_Handler)  (void);//服务中断B处理

			void (*_ServiceIRQ_C_EN) (unsigned short TimeCouValue);
			void (*_ServiceIRQ_C_DIS) (void);
			void (*_ServiceIRQ_C_Handler)  (void);//服务中断C处理

			void (*_ServiceIRQ_D_EN) (unsigned short TimeCouValue);
			void (*_ServiceIRQ_D_DIS) (void);
			void (*_ServiceIRQ_D_Handler)  (void);//服务中断D处理
			
			
			void (*_PrinterIdle) (void);
			void (*_PrinterBusy) (void);
			
		}SERVICE;		
		struct _IIC
		{
			void (*_RST_L) (void);//
			void (*_RST_H) (void);//			
			void (*_SCL_L) (void);//
			void (*_SCL_H) (void);//
			void (*_SDA_L) (void);//
			void (*_SDA_H) (void);//
			void (*_SDA_InputEn) (void);//
			unsigned short (*SDA_read) (void);//
			
		}IIC;	
}_PDL;


unsigned int RevInt(unsigned int x);
int CalintCount(unsigned int x);
void Transpose32(unsigned int a[32]);















void PDL_SYS_SystemInit (void); 
void PDL_SYS_SystemIOInit (void); 
void PDL_SYS_SystemPrintIRQInit (void);
void PDL_SYS_SystemPrintLineDatInit (void);
void PDL_SYS_SystemSysTickInit (void);
void PDL_SYS_SystemPrintIRQ_EN (unsigned short mTimeLen);
void PDL_SYS_SystemPrintIRQ_DIS (void); 
void PDL_SYS_SystemPrintLineDat_EN (unsigned short mTimeLen); 
void PDL_SYS_SystemPrintLineDat_DIS (void); 
void PDL_SYS_SystemPrintIRQHandler (void); 
void PDL_SYS_SystemPrintLineDatIRQHandler (void); 
void PDL_SYS_SystemSysTickIRQHandler (void); 
void PDL_SYS_LAT_L (void); 
void PDL_SYS_LAT_H (void); 
void PDL_SYS_STB_ALL_CLEAR (void); 
void PDL_SYS_STB_ALL_SET (void); 
void PDL_SYS_STB_A_EN (void); 
void PDL_SYS_STB_A_DIS (void); 
void PDL_SYS_STB_B_EN (void); 
void PDL_SYS_STB_B_DIS (void); 
void PDL_SYS_Charge_LED1_ON (void) ;
void PDL_SYS_Charge_LED1_OFF (void) ;
void PDL_SYS_Charge_LED2_ON (void) ;
void PDL_SYS_Charge_LED2_OFF (void) ;
void PDL_SYS_Charge_LED3_ON (void) ;
void PDL_SYS_Charge_LED3_OFF (void) ;
void PDL_SYS_BEEP_EN (void); 
void PDL_SYS_BEEP_DIS (void); 

void PDL_SYS_LED_Power_ON (void) ;
void PDL_SYS_LED_Power_OFF (void) ;
void PDL_SYS_LED_Power_Trig (void) ;
unsigned int PDL_SYS_GET_LED_Power (void) ;

void PDL_SYS_LED_Stauts_ON (void); 
void PDL_SYS_LED_Stauts_OFF (void); 
void PDL_SYS_LED_Stauts_Trig (void); 
unsigned int PDL_SYS_GET_LED_Stauts (void) ;
void PDL_SYS_LED_Charge_ON (void); 
void PDL_SYS_LED_Charge_OFF (void); 
void PDL_SYS_LED_Charge_Trig (void); 
unsigned int PDL_SYS_GET_GET_Charge_LED1 (void) ;
void PDL_SYS_LED_WIFI_Link_ON (void); 
void PDL_SYS_LED_WIFI_Link_OFF (void); 
void PDL_SYS_LED_WIFI_Link_Trig (void); 
unsigned int PDL_SYS_GET_LED_WIFI_Link (void) ;
void PDL_SYS_LED_BT_Link_ON (void);
void PDL_SYS_LED_BT_Link_OFF (void); 
void PDL_SYS_LED_BT_Link_Trig (void); 
unsigned int PDL_SYS_GET_LED_BT_Link (void) ;
unsigned int PDL_SYS_GetDIPswitchStatus (void); 
void PDL_SYS_POWER_EN (void); 
void PDL_SYS_POWER_DIS (void); 
void PDL_SYS_PeripheralVcc_EN (void); 
void PDL_SYS_PeripheralVcc_DIS (void); 
unsigned int PDL_SYS_GetKey_POW (void); 
unsigned int PDL_SYS_GetKey_LF (void); 
unsigned int PDL_SYS_GetKey_SEL (void); 
unsigned int PDL_SYS_Get_Plus_POW (void); 
unsigned int PDL_SYS_Get_Plus_LF (void);
unsigned int PDL_SYS_Get_Plus_SEL (void); 
void PDL_SYS_Drive_Power_EN (void); 
void PDL_SYS_Drive_Power_DIS (void); 
void PDL_SYS_Drive_Power_Trig (void); 
void PDL_SYS_WDG_ReloadCounter (void); 
unsigned int PDL_SYS_GetHSStatus (void) ;
unsigned int PDL_SYS_GetChargeNCHRG (void) ;
unsigned int PDL_SYS_GetChargeNSTDBY(void) ;
void PDL_SYS_Charge_EN (void) ;
void PDL_SYS_Charge_DIS (void) ;
unsigned int PDL_SYS_GetCurVoltageLevel (void) ;
unsigned int PDL_SYS_GetCurTemperatureLevel (void) ;
void PDL_SYS_SavePrinterConfig (unsigned char * pbuf,unsigned short mplen);
void PDL_SYS_ReadPrinterConfig (unsigned char * pbuf,unsigned short mplen) ;
void PDL_SYS_DelayNus(unsigned short mTimeLen);
unsigned int PDL_SYS_PaperOutDet(void);
unsigned int PDL_SYS_PaperGapDet(void);


void PDL_MOTO_Init_Fun (void); 
void PDL_MOTO_MotoDisable (void); 
void PDL_MOTO_SetMotoTimeOutFalg (unsigned short TimeOutFalg); 
void PDL_MOTO_Moto_Timer_EN (unsigned short mTimeCount); 
unsigned int PDL_MOTO_MotoTimeOutFalg (void); 
void PDL_MOTO_SetMotoOutputPWM (unsigned short mMOtoIndex,unsigned short mPwmValue); 
void PDL_MOTO_SetMotoOutputIO (unsigned short mMOtoIndex); 
void PDL_MOTO_MotoRun_FX(unsigned short * p_g_MotoAll,unsigned short mRunStep);
void PDL_MOTO_MotoRun_RW(unsigned short * p_g_MotoAll,unsigned short mRunStep);


void PDL_PARAL_Init_Fun (void); 
void PDL_PARAL_SetBusy (void); 
void PDL_PARAL_ClearBusy (void); 
void PDL_PARAL_SetACK (void); 
void PDL_PARAL_ClearACK (void); 
void PDL_PARAL_SetPE (void); 
void PDL_PARAL_ClearPE (void); 
unsigned char PDL_PARAL_ReadChar (void); 
unsigned short PDL_PARAL_EXTInterruptStatus (void); 
void PDL_PARAL_EXTInterruptIRQHandler (void); 


void PDL_TTL_Init_Fun (unsigned short mSerPar); 
unsigned int PDL_TTL_GetBusy (void); 
void PDL_TTL_SetBusy (void); 
void PDL_TTL_ClearBusy (void); 
void PDL_TTL_WriteChar (unsigned char mSdat) ;
void PDL_TTL_SendChar (unsigned char mSdat); 
void PDL_TTL_SendBuf(unsigned char *mSBuf,unsigned short mLength);
unsigned char PDL_TTL_ReceivedChar (void); 
unsigned int PDL_TTL_RxInterruptStatus (void); 
unsigned int PDL_TTL_IsRXFIFONotEmpty (void) ;
void PDL_TTL_RxInterruptIRQHandler (void); 


void PDL_RS232_Init_Fun (unsigned short mSerPar); 
unsigned int PDL_RS232_GetBusy (void); 
void PDL_RS232_SetBusy (void); 
void PDL_RS232_ClearBusy (void); 
void PDL_RS232_WriteChar (unsigned char mSdat) ;
void PDL_RS232_SendChar (unsigned char mSdat); 
void PDL_RS232_SendBuf(unsigned char *mSBuf,unsigned short mLength);
unsigned char PDL_RS232_ReceivedChar (void); 
unsigned int PDL_RS232_RxInterruptStatus (void); 
unsigned int PDL_RS232_IsRXFIFONotEmpty (void) ;
void PDL_RS232_RxInterruptIRQHandler (void); 


void PDL_BT_Init_Fun (unsigned short mSerPar); 
void PDL_BT_SetBTName (unsigned char* mNameBuf);// 
void PDL_BT_SetBTKEY (unsigned char* mNameBuf);
unsigned int PDL_BT_GetConnect (void); 
unsigned int PDL_BT_GetBusy (void); 
void PDL_BT_SetBusy (void);
void PDL_BT_ClearBusy (void); 
void PDL_BT_WriteChar (unsigned char mSdat) ;
void PDL_BT_SendChar (unsigned char mSdat); 
unsigned char PDL_BT_ReceivedChar (void); 
unsigned int PDL_BT_RxInterruptStatus (void); 
void PDL_BT_RxInterruptIRQHandler(void);
unsigned short PDL_BT_ReceiveNBytes (unsigned char *dataBuffer,unsigned short len) ;
void PDL_BT_ReturnNBytes (unsigned char *dataBuffer,unsigned short len) ;
void PDL_BT_DisConnect (void) ;

void PDL_WIFI_Init_Fun (unsigned short mSerPar); 
unsigned int PDL_WIFI_GetBusy (void); 
void PDL_WIFI_SetBusy (void); 
void PDL_WIFI_ClearBusy (void); 
void PDL_WIFI_SetRest (void); 
void PDL_WIFI_ClearRest (void); 
unsigned int PDL_WIFI_GetLink (void); 
void PDL_WIFI_WriteChar (unsigned char mSdat) ;
void PDL_WIFI_SendChar (unsigned char mSdat); 
unsigned char PDL_WIFI_ReceivedChar (void); 
unsigned int PDL_WIFI_RxInterruptStatus (void); 
void PDL_WIFI_RxInterruptIRQHandler(void);



void PDL_ADC_Init_Fun (void); 
void PDL_ADC_HVIN_Init_Fun (void) ;
void PDL_ADC_GPIO_Init_Fun (unsigned short num)  ;
unsigned short PDL_ADC_GetDriveVoltage (void); 
unsigned short PDL_ADC_GetBattyPer (void) ;
unsigned short PDL_ADC_GetRefPhotoelectric (void); 
unsigned short PDL_ADC_GetCorPhotoelectric (void);
unsigned short PDL_ADC_GetPTemperature (void);

void PDL_SDTH_Init_Fun (void); 
unsigned short PDL_SDTH_SendToHead_u16 (unsigned short* mSDBuffer,unsigned short mLen); 
unsigned short PDL_SDTH_SendToHead_u32 (unsigned int* mSDBuffer,unsigned short mLen);
unsigned short PDL_SDTH_DMA_SendToHead_u16 (unsigned short* mSDBuffer,unsigned short mLen);
unsigned short PDL_SDTH_DMA_SendToHead_u32 (unsigned int* mSDBuffer,unsigned short mLen);
unsigned short PDL_SDTH_IS_Send_Empty (void); 
void PDL_SDTH_DMA_SendToHead_u8(unsigned char* mSDBuffer,unsigned short mLen);


void PDL_RFID_Init_Fun (void); 
void PDL_RFID_VCC_EN (void); 
void PDL_RFID_VCC_DIS (void); 
void PDL_RFID_CS_EN (void); 
void PDL_RFID_CS_DIS (void); 
void PDL_RFID_SetRest (void); 
void PDL_RFID_ClearRest (void); 
unsigned char PDL_RFID_WriteByte (unsigned char mRdat); 
unsigned char PDL_RFID_ReadByte (void); 
void PDL_NET_Init_Fun (void); 
void PDL_NET_CS_EN (void); 
void PDL_NET_CS_DIS (void); 
void PDL_NET_SetRest (void); 
void PDL_NET_ClearRest (void); 
unsigned char PDL_NET_WriteByte (unsigned char mRdat); 
unsigned char PDL_NET_ReadByte (void); 

void PDL_SPI_Init_Fun (void); 
void PDL_SPI_CS_EN (void); 
void PDL_SPI_CS_DIS (void); 
void PDL_SPI_CS_E1_EN (void); 
void PDL_SPI_CS_E1_DIS (void); 
unsigned int PDL_SPI_SendDat_u32 (unsigned int mSdat); 
unsigned short PDL_SPI_SendDat_u16 (unsigned short mSdat); 
unsigned char PDL_SPI_SendDat_u8 (unsigned char mSdat); 
void PDL_SPI_SendData (unsigned char data) ;
void PDL_SPI_SendBuff(unsigned char *buff, int len);
void PDL_SPI_SendAndReceiveData( unsigned char *TxBuff, unsigned short TxLen, unsigned char *RxBuff, unsigned short RxLen);

void PDL_USB_Init_Fun (void); 
void PDL_USB_POWER_EN (void); 
void PDL_USB_POWER_DIS (void); 
void PDL_USB_Connect (void); 
void PDL_USB_DisConnect (void); 
unsigned short PDL_USB_GetUSBDet (void); 
unsigned short PDL_USB_ReceiveNBytes (unsigned char *dataBuffer,unsigned short len); 
unsigned short PDL_USB_ReturnNBytes (unsigned char *dataBuffer,unsigned short len);
void PDL_USB_ReceiveEPValid (void); 
void PDL_USB_Judge_EN_EPValid (void); 

void PDL_SERVICE_Init_Fun (void);
void PDL_SERVICE_ServiceIRQ_A_EN (unsigned short TimeCouValue);
void PDL_SERVICE_ServiceIRQ_A_DIS (void);
void PDL_SERVICE_ServiceIRQ_A_Handler  (void);
void PDL_SERVICE_ServiceIRQ_B_EN (unsigned short TimeCouValue);
void PDL_SERVICE_ServiceIRQ_B_DIS (void);
void PDL_SERVICE_ServiceIRQ_B_Handler  (void);
void PDL_SERVICE_ServiceIRQ_C_EN (unsigned short TimeCouValue)	;
void PDL_SERVICE_ServiceIRQ_C_DIS (void);
void PDL_SERVICE_ServiceIRQ_C_Handler  (void);
void PDL_SERVICE_ServiceIRQ_D_EN (unsigned short TimeCouValue);
void PDL_SERVICE_ServiceIRQ_D_DIS (void);
void PDL_SERVICE_ServiceIRQ_D_Handler  (void);
void PDL_SERVICE_PrinterIdle  (void);
void PDL_SERVICE_PrinterBusy  (void);

void PDL_IIC_RST_L (void);
void PDL_IIC_RST_H (void);
void PDL_IIC_SCL_L (void);
void PDL_IIC_SCL_H (void);
void PDL_IIC_SDA_L (void);
void PDL_IIC_SDA_H (void);
void PDL_IIC_SDA_InputEn (void);
unsigned short PDL_IIC_SDA_read (void);






#endif //__USB_CONF__H__

