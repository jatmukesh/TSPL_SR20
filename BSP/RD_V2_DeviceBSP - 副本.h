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
#include "board_config.h"
#include "yc_power.h"
#include "system.h"
#include "usb.h"

#define PRINTER_TYPE_STB1		1
#define PRINTER_TYPE_STB2		1
#define PRINTER_TYPE_RS232	1
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

// ---- 指示灯
#define O_LEDG_PORT     (GPIOC)/* PC14 ++ */
#define O_LEDG_NUM      (14)
#define O_LEDG_PIN      (unsigned int)(1<<O_LEDG_NUM)


#define GET_LEDG_OUT		(GPIO_IN(O_LEDG_PORT) & O_LEDG_PIN)
#define SET_H_O_LEDG		(GPIO_CONFIG(O_LEDG_PORT * GPIO_PIN_NUM + O_LEDG_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_LEDG		(GPIO_CONFIG(O_LEDG_PORT * GPIO_PIN_NUM + O_LEDG_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// ---- 按键输入
#define I_LF_PORT     (GPIOB)/* PB06 ++ */
#define I_LF_NUM      (6)
#define I_LF_PIN      (unsigned int)(1<<I_LF_NUM)

#define GET_I_KEY_LF			(GPIO_IN(I_LF_PORT) & I_LF_PIN)

// ----- ADC
//温度检测
#define A_TM_PORT     (GPIOC)/* PC06 ++ */
#define A_TM_NUM      (6)
#define A_TM_PIN      (unsigned int)(1<<A_TM_NUM)
//纸张检测
#define A_LPS_PORT     (GPIOC)/* PC09 ++ */
#define A_LPS_NUM      (9)
#define A_LPS_PIN      (unsigned int)(1<<A_LPS_NUM)


// --------R6 发射管控制
#define O_R6_PORT     (GPIOC)/* PC08 ++ */
#define O_R6_NUM      (10)
#define O_R6_PIN      (unsigned int)(1<<O_R6_NUM)

#define SET_H_O_R6		(GPIO_CONFIG(O_R6_PORT * GPIO_PIN_NUM + O_R6_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_R6		(GPIO_CONFIG(O_R6_PORT * GPIO_PIN_NUM + O_R6_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// --------CLK
#define O_CLK_PORT     (GPIOC)/* PC07 ++ */
#define O_CLK_NUM      (7)
#define O_CLK_PIN      (unsigned int)(1<<O_CLK_NUM)

// --------DI
#define O_DI_PORT     (GPIOC)/* PC08 ++ */
#define O_DI_NUM      (8)
#define O_DI_PIN      (unsigned int)(1<<O_DI_NUM)



// --------RXD
#define I_RXD1_PORT     (GPIOC)/* PC11 ++ */
#define I_RXD1_NUM      (11)
#define I_RXD1_PIN      (unsigned int)(1<<I_RXD1_NUM)

// --------TXD
#define O_TXD1_PORT     (GPIOC)/* PC12 ++ */
#define O_TXD1_NUM      (12)
#define O_TXD1_PIN      (unsigned int)(1<<O_TXD1_NUM)



// --------BUSY
#define O_BUSY_PORT     (GPIOC)/* PC13 ++ */
#define O_BUSY_NUM      (13)
#define O_BUSY_PIN      (unsigned int)(1<<O_BUSY_NUM)
	
#define GET_BUSY_OUT		(GPIO_IN(O_BUSY_PORT) & O_BUSY_PIN)
#define SET_H_O_BUSY		(GPIO_CONFIG(O_BUSY_PORT * GPIO_PIN_NUM + O_BUSY_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_BUSY		(GPIO_CONFIG(O_BUSY_PORT * GPIO_PIN_NUM + O_BUSY_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// --------SWITCH
#define O_SWITCH_PORT     (GPIOC)/* PC15 ++ */
#define O_SWITCH_NUM      (15)
#define O_SWITCH_PIN      (unsigned int)(1<<O_SWITCH_NUM)

#define GET_SWITCH_OUT		(GPIO_IN(O_SWITCH_PORT) & O_SWITCH_PIN)
#define SET_H_O_SWITCH		(GPIO_CONFIG(O_SWITCH_PORT * GPIO_PIN_NUM + O_SWITCH_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_SWITCH		(GPIO_CONFIG(O_SWITCH_PORT * GPIO_PIN_NUM + O_SWITCH_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// --------RXD0
#define I_RXD0_PORT     (GPIOA)/* PA0 ++ */
#define I_RXD0_NUM      (0)
#define I_RXD0_PIN      (unsigned int)(1<<I_RXD0_NUM)

// --------TXD0
#define O_TXD0_PORT     (GPIOA)/* PA1 ++ */
#define O_TXD0_NUM      (1)
#define O_TXD0_PIN      (unsigned int)(1<<O_TXD0_NUM)


// --------SPI

#define O_MX25_CS_PORT     (GPIOA)/* PA2 ++ */
#define	O_MX25_CS_NUM      (2)
#define O_MX25_CS_PIN      (unsigned int)(1<<O_MX25_CS_NUM)

#define O_MX25_SO_PORT     (GPIOA)/* PA3 ++ */
#define	O_MX25_SO_NUM      (3)
#define O_MX25_SO_PIN      (unsigned int)(1<<O_MX25_SO_NUM)

#define I_MX25_SI_PORT     (GPIOA)/* PA4 ++ */
#define	I_MX25_SI_NUM      (4)
#define I_MX25_SI_PIN      (unsigned int)(1<<I_MX25_SI_NUM)

#define O_MX25_SCK_PORT    (GPIOA)/* PA5 ++ */
#define	O_MX25_SCK_NUM     (5)
#define O_MX25_SCK_PIN     (unsigned int)(1<<O_MX25_SCK_NUM)


#define SET_H_O_MX25_CS		(GPIO_CONFIG(O_MX25_CS_PORT * GPIO_PIN_NUM + O_MX25_CS_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MX25_CS		(GPIO_CONFIG(O_MX25_CS_PORT * GPIO_PIN_NUM + O_MX25_CS_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// -------- MOTO
#define O_MOTO1_PORT     (GPIOA)/* PA10 ++ */
#define	O_MOTO1_NUM      (10)
#define O_MOTO1_PIN      (unsigned int)(1<<O_MOTO1_NUM)

#define SET_H_O_MOTO1		(GPIO_CONFIG(O_MOTO1_PORT * GPIO_PIN_NUM + O_MOTO1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO1		(GPIO_CONFIG(O_MOTO1_PORT * GPIO_PIN_NUM + O_MOTO1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

#define O_MOTO2_PORT     (GPIOA)/* PA11 ++ */
#define	O_MOTO2_NUM      (11)
#define O_MOTO2_PIN      (unsigned int)(1<<O_MOTO2_NUM)

#define SET_H_O_MOTO2		(GPIO_CONFIG(O_MOTO2_PORT * GPIO_PIN_NUM + O_MOTO2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO2		(GPIO_CONFIG(O_MOTO2_PORT * GPIO_PIN_NUM + O_MOTO2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)


#define O_MOTO3_PORT     (GPIOA)/* PA14 ++ */
#define	O_MOTO3_NUM      (14)
#define O_MOTO3_PIN      (unsigned int)(1<<O_MOTO3_NUM)

#define SET_H_O_MOTO3		(GPIO_CONFIG(O_MOTO3_PORT * GPIO_PIN_NUM + O_MOTO3_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO3		(GPIO_CONFIG(O_MOTO3_PORT * GPIO_PIN_NUM + O_MOTO3_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)


#define O_MOTO4_PORT     (GPIOA)/* PA15 ++ */
#define	O_MOTO4_NUM      (15)
#define O_MOTO4_PIN      (unsigned int)(1<<O_MOTO4_NUM)

#define SET_H_O_MOTO4		(GPIO_CONFIG(O_MOTO4_PORT * GPIO_PIN_NUM + O_MOTO4_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_MOTO4		(GPIO_CONFIG(O_MOTO4_PORT * GPIO_PIN_NUM + O_MOTO4_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// -------- HEAD


#define O_LAT_PORT     (GPIOB)/* PB00 ++ */
#define	O_LAT_NUM      (0)
#define O_LAT_PIN      (unsigned int)(1<<O_LAT_NUM)

#define SET_H_O_LAT		(GPIO_CONFIG(O_LAT_PORT * GPIO_PIN_NUM + O_LAT_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_LAT		(GPIO_CONFIG(O_LAT_PORT * GPIO_PIN_NUM + O_LAT_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

#define O_STB1_PORT     (GPIOB)/* PB01 ++ */
#define	O_STB1_NUM      (1)
#define O_STB1_PIN      (unsigned int)(1<<O_STB1_NUM)


#define SET_H_O_STB1		(GPIO_CONFIG(O_STB1_PORT * GPIO_PIN_NUM + O_STB1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_STB1		(GPIO_CONFIG(O_STB1_PORT * GPIO_PIN_NUM + O_STB1_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

#define O_STB2_PORT     (GPIOB)/* PB02 ++ */
#define	O_STB2_NUM      (2)
#define O_STB2_PIN      (unsigned int)(1<<O_STB2_NUM)

#define SET_H_O_STB2		(GPIO_CONFIG(O_STB2_PORT * GPIO_PIN_NUM + O_STB2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_HIGH)
#define SET_L_O_STB2		(GPIO_CONFIG(O_STB2_PORT * GPIO_PIN_NUM + O_STB2_NUM) = GPIO_TYPE_PULL_UP|GPIO_OUTPUT_LOW)

// --------



unsigned short ReadLFKeyPlus(void);
void WDG_ResetCounter(void);
void GPIO_Configuration(void);
void UART1_Configuration(unsigned int mBaud);
void Debug_Configuration(void);
void HVIN_ADC_Configuration(void);
uint16_t  Get_HVIN_ADC_Value(void);
void GPIO_ADC1_Configuration(void);
void GPIO_ADC4_Configuration(void);

uint16_t  Get_GPIO_ADC_Value(ADC_ChxTypeDef ADC_Channel);

void SPIPrinter_init(void);
void SPI_Configuration(void);

void System_TIMER_Configuration(void);
void TIMER_Configuration(void);
void Printer_TIMER_Configuration(void);
void EnableTimer(TIM_NumTypeDef TIMx,unsigned int mperiod);
void DisableTimer(TIM_NumTypeDef TIMx);


int  BTSysInit(void);
void ClearMoto(void);
void SetMotoPWM_A(unsigned int value);
void SetMotoPWM(unsigned int value,unsigned short ParTime);
void MOTORUN(unsigned short * p_g_MotoAll,unsigned short mRunStep);










#endif //__USB_CONF__H__

