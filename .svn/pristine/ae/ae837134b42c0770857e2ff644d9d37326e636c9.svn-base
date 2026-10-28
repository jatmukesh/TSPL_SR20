/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_app_conf.h
 **
 ** A detailed description is available at
 ** @link General low level driver configuration @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __BSP_CONF__H__
#define __BSP_CONF__H__
#include "yc_gpio.h"
#include "yc_spi.h"
#include "yc_timer.h"
#include "misc.h"
#include "yc_adc.h"
#include "yc3121.h"
#include "cortexm3_macro.h"
#include "yc_bt.h"
#include "yc_systick.h"
#include "yc_qspi.h"
#include "yc_ipc.h"

#include "yc_power.h"
#include "system.h"
#include "usb.h"
#include "yc_wdt.h"
#include "macros.h"



#define PRINTER_TYPE_STB1		1
#define PRINTER_TYPE_STB2		1
#define PRINTER_TYPE_RS232	1


/*******************************New Firmware Modification**********************************/
#ifdef DOUBLE_TAP_SELFTEST_PRINT
extern volatile uint8_t g_K2_Event; 	// Event flag to pass button state

void K2_Driver_Update(void);			// Monitor the physical state of LF_KEY

/* K2 Event Definitions */
#define K2_EVT_NONE        0			// Default state; no user interaction 
#define K2_EVT_SINGLE_TAP  1			// Indicates single press ( paper feed  action)
#define K2_EVT_DOUBLE_TAP  2			// Indicates double tap ( self test action )
#define K2_EVT_HOLDING     3			// Indicates button is held more tha 500ms 

#endif

/*******************************************************************************
 * V2 IO
 
[7:6] IO mode 00:FLOAT 01:PULL UP 10:PULL DOWN  11: ANALOG
[5:0] IOfunction 赋值与功能对应关系见下表

62------GPIO_OUTPUT_LOW
63------GPIO_OUTPUT_HIGH
 ******************************************************************************/
#define	GPIO_TYPE_FLOAT				0x0
#define	GPIO_TYPE_PULL_UP			0x40
#define	GPIO_TYPE_PULL_DOWN		0x80
#define	GPIO_TYPE_ANALOG			0xC0

#define	GPIO_OUTPUT_LOW				62
#define	GPIO_OUTPUT_HIGH			63

// ==============================================================================

// -debug
// --------RXD0
#define I_RXD0_PORT     (GPIOA)/* PA0 ++ */
#define I_RXD0_NUM      (0)
#define I_RXD0_PIN      (unsigned int)(1<<I_RXD0_NUM)

// --------TXD0
#define O_TXD0_PORT     (GPIOA)/* PA1 ++ */
#define O_TXD0_NUM      (1)
#define O_TXD0_PIN      (unsigned int)(1<<O_TXD0_NUM)





// -------- MOTO
#define O_MOTO1_PORT     (GPIOB)/* PB5  +*/
#define	O_MOTO1_NUM      (5)
#define O_MOTO1_PIN      (unsigned int)(1<<O_MOTO1_NUM)

#define SET_H_O_MOTO1		(GPIO_CONFIG(O_MOTO1_PORT * GPIO_PIN_NUM + O_MOTO1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO1		(GPIO_CONFIG(O_MOTO1_PORT * GPIO_PIN_NUM + O_MOTO1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

#define O_MOTO2_PORT     (GPIOB)/* PB6 +*/
#define	O_MOTO2_NUM      (6)
#define O_MOTO2_PIN      (unsigned int)(1<<O_MOTO2_NUM)

#define SET_H_O_MOTO2		(GPIO_CONFIG(O_MOTO2_PORT * GPIO_PIN_NUM + O_MOTO2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO2		(GPIO_CONFIG(O_MOTO2_PORT * GPIO_PIN_NUM + O_MOTO2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)


#define O_MOTO3_PORT     (GPIOB)/* PB11 +*/
#define	O_MOTO3_NUM      (11)
#define O_MOTO3_PIN      (unsigned int)(1<<O_MOTO3_NUM)

#define SET_H_O_MOTO3		(GPIO_CONFIG(O_MOTO3_PORT * GPIO_PIN_NUM + O_MOTO3_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO3		(GPIO_CONFIG(O_MOTO3_PORT * GPIO_PIN_NUM + O_MOTO3_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)


#define O_MOTO4_PORT     (GPIOB)/* PB10 +*/
#define	O_MOTO4_NUM      (10)
#define O_MOTO4_PIN      (unsigned int)(1<<O_MOTO4_NUM)

#define SET_H_O_MOTO4		(GPIO_CONFIG(O_MOTO4_PORT * GPIO_PIN_NUM + O_MOTO4_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO4		(GPIO_CONFIG(O_MOTO4_PORT * GPIO_PIN_NUM + O_MOTO4_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// -------- HEAD


#define O_LAT_PORT     (GPIOC)/* PBC4 + */
#define	O_LAT_NUM      (4)
#define O_LAT_PIN      (unsigned int)(1<<O_LAT_NUM)

#define SET_H_O_LAT		(GPIO_CONFIG(O_LAT_PORT * GPIO_PIN_NUM + O_LAT_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_LAT		(GPIO_CONFIG(O_LAT_PORT * GPIO_PIN_NUM + O_LAT_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)



#define O_STB1_PORT     (GPIOC)/* PC2 +*/
#define	O_STB1_NUM      (2)
#define O_STB1_PIN      (unsigned int)(1<<O_STB1_NUM)


#define SET_H_O_STB1		(GPIO_CONFIG(O_STB1_PORT * GPIO_PIN_NUM + O_STB1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_STB1		(GPIO_CONFIG(O_STB1_PORT * GPIO_PIN_NUM + O_STB1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

#define O_STB2_PORT     (GPIOC)/* PC2 + */
#define	O_STB2_NUM      (2)
#define O_STB2_PIN      (unsigned int)(1<<O_STB2_NUM)

#define SET_H_O_STB2		(GPIO_CONFIG(O_STB2_PORT * GPIO_PIN_NUM + O_STB2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_STB2		(GPIO_CONFIG(O_STB2_PORT * GPIO_PIN_NUM + O_STB2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)
// --------DI
#define O_DI_PORT     (GPIOB)/* PB15 +*/
#define O_DI_NUM      (15)
#define O_DI_PIN      (unsigned int)(1<<O_DI_NUM)

// --------CLK
#define O_CLK_PORT     (GPIOC)/* PC0 +*/
#define O_CLK_NUM      (0)
#define O_CLK_PIN      (unsigned int)(1<<O_CLK_NUM)


// USB -----检测-------------
#define I_USB_DET_PORT     (GPIOB)/* PB2 +*/
#define I_USB_DET_NUM      (2)
#define I_USB_DET_PIN      (unsigned int)(1<<I_USB_DET_NUM)

#define GET_I_USB_DET			(GPIO_IN(I_USB_DET_PORT) & I_USB_DET_PIN)

// ----- ADC

#define A_TM_PORT     (GPIOC)/* PC5 AD0 +*/
#define A_TM_NUM      (5)
#define A_TM_PIN      (unsigned int)(1<<A_TM_NUM)
//纸张检测
#define A_DPS_PORT     (GPIOC)/* PC6 AD1 + */
#define A_DPS_NUM      (6)
#define A_DPS_PIN      (unsigned int)(1<<A_DPS_NUM)

//对射
#define A_LPS_PORT     (GPIOC)/* PC7 AD2 + */
#define A_LPS_NUM      (7)
#define A_LPS_PIN      (unsigned int)(1<<A_LPS_NUM)



// 胶辊--检测-------------
#define I_HS_PORT     (GPIOC)/* PC8 +*/
#define I_HS_NUM      (8)
#define I_HS_PIN      (unsigned int)(1<<I_HS_NUM)

#define GET_I_HS			(GPIO_IN(I_HS_PORT) & I_HS_PIN)

// 胶辊--检测-------------
#define A_VOL_PORT     (GPIOC)/* PC11 AD6 +*/
#define A_VOL_NUM      (11)
#define A_VOL_PIN      (unsigned int)(1<<A_VOL_NUM)



// ---- 按键输入
#define I_LF_PORT     (GPIOA)/* PA6 + */
#define I_LF_NUM      (6)
#define I_LF_PIN      (unsigned int)(1<<I_LF_NUM)

#define GET_I_KEY_LF			(GPIO_IN(I_LF_PORT) & I_LF_PIN)
// ---- 按键输入

#define I_POW_PORT     (GPIOB)/* PB1 +*/
#define I_POW_NUM      (1)
#define I_POW_PIN      (unsigned int)(1<<I_POW_NUM)

#define GET_I_KEY_POW			(GPIO_IN(I_POW_PORT) & I_POW_PIN)



// ---- 蜂鸣器
#define O_BEEP_PORT     (GPIOA)/* PA11 + */
#define O_BEEP_NUM      (11)
#define O_BEEP_PIN      (unsigned int)(1<<O_BEEP_NUM)

#define GET_BEEP_OUT		(GPIO_IN(O_BEEP_PORT) & O_BEEP_PIN)
#define SET_H_O_BEEP		(GPIO_CONFIG(O_BEEP_PORT * GPIO_PIN_NUM + O_BEEP_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_BEEP		(GPIO_CONFIG(O_BEEP_PORT * GPIO_PIN_NUM + O_BEEP_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)








// --------SWITCH
#define O_SWITCH_PORT     (GPIOC)/* PC9 + */
#define O_SWITCH_NUM      (9)
#define O_SWITCH_PIN      (unsigned int)(1<<O_SWITCH_NUM)

#define GET_SWITCH_OUT		(GPIO_IN(O_SWITCH_PORT) & O_SWITCH_PIN)
#define SET_H_O_SWITCH		(GPIO_CONFIG(O_SWITCH_PORT * GPIO_PIN_NUM + O_SWITCH_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_SWITCH		(GPIO_CONFIG(O_SWITCH_PORT * GPIO_PIN_NUM + O_SWITCH_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// --------POWER EN
#define O_POWER_EN_PORT     (GPIOC)/* PC10 + */
#define O_POWER_EN_NUM      (10)
#define O_POWER_EN_PIN      (unsigned int)(1<<O_POWER_EN_NUM)


#define SET_H_O_POWER_EN		(GPIO_CONFIG(O_POWER_EN_PORT * GPIO_PIN_NUM + O_POWER_EN_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_POWER_EN		(GPIO_CONFIG(O_POWER_EN_PORT * GPIO_PIN_NUM + O_POWER_EN_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)


// --------SPI

#define O_MX25_CS_PORT     (GPIOA)/* PA14 +*/
#define	O_MX25_CS_NUM      (14)
#define O_MX25_CS_PIN      (unsigned int)(1<<O_MX25_CS_NUM)

#define O_MX25_SO_PORT     (GPIOA)/* PA15 +*/
#define	O_MX25_SO_NUM      (15)
#define O_MX25_SO_PIN      (unsigned int)(1<<O_MX25_SO_NUM)

#define I_MX25_SI_PORT     (GPIOA)/* PA12 +*/
#define	I_MX25_SI_NUM      (12)
#define I_MX25_SI_PIN      (unsigned int)(1<<I_MX25_SI_NUM)

#define O_MX25_SCK_PORT    (GPIOA)/* PA13 +*/
#define	O_MX25_SCK_NUM     (13)
#define O_MX25_SCK_PIN     (unsigned int)(1<<O_MX25_SCK_NUM)


#define SET_H_O_MX25_CS		(GPIO_CONFIG(O_MX25_CS_PORT * GPIO_PIN_NUM + O_MX25_CS_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MX25_CS		(GPIO_CONFIG(O_MX25_CS_PORT * GPIO_PIN_NUM + O_MX25_CS_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// ---- 充电状态输入 充电低  不充电高  开漏输出充电状态指示
#define I_CHARGE_PORT     (GPIOC)/* PC12 +*/
#define I_CHARGE_NUM      (12)
#define I_CHARGE_PIN      (unsigned int)(1<<I_CHARGE_NUM)

#define GET_I_CHARGE			(GPIO_IN(I_CHARGE_PORT) & I_CHARGE_PIN)


// --------IIC
#define O_SDA_PORT     (GPIOA)/* PA8 + */
#define O_SDA_NUM      (8)
#define O_SDA_PIN      (unsigned int)(1<<O_SDA_NUM)

#define GET_SDA_OUT		(GPIO_IN(O_SDA_PORT) & O_SDA_PIN)
#define SET_H_O_SDA		(GPIO_CONFIG(O_SDA_PORT * GPIO_PIN_NUM + O_SDA_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_SDA		(GPIO_CONFIG(O_SDA_PORT * GPIO_PIN_NUM + O_SDA_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

#define O_SCL_PORT     (GPIOA)/* PA7 + */
#define O_SCL_NUM      (7)
#define O_SCL_PIN      (unsigned int)(1<<O_SCL_NUM)


#define SET_H_O_SCL		(GPIO_CONFIG(O_SCL_PORT * GPIO_PIN_NUM + O_SCL_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_SCL		(GPIO_CONFIG(O_SCL_PORT * GPIO_PIN_NUM + O_SCL_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)


#define O_OLED_RST_PORT     (GPIOA)/* PA9 + */
#define O_OLED_RST_NUM      (9)
#define O_OLED_RST_PIN      (unsigned int)(1<<O_OLED_RST_NUM)


#define SET_H_O_OLED_RST		(GPIO_CONFIG(O_OLED_RST_PORT * GPIO_PIN_NUM + O_OLED_RST_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_OLED_RST		(GPIO_CONFIG(O_OLED_RST_PORT * GPIO_PIN_NUM + O_OLED_RST_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)



// ==============================================================================

/*
 * Expose thermistor conversion only when the protected print path needs it.
 * The normal firmware has no caller and no linked implementation of this API.
 */
#ifdef PSF20_R002_TEMPERATURE_SAFE_PRINTING
unsigned short Read_Thermistor_Temperature(unsigned short adc_value);
#endif

void SetBTName(unsigned char *Pbabuf);
unsigned short ReadLFKeyPlus(void);
void WDG_ResetCounter(void);
void GPIO_Configuration(void);
void UART1_Configuration(unsigned int mBaud);
void Debug_Configuration(void);
void HVIN_ADC_Configuration(void);
uint16_t  Get_HVIN_ADC_Value(void);
void GPIO_ADC6_Configuration(void);
unsigned char GetBattyPer(void);
unsigned char GetCurrentVol(void);
uint16_t  Get_GPIO_ADC_Value(ADC_ChxTypeDef ADC_Channel);

void SPIPrinter_init(void);
void SPI_Configuration(void);

void System_TIMER_Configuration(void);
void TIMER_Configuration(void);
void Printer_TIMER_Configuration(void);
void EnableTimer(TIM_NumTypeDef TIMx,unsigned int mperiod);
void DisableTimer(TIM_NumTypeDef TIMx);
Boolean GetUSBCharge(void);

int  BTSysInit(void);
void ClearMoto(void);
void SetMotoPWM_A(unsigned int value);
void SetMotoPWM(unsigned int value,unsigned short ParTime);
void MOTORUN(unsigned short * p_g_MotoAll,unsigned short mRunStep);
void PWM_Configuration(void);
void WDT_Configuration(void);
void WDG_ResetCounter(void);
unsigned short GetCurrentTM(void);
uint16_t  Get_LPS_ADC_Value(void);

void TIMER3_Configuration(void);
void PrintBle_SPP_Send(unsigned char * mBuf,unsigned char mLen);

void SetBT_KEY(unsigned char *Pbabuf);
void ReadBTKEY(unsigned char *Pbabuf);

void MOTORUN_FX(unsigned short * p_g_MotoAll,unsigned short mRunStep);

void SaveCOnfigtoFlash (unsigned char * pbuf,unsigned short mplen);
void ReadCOnfigFromFlash (unsigned char * pbuf,unsigned short mplen);
unsigned short ReadPOWKeyPlus(void);
void SDA_InputEn (void);
unsigned short GetVolteV(void);
#endif //__USB_CONF__H__

