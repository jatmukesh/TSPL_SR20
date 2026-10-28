/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief USB custom hid example.
 **
 **   - 2021-04-16  CDT   First version for USB custom hid device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
 #include "RD_BT_BSP.h"
#include <stdio.h>
#include "RD_V2_DeviceBSP.h"
#include "macros.h"
#include "PDL.h"

/*******************************New Firmware Modification**********************************/
#ifdef DOUBLE_TAP_SELFTEST_PRINT
volatile uint8_t g_K2_Event = 0; 

typedef enum {
    K2_IDLE, 								// Waiting for initial press
		K2_DEBOUNCE_PRESS, 			// filtering noise on press
		K2_WAIT_RELEASE, 				// Button confirmed down(pressed) ; Timing for tap vs hold
    K2_DEBOUNCE_RELEASE, 		// filtering noise on release
		K2_WAIT_SECOND_TAP,			// Monitoring 300ms window for double-tap detection
		K2_HOLD									// K2 button held > 500ms continous paper feed action
} K2_STATE_T;

extern _PDL		PRINTER_PDL ;
#endif

#ifdef BATTERY_INDICATION_SOLUTION

#define BATTERY_BAR_CAL_GAIN_PERMILLE  1422UL
#define BATTERY_BAR_CAL_OFFSET_MV      3026000UL

static unsigned short BatteryBar_CalibrateVoltage(unsigned short measured_mV);
#endif	


//-----------------------------------------------------------------------------------------------------------

extern const unsigned int PrintMotoHalfStep[8];
unsigned short g_ADCValue[3];
extern 	uint8_t chip_id[6];
void vcard_control(uint8_t state)
{
    if (state == 1)
    {
        SYSCTRL_HWCTRL(0) |= 0x1f;
    }
    else if (state == 2)
    {
        SYSCTRL_HWCTRL(0) &= 0xef;
        SYSCTRL_HWCTRL(0) |= 0x8;
    }
    else
    {
        SYSCTRL_HWCTRL(0) &= 0xf7;
    }
}	


Boolean GetUSBCharge(void)
{

}


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

	WDT_ReloadCounter();
	//MOTO
	GPIO_InitStruct.GPIO_Pin = O_MOTO1_PIN|O_MOTO2_PIN|O_MOTO3_PIN|O_MOTO4_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_MOTO1_PORT, &GPIO_InitStruct);	
	//LAT
	GPIO_InitStruct.GPIO_Pin = O_LAT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_LAT_PORT, &GPIO_InitStruct);		
	//STB
	GPIO_InitStruct.GPIO_Pin = O_STB1_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_STB1_PORT, &GPIO_InitStruct);				

	
	// USB -----检测--
	GPIO_InitStruct.GPIO_Pin = I_USB_DET_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(I_USB_DET_PORT, &GPIO_InitStruct);
	
	// 胶辊--检测---------
	GPIO_InitStruct.GPIO_Pin = I_HS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(I_HS_PORT, &GPIO_InitStruct);	
	//按键
	GPIO_InitStruct.GPIO_Pin = I_LF_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(I_LF_PORT, &GPIO_InitStruct);		

	GPIO_InitStruct.GPIO_Pin = I_POW_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(I_POW_PORT, &GPIO_InitStruct);		



	//蜂鸣器
	GPIO_InitStruct.GPIO_Pin = O_BEEP_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_BEEP_PORT, &GPIO_InitStruct);



	//SWITCH
	GPIO_InitStruct.GPIO_Pin = O_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_SWITCH_PORT, &GPIO_InitStruct);	
	//POWER EN
	GPIO_InitStruct.GPIO_Pin = O_POWER_EN_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_POWER_EN_PORT, &GPIO_InitStruct);	

	//模拟输入 //温度检测
	GPIO_Config(A_TM_PORT, A_TM_PIN, ANALOG);
	GPIO_Config(A_DPS_PORT, A_DPS_PIN, ANALOG);	
	GPIO_Config(A_LPS_PORT, A_LPS_PIN, ANALOG);	
	GPIO_Config(A_VOL_PORT, A_VOL_PIN, ANALOG);	



	//FLASH CS
	GPIO_InitStruct.GPIO_Pin = O_MX25_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_MX25_CS_PORT, &GPIO_InitStruct);		

	//充电
	GPIO_InitStruct.GPIO_Pin = I_CHARGE_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(I_CHARGE_PORT, &GPIO_InitStruct);
	
	
	//OLED
	GPIO_InitStruct.GPIO_Pin = O_SDA_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_SDA_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = O_SCL_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_SCL_PORT, &GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Pin = O_OLED_RST_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(O_OLED_RST_PORT, &GPIO_InitStruct);	
	

	SET_L_O_POWER_EN;
	SET_L_O_SWITCH;
	SET_L_O_STB1;
	SET_L_O_STB2;
	SET_L_O_BEEP;

}

void SDA_InputEn (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = O_SDA_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(O_SDA_PORT, &GPIO_InitStruct);
}
void WDT_Configuration(void)
{
    WDT_SetReload(29);////(2^(26-1))/96*1000000 = 0.699(s)
    WDT_ModeConfig(WDT_CPUReset);
    WDT_Enable();
}
void WDG_ResetCounter(void)
{
	WDT_ReloadCounter();
}

unsigned short g_pow_key_Cont = 0;

unsigned short ReadPOWKeyPlus(void)
{
	unsigned short i,temp,key_Trg;
star_key:

	key_Trg = GET_I_KEY_POW;
	for(i=0;i<800;i++)
	{
		WDG_ResetCounter();
	}
	if(GET_I_KEY_POW != key_Trg)
	{
		goto star_key;
	}
	for(i=0;i<1000;i++)
	{
		WDG_ResetCounter();
	}
	if(GET_I_KEY_POW != key_Trg)
	{

		goto star_key;
	}

	temp = key_Trg^0xFFFF;// 1
	key_Trg = temp & (temp ^ g_pow_key_Cont);// 2
	g_pow_key_Cont = temp;// 3


	if(key_Trg & I_POW_PIN)
	{
		return 1;
	}
	return 0;
}


unsigned short g_LF_key_Cont = 0;

unsigned short ReadLFKeyPlus(void)
{
	unsigned short i,temp,key_Trg;
star_key:

	key_Trg = GET_I_KEY_LF;
	for(i=0;i<800;i++)
	{
		WDG_ResetCounter();
	}
	if(GET_I_KEY_LF != key_Trg)
	{
		goto star_key;
	}
	for(i=0;i<1000;i++)
	{
		WDG_ResetCounter();
	}
	if(GET_I_KEY_LF != key_Trg)
	{

		goto star_key;
	}

	temp = key_Trg^0xFFFF;// 1
	key_Trg = temp & (temp ^ g_LF_key_Cont);// 2
	g_LF_key_Cont = temp;// 3


	if(key_Trg & I_LF_PIN)
	{
		return 1;
	}
	return 0;
}

void SPIPrinter_init(void)
{
    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.CPHA = SPI_CPHA_First_Edge;
    SPI_InitStruct.CPOL = SPI_CPOL_Low;
    SPI_InitStruct.Mode =  SPI_Mode_Master;
    SPI_InitStruct.RW_Delay = 5;
    GPIO_Config(O_CLK_PORT, O_CLK_PIN, SPID0_SCK);
    GPIO_Config(O_DI_PORT, O_DI_PIN, SPID0_MOSI);
    SPI_Init(SPI0, &SPI_InitStruct);
}


void SPI_Configuration(void)
{
    SPI_InitTypeDef SPI_InitStruct;

    GPIO_Config(O_MX25_SO_PORT, O_MX25_SO_PIN, SPID1_MISO);
    GPIO_Config(I_MX25_SI_PORT, I_MX25_SI_PIN, SPID1_MOSI);
    GPIO_Config(O_MX25_CS_PORT, O_MX25_CS_PIN, SPID1_NCS);
    GPIO_Config(O_MX25_SCK_PORT, O_MX25_SCK_PIN, SPID1_SCK);

    SPI_InitStruct.Mode = SPI_Mode_Master;
    SPI_InitStruct.BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct.CPHA = SPI_CPHA_First_Edge;
    SPI_InitStruct.CPOL = SPI_CPOL_Low;
    SPI_InitStruct.RW_Delay = 33;

    SPI_Init(SPI1, &SPI_InitStruct);
}

/**
  * @brief  Serial port 0 initialization function.
  * @param  None
  * @retval None
  */
void UART1_Configuration(unsigned int mBaud)
{
	
}

/**
  * @brief  UART1 interrupt service function.
  * @param  None
  * @retval None
  */

extern void RS23_IRQ_DataRecive(void);
void UART1_IRQHandler(void)
{
    if (UART_GetITIdentity(UART1) == UART_IT_RX)
    {
       // UART_ClearIT(UART1);
     //  MyPrintf("UART1 RX data:\n");
       // while(UART_IsRXFIFONotEmpty(UART1))
        {
					//dat = UART_ReceiveData(UART1);
					//RS23_IRQ_DataRecive();
           // MyPrintf("%c",dat);
        }

    }
//    else if (UART_GetITIdentity(UART1) == UART_IT_TX)
//    {
//        UART_ClearIT(UART1);
//        MyPrintf("UART1 TX INTERRUPT test successful!\n");
//    }
}




/**
  * @brief  Serial port 0 initialization function.
  * @param  None
  * @retval None
  */
void Debug_Configuration(void)
{
	UART_InitTypeDef UART_InitStruct;

	/* Configure serial ports 0 RX and TX for IO. */
	GPIO_Config(O_TXD0_PORT, O_TXD0_PIN, UART0_TXD);
	GPIO_Config(I_RXD0_PORT, I_RXD0_PIN, UART0_RXD);

	/* USARTx configured as follow:
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */

	UART_InitStruct.BaudRate = 256000;

	UART_InitStruct.DataBits = Databits_8b;
	UART_InitStruct.StopBits = StopBits_1;
	UART_InitStruct.Parity = Parity_None;
	UART_InitStruct.FlowCtrl = FlowCtrl_None;
	UART_InitStruct.Mode = Mode_duplex;

	UART_Init(UART0, &UART_InitStruct);
}


#if 1

#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
int _ttywrch(int ch)
{
	ch = ch;
	return ch;
}
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	unsigned char dat;
	dat = ch;
	 UART_SendBuf(UART0, &dat, 1);
	return ch;
}
#endif
/**
  * @brief  TIM configuration function.
  * @param  None
  * @retval None
  */
void TIMER_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 96000;

    TIM_InitStruct.TIMx = TIM0;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM1;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM2;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM3;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM4;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM5;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM6;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM7;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM8;
    TIM_Init(&TIM_InitStruct);

    /* Configure timer for counting mode */
    TIM_ModeConfig(TIM0, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM1, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM2, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM3, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM4, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM5, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM6, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM7, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM8, TIM_Mode_TIMER);

    /* The last step must be enabled */
    TIM_Cmd(TIM0, DISABLE);
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
    TIM_Cmd(TIM5, DISABLE);
    TIM_Cmd(TIM6, DISABLE);
    TIM_Cmd(TIM7, DISABLE);
    TIM_Cmd(TIM8, DISABLE);
		
		TIM_SetPeriod(TIM0,96000);
		
		/*
		* mukesh - TIM0 will generate an interrupt every 1ms 
		* TIM0 is the background 1 ms system tick. Keep it below TIM3 so it cannot extend a print-head heat pulse
		* mukesh -  changed priority from 0 -> 1
		*/
    NVIC_EnableIRQ(TIM0_IRQn);
    NVIC_SetPriority(TIM0_IRQn, 1); // changed from 0 -> 1

    NVIC_EnableIRQ(TIM1_IRQn);
    NVIC_SetPriority(TIM1_IRQn, 0);

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);

    NVIC_EnableIRQ(TIM4_IRQn);
    NVIC_SetPriority(TIM4_IRQn, 0);

    NVIC_EnableIRQ(TIM5_IRQn);
    NVIC_SetPriority(TIM5_IRQn, 0);

    NVIC_EnableIRQ(TIM6_IRQn);
    NVIC_SetPriority(TIM6_IRQn, 0);

    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, 0);

    NVIC_EnableIRQ(TIM8_IRQn);
    NVIC_SetPriority(TIM8_IRQn, 0);		


		 TIM_Cmd(TIM0, ENABLE);
}


/**
  * @brief  TIM configuration function.
  * @param  None
  * @retval None
  */
void System_TIMER_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 96000;

    TIM_InitStruct.TIMx = TIM0;
    TIM_Init(&TIM_InitStruct);

    /* Configure timer for counting mode */
    TIM_ModeConfig(TIM0, TIM_Mode_TIMER);
	
			
    NVIC_EnableIRQ(TIM0_IRQn);
    NVIC_SetPriority(TIM0_IRQn, 3);
	
    /* The last step must be enabled */
 		 TIM_Cmd(TIM0, ENABLE);
}


void Printer_TIMER_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 96000;

    TIM_InitStruct.TIMx = TIM1;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM2;
    TIM_Init(&TIM_InitStruct);	
	
    /* Configure timer for counting mode */
		TIM_ModeConfig(TIM1, TIM_Mode_TIMER);
		TIM_ModeConfig(TIM2, TIM_Mode_TIMER);
		
			
    NVIC_EnableIRQ(TIM1_IRQn);
    NVIC_SetPriority(TIM1_IRQn, 1);

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 2);
	
    /* The last step must be enabled */
 		 TIM_Cmd(TIM1, DISABLE);
		 TIM_Cmd(TIM2, DISABLE);
}
void TIMER3_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 96000;


	
    TIM_InitStruct.TIMx = TIM3;
    TIM_Init(&TIM_InitStruct);	
	
	
    /* Configure timer for counting mode */

		TIM_ModeConfig(TIM3, TIM_Mode_TIMER);
		
			
		/* 
		TIM3 controls print-head latch/strobe edges. It has the unique highest priority so a delayed edge cannot corrupt raster rows. 
		mukesh -  changed priority form 1 -> 0
	*/
		NVIC_EnableIRQ(TIM3_IRQn);
		NVIC_SetPriority(TIM3_IRQn, 0);	// changed from 1 -> 0		
	
    /* The last step must be enabled */

		 TIM_Cmd(TIM3, DISABLE);
		 
		 
		 
		 
}


void PWM_Configuration(void)
{
    PWM_InitTypeDef PWM_init_struct;
    GPIO_Config(O_MOTO1_PORT, O_MOTO1_PIN, GPCFG_PWM_OUT5);
    GPIO_Config(O_MOTO2_PORT, O_MOTO2_PIN, GPCFG_PWM_OUT6);	
    GPIO_Config(O_MOTO3_PORT, O_MOTO3_PIN, GPCFG_PWM_OUT7);
    GPIO_Config(O_MOTO4_PORT, O_MOTO4_PIN, GPCFG_PWM_OUT8);	

	//----------------------TIM5------------------------
    PWM_init_struct.TIMx = TIM5;
    PWM_init_struct.LowLevelPeriod = 0;
    PWM_init_struct.HighLevelPeriod = 0;
    PWM_init_struct.SatrtLevel = OutputLow;
    TIM_PWMInit(&PWM_init_struct);
    /* Configure PWM for counting mode */
    TIM_ModeConfig(TIM5, TIM_Mode_PWM);
    /* The last step must be enabled */
    TIM_Cmd(TIM5, ENABLE);
	
	//----------------------TIM6------------------------
    PWM_init_struct.TIMx = TIM6;
    PWM_init_struct.LowLevelPeriod = 0;
    PWM_init_struct.HighLevelPeriod = 0;
    PWM_init_struct.SatrtLevel = OutputLow;
    TIM_PWMInit(&PWM_init_struct);
    /* Configure PWM for counting mode */
    TIM_ModeConfig(TIM6, TIM_Mode_PWM);
    /* The last step must be enabled */
    TIM_Cmd(TIM6, ENABLE);
	
	//----------------------TIM7------------------------
    PWM_init_struct.TIMx = TIM7;
    PWM_init_struct.LowLevelPeriod = 0;
    PWM_init_struct.HighLevelPeriod = 0;
    PWM_init_struct.SatrtLevel = OutputLow;
    TIM_PWMInit(&PWM_init_struct);
    /* Configure PWM for counting mode */
    TIM_ModeConfig(TIM7, TIM_Mode_PWM);
    /* The last step must be enabled */
    TIM_Cmd(TIM7, ENABLE);
	
	//----------------------TIM8------------------------
    PWM_init_struct.TIMx = TIM8;
    PWM_init_struct.LowLevelPeriod = 0;
    PWM_init_struct.HighLevelPeriod = 0;
    PWM_init_struct.SatrtLevel = OutputLow;
    TIM_PWMInit(&PWM_init_struct);
    /* Configure PWM for counting mode */
    TIM_ModeConfig(TIM8, TIM_Mode_PWM);
    /* The last step must be enabled */
    TIM_Cmd(TIM8, ENABLE);
	//---------------------
	//---------------------
	//---------------------
	//---------------------

//	TIM_SetPWMPeriod(TIM5,0,0);
//	TIM_SetPWMPeriod(TIM6,0,0);
//	TIM_SetPWMPeriod(TIM7,0,0);
//	TIM_SetPWMPeriod(TIM8,0,0);
	TIM_SetPWMPeriod(TIM5,96*100,0);	
	TIM_SetPWMPeriod(TIM6,96*100,0);
	TIM_SetPWMPeriod(TIM7,96*100,0);	
	TIM_SetPWMPeriod(TIM8,96*100,0);
		
	
}

//按照us为基准进行设置
void EnableTimer(TIM_NumTypeDef TIMx,unsigned int mperiod)
{
	TIM_Cmd(TIMx, DISABLE);
	TIM_SetPeriod(TIMx,mperiod*96);
	TIM_Cmd(TIMx, ENABLE);
}

void DisableTimer(TIM_NumTypeDef TIMx)
{
	TIM_Cmd(TIMx, DISABLE);
}



/**
  * @brief  TIM0~TIM8 Interrupt service function.
  * @param  None
  * @retval None
  */
unsigned short g_ADContinueFlag;
extern void Pro_SysTick_IrqHandler(void);
extern void BT_Progress();
unsigned short g_BTCount;

unsigned short g_UsbStick;
extern void usb_last_packet(void);
extern int usb_get_buffer(uint8_t *buf);

extern unsigned short USB_IRQ_DataRecive(unsigned char *mbuff,unsigned short mlen);
uint8_t buf[65] = {0};
extern int buflen();
unsigned short g_big_buffer_have_space = 0;
unsigned int g_totaldatanum;
#include "PrinterParType.h"
unsigned short g_ADValueCol[4];
unsigned short g_ADValueCount;
extern unsigned int 	Receive_count;
extern unsigned short g_Bt_Connect;

void TIMER0_IRQHandler(void)
{
	//1mS采样一次
	  uint8_t recvlen,i;
	Pro_SysTick_IrqHandler();

	g_UsbStick++;
	if(g_UsbStick&2)
	{
		g_UsbStick = 0;
		usb_last_packet();

		recvlen = buflen();
		if (recvlen > 0)
		{
			if (g_big_buffer_have_space == 0)
			{
				/*receive data in to big buffer*/
				recvlen = usb_get_buffer(buf);
				g_totaldatanum += recvlen;
				g_big_buffer_have_space = USB_IRQ_DataRecive(buf,recvlen);
			}
		}
	}
	g_BTCount++;
	if(g_BTCount > 3)
	{
		g_BTCount = 0;

			if(Receive_count < PRINTER_RECEIVE_L)
			{
				PrintBle_FC_Send();	
				PrintSPP_FC_Send();
			}
		
			BT_Progress();
		//------------------------------------------
	}
	//
}
extern void PrintRun_IrqHandler(void);
void TIMER1_IRQHandler(void)
{
	PrintRun_IrqHandler();
}
extern void Pro_LineData_IrqHandler(void);
void TIMER2_IRQHandler(void)
{
	Pro_LineData_IrqHandler();
}

extern void Pro_ServiceIRQ_A_Handler(void);
void TIMER3_IRQHandler(void)
{
	Pro_ServiceIRQ_A_Handler();
}


void TIMER4_IRQHandler(void)
{

}

void TIMER5_IRQHandler(void)
{

}

void TIMER6_IRQHandler(void)
{

}

void TIMER7_IRQHandler(void)
{

}

void TIMER8_IRQHandler(void)
{

}


void HVIN_ADC_Configuration(void)
{
	ADC_InitTypeDef ADCInitStruct;
	__SETPRIMASK(); 
	g_ADContinueFlag = 0;
	ADCInitStruct.ADC_Channel = ADC_CHANNEL_0;
	ADCInitStruct.ADC_Mode = ADC_HVIN;
	ADC_Init(&ADCInitStruct);
	__RESETPRIMASK();
}

uint16_t  Get_HVIN_ADC_Value(void)
{
	uint16_t V = 0;
//	delay_ms(500);
	V = ADC_GetVoltage(ADC_CHANNEL_0);
#ifdef MY_DEBUG_ENABLE	
	MyPrintf("ADC_Result:%d mV\n", V);
#endif	
	return V;
}

/**
  * @brief  ADC initialization function.
  * @param  None
  * @retval None
  */
void GPIO_ADC6_Configuration(void)
{
	ADC_InitTypeDef ADCInitStruct;
	g_ADContinueFlag = 0;
	
	GPIO_Config(A_TM_PORT, A_TM_PIN, ANALOG);
	GPIO_Config(A_DPS_PORT, A_DPS_PIN, ANALOG);	
	GPIO_Config(A_LPS_PORT, A_LPS_PIN, ANALOG);	
	GPIO_Config(A_VOL_PORT, A_VOL_PIN, ANALOG);	
	
	
	
	ADCInitStruct.ADC_Channel = ADC_CHANNEL_6;
	ADCInitStruct.ADC_Mode = ADC_GPIO;
	ADC_Init(&ADCInitStruct);
}
			


uint16_t  Get_GPIO_ADC_Value(ADC_ChxTypeDef ADC_Channel)
{
	return ADC_Quick_GetVoltage(ADC_Channel,ADC_GPIO);
}


uint16_t  Get_LPS_ADC_Value(void)
{

		return g_ADCValue[0];//;


}



#if 1

/*          +50
-20		269  	1.012
-15		208 	0.967
-10		178 	0.937
-5		124 	0.855
0			100 	0.800
5			 78   0.731
10		 60   0.656 
15		47.1  0.582
20		37.5  0.514
25		30.0  0.450
30		24.2  0.391
35		19.6  0.338
40		15.9  0.290
45		13.1  0.249
50		10.8  0.213
55		8.91 	0.181
60		7.41 	0.155
65		6.2   0.132
70		5.21  0.113
75		4.4   0.097

*/

unsigned short TMDetStand[]= {97,113,132,155,181,213,249,338,391,450,514,582,656,731,800,855,937,967,1012};
unsigned short g_TMVoltage;

unsigned short GetCurrentTM(void)
{
	unsigned int data,k;

	data =  ADC_Quick_GetVoltage(ADC_CHANNEL_0,ADC_GPIO);
	
	k = 0;
	while(1)
	{
		if(data < TMDetStand[k])
		{
			break;
		}
		k++;
		if(k > 19)
		{
			k = 19;
			break;
		}
	}
	if(k)
	{
		k--;
	}
	g_TMVoltage = data;
	return k;
}

#endif

/*
 * The protected firmware converts the thermistor ADC sample to temperature
 * through this calibrated lookup table; the legacy build does not include it.
 */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
static const unsigned short Thermistor_ADC_Table[] =
{
    /*  0癈 */ 684,
    /*  1癈 */ 672,
    /*  2癈 */ 660,
    /*  3癈 */ 648,
    /*  4癈 */ 636,
    /*  5癈 */ 623,
    /*  6癈 */ 611,
    /*  7癈 */ 598,
    /*  8癈 */ 586,
    /*  9癈 */ 573,
    /* 10癈 */ 561,
    /* 11癈 */ 548,
    /* 12癈 */ 536,
    /* 13癈 */ 523,
    /* 14癈 */ 511,
    /* 15癈 */ 499,
    /* 16癈 */ 487,
    /* 17癈 */ 475,
    /* 18癈 */ 463,
    /* 19癈 */ 451,
    /* 20癈 */ 440,
    /* 21癈 */ 428,
    /* 22癈 */ 417,
    /* 23癈 */ 406,
    /* 24癈 */ 395,
    /* 25癈 */ 384,
    /* 26癈 */ 374,
    /* 27癈 */ 363,
    /* 28癈 */ 353,
    /* 29癈 */ 343,
    /* 30癈 */ 333,
    /* 31癈 */ 324,
    /* 32癈 */ 314,
    /* 33癈 */ 305,
    /* 34癈 */ 296,
    /* 35癈 */ 288,
    /* 36癈 */ 279,
    /* 37癈 */ 271,
    /* 38癈 */ 263,
    /* 39癈 */ 255,
    /* 40癈 */ 247,
    /* 41癈 */ 240,
    /* 42癈 */ 233,
    /* 43癈 */ 226,
    /* 44癈 */ 219,
    /* 45癈 */ 212,
    /* 46癈 */ 206,
    /* 47癈 */ 199,
    /* 48癈 */ 193,
    /* 49癈 */ 187,
    /* 50癈 */ 182,
    /* 51癈 */ 176,
    /* 52癈 */ 171,
    /* 53癈 */ 165,
    /* 54癈 */ 160,
    /* 55癈 */ 155,
    /* 56癈 */ 151,
    /* 57癈 */ 146,
    /* 58癈 */ 142,
    /* 59癈 */ 137,
    /* 60癈 */ 133,
    /* 61癈 */ 129,
    /* 62癈 */ 125,
    /* 63癈 */ 121,
    /* 64癈 */ 118,
    /* 65癈 */ 114,
    /* 66癈 */ 111,
    /* 67癈 */ 107,
    /* 68癈 */ 104,
    /* 69癈 */ 101,
    /* 70癈 */  98,
    /* 71癈 */  95,
    /* 72癈 */  92,
    /* 73癈 */  89,
    /* 74癈 */  87,
    /* 75癈 */  84,
    /* 76癈 */  82,
    /* 77癈 */  79,
    /* 78癈 */  77,
    /* 79癈 */  75,
    /* 80癈 */  73
};

unsigned short Read_Thermistor_Temperature(unsigned short adc_value)
{
    unsigned short low;
    unsigned short high;
    unsigned short mid;

    unsigned short adc_at_low_temp;
    unsigned short adc_at_high_temp;

    unsigned short temperature;
    unsigned short fraction;

#ifdef TEMP_SET_ADC_VALUE_DEFAULT80_ISSUE	
    /* ADC above 0癈 point -> temperature below 0癈 */
    if (adc_value >= Thermistor_ADC_Table[0])
    {
        return 0;
    }

    /* ADC below 80癈 point -> temperature above 80癈 */
    if (adc_value <= Thermistor_ADC_Table[80])
    {
        return 800;
    }
#endif
    low = 0;
    high = 80;

    /*
     * Binary search.
     *
     * ADC decreases as temperature increases.
     */
    while ((high - low) > 1)
    {
        mid = (low + high) / 2;

        if (adc_value > Thermistor_ADC_Table[mid])
        {
            high = mid;
        }
        else
        {
            low = mid;
        }
    }

    adc_at_low_temp  = Thermistor_ADC_Table[low];
    adc_at_high_temp = Thermistor_ADC_Table[high];

    /*
     * Whole-degree temperature.
     *
     * Example:
     * low = 54
     * high = 55
     */
    temperature = low * 10;

    /*
     * Calculate tenths of a degree.
     */
    fraction =((adc_at_low_temp - adc_value) * 10)/(adc_at_low_temp - adc_at_high_temp);

    temperature += fraction;

    return temperature;
}



#endif


//对输入电压进行AD采样后中值滤波
#define N 11
#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
static BatteryVoltageDebugSample s_LastBatteryDebugSample;
extern unsigned int g_sysTime;
void BatteryVoltageDebug_GetSnapshot(BatteryVoltageDebugSample *sample)
{
    *sample = s_LastBatteryDebugSample;
}
#endif

#ifndef DISPLAY_FREEZE_ISSUE_SOLUTION
unsigned short GetVolteV(void)
{
	unsigned short value_buf[N];
	unsigned short count,i,j,temp,calculated_Batt_Vol,corrected_Batt_Vol;
	unsigned int sum;
	for ( count=0;count<N;count++)
	{
		value_buf[count] = Get_GPIO_ADC_Value(ADC_CHANNEL_6);
		for(j=0;j<200;j++);
		WDG_ResetCounter();
	}
	//Bubble sort 
	for (j=0;j<N-1;j++)
	{
		WDG_ResetCounter();
		for (i=0;i<N-j-1;i++)
		{
			if ( value_buf[i]>value_buf[i+1] )
			{
				temp = value_buf[i];
				value_buf[i] = value_buf[i+1];
				value_buf[i+1] = temp;
			}
		}
	}
	

	calculated_Batt_Vol = value_buf[(N-1)/2]; 	// mukesh - Picks the middle element (index 5, since N=11). data hold the ADC value now 
	sum = (calculated_Batt_Vol * 17)/2;
	calculated_Batt_Vol = sum;

	corrected_Batt_Vol = BatteryBar_CalibrateVoltage(calculated_Batt_Vol);

	
	
#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
    s_LastBatteryDebugSample.adc_pin_mV = value_buf[(N-1)/2];
    s_LastBatteryDebugSample.raw_pack_mV = calculated_Batt_Vol;
    s_LastBatteryDebugSample.corrected_pack_mV = corrected_Batt_Vol;
    s_LastBatteryDebugSample.sample_time_ms = g_sysTime;
    s_LastBatteryDebugSample.valid = 1;
#endif
	return calculated_Batt_Vol;
}
#else
unsigned short GetVolteV(void)
{
	unsigned short value_buf[N];
	unsigned short count,i,j,temp,data;
	unsigned short corrected_Batt_Vol;
	unsigned int sum;
	for ( count=0;count<N;count++)
	{
		value_buf[count] = Get_GPIO_ADC_Value(ADC_CHANNEL_6);
		for(j=0;j<200;j++);
		WDG_ResetCounter();
	}
	for (j=0;j<N-1;j++)
	{
		WDG_ResetCounter();
		for (i=0;i<N-j;i++)
		{
			if ( value_buf[i]>value_buf[i+1] )
			{
				temp = value_buf[i];
				value_buf[i] = value_buf[i+1];
				value_buf[i+1] = temp;
			}
		}
	}
	

	data = value_buf[(N-1)/2];
	sum = (data * 17)/2;
	data = sum;

#if defined(mukesh_DEBUG_BATTERY_IDLE) || defined(mukesh_DEBUG_BATTERY_PRINT)
		corrected_Batt_Vol = BatteryBar_CalibrateVoltage(data);
    s_LastBatteryDebugSample.adc_pin_mV = value_buf[(N-1)/2];
    s_LastBatteryDebugSample.raw_pack_mV = corrected_Batt_Vol;
    s_LastBatteryDebugSample.corrected_pack_mV = corrected_Batt_Vol;
    s_LastBatteryDebugSample.sample_time_ms = g_sysTime;
    s_LastBatteryDebugSample.valid = 1;
#endif	
	
	return data;
}
#endif

/*
	8.50	255

19	8.25	255
				250
18	8.00	247
				243
17	7.75	239
				235
16	7.50	231
				228
15	7.25	224
				220
14	7.00	216
				212
13	6.75	208
				205
12	6.50	201
				197
11	6.25	193
				190
10	6.00	185
				182
9	5.75	178
				175
8	5.50	170
				167
7	5.25	162
				159
6	5.00	155
				150
5	4.75	147
				145
4	4.50	139
				136
3	4.25	131
				128
2	4.00	123
				120
1	3.75	116
				112
0	3.50	108
*/
const unsigned short VolDetStand[]= {3500,3750,4000,4250,4500,4750,5000,5250,5500,5750,6000,6250,6500,6750,7000,7250,7500,7750,8000,8250,8500,8750};

unsigned char GetCurrentVol(void)
{
	unsigned int data,k;
	unsigned int sum;

	WDG_ResetCounter();

	data = GetVolteV();
	if(data > 8500)
	{
		data = 8500; 
	}


	k = 0;
	while(1)
	{
		if(data < VolDetStand[k])
		{
			break;
		}
		k++;
	}
	if(k)
	{
		k--;
	}
	if((k + 2 ) > 19)
	{
		k = 19-2;
	}

	if(k> 1)
	{
		k -= 1;
	}

	return k;
}



//8v 以上100%
//7.5v 以上75%
//7.3v y以上50%
//7.1v y以上25%
//3.95  7.9 以上 百分之66% -- 100% 显示3格
//3.74  7.48 以上  百分之33% -- 66%  显示 2格
//3.62  7.24 以上 百分之0% ---- 33%  显示 1格
//3.62  7.24 以下 闪烁提示
//3.44  6.88 以下 自动关机
//#define		BATTERY_POWER_100		8000
//#define		BATTERY_POWER_75		7500
//#define		BATTERY_POWER_50		7300
//#define		BATTERY_POWER_25		7100
//#define		BATTERY_POWER_00		6900
unsigned short g_Battycal;
unsigned char GetBattyPer(void)
{
#ifdef BATTERY_INDICATION_SOLUTION	
	unsigned char dat;
	unsigned short cal1;
	unsigned short raw_Batt_voltage;
	unsigned short cal_before;
	raw_Batt_voltage = GetVolteV();
	//cal1 = raw_Batt_voltage;
	cal1 = BatteryBar_CalibrateVoltage(raw_Batt_voltage);
	cal_before = g_Battycal;

	/*
	 * The print-bar state machine confirms a changed level twice, three
	 * seconds apart. Do not retain an old voltage here: a gradual recovery
	 * smaller than 100 mV per sample can otherwise remain below a bar
	 * threshold forever and the continuous idle monitor can never see it.
	 */
	
	g_Battycal = cal1;
	cal1 = g_Battycal;
	
	if(cal1 <7100)
	{
		dat = 0;
	}
	else if(cal1 < 7300)
	{
		dat = 1;
	}
	else if(cal1 < 7500)
	{
		dat = 2;
	}
	else if(cal1 < 8000)
	{
		dat = 3;
	}
	else 
	{
		dat = 4;
	}

		#ifdef DEBUG_BATTERY_INDICATION_FIX
			/* Log only a meaningful voltage/filter/level change to avoid UART flooding during printing. */
			{
				static unsigned short last_raw = 0xFFFF;
				static unsigned short last_cal = 0xFFFF;
				static unsigned char last_level = 0xFF;
				if ((last_raw == 0xFFFF) || (raw_voltage > (last_raw + 100)) || (last_raw > (raw_voltage + 100)) || (g_Battycal != last_cal) || (dat != last_level))
				{
					printf("BAT_ADC: raw=%u cal_before=%u cal_after=%u level=%u\r\n",
						raw_voltage, cal_before, g_Battycal, dat);
					last_raw = raw_voltage;
					last_cal = g_Battycal;
					last_level = dat;
				}
			}
		#endif
	return dat;
#else	   //Old Code
	unsigned char dat;
	unsigned short cal1;
	cal1 = GetVolteV();

	
	if(g_Battycal > (cal1 + 100))
	{
		g_Battycal = cal1;
	}
	else if(cal1 > ( g_Battycal + 100))
	{
		g_Battycal = cal1;
	}
	cal1 = g_Battycal;
	
	if(cal1 <7100)
	{
		dat = 0;
	}
	else if(cal1 < 7300)
	{
		dat = 1;
	}
	else if(cal1 < 7500)
	{
		dat = 2;
	}
	else if(cal1 < 8000)
	{
		dat = 3;
	}
	else 
	{
		dat = 4;
	}
	return dat;
#endif	
	
}

void Power_off_test(void)
{
//    uint8_t ret;

//    if (TRUE == Power_Keyscan())
//    {
//        /* 如果power_key需要做独立按键可进行长短按设计 */
//        delay_ms(1000);
//		
//        if (TRUE == Power_Keyscan())
//        {
//						
//            MyPrintf("poweroff\n");
//            /* 操作Power_off函数之前先进行关闭LCD等外设的操作 */
//            ret = Power_off(); //如果插入外电，则不会关机，返回FALSE
//					if(ret == FALSE)
//					{
//						 MyPrintf("poweroff failed\n");
//					}
//        }
//    }
}
#ifndef MOTO_PWM_EN


void SetMotoPWM(unsigned int value,unsigned short ParTime)
{
	if(value & O_MOTO1_PIN)
	{
		SET_H_O_MOTO1;
	}
	else
	{
		SET_L_O_MOTO1;
		
	}
	
	if(value & O_MOTO2_PIN)
	{
		SET_H_O_MOTO2;
	}
	else
	{
		SET_L_O_MOTO2;
		
	}
	
	
	if(value & O_MOTO3_PIN)
	{
		SET_H_O_MOTO3;
	}
	else
	{
		SET_L_O_MOTO3;

	}
	

	if(value & O_MOTO4_PIN)
	{
		SET_H_O_MOTO4;
	}
	else
	{
		SET_L_O_MOTO4;
		
	}
	
}

void SetMotoPWM_A(unsigned int value)
{
	if(value & O_MOTO1_PIN)
	{
		SET_H_O_MOTO1;
	}
	else
	{
		SET_L_O_MOTO1;
		
	}
	
	if(value & O_MOTO2_PIN)
	{
		SET_H_O_MOTO2;
	}
	else
	{
		SET_L_O_MOTO2;
		
	}
	
	
	if(value & O_MOTO3_PIN)
	{
		SET_H_O_MOTO3;
	}
	else
	{
		SET_L_O_MOTO3;
		
	}
	

	if(value & O_MOTO4_PIN)
	{
		SET_H_O_MOTO4;
	}
	else
	{
		SET_L_O_MOTO4;
		
	}
	
}


void ClearMoto(void)
{
	SET_L_O_MOTO1;
	SET_L_O_MOTO2;
	SET_L_O_MOTO3;
	SET_L_O_MOTO4;
}

#endif
#ifdef MOTO_PWM_EN


void SetMotoPWM(unsigned int value,unsigned short ParTime)
{
	unsigned short m_ParTime;
		
	m_ParTime = ParTime/10;
	if(value & O_MOTO1_PIN)
	{
		TIM_SetPWMPeriod(TIM5,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM5,96*(100 - m_ParTime),96*m_ParTime);
	}
	
	if(value & O_MOTO2_PIN)
	{
		TIM_SetPWMPeriod(TIM6,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM6,96*(100 - m_ParTime),96*m_ParTime);
	}
	
	
	if(value & O_MOTO3_PIN)
	{
		TIM_SetPWMPeriod(TIM7,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM7,96*(100 - m_ParTime),96*m_ParTime);
	}

	if(value & O_MOTO4_PIN)
	{
		TIM_SetPWMPeriod(TIM8,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM8,96*(100 - m_ParTime),96*m_ParTime);
	}	
}

void SetMotoPWM_A(unsigned int value)
{
	
	if(value & O_MOTO1_PIN)
	{
		TIM_SetPWMPeriod(TIM5,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM5,96*100,0);
	}
	
	if(value & O_MOTO2_PIN)
	{
		TIM_SetPWMPeriod(TIM6,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM6,96*100,0);		
	}
	
	
	if(value & O_MOTO3_PIN)
	{
		TIM_SetPWMPeriod(TIM7,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM7,96*100,0);		
	}

	if(value & O_MOTO4_PIN)
	{
		TIM_SetPWMPeriod(TIM8,0,96*100);
	}
	else
	{
		TIM_SetPWMPeriod(TIM8,96*100,0);		
	}
	
}


void ClearMoto(void)
{

	TIM_SetPWMPeriod(TIM5,96*100,0);	
	TIM_SetPWMPeriod(TIM6,96*100,0);
	TIM_SetPWMPeriod(TIM7,96*100,0);	
	TIM_SetPWMPeriod(TIM8,96*100,0);
	
	
}


#endif





void MOTORUN(unsigned short * p_g_MotoAll,unsigned short mRunStep)
{
	int i,j;
	for(i=0;i<mRunStep;i++)
	{
//		ClearMoto();
		WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		//SetMotoPWM(PrintMotoHalfStep[*p_g_MotoAll  & 7],100);
		SetMotoPWM_A(PrintMotoHalfStep[(*p_g_MotoAll)  & 7]);
		
		(*p_g_MotoAll)++;
		for(j=0;j<0x3000;j++);
		
		MyPrintf("%02X %02X ",g_ADCValue[0]&0xFF,(g_ADCValue[0] >>8)&0xFF );
		
	}
	ClearMoto();
}



void MOTORUN_FX(unsigned short * p_g_MotoAll,unsigned short mRunStep)
{
	int i,j;
//	GPIO_ADC5_Configuration();
//	WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
//	delay_ms(200);
//WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		SetMotoPWM_A(PrintMotoHalfStep[(*p_g_MotoAll)  & 7]);
		

		for(j=0;j<0x2000;j++)
		{
			WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		}		
	for(i=0;i<mRunStep;i++)
	{
//		ClearMoto();
		WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		//SetMotoPWM(PrintMotoHalfStep[*p_g_MotoAll  & 7],100);
				(*p_g_MotoAll)--;
		SetMotoPWM_A(PrintMotoHalfStep[(*p_g_MotoAll)  & 7]);
		

		for(j=0;j<0x2000;j++);
//		j = 1200 - g_ADCValue[0];
//		MyPrintf("%02X %02X ",j&0xFF,(j >>8)&0xFF );
	}
	
		SetMotoPWM_A(PrintMotoHalfStep[(*p_g_MotoAll)  & 7]);
		for(j=0;j<0x1000;j++)
		{
			WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		}	
	
	for(i=0;i<16;i++)
	{
		WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		(*p_g_MotoAll)++;
		SetMotoPWM_A(PrintMotoHalfStep[(*p_g_MotoAll)  & 7]);
		for(j=0;j<0x2000;j++);
	}	
	
	
	ClearMoto();
}
 #define RFID_Flash_Dat (0x1080000 - 0x1000)


void SaveCOnfigtoFlash (unsigned char * pbuf,unsigned short mplen)
{
	unsigned short m_pag,k;
	unsigned char mbuf[128];
	memset(mbuf,0,mplen);
	memcpy(mbuf,pbuf,mplen);
	m_pag = (mplen + 31)/32;
	for(k=0;k<m_pag;k++)
	{
		 enc_erase_flash_32byte((unsigned int)RFID_Flash_Dat + k*32);
		 enc_write_flash((unsigned int)RFID_Flash_Dat + k*32, (unsigned char *)&mbuf[k*32], 32);		
	}
	
}



void ReadCOnfigFromFlash (unsigned char * pbuf,unsigned short mplen)
{
	
	enc_read_flash((unsigned int)RFID_Flash_Dat, pbuf,mplen);
}


#ifdef DOUBLE_TAP_SELFTEST_PRINT
void K2_Driver_Update(void) {
    static K2_STATE_T state = K2_IDLE;
    static uint16_t timer = 0;
    
    // PC9 is Active LOW
    uint8_t pressed = GET_I_KEY_LF;//PRINTER_PDL.SYS._GetKey_LF();//(GET_I_KEY_LF == 0); 

    switch (state) {
        case K2_IDLE:
            if (pressed) { timer = 0; state = K2_DEBOUNCE_PRESS; }
            break;

        case K2_DEBOUNCE_PRESS:
            if (++timer >= 20) { state = pressed ? K2_WAIT_RELEASE : K2_IDLE; timer = 0; }
            break;

        case K2_WAIT_RELEASE:
            timer++;
            if (!pressed) { timer = 0; state = K2_DEBOUNCE_RELEASE; }
            else if (timer >= 500) { g_K2_Event = K2_EVT_HOLDING; state = K2_HOLD; }
            break;

        case K2_DEBOUNCE_RELEASE:
            if (++timer >= 20) { timer = 0; state = K2_WAIT_SECOND_TAP; }
            break;

        case K2_WAIT_SECOND_TAP:
            if (pressed) { g_K2_Event = K2_EVT_DOUBLE_TAP; state = K2_DEBOUNCE_PRESS; timer = 0; }
            else if (++timer >= 300) { g_K2_Event = K2_EVT_SINGLE_TAP; state = K2_IDLE; }
            break;

        case K2_HOLD:
            if (!pressed) { g_K2_Event = K2_EVT_NONE; state = K2_IDLE; }
            break;
    }
}
#endif

#ifdef BATTERY_INDICATION_SOLUTION
static unsigned short BatteryBar_CalibrateVoltage(unsigned short measured_mV)
{
	unsigned long corrected_mV;
 
	corrected_mV = ((unsigned long)measured_mV * BATTERY_BAR_CAL_GAIN_PERMILLE);
	if (corrected_mV <= BATTERY_BAR_CAL_OFFSET_MV) return 0;
	return (unsigned short)((corrected_mV - BATTERY_BAR_CAL_OFFSET_MV) / 1000UL);
}

#endif


