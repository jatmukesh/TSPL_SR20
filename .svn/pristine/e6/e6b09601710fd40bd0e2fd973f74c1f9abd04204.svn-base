/* ******************************************************************************
打印机驱动层，主要用于衔接打印机应用层与芯片硬件


****************************************************************************** */
/*******************************************************************************
 * Include files
 ******************************************************************************/

#include "PDL.h"
#include "macros.h"     


const unsigned int PrintMotoHalfStep[8]={MOTO_RUN_HALF_1_CODR,MOTO_RUN_HALF_2_CODR,MOTO_RUN_HALF_3_CODR,MOTO_RUN_HALF_4_CODR,MOTO_RUN_HALF_5_CODR,MOTO_RUN_HALF_6_CODR,MOTO_RUN_HALF_7_CODR,MOTO_RUN_HALF_8_CODR};
_PDL		PRINTER_PDL =
{
	{
		PDL_SYS_SystemInit , 
		PDL_SYS_SystemIOInit , 
		PDL_SYS_SystemPrintIRQInit ,
		PDL_SYS_SystemPrintLineDatInit ,
		PDL_SYS_SystemSysTickInit ,
		PDL_SYS_SystemPrintIRQ_EN ,
		PDL_SYS_SystemPrintIRQ_DIS , 
		PDL_SYS_SystemPrintLineDat_EN , 
		PDL_SYS_SystemPrintLineDat_DIS , 
		PDL_SYS_SystemPrintIRQHandler , 
		PDL_SYS_SystemPrintLineDatIRQHandler , 
		PDL_SYS_SystemSysTickIRQHandler , 
		PDL_SYS_LAT_L , 
		PDL_SYS_LAT_H , 
		PDL_SYS_STB_ALL_CLEAR , 
		PDL_SYS_STB_ALL_SET , 
		PDL_SYS_STB_A_EN , 
		PDL_SYS_STB_A_DIS , 
		PDL_SYS_STB_B_EN , 
		PDL_SYS_STB_B_DIS , 
		PDL_SYS_Charge_LED1_ON , 
		PDL_SYS_Charge_LED1_OFF , 
		PDL_SYS_Charge_LED2_ON , 
		PDL_SYS_Charge_LED2_OFF , 
		PDL_SYS_Charge_LED3_ON , 
		PDL_SYS_Charge_LED3_OFF , 
		PDL_SYS_BEEP_EN , 
		PDL_SYS_BEEP_DIS , 
		PDL_SYS_LED_Power_ON , 
		PDL_SYS_LED_Power_OFF , 
		PDL_SYS_LED_Power_Trig , 
		PDL_SYS_GET_LED_Power,		
		PDL_SYS_LED_Stauts_ON , 
		PDL_SYS_LED_Stauts_OFF , 
		PDL_SYS_LED_Stauts_Trig , 
		PDL_SYS_GET_LED_Stauts,
		PDL_SYS_LED_Charge_ON , 
		PDL_SYS_LED_Charge_OFF , 
		PDL_SYS_LED_Charge_Trig , 
		PDL_SYS_GET_GET_Charge_LED1,
		PDL_SYS_LED_WIFI_Link_ON , 
		PDL_SYS_LED_WIFI_Link_OFF , 
		PDL_SYS_LED_WIFI_Link_Trig , 
		PDL_SYS_GET_LED_WIFI_Link,
		PDL_SYS_LED_BT_Link_ON ,
		PDL_SYS_LED_BT_Link_OFF , 
		PDL_SYS_LED_BT_Link_Trig ,
		PDL_SYS_GET_LED_BT_Link,
		PDL_SYS_GetDIPswitchStatus , 
		PDL_SYS_POWER_EN , 
		PDL_SYS_POWER_DIS , 
		PDL_SYS_PeripheralVcc_EN , 
		PDL_SYS_PeripheralVcc_DIS , 
		PDL_SYS_GetKey_POW , 
		PDL_SYS_GetKey_LF , 
		PDL_SYS_GetKey_SEL , 
		PDL_SYS_Get_Plus_POW , 
		PDL_SYS_Get_Plus_LF ,
		PDL_SYS_Get_Plus_SEL , 
		PDL_SYS_Drive_Power_EN , 
		PDL_SYS_Drive_Power_DIS , 
		PDL_SYS_Drive_Power_Trig , 
		PDL_SYS_WDG_ReloadCounter,
		PDL_SYS_GetHSStatus,
		PDL_SYS_GetChargeNCHRG,
		PDL_SYS_GetChargeNSTDBY,
		PDL_SYS_Charge_EN ,
		PDL_SYS_Charge_DIS,
		PDL_SYS_GetCurVoltageLevel,
		PDL_SYS_GetCurTemperatureLevel,
		PDL_SYS_SavePrinterConfig,
		PDL_SYS_ReadPrinterConfig,
		PDL_SYS_DelayNus,
		PDL_SYS_PaperOutDet,
		PDL_SYS_PaperGapDet				
	},
	{
		PDL_MOTO_Init_Fun , 
		PDL_MOTO_MotoDisable , 
		PDL_MOTO_SetMotoOutputPWM , 
		PDL_MOTO_SetMotoOutputIO ,
		PDL_MOTO_MotoRun_FX,
		PDL_MOTO_MotoRun_RW
	},
	{
		PDL_PARAL_Init_Fun , 
		PDL_PARAL_SetBusy , 
		PDL_PARAL_ClearBusy , 
		PDL_PARAL_SetACK , 
		PDL_PARAL_ClearACK , 
		PDL_PARAL_SetPE , 
		PDL_PARAL_ClearPE , 
		PDL_PARAL_ReadChar , 
		PDL_PARAL_EXTInterruptStatus , 
		PDL_PARAL_EXTInterruptIRQHandler 
	},
	{
		PDL_TTL_Init_Fun , 
		PDL_TTL_GetBusy , 
		PDL_TTL_SetBusy , 
		PDL_TTL_ClearBusy , 
		PDL_TTL_WriteChar,
		PDL_TTL_SendChar , 
		PDL_TTL_SendBuf,
		PDL_TTL_ReceivedChar , 
		PDL_TTL_RxInterruptStatus , 
		PDL_TTL_IsRXFIFONotEmpty , 
		PDL_TTL_RxInterruptIRQHandler 
	},
	{
		PDL_RS232_Init_Fun , 
		PDL_RS232_GetBusy , 
		PDL_RS232_SetBusy , 
		PDL_RS232_ClearBusy , 
		PDL_RS232_WriteChar,
		PDL_RS232_SendChar , 
		PDL_RS232_SendBuf,
		PDL_RS232_ReceivedChar , 
		PDL_RS232_RxInterruptStatus , 
		PDL_RS232_IsRXFIFONotEmpty , 
		PDL_RS232_RxInterruptIRQHandler 
	},
	{
		PDL_BT_Init_Fun , 
		PDL_BT_SetBTName,
		PDL_BT_SetBTKEY,
		PDL_BT_GetConnect , 
		PDL_BT_GetBusy , 
		PDL_BT_SetBusy ,
		PDL_BT_ClearBusy ,
		PDL_BT_WriteChar,		
		PDL_BT_SendChar , 
		PDL_BT_ReceivedChar , 
		PDL_BT_RxInterruptStatus , 
		PDL_BT_RxInterruptIRQHandler,
		PDL_BT_ReceiveNBytes,
		PDL_BT_ReturnNBytes,
		PDL_BT_DisConnect
	},	
	{
		PDL_WIFI_Init_Fun , 
		PDL_WIFI_GetBusy , 
		PDL_WIFI_SetBusy , 
		PDL_WIFI_ClearBusy , 
		PDL_WIFI_SetRest , 
		PDL_WIFI_ClearRest , 
		PDL_WIFI_GetLink , 
		PDL_WIFI_WriteChar,
		PDL_WIFI_SendChar , 
		PDL_WIFI_ReceivedChar , 
		PDL_WIFI_RxInterruptStatus , 
		PDL_WIFI_RxInterruptIRQHandler
	},	
	{
		PDL_ADC_Init_Fun , 
		PDL_ADC_HVIN_Init_Fun	,	
		PDL_ADC_GPIO_Init_Fun	,	
		PDL_ADC_GetDriveVoltage ,
		PDL_ADC_GetBattyPer,
		PDL_ADC_GetRefPhotoelectric , 
		PDL_ADC_GetCorPhotoelectric ,
		PDL_ADC_GetPTemperature 
	},	
	{
		PDL_SDTH_Init_Fun , 
		PDL_SDTH_SendToHead_u16 , 
		PDL_SDTH_SendToHead_u32 ,
		PDL_SDTH_DMA_SendToHead_u16 ,
		PDL_SDTH_DMA_SendToHead_u32 ,
		PDL_SDTH_IS_Send_Empty ,
		PDL_SDTH_DMA_SendToHead_u8
	},	
	{
		PDL_RFID_Init_Fun , 
		PDL_RFID_VCC_EN , 
		PDL_RFID_VCC_DIS , 
		PDL_RFID_CS_EN , 
		PDL_RFID_CS_DIS , 
		PDL_RFID_SetRest , 
		PDL_RFID_ClearRest , 
		PDL_RFID_WriteByte , 
		PDL_RFID_ReadByte  	
	},
	{
		PDL_NET_Init_Fun , 
		PDL_NET_CS_EN , 
		PDL_NET_CS_DIS , 
		PDL_NET_SetRest , 
		PDL_NET_ClearRest , 
		PDL_NET_WriteByte , 
		PDL_NET_ReadByte  
	},
	{
		PDL_SPI_Init_Fun , 
		PDL_SPI_CS_EN , 
		PDL_SPI_CS_DIS , 
		PDL_SPI_CS_E1_EN , 
		PDL_SPI_CS_E1_DIS , 
		PDL_SPI_SendDat_u32 , 
		PDL_SPI_SendDat_u16 , 
		PDL_SPI_SendDat_u8  ,
		PDL_SPI_SendData,
		PDL_SPI_SendBuff,
		PDL_SPI_SendAndReceiveData
	},
	{
		PDL_USB_Init_Fun , 
		PDL_USB_POWER_EN , 
		PDL_USB_POWER_DIS , 
		PDL_USB_Connect , 
		PDL_USB_DisConnect , 
		PDL_USB_GetUSBDet , 
		PDL_USB_ReceiveNBytes , 
		PDL_USB_ReturnNBytes,
		PDL_USB_ReceiveEPValid , 
		PDL_USB_Judge_EN_EPValid 
	},
	{
		PDL_SERVICE_Init_Fun,
		PDL_SERVICE_ServiceIRQ_A_EN,
		PDL_SERVICE_ServiceIRQ_A_DIS,
		PDL_SERVICE_ServiceIRQ_A_Handler,
		PDL_SERVICE_ServiceIRQ_B_EN,
		PDL_SERVICE_ServiceIRQ_B_DIS,
		PDL_SERVICE_ServiceIRQ_B_Handler,
		PDL_SERVICE_ServiceIRQ_C_EN,
		PDL_SERVICE_ServiceIRQ_C_DIS,
		PDL_SERVICE_ServiceIRQ_C_Handler,
		PDL_SERVICE_ServiceIRQ_D_EN,
		PDL_SERVICE_ServiceIRQ_D_DIS,
		PDL_SERVICE_ServiceIRQ_D_Handler,
		PDL_SERVICE_PrinterIdle,
		PDL_SERVICE_PrinterBusy		
	},
	{
		PDL_IIC_RST_L,
		PDL_IIC_RST_H,
		PDL_IIC_SCL_L,
		PDL_IIC_SCL_H,
		PDL_IIC_SDA_L,
		PDL_IIC_SDA_H,
		PDL_IIC_SDA_InputEn,
		PDL_IIC_SDA_read
	}

};


//=============================================================

 /****************************************************************************
 * 函数名：RAMAIC_EndISR()
 * 功  能：反转一个字
  ****************************************************************************/
unsigned int RevInt(unsigned int x) {
   x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
   x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) |
       ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}
 /****************************************************************************
 * 函数名：CalintCount()
 * 功  能：计算一个字中1的个数
  ****************************************************************************/
int CalintCount(unsigned int x) {
   x = x - ((x >> 1) & 0x55555555);
   x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
   x = (x + (x >> 4)) & 0x0F0F0F0F;
   x = x + (x >> 8);
   x = x + (x >> 16);
   return x & 0x0000003F;
}
 /****************************************************************************
 * 函数名：Transpose32()
 * 功  能：旋转32X32位的图形
  ****************************************************************************/
void Transpose32(unsigned int a[32])
{
   int j, k;
   unsigned int m, t;
   for (j = 16, m = 0x0000FFFF; j; j >>= 1, m ^= m << j)
   {
      for (k = 0; k < 32; k = ((k | j) + 1) & ~j)
      {
         t = (a[k] ^ (a[k|j] >> j)) & m;
         a[k] ^= t;
         a[k|j] ^= (t << j);
      }
   }
}
 /****************************************************************************
 * 函数名：Transpose16()
 * 功  能：旋转16X16位的图形
  ****************************************************************************/
void Transpose16(unsigned short a[16])
{
   int j, k;
   unsigned int m, t;
   for (j = 8, m = 0x00FF; j; j >>= 1, m ^= m << j)
   {
      for (k = 0; k < 16; k = ((k | j) + 1) & ~j)
      {
         t = (a[k] ^ (a[k|j] >> j)) & m;
         a[k] ^= t;
         a[k|j] ^= (t << j);
      }
   }
}
 /****************************************************************************
 * 函数名：Transpose16()
 * 功  能：旋转64X64位的图形
  ****************************************************************************/
void Transpose64(unsigned long long a[64])
{
   unsigned long long j, k;
   unsigned long long m, t;
   for (j = 32, m = 0x00000000FFFFFFFF; j; j >>= 1, m ^= m << j)
   {
      for (k = 0; k < 64; k = ((k | j) + 1) & ~j)
      {
         t = (a[k] ^ (a[k|j] >> j)) & m;
         a[k] ^= t;
         a[k|j] ^= (t << j);
      }
   }
}

//=============================================================
//=============================================================
//=============================================================
//=============================================================
//=============================================================
//=============================================================
//=============================================================
//=============================================================
//=============================================================
//===============系统定时器中断==============================================
//extern uint16_t m_au16Adc1SaValue[ADC1_CH_COUNT];
void Pro_SysTick_IrqHandler(void)
{
	//调用客户端定时器处理中断
	PRINTER_PDL.SYS._SystemSysTickIRQHandler();
#ifdef DOUBLE_TAP_SELFTEST_PRINT
	K2_Driver_Update();
#endif	
}

//==============打印运行核心中断==============================================
void PrintRun_IrqHandler(void)
{
	//调用客户端定时器处理中断
	PRINTER_PDL.SYS._SystemPrintIRQ_DIS();
	PRINTER_PDL.SYS._SystemPrintIRQHandler();
}
//==============处理一行打印数据中断==============================================
void Pro_LineData_IrqHandler(void)
{
	//调用客户端定时器处理中断
	PRINTER_PDL.SYS._SystemPrintLineDat_DIS();
	PRINTER_PDL.SYS._SystemPrintLineDatIRQHandler();
}
//=============USB中断处理中调用=接收USB数据=======================================
unsigned short USB_IRQ_DataRecive(unsigned char *mbuff,unsigned short mlen)
{
	return PRINTER_PDL.USB._ReceiveNBytes(mbuff,mlen);
}

//=============USB中断处理中调用=接收USB数据=======================================
unsigned short BT_IRQ_ReceiveNBytes(unsigned char *mbuff,unsigned short mlen)
{
	return PRINTER_PDL.BT._ReceiveNBytes(mbuff,mlen);
}


//=============蓝牙串口中断处理中调用=======================================
void BT_IRQ_DataRecive(void)
{
	PRINTER_PDL.BT._RxInterruptIRQHandler();
}
//=============232串口中断处理中调用=======================================
void RS23_IRQ_DataRecive(void)
{
	PRINTER_PDL.RS232._RxInterruptIRQHandler();
}
//=============TTL串口中断处理中调用=======================================
void TTL_IRQ_DataRecive(void)
{
	PRINTER_PDL.TTL._RxInterruptIRQHandler();
}

//=============EXTI外部io中断处理中调用=======================================
void PARALLEL_IRQ_DataRecive(void)
{
	PRINTER_PDL.PARALLEL._EXTInterruptIRQHandler ();
}


//==============服务中断回调处理函数==============================================
void Pro_ServiceIRQ_A_Handler(void)
{
	//先关闭再调用处理函数
	PRINTER_PDL.SERVICE._ServiceIRQ_A_DIS();
	PRINTER_PDL.SERVICE._ServiceIRQ_A_Handler();
}

void Pro_ServiceIRQ_B_Handler(void)
{
	PRINTER_PDL.SERVICE._ServiceIRQ_B_DIS();
	PRINTER_PDL.SERVICE._ServiceIRQ_B_Handler();
}

void Pro_ServiceIRQ_C_Handler(void)
{
	PRINTER_PDL.SERVICE._ServiceIRQ_C_DIS();
	PRINTER_PDL.SERVICE._ServiceIRQ_C_Handler();
}

void Pro_ServiceIRQ_D_Handler(void)
{
	PRINTER_PDL.SERVICE._ServiceIRQ_C_DIS();
	PRINTER_PDL.SERVICE._ServiceIRQ_D_Handler();
}

/*==================================================================
													系统方法
==================================================================*/
/****************************************************************************
* 名称: PDL_SYS_SystemInit()
* 功能: 启用系统工能，时钟，看门狗.......
* 入口参数: 无

* 出口参数: 无
* 说明: 系统初始化,看门狗 ，中断，内核功能
****************************************************************************/
void PDL_SYS_SystemInit (void) 
{
	WDT_Configuration();
	Debug_Configuration();
}
/****************************************************************************
* 名称: PDL_SYS_SystemIOInit()
* 功能: 系统io初始化
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemIOInit (void) 
{
	GPIO_Configuration();
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintIRQInit()
* 功能: 打印中断初始化
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintIRQInit (void)
{
	Printer_TIMER_Configuration();
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintLineDatInit()
* 功能: 处理一行打印数据中断初始化
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintLineDatInit (void)
{
	//一起初始化
	//Printer_TIMER_Configuration();
}
/****************************************************************************
* 名称: PDL_SYS_SystemSysTickInit()
* 功能: 系统定时器初始化
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemSysTickInit (void)
{
	System_TIMER_Configuration();
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintIRQ_EN()
* 功能: 打印中断初使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintIRQ_EN (unsigned short mTimeLen)
{
	EnableTimer(TIM1,mTimeLen);
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintIRQ_DIS()
* 功能: 打印中断禁止
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintIRQ_DIS (void) 
{
	DisableTimer(TIM1);
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintLineDat_EN()
* 功能: 处理一行打印数据中断使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintLineDat_EN (unsigned short mTimeLen) 
{
	EnableTimer(TIM2,mTimeLen);
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintLineDat_DIS()
* 功能: 处理一行打印数据中断禁止
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintLineDat_DIS (void) 
{
	DisableTimer(TIM2);
}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintIRQHandler()
* 功能: 打印中断处理
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintIRQHandler (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_SystemPrintLineDatIRQHandler()
* 功能: 处理一行打印数据中断处理
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemPrintLineDatIRQHandler (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_SystemSysTickIRQHandler()
* 功能: 系统定时器处理中断
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SystemSysTickIRQHandler (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_LAT_L()
* 功能: 锁存信号低 
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LAT_L (void) 
{
	SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;
	SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;
	SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;
	SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;
	SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;
	SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;SET_L_O_LAT;
}
/****************************************************************************
* 名称: PDL_SYS_LAT_H()
* 功能: 锁存信号高
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LAT_H (void) 
{
	SET_H_O_LAT;
}
/****************************************************************************
* 名称: PDL_SYS_STB_ALL_CLEAR()
* 功能: 所有加热线清除加热状态
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_STB_ALL_CLEAR (void) 
{
	SET_L_O_STB1;
	SET_L_O_STB2;
}
/****************************************************************************
* 名称: PDL_SYS_STB_ALL_SET()
* 功能: 所有加热线设置加热状态 
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_STB_ALL_SET (void) 
{
	SET_H_O_STB1;
	SET_H_O_STB2;

}
/****************************************************************************
* 名称: PDL_SYS_STB_A_EN()
* 功能: 加热线A组 使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_STB_A_EN (void) 
{

	SET_H_O_STB1;

}
/****************************************************************************
* 名称: PDL_SYS_STB_A_DIS()
* 功能: 加热线A组 清除
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_STB_A_DIS (void) 
{

	SET_L_O_STB1;

}
/****************************************************************************
* 名称: PDL_SYS_STB_B_EN()
* 功能: 加热线B组 使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_STB_B_EN (void) 
{

	SET_H_O_STB2;

}
/****************************************************************************
* 名称: PDL_SYS_STB_B_DIS()
* 功能: 加热线B组 清除
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_STB_B_DIS (void) 
{

	SET_L_O_STB2;

}
/****************************************************************************
* 名称: PDL_SYS_STB_C_EN()
* 功能: 加热线C组 使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Charge_LED1_ON (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_STB_C_DIS()
* 功能: 加热线C组 清除
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Charge_LED1_OFF (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_STB_D_EN()
* 功能: 加热线D组 使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Charge_LED2_ON (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_STB_D_DIS()
* 功能: 加热线B组 清除
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Charge_LED2_OFF (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_STB_E_EN()
* 功能: 加热线E组 使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/


void PDL_SYS_Charge_LED3_ON (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_STB_E_DIS()
* 功能:  加热线E组 清除
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Charge_LED3_OFF (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_BEEP_EN()
* 功能: 加热线F组 使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_BEEP_EN (void) 
{
	SET_H_O_BEEP;
	//SET_L_O_BEEP;
}
/****************************************************************************
* 名称: PDL_SYS_STB_F_DIS()
* 功能:加热线F组 清除
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_BEEP_DIS (void) 
{
	SET_L_O_BEEP;
}

/****************************************************************************
* 名称: PDL_SYS_LED_Power_ON()
* 功能: 状态指示灯 ON
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Power_ON (void) 
{

	

}
/****************************************************************************
* 名称: PDL_SYS_LED_Power_OFF()
* 功能: 状态指示灯 OFF
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Power_OFF (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_LED_Power_Trig()
* 功能:  状态指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Power_Trig (void) 
{
	#ifdef POWER_LED_EN
	if(GET_LEDR_OUT)
	{
		SET_L_O_LEDR;
	}
	else
	{
		SET_H_O_LEDR;
	}
	#endif	
}

/****************************************************************************
* 名称: PDL_SYS_GET_LED_Power()
* 功能:  状态指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GET_LED_Power (void) 
{
	#ifdef POWER_LED_EN
	return GET_LEDR_OUT;
	#endif
	return 0;

}

/****************************************************************************
* 名称: PDL_SYS_LED_Stauts_ON()
* 功能: 状态指示灯 ON
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Stauts_ON (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_LED_Stauts_OFF()
* 功能: 状态指示灯 OFF
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Stauts_OFF (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_LED_Stauts_Trig()
* 功能:  状态指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Stauts_Trig (void) 
{

	
}

/****************************************************************************
* 名称: PDL_SYS_GET_LED_Stauts()
* 功能:  状态指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GET_LED_Stauts (void) 
{
	return 0;
}



/****************************************************************************
* 名称: PDL_SYS_LED_Charge_ON()
* 功能: 充电指示灯 ON
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Charge_ON (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_LED_Charge_OFF()
* 功能: 充电指示灯 ON
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Charge_OFF (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_LED_Charge_Trig()
* 功能: 充电指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_Charge_Trig (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_GET_GET_Charge_LED1()
* 功能:  状态指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GET_GET_Charge_LED1 (void) 
{
	return 0;
}


/****************************************************************************
* 名称: PDL_SYS_LED_WIFI_Link_ON()
* 功能: wifi 链接 指示灯 ON
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_WIFI_Link_ON (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_LED_WIFI_Link_OFF()
* 功能: wifi 链接 指示灯 OFF
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_WIFI_Link_OFF (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_LED_WIFI_Link_Trig()
* 功能: wifi 链接 指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_WIFI_Link_Trig (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_GET_LED_WIFI_Link()
* 功能:  wifi 状态指示灯
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GET_LED_WIFI_Link (void) 
{
	return 0;
}
/****************************************************************************
* 名称: PDL_SYS_LED_BT_Link_ON()
* 功能: BT 链接 指示灯 ON
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_BT_Link_ON (void)
{
	
}
/****************************************************************************
* 名称: PDL_SYS_LED_BT_Link_OFF()
* 功能: BT 链接 指示灯 OFF
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_BT_Link_OFF (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_LED_BT_Link_Trig()
* 功能: BT 链接 指示灯 Trig
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_LED_BT_Link_Trig (void) 
{

}
/****************************************************************************
* 名称: PDL_SYS_GET_LED_BT_Link()
* 功能:  BT 状态指示灯
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GET_LED_BT_Link (void) 
{
	return 0;
}
/****************************************************************************
* 名称: PDL_SYS_GetDIPswitchStatus()
* 功能: 得到拨码开关状态
* 入口参数: 无

* 出口参数: 无
* 说明: 拨码开关状态需要做定义
****************************************************************************/
unsigned int PDL_SYS_GetDIPswitchStatus (void) 
{
	return 0;
}
/****************************************************************************
* 名称: PDL_SYS_POWER_EN()
* 功能: 总电源使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_POWER_EN (void) 
{
	SET_H_O_POWER_EN;
}
/****************************************************************************
* 名称: PDL_SYS_POWER_DIS()
* 功能: 总电源不使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_POWER_DIS (void) 
{
	SET_L_O_POWER_EN;
}
/****************************************************************************
* 名称: PDL_SYS_PeripheralVcc_EN()
* 功能: 外围电源使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_PeripheralVcc_EN (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_PeripheralVcc_DIS()
* 功能: 外围电源不使能		
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_PeripheralVcc_DIS (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_GetKey_POW()
* 功能: 得到电源开关 当前输入状态
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GetKey_POW (void) 
{
	
	return GET_I_KEY_POW;
	
}
/****************************************************************************
* 名称: PDL_SYS_GetKey_LF()
* 功能: 得到走纸开关 当前输入状态
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GetKey_LF (void) 
{
	return GET_I_KEY_LF;
}
/****************************************************************************
* 名称: PDL_SYS_GetKey_SEL()
* 功能: 自检开关 当前输入状态
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GetKey_SEL (void) 
{
	return 0;
}
/****************************************************************************
* 名称: PDL_SYS_Get_Plus_POW()
* 功能: 得到 电源按键 次数脉冲
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_Get_Plus_POW (void) 
{
	
	return ReadPOWKeyPlus();
}
/****************************************************************************
* 名称: PDL_SYS_Get_Plus_LF()
* 功能: 得到走纸按键次数脉冲
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_Get_Plus_LF (void)
{
	return ReadLFKeyPlus();
}
/****************************************************************************
* 名称: PDL_SYS_Get_Plus_SEL()
* 功能: 得到自检按键次数脉冲
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_Get_Plus_SEL (void) 
{
	return 0;
}
/****************************************************************************
* 名称: PDL_SYS_Drive_Power_EN()
* 功能: 驱动电源输出
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Drive_Power_EN (void) 
{
	SET_H_O_SWITCH;
}
/****************************************************************************
* 名称: PDL_SYS_Drive_Power_DIS()
* 功能: 驱动禁止输出
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Drive_Power_DIS (void) 
{
	SET_L_O_SWITCH;
}
/****************************************************************************
* 名称: PDL_SYS_Drive_Power_Trig()
* 功能: 驱动控制输出脉冲
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Drive_Power_Trig (void) 
{
	SET_H_O_SWITCH;
//	if(GET_SWITCH_OUT)
//	{
//		SET_L_O_SWITCH;
//	}
//	else
//	{
//		SET_H_O_SWITCH;
//	}
} 
/****************************************************************************
* 名称: PDL_SYS_WDG_ReloadCounter()
* 功能: 看门狗复位
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_WDG_ReloadCounter (void) 
{
	WDG_ResetCounter();
}

/****************************************************************************
* 名称: PDL_SYS_GetHSStatus()
* 功能: 得到关门开关状态
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/

unsigned int PDL_SYS_GetHSStatus (void) 
{
	return GET_I_HS;
}
/****************************************************************************
* 名称: _GetChargeStatus()
* 功能: 得到充电状态
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/

unsigned int PDL_SYS_GetChargeNCHRG (void) 
{
	return GET_I_CHARGE;
}
/****************************************************************************
* 名称: _GetChargeStatus()
* 功能: 得到充电完成
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/

unsigned int PDL_SYS_GetChargeNSTDBY (void) 
{
	return 0;
}

/****************************************************************************
* 名称: PDL_SYS_Charge_EN()
* 功能: 充电使能
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_Charge_EN (void) 
{
	
}
/****************************************************************************
* 名称: PDL_SYS_Charge_EN()
* 功能: 充电禁止
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
	
void PDL_SYS_Charge_DIS (void) 
{
	
}

/****************************************************************************
* 名称: PDL_SYS_GetCurVoltageLevel()
* 功能: 得当前电压索引等级
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GetCurVoltageLevel (void) 
{
	return GetCurrentVol();
}

/****************************************************************************
* 名称: PDL_SYS_GetCurTemperatureLevel()
* 功能: 得到当前温度索引等级
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_GetCurTemperatureLevel (void) 
{
	return GetCurrentTM();
}
/****************************************************************************
* 名称: PDL_SYS_SavePrinterConfig()
* 功能: 设置打印机参数
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_SavePrinterConfig (unsigned char * pbuf,unsigned short mplen)
{

	SaveCOnfigtoFlash(pbuf,mplen);
	
}
/****************************************************************************
* 名称: PDL_SYS_ReadPrinterConfig()
* 功能: 读取参数
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_ReadPrinterConfig (unsigned char * pbuf,unsigned short mplen) 
{
	ReadCOnfigFromFlash(pbuf,mplen);

}



/****************************************************************************
* 名称: PDL_SYS_DelayNus()
* 功能: 读取参数
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_SYS_DelayNus(unsigned short mTimeLen)
{

	delay_us(mTimeLen);
}

/****************************************************************************
* 名称: PDL_SYS_PaperOutDet()
* 功能: 缺纸检测
* 入口参数: 无

* 出口参数: 无
* 说明: 一般打印头缺纸检测都是反射开关返回光电值
****************************************************************************/
unsigned int PDL_SYS_PaperOutDet(void)
{

	return PDL_ADC_GetRefPhotoelectric();
}
/****************************************************************************
* 名称: PDL_SYS_DelayNus()
* 功能: 纸缝检测
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned int PDL_SYS_PaperGapDet(void)
{
	return PDL_ADC_GetCorPhotoelectric();

}









/*==================================================================
					
														电机方法

==================================================================*/
/****************************************************************************
* 名称: PDL_MOTO_Init_Fun()
* 功能: 电机功能初始化 
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_MOTO_Init_Fun (void) 
{
#ifdef MOTO_PWM_EN
	PWM_Configuration();
	#endif
}
/****************************************************************************
* 名称: PDL_MOTO_MotoDisable()
* 功能: 关电机输出 
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_MOTO_MotoDisable (void) 
{
	ClearMoto();
}

/****************************************************************************
* 名称: PDL_MOTO_SetMotoOutputPWM()
* 功能: PWM电机输出 
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_MOTO_SetMotoOutputPWM (unsigned short mMOtoIndex,unsigned short mPwmValue) 
{
	SetMotoPWM(mMOtoIndex,mPwmValue);
}
/****************************************************************************
* 名称: PDL_MOTO_SetMotoOutputIO()
* 功能: IO电机输出
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_MOTO_SetMotoOutputIO (unsigned short mMOtoIndex) 
{
	SetMotoPWM_A(mMOtoIndex);
}



/****************************************************************************
* 名称: PDL_MOTO_SetMotoOutputIO()
* 功能: /电机反转
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_MOTO_MotoRun_FX (unsigned short * p_g_MotoAll,unsigned short mRunStep)
{
		MOTORUN_FX(p_g_MotoAll,mRunStep);
}

/****************************************************************************
* 名称: PDL_MOTO_SetMotoOutputIO()
* 功能: 电机正转
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_MOTO_MotoRun_RW(unsigned short * p_g_MotoAll,unsigned short mRunStep)
{
	MOTORUN(p_g_MotoAll,mRunStep);
}












/*==================================================================
												并口方法
==================================================================*/
/****************************************************************************
* 名称: PDL_PARAL_Init_Fun()
* 功能: 并口功能模块初始化
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_Init_Fun (void) 
{

}
/****************************************************************************
* 名称: PDL_PARAL_SetBusy()
* 功能: 并口 置忙
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_SetBusy (void) 
{

}
/****************************************************************************
* 名称: PDL_PARAL_ClearBusy()
* 功能: 并口 空闲
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_ClearBusy (void) 
{

}
/****************************************************************************
* 名称: PDL_PARAL_SetACK()
* 功能: 并口ACK 拉高
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_SetACK (void) 
{

}
/****************************************************************************
* 名称: PDL_PARAL_ClearACK()
* 功能: 并口ACK 清零
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_ClearACK (void) 
{


}
/****************************************************************************
* 名称: PDL_PARAL_SetPE()
* 功能: 并口缺纸 置位
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_SetPE (void) 
{

}
/****************************************************************************
* 名称: PDL_PARAL_ClearPE()
* 功能: 并口缺纸 清零
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_ClearPE (void) 
{

}
/****************************************************************************
* 名称: PDL_PARAL_ReadChar()
* 功能: 读取端口一个字节数据
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
unsigned char PDL_PARAL_ReadChar (void) 
{
	unsigned char mDat;
	mDat = mDat;
	return mDat;
}
/****************************************************************************
* 名称: PDL_PARAL_EXTInterruptStatus()
* 功能: 判断是否发生中断
* 入口参数: 无

* 出口参数: 无
* 说明: 是否产生有效接收中断，有中断发生，返回中断状态，并清掉中断
****************************************************************************/
unsigned short PDL_PARAL_EXTInterruptStatus (void) 
{
	unsigned short mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}

/****************************************************************************
* 名称: PDL_PARAL_EXTInterruptIRQHandler()
* 功能: 中断回调函数
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_PARAL_EXTInterruptIRQHandler (void) 
{


}


/*==================================================================
											TTL串口方法
==================================================================*/
/****************************************************************************
* 名称: PDL_TTL_Init_Fun (unsigned short mSerPar) 
* 功能: TTL串口初始化
* 入口参数: 无

* 出口参数: 无
* 说明: 
****************************************************************************/
void PDL_TTL_Init_Fun (unsigned short mSerPar) 
{

}
/****************************************************************************
* 名称: PDL_TTL_GetBusy (unsigned short mSerPar) 
* 功能: 返回忙信号状态
* 入口参数: 无

* 出口参数: 返回忙信号状态
* 说明: 
****************************************************************************/
unsigned int PDL_TTL_GetBusy (void) 
{

	return 1;
}
/****************************************************************************
* 名称: PDL_TTL_SetBusy (void) 
* 功能: 置忙
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_TTL_SetBusy (void) 
{
	
}
/****************************************************************************
* 名称: PDL_TTL_ClearBusy (void) 
* 功能: 清除忙信号
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_TTL_ClearBusy (void) 
{
	
}

/****************************************************************************
* 名称: void PDL_TTL_WriteChar (unsigned char mSdat)
* 功能: 写一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_TTL_WriteChar (unsigned char mSdat) 
{
	
}
/****************************************************************************
* 名称: void PDL_TTL_SendChar (unsigned char mSdat)
* 功能: 发送一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_TTL_SendChar (unsigned char mSdat) 
{
	
}
/****************************************************************************
* 名称: void PDL_TTL_SendBuf (unsigned char mSdat)
* 功能: 发送缓冲区
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/

void PDL_TTL_SendBuf(unsigned char *mSBuf,unsigned short mLength)
{

}
/****************************************************************************
* 名称: unsigned char PDL_TTL_ReceivedChar (void) 
* 功能: T得到一个字节数据
* 入口参数: 无

* 出口参数: 一个字节数据
* 说明: 
****************************************************************************/
unsigned char PDL_TTL_ReceivedChar (void) 
{
	return 	0;
}
/****************************************************************************
* 名称:  unsigned int PDL_TTL_RxInterruptStatus (void) 
* 功能: 是否产生有效接收中断
* 入口参数: 无

* 出口参数: 中断状态
* 说明: 
****************************************************************************/
unsigned int PDL_TTL_RxInterruptStatus (void) 
{
	return 0;
}

/****************************************************************************
* 名称:  unsigned int PDL_TTL_IsRXFIFONotEmpty (void) 
* 功能: 数据是否读完
* 入口参数: 无

* 出口参数: 中断状态
* 说明: 
****************************************************************************/
unsigned int PDL_TTL_IsRXFIFONotEmpty (void) 
{
	return 0;
}
/****************************************************************************
* 名称: void PDL_TTL_RxInterruptIRQHandler (void) 
* 功能: 接收中断回调函数
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_TTL_RxInterruptIRQHandler (void) 
{

}
/*==================================================================
											RS232串口方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_RS232_Init_Fun (unsigned short mSerPar) 
* 功能: RS232串口初始化
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_RS232_Init_Fun (unsigned short mSerPar) 
{
	
		
}
/****************************************************************************
* 名称: unsigned int PDL_RS232_GetBusy (void)
* 功能: 得到忙信号状态
* 入口参数: 无
* 出口参数: 返回忙信号状态
* 说明: 
****************************************************************************/
unsigned int PDL_RS232_GetBusy (void) 
{
	return 0;
}
/****************************************************************************
* 名称: void PDL_RS232_SetBusy (void)
* 功能: 置忙
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_RS232_SetBusy (void) 
{

}
/****************************************************************************
* 名称: void PDL_RS232_ClearBusy (void) 
* 功能: 空闲
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_RS232_ClearBusy (void) 
{

}
/****************************************************************************
* 名称: void PDL_TTL_WriteChar (unsigned char mSdat)
* 功能: 写一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_RS232_WriteChar (unsigned char mSdat) 
{

}
/****************************************************************************
* 名称: void PDL_RS232_SendChar (unsigned char mSdat)
* 功能: 发送一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_RS232_SendChar (unsigned char mSdat) 
{
	
	
}
/****************************************************************************
* 名称: void PDL_RS232_SendBuf (unsigned char mSdat)
* 功能: 发送缓冲区
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/

void PDL_RS232_SendBuf(unsigned char *mSBuf,unsigned short mLength)
{

}

/****************************************************************************
* 名称: unsigned char PDL_RS232_ReceivedChar (void)
* 功能: 得到一个字节数据	
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
unsigned char PDL_RS232_ReceivedChar (void) 
{
	return UART_ReceiveData(UART1);
}
/****************************************************************************
* 名称: unsigned int PDL_RS232_RxInterruptStatus (void)
* 功能: 是否产生有效接收中断
* 入口参数: 无

* 出口参数: 中断状态
* 说明: 
****************************************************************************/
unsigned int PDL_RS232_RxInterruptStatus (void) 
{
	return UART_GetITIdentity(UART1) & UART_IT_RX;
}
/****************************************************************************
* 名称:  unsigned int PDL_TTL_IsRXFIFONotEmpty (void) 
* 功能: 数据是否读完
* 入口参数: 无

* 出口参数: 中断状态
* 说明: 
****************************************************************************/
unsigned int PDL_RS232_IsRXFIFONotEmpty (void) 
{
	return UART_IsRXFIFONotEmpty(UART1);
}

/****************************************************************************
* 名称: void PDL_RS232_RxInterruptIRQHandler (void) 
* 功能: 接收中断回调函数
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_RS232_RxInterruptIRQHandler (void) 
{

}	
/*==================================================================
													蓝牙方法
==================================================================*/

/****************************************************************************
* 名称: void PDL_BT_Init_Fun (unsigned short mSerPar) 
* 功能: 蓝牙
* 入口参数: 设置波特率参数

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_Init_Fun (unsigned short mSerPar) 
{
	BTSysInit();
} 
/****************************************************************************
* 名称: void PDL_BT_SetBTName (unsigned char* mNameBuf)
* 功能: 蓝牙
* 入口参数: 设置波特率参数

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_SetBTName (unsigned char* mNameBuf)
{
	SetBTName(mNameBuf);
} 

/****************************************************************************
* 名称: void PDL_BT_SetBTName (unsigned char* mNameBuf)
* 功能: 蓝牙
* 入口参数: 设置波特率参数

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_SetBTKEY (unsigned char* mNameBuf)
{
	SetBT_KEY(mNameBuf);
} 

/****************************************************************************
* 名称: unsigned int PDL_BT_GetConnect (void) 
* 功能: 得到蓝牙连接状态
* 入口参数: 无

* 出口参数: 返回蓝牙的链接状态
* 说明: 
****************************************************************************/	
extern unsigned short g_Bt_Connect;


unsigned int PDL_BT_GetConnect (void) 
{
	return g_Bt_Connect;
}
/****************************************************************************
* 名称: unsigned int PDL_BT_GetBusy (void) 
* 功能: 得到忙信号输出状态
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
unsigned int PDL_BT_GetBusy (void) 
{
	return 0;
}
/****************************************************************************
* 名称: void PDL_BT_SetBusy (void)
* 功能: 置忙
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_SetBusy (void)
{

}
/****************************************************************************
* 名称: void PDL_BT_ClearBusy (void) 
* 功能: 空闲
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_ClearBusy (void) 
{

}
/****************************************************************************
* 名称: void PDL_BT_WriteChar (unsigned char mSdat)
* 功能: 写一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_WriteChar (unsigned char mSdat) 
{

}
/****************************************************************************
* 名称: void PDL_BT_SendChar (unsigned char mSdat)
* 功能: 发送一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_BT_SendChar (unsigned char mSdat) 
{
	
}
/****************************************************************************
* 名称: unsigned char PDL_BT_ReceivedChar (void)
* 功能: 接收一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
unsigned char PDL_BT_ReceivedChar (void) 
{

}

/****************************************************************************
* 名称: unsigned int PDL_BT_RxInterruptStatus (void)
* 功能: 是否产生有效接收中断
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
unsigned int PDL_BT_RxInterruptStatus (void) 
{
	return 0;
}
/****************************************************************************
* 名称: void PDL_BT_RxInterruptIRQHandler(void)
* 功能: 中断处理函数
* 入口参数: 无

* 出口参数: 
* 说明: 在主程序中需要重新定向
****************************************************************************/	
void PDL_BT_RxInterruptIRQHandler(void)
{
	
}

/****************************************************************************
* 名称: void PDL_BT_ReceiveNBytes (void)
* 功能: 接收N字节数据到打印机缓冲区
* 入口参数: 无

* 出口参数: 
* 说明: 缓冲区不满返回0 缓冲区将满返回1
****************************************************************************/	
unsigned short PDL_BT_ReceiveNBytes (unsigned char *dataBuffer,unsigned short len) 
{

	return 0;
}

/****************************************************************************
* 名称: void PDL_BT_ReturnNBytes (void)
* 功能: 返回n字节
* 入口参数: 无

* 出口参数: 
* 说明: 缓冲区不满返回0 缓冲区将满返回1
****************************************************************************/	
void PDL_BT_ReturnNBytes (unsigned char *dataBuffer,unsigned short len) 
{

	PrintBle_SPP_Send(dataBuffer,len);
}

/****************************************************************************
* 名称: void PDL_BT_ReturnNBytes (void)
* 功能: 返回n字节
* 入口参数: 无

* 出口参数: 
* 说明: 缓冲区不满返回0 缓冲区将满返回1
****************************************************************************/	
extern unsigned short g_ConnectFlag;
void PDL_BT_DisConnect (void) 
{
	if(g_ConnectFlag == 1)
	{
		BT_BtDisconnect();
	}
	if(g_ConnectFlag == 2)
	{
		BT_BleDisconnect();
	}
	
}

/*==================================================================
												WIFI方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_WIFI_Init_Fun (unsigned short mSerPar) 
* 功能: wifi通讯模块初始化
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_WIFI_Init_Fun (unsigned short mSerPar) 
{
	
} 
/****************************************************************************
* 名称: unsigned int PDL_WIFI_GetBusy (void) 
* 功能: 得到忙信号输出状态
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
unsigned int PDL_WIFI_GetBusy (void) 
{
	unsigned int mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}
/****************************************************************************
* 名称: void PDL_WIFI_SetBusy (void)
* 功能: 置忙
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_WIFI_SetBusy (void) 
{

}
/****************************************************************************
* 名称: void PDL_WIFI_ClearBusy (void)
* 功能: 空闲
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_WIFI_ClearBusy (void) 
{

}

/****************************************************************************
* 名称: void PDL_WIFI_SetRest (void)
* 功能: 复位置高
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_WIFI_SetRest (void) 
{

}
/****************************************************************************
* 名称: void PDL_WIFI_ClearRest (void)
* 功能: 复位清零
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_WIFI_ClearRest (void) 
{

}
/****************************************************************************
* 名称: unsigned int PDL_WIFI_GetLink (void) 
* 功能: 得到wifi连接状态
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned int PDL_WIFI_GetLink (void) 
{
	unsigned int mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}
/****************************************************************************
* 名称: void PDL_WIFI_WriteChar (unsigned char mSdat)
* 功能: 写一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/
void PDL_WIFI_WriteChar (unsigned char mSdat) 
{

}
/****************************************************************************
* 名称: void PDL_WIFI_SendChar (unsigned char mSdat)
* 功能: 发送一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_WIFI_SendChar (unsigned char mSdat) 
{

}
/****************************************************************************
* 名称: unsigned char PDL_WIFI_ReceivedChar (void)
* 功能: 得到一个字节数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned char PDL_WIFI_ReceivedChar (void) 
{
	unsigned char mDat;
	mDat = mDat;
	return mDat;
}
/****************************************************************************
* 名称: unsigned int PDL_WIFI_RxInterruptStatus (void)
* 功能: 是否产生有效接收中断
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned int PDL_WIFI_RxInterruptStatus (void) 
{
	unsigned int mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}
/****************************************************************************
* 名称: void PDL_WIFI_RxInterruptIRQHandler(void)
* 功能: 接收中断
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	

void PDL_WIFI_RxInterruptIRQHandler(void)
{
	
}
/*==================================================================
												ADC方法

//			printf("AD4:%d\r\n",m_au16Adc1SaValue[4]);
//			printf("AD6:%d\r\n",m_au16Adc1SaValue[6]);
//			printf("AD8:%d\r\n",m_au16Adc1SaValue[8]);
//			printf("AD9:%d\r\n",m_au16Adc1SaValue[9]);
==================================================================*/
/****************************************************************************
* 名称: void PDL_ADC_Init_Fun (void)
* 功能: 初始化ADC 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_ADC_Init_Fun (void) 
{
	
}
/****************************************************************************
* 名称: void PDL_ADC_Init_Fun (void)
* 功能: 初始化ADC 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_ADC_HVIN_Init_Fun (void) 
{

}
/****************************************************************************
* 名称: void PDL_ADC_Init_Fun (void)
* 功能: 初始化ADC 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_ADC_GPIO_Init_Fun (unsigned short num) 
{
	GPIO_ADC6_Configuration();
}

/****************************************************************************
* 名称: unsigned short PDL_ADC_GetDriveVoltage (void)
* 功能: 得到驱动电压
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_ADC_GetDriveVoltage (void) 
{
	return GetVolteV();
}

/****************************************************************************
* 名称: unsigned short PDL_ADC_GetDriveVoltage (void)
* 功能: 得到驱动电压
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_ADC_GetBattyPer (void) 
{
	return GetBattyPer();
}

/****************************************************************************
* 名称: unsigned short PDL_ADC_GetRefPhotoelectric (void) 
* 功能: 反射光电值
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_ADC_GetRefPhotoelectric (void) 
{
	return 1200 - ADC_Quick_GetVoltage(ADC_CHANNEL_2,ADC_GPIO);
}
/****************************************************************************
* 名称: unsigned short PDL_ADC_GetCorPhotoelectric (void)
* 功能: 对射光电值
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_ADC_GetCorPhotoelectric (void)
{
	return 1200 - ADC_Quick_GetVoltage(ADC_CHANNEL_1,ADC_GPIO);
}
/****************************************************************************
* 名称: unsigned short PDL_ADC_GetPTemperature (void)
* 功能: 得到打印头温度
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_ADC_GetPTemperature (void)
{
	return ADC_Quick_GetVoltage(ADC_CHANNEL_0,ADC_GPIO);
}




/*==================================================================
										加热片控制方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_SDTH_Init_Fun (void)
* 功能: 初始化打印头数据发送 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SDTH_Init_Fun (void) 
{
	SPIPrinter_init();
}
/****************************************************************************
* 名称: PDL_SDTH_SendToHead_u16
* 功能: 发送数据到打印头，发送处于16bit模式
* 入口参数: 无

* 出口参数: 返回数据1的个数
* 说明: 
****************************************************************************/	
unsigned short PDL_SDTH_SendToHead_u16 (unsigned short* mSDBuffer,unsigned short mLen) 
{
	return 0;
}
/****************************************************************************
* 名称: PDL_SDTH_SendToHead_u32
* 功能: 发送数据到打印头，发送处于32bit模式
* 入口参数: 无

* 出口参数: 返回数据1的个数
* 说明: 
****************************************************************************/	
unsigned short PDL_SDTH_SendToHead_u32 (unsigned int* mSDBuffer,unsigned short mLen)
{
	unsigned short mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}
/****************************************************************************
* 名称: PDL_SDTH_DMA_SendToHead_u16
* 功能: DMA发送数据到打印头，发送处于16bit模式
* 入口参数: 无

* 出口参数: 返回数据1的个数
* 说明: 
****************************************************************************/	
unsigned short PDL_SDTH_DMA_SendToHead_u16 (unsigned short* mSDBuffer,unsigned short mLen)
{
	unsigned short mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}
/****************************************************************************
* 名称: PDL_SDTH_SendToHead_u32
* 功能: DMA发送数据到打印头，发送处于32bit模式
* 入口参数: 无

* 出口参数: 返回数据1的个数
* 说明: 
****************************************************************************/	
unsigned short PDL_SDTH_DMA_SendToHead_u32 (unsigned int* mSDBuffer,unsigned short mLen)
{
	unsigned short mStatus;
	mStatus = mStatus;
	
	
	return mStatus;
}
/****************************************************************************
* 名称: unsigned short PDL_SDTH_IS_Send_Empty (void) 
* 功能: 发送是否空，用于判断数据是否发送完成
* 入口参数: 无

* 出口参数: 
* 说明: 非0表示发送完成
****************************************************************************/	
unsigned short PDL_SDTH_IS_Send_Empty (void) 
{
	
	return (DMA_STATUS(SPI0) & 1);
}




/****************************************************************************
* 名称: PDL_SDTH_DMA_SendToHead_u8
* 功能: DMA发送数据到打印头，发送处于32bit模式
* 入口参数: 无

* 出口参数: 返回数据1的个数
* 说明: 
****************************************************************************/	
void PDL_SDTH_DMA_SendToHead_u8(unsigned char* mSDBuffer,unsigned short mLen)
{
	SPI_SendBuff_OR(SPI0,mSDBuffer,mLen);
}





/*==================================================================
											RFID方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_RFID_Init_Fun (void)
* 功能: 初始化RFID 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_RFID_Init_Fun (void) 
{
	#ifdef RFID_FUN_EN
	RFID_IO_Config();
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_VCC_EN (void)
* 功能: 电源选使能
* 入口参数: 无

* 出口参数: 
* 说明: 打开rfid模块的电源
****************************************************************************/	
void PDL_RFID_VCC_EN (void) 
{
	#ifdef RFID_FUN_EN
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_VCC_DIS (void)
* 功能: 电源不使能
* 入口参数: 无

* 出口参数: 
* 说明: 关闭rfid模块电源
****************************************************************************/	
void PDL_RFID_VCC_DIS (void) 
{
	#ifdef RFID_FUN_EN
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_CS_EN (void)
* 功能: 片选使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_RFID_CS_EN (void) 
{
	#ifdef RFID_FUN_EN
	SET_L_O_RFID_CS;
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_CS_DIS (void)
* 功能: 片选不使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_RFID_CS_DIS (void) 
{
	#ifdef RFID_FUN_EN
	SET_H_O_RFID_CS;
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_SetRest (void)
* 功能: 复位 置位
* 入口参数: 无

* 出口参数: 
* 说明: 
***************************************************************************/	
void PDL_RFID_SetRest (void) 
{
	#ifdef RFID_FUN_EN
	SET_H_O_RFID_RST;
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_ClearRest (void)
* 功能: 复位清零
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_RFID_ClearRest (void) 
{
	#ifdef RFID_FUN_EN
	SET_L_O_RFID_RST;
	#endif
}
/****************************************************************************
* 名称: void PDL_RFID_WriteByte (void)
* 功能: 写入一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned char PDL_RFID_WriteByte (unsigned char mRdat) 
{
	#ifdef RFID_FUN_EN
	unsigned char dat;
	
	__SETPRIMASK();
	dat =  RFID_SPI_SendByte(mRdat);
	__RESETPRIMASK();
	
	return dat;
	#endif
	return 0;

}
/****************************************************************************
* 名称: void PDL_RFID_ReadByte (void)
* 功能: 读入一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned char PDL_RFID_ReadByte (void) 
{
	#ifdef RFID_FUN_EN
	unsigned char dat;
	
	__SETPRIMASK();
	dat =  RFID_SPI_SendByte(0xFF);
	__RESETPRIMASK();
	
	return dat;	
	#endif
	return 0;
}
/*==================================================================
											以太网方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_NET_Init_Fun (void)
* 功能: 初始化打印头数据发送 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_NET_Init_Fun (void) 
{

}
/****************************************************************************
* 名称: void PDL_NET_Init_Fun (void)
* 功能: 片选使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/		
void PDL_NET_CS_EN (void) 
{

}
/****************************************************************************
* 名称: void PDL_NET_Init_Fun (void)
* 功能: 片选不使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_NET_CS_DIS (void) 
{

}
/****************************************************************************
* 名称: void PDL_NET_SetRest (void)
* 功能: 复位置位
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_NET_SetRest (void) 
{

}
/****************************************************************************
* 名称: void PDL_NET_ClearRest (void)
* 功能: 复位清零
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_NET_ClearRest (void) 
{

}
/****************************************************************************
* 名称: void PDL_NET_WriteByte (void)
* 功能: 写入一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned char PDL_NET_WriteByte (unsigned char mRdat) 
{
	return mRdat;
}
/****************************************************************************
* 名称: void PDL_NET_ReadByte (void)
* 功能: 读入一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned char PDL_NET_ReadByte (void) 
{
	unsigned char mRdat;
	mRdat = mRdat;
	
	return mRdat;
}
/*==================================================================
										字库系统方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_SPI_Init_Fun (void)
* 功能: 初始化打印头数据发送 模块功能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_Init_Fun (void) 
{
		SPI_Configuration();
}
/****************************************************************************
* 名称: void PDL_SPI_CS_EN (void)
* 功能: 片选使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_CS_EN (void) 
{

}
/****************************************************************************
* 名称: void PDL_SPI_CS_DIS (void)
* 功能: 片选不使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_CS_DIS (void) 
{

}
/****************************************************************************
* 名称: void PDL_SPI_CS_E1_EN (void)
* 功能: 片1选使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_CS_E1_EN (void) 
{

}
/****************************************************************************
* 名称: void PDL_SPI_CS_E1_DIS (void)
* 功能: 片选1不使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_CS_E1_DIS (void) 
{

}

/****************************************************************************
* 名称: void PDL_SPI_SendDat_u32 (void)
* 功能: 发送并返回一个int
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned int PDL_SPI_SendDat_u32 (unsigned int mSdat) 
{
	return mSdat;
}
/****************************************************************************
* 名称: void PDL_NET_ReadByte (void)
* 功能: 发送并返回一个short
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_SPI_SendDat_u16 (unsigned short mSdat) 
{
	return mSdat;
}
/****************************************************************************
* 名称: void PDL_NET_ReadByte (void)
* 功能: 发送并返回一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned char PDL_SPI_SendDat_u8 (unsigned char mSdat) 
{
	return 0;
}
/****************************************************************************
* 名称: void PDL_SPI_SendData (void)
* 功能: 发送一个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_SendData (unsigned char data) 
{
	
	SPI_SendData(SPI1,data);
}


/****************************************************************************
* 名称: void PDL_SPI_SendBuff (void)
* 功能: 发送n个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_SendBuff(unsigned char *buff, int len)
{
	SPI_SendBuff(SPI1,buff,len);
}
/****************************************************************************
* 名称: void PDL_NET_ReadByte (void)
* 功能: 发送并返回n个字节
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SPI_SendAndReceiveData( unsigned char *TxBuff, unsigned short TxLen, unsigned char *RxBuff, unsigned short RxLen)
{
	SPI_SendAndReceiveData(SPI1,TxBuff,TxLen,RxBuff,RxLen);
}











/*==================================================================
													USB方法
==================================================================*/
/****************************************************************************
* 名称: void PDL_USB_Init_Fun (void)
* 功能: 初始化USB 模块功能
* 入口参数: 无
* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_USB_Init_Fun (void) 
{
	usb_main();
}
/****************************************************************************
* 名称: void PDL_USB_POWER_EN (void)
* 功能: 电源使能OTG HOST
* 入口参数: 无

* 出口参数: 
* 说明: 用于给外设输出电流
****************************************************************************/	
void PDL_USB_POWER_EN (void) 
{

}
/****************************************************************************
* 名称: void PDL_USB_POWER_DIS (void)
* 功能: OTG HOST电源不使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_USB_POWER_DIS (void) 
{

}
/****************************************************************************
* 名称: void PDL_USB_Connect (void)
* 功能: 连接使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_USB_Connect (void) 
{

}
/****************************************************************************
* 名称: void PDL_USB_DisConnect (void)
* 功能: 不连接
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_USB_DisConnect (void) 
{

}
/****************************************************************************
* 名称: void PDL_USB_GetUSBDet (void)
* 功能: 判断usb是否插入，是否连接
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_USB_GetUSBDet (void) 
{
	if(GET_I_USB_DET)
	{
		return 0;
	}
	return 1;
}
/****************************************************************************
* 名称: void PDL_USB_ReceiveNBytes (void)
* 功能: 接收N字节数据到打印机缓冲区
* 入口参数: 无

* 出口参数: 
* 说明: 缓冲区不满返回0 缓冲区将满返回1
****************************************************************************/	
unsigned short PDL_USB_ReceiveNBytes (unsigned char *dataBuffer,unsigned short len) 
{

	return 0;
}

/****************************************************************************
* 名称: void PDL_USB_ReturnNBytes (void)
* 功能: 返回N字节数据到主机
* 入口参数: 无

* 出口参数: 
* 说明: 缓冲区不满返回0 缓冲区将满返回1
****************************************************************************/	
unsigned short PDL_USB_ReturnNBytes (unsigned char *dataBuffer,unsigned short len) 
{
	return 0;
}

/****************************************************************************
* 名称: void PDL_USB_ReceiveEPValid (void)
* 功能: /接收端点有效用于使能继续接收数据
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
extern unsigned short g_big_buffer_have_space;
void PDL_USB_ReceiveEPValid (void) 
{
	g_big_buffer_have_space = 0;
}
/****************************************************************************
* 名称: void PDL_USB_Judge_EN_EPValid (void)
* 功能: 用于有效使能，内部判断流控标志
* 入口参数: 无

* 出口参数: 
* 说明: 在数据缓冲区小于预警值调用,如果有标志，则使能，并清除标志
****************************************************************************/	
void PDL_USB_Judge_EN_EPValid (void) 
{
	g_big_buffer_have_space = 0;
}

/****************************************************************************
* 名称: void PDL_SERVICE_Init_Fun (void)
* 功能: 服务性中断初始化，基本就是定时器
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_Init_Fun (void)
{
	
	TIMER3_Configuration();
}

/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_A_EN (void)
* 功能: 中断使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_A_EN (unsigned short TimeCouValue)
{
	EnableTimer(TIM3,TimeCouValue);
}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_A_DIS (void)
* 功能: 中断 禁止
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_A_DIS (void)
{
		DisableTimer(TIM3);
}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_A_Handler (void)
* 功能: 服务中断A处理
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_A_Handler  (void)
{
	
}


/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_B_EN (void)
* 功能: 中断使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_B_EN (unsigned short TimeCouValue)
{
		
}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_B_DIS (void)
* 功能: 中断 禁止
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_B_DIS (void)
{

}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_B_Handler (void)
* 功能: 服务中断A处理
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_B_Handler  (void)
{
	
}


/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_C_EN (void)
* 功能: 中断使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_C_EN (unsigned short TimeCouValue)
{
		
}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_C_DIS (void)
* 功能: 中断 禁止
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_C_DIS (void)
{

}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_C_Handler (void)
* 功能: 服务中断A处理
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_C_Handler  (void)
{
	
}


/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_D_EN (void)
* 功能: 中断使能
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_D_EN (unsigned short TimeCouValue)
{
		
}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_D_DIS (void)
* 功能: 中断 禁止
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_D_DIS (void)
{

}
/****************************************************************************
* 名称: void PDL_SERVICE_ServiceIRQ_D_Handler (void)
* 功能: 服务中断A处理
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_ServiceIRQ_D_Handler  (void)
{
	
}

/****************************************************************************
* 名称: void PDL_SERVICE_PrinterIdle (void)
* 功能: 打印机空闲
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_PrinterIdle  (void)
{

}

/****************************************************************************
* 名称: void PDL_SERVICE_PrinterBusy (void)
* 功能: 打印机忙碌
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_SERVICE_PrinterBusy  (void)
{

}





/****************************************************************************
* 名称: void PDL_IIC_RST_L (void)
* 功能:复位管脚低
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_RST_L (void)
{
	
			SET_L_O_OLED_RST;
	
}
	
/****************************************************************************
* 名称: void PDL_IIC_RST_H (void)
* 功能: 复位管脚高
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_RST_H (void)
{
	
		SET_H_O_OLED_RST;
	
}

/****************************************************************************
* 名称: void PDL_IIC_SCL_L (void)
* 功能: 时钟管脚低
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_SCL_L (void)
{
	
	//		SET_L_O_SCL;
	GPIO_ResetBits(O_SCL_PORT,O_SCL_PIN);
}
	
/****************************************************************************
* 名称: void PDL_IIC_SCL_H (void)
* 功能: 时钟管脚高
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_SCL_H (void)
{
	
		//SET_H_O_SCL;
GPIO_SetBits(O_SCL_PORT,O_SCL_PIN);	
}



/****************************************************************************
* 名称: void PDL_IIC_SDA_L (void)
* 功能: 时钟管脚低
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_SDA_L (void)
{
	
		SET_L_O_SDA;
	
}
	
/****************************************************************************
* 名称: void PDL_IIC_SDA_H (void)
* 功能: 时钟管脚高
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_SDA_H (void)
{
	
		SET_H_O_SDA;
	
}


/****************************************************************************
* 名称: void SDA_InputEn (void)
* 功能: 是能sda输入
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
void PDL_IIC_SDA_InputEn (void)
{
	
	SDA_InputEn();
	
}


/****************************************************************************
* 名称: void PDL_IIC_SDA_read (void)
* 功能: 读取sda
* 入口参数: 无

* 出口参数: 
* 说明: 
****************************************************************************/	
unsigned short PDL_IIC_SDA_read (void)
{
	return GET_SDA_OUT;
}

#ifdef BT_NACK
/**
* @FuncName  BT_SendNACK
* @param     BT_NACK_Reason reason - NACK error condition enum code:
*                                    BT_NACK_INVALID_CMD (0),
*                                    BT_NACK_SIZE_MISMATCH (1),
*                                    BT_NACK_BUFFER_OVERFLOW (2)
* @retval    void
* @LocalVar  char *nack_str - Pointer to NACK response text string
*            char *oled_msg - Pointer to short OLED alert display text string
*            int len        - Length of NACK response string in bytes
*            int m_w        - Computed text width for OLED display centering
* @GlobalVar PRINTER_PDL
* @Description Transmits a Negative Acknowledgment (NACK) string over the active Bluetooth link
*              and renders visual error feedback on the OLED screen.
*
*              Execution Breakdown:
*              1. Error String Mapping:
*                 - `BT_NACK_INVALID_CMD`: Sets `nack_str = "BT_NACK: Invalid Command\r\n"`,`oled_msg = "Invalid Cmd"`.
*                 - `BT_NACK_SIZE_MISMATCH`: Sets `nack_str = "BT_NACK: Label SizeMismatch\r\n"`, `oled_msg = "Size Overflow"`.
*                 - `BT_NACK_BUFFER_OVERFLOW`: Sets `nack_str = "BT_NACK: BufferOverflow\r\n"`, `oled_msg = "Buf Overflow"`.
*              2. OLED Graphic Feedback:
*                 If error is `BT_NACK_SIZE_MISMATCH`, clears line 14 (`oled_ClearMap()`),
*                 centers and displays `oled_msg`, and updates the screen.
*              3. Bluetooth Transmission:
*                 Invokes `PRINTER_PDL.BT._ReturnNBytes((unsigned char *)nack_str, len)` tosend response.
*/
void BT_SendNACK(BT_NACK_Reason reason)
{
	char *nack_str;
	char *oled_msg;
	int len = 0;
	int m_w;
	extern void MyPrintf(char *format, ...);
	extern void DisplayStr8x16char(char * pstr,unsigned char x,unsigned char y,unsigned char m_Type);
	extern unsigned char GetStr8x16Wide(char * pstr,unsigned char m_Type);
	extern void oled_ClearMap(unsigned char x,unsigned char y,unsigned char mClearwide);
	extern void DisplayBufEn(void);
	extern void Delay1ms(unsigned short mdelay);
	extern void OLED_RestoreHomeScreen(void);

	switch (reason) {
		case BT_NACK_INVALID_CMD:
			nack_str = "BT_NACK: Invalid Command\r\n";
			oled_msg = "Invalid Cmd";
			len = 26;
//			MyPrintf("DEBUG: NACK Triggered - Invalid Command\r\n");
			break;
		case BT_NACK_SIZE_MISMATCH:
			nack_str = "BT_NACK: Label Size Mismatch\r\n";
			oled_msg = "Size Overflow";
			len = 30;
//			MyPrintf("DEBUG: NACK Triggered - Size Mismatch\r\n");
			break;
		case BT_NACK_BUFFER_OVERFLOW:
			nack_str = "BT_NACK: Buffer Overflow\r\n";
			oled_msg = "Buf Overflow";
			len = 26;
//			MyPrintf("DEBUG: NACK Triggered - Buffer Overflow\r\n");
			break;
		default:
			nack_str = "BT_NACK: Error\r\n";
			oled_msg = "BT Error";
			len = 16;
//			MyPrintf("DEBUG: NACK Triggered - Unknown Error\r\n");
			break;
	}

	// OLED Display Feedback
	if (reason == BT_NACK_SIZE_MISMATCH)
	{
		oled_ClearMap(0, 14, 128); // Clear the middle line (standard position for status msgs)
		m_w = GetStr8x16Wide(oled_msg, 1);
		DisplayStr8x16char(oled_msg, (128 - m_w) / 2, 14, 1);
		DisplayBufEn();
	}

	if (PRINTER_PDL.BT._ReturnNBytes != 0) {
		PRINTER_PDL.BT._ReturnNBytes((unsigned char *)nack_str, len);
	}

	if (reason == BT_NACK_SIZE_MISMATCH)
	{
		Delay1ms(2000); // Show message for 2 seconds
		//OLED_RestoreHomeScreen(); // Return to previous state (home screen)
	}
}

#endif





