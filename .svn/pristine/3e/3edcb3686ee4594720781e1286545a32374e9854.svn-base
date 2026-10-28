/*----------------------------------------------------------------------------
*         ATMEL Microcontroller Software Support  -  ROUSSET  -
*----------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*----------------------------------------------------------------------------
* File Name           : Board.h
* Object              : AT91SAM7S Evaluation Board Features Definition File.
*
* Creation            : JPP   16/Jun/2004
*----------------------------------------------------------------------------

管脚定义		OEM57V6			EH32V2			VII32
PA0-10		KEY				KEY				KEY
PA1-11		Collect-VPP		Collect-VPP		Collect-VPP
PA2-12		TXD2			TXD2			TXD2
PA3-13		RXD2			RXD2			RXD2
PA4-14		NULL			NULL			NULL
PA5-15		CLK				CLK				CLK
PA6-16		LPS-BM			LPS-BM			LPS-BM
PA7-17		DI	DI			DI
PA8-29		MOTO1(T1-PWM)	MOTO1(T1-PWM)	MOTO1(T1-PWM)
PA9-30		MOTO2(T1-PWM)	MOTO2(T1-PWM)	MOTO2(T1-PWM)
PA10-31		MOTO3(T1-PWM)	MOTO3(T1-PWM)	MOTO3(T1-PWM)
PA11-32		MOTO4(T1-PWM)	MOTO4(T1-PWM)	MOTO4(T1-PWM)
PA12-33		BUSY			BUSY			BUSY
PA13-34		LED				LED				LED
PA14-37		NULL			NULL			KEY2
PA15-38		NULL			NULL			LED2
PB0-18		TM				TM				TM
PB1-19		NULL-ADCIN_9	NULL-ADCIN_9	NULL-ADCIN_9
PB2-20		NULL			NULL			NULL
PB3-39		LAT				LAT				LAT
PB4-40		STB1			STB1			STB1
PB5-41		STB2			STB2			STB2
PB6-42		STB3			STB3			STB3
PB7-43		STB4			STB4			STB4
PB8-45		STB5			STB5			STB5
PB9-46		STB6			STB6			STB6
PB10-21		LPS-BM			LPS-BM			LPS-BM
PB11-22		Logic_Power_ON	Logic_Power_ON	Logic_Power_ON
PB12-25		Flash_CS		Flash_CS		Flash_CS
PB13-26		Flash_SCK		Flash_SCK		Flash_SCK
PB14-27		Flash_SO		Flash_SO		Flash_SO
PB15-28		Flash_SI		Flash_SI		Flash_SI
PC13-2		NULL			NULL			NULL
PC14-3		NULL			NULL			NULL
PC15-4		CON_DRV_POWER	CON_DRV_POWER	CON_DRV_POWER
*/
#ifndef RD_OEM_BOARD_H
#define RD_OEM_BOARD_H

#include "stm32f10x_gpio.h"



//**********************管脚定义****************************
//PA0-10		KEY
#define I_KEY_LF		((unsigned int)1<<0)	/* PA0 */
//------------------------------------------------------------
//PA1-11		Collect-VPP
#define I_GET_VOLTAGE	((unsigned int)1<<1)	/* PA1 */
//------------------------------------------------------------

//PA2-12		TXD2
//PA3-13		RXD2
//PA4-14		NULL
//PA5-15		CLK

//PA6-16		LPS-BM
//黑标检测
#define I_BL_Mark		((unsigned int)1<<6)	/* PA6 */
//------------------------------------------------------------

//PA7-17		DI	DI


//PA8-29		MOTO1(T1-PWM)
//PA9-30		MOTO2(T1-PWM)
//PA10-31		MOTO3(T1-PWM)
//PA11-32		MOTO4(T1-PWM)
//电机驱动信号
#define O_PWM_MOTO1		((unsigned int)1<<8)	/// PA8
#define O_PWM_MOTO2		((unsigned int)1<<9)	/// PA9
#define O_PWM_MOTO3		((unsigned int)1<<10)	/// PA10
#define O_PWM_MOTO4		((unsigned int)1<<11)	/// PA11
//------------------------------------------------------------


//PA12-33		BUSY
//忙信号
#define O_BUSY_S		((unsigned int)1<<12)	/* PA12 */
//------------------------------------------------------------

//PA13-34		LED
//LED信号
#define O_LED		((unsigned int)1<<13)	/* PA13 */
//------------------------------------------------------------

//PA14-37		NULL
//PA15-38		NULL
//PB0-18		TM

//PB1-19		NULL-ADCIN_9
//PB2-20		NULL

//PB3-39		LAT
//打印头数据锁存信号
#define O_LAT		((unsigned int)1<<3)	/* PB3 */
//------------------------------------------------------------

//PB4-40		STB1
//PB5-41		STB2
//PB6-42		STB3
//PB7-43		STB4
//PB8-45		STB5
//PB9-46		STB6
//加热STB信号
#define O_STB1			((unsigned int)1<<4)	/* PB4 */
#define O_STB2			((unsigned int)1<<5)	/* PB5 */
#define O_STB3			((unsigned int)1<<6)	/* PB6 */
#define O_STB4			((unsigned int)1<<7)	/* PB7 */
#define O_STB5			((unsigned int)1<<8)	/* PB8 */
#define O_STB6			((unsigned int)1<<9)	/* PB9 */
//------------------------------------------------------------

//PB10-21		LPS-BM
//缺纸检测
#define I_LPS			((unsigned int)1<<10)	/* PB10 */
#define I_TM			((unsigned int)1<<0)	/* PB0 */
//------------------------------------------------------------

//PB11-22		Logic_Power_ON
//打印头逻辑电源控制信号
#define O_VCC_CON			((unsigned int)1<<11)	/* PB11 */
//------------------------------------------------------------
//PB12-25		Flash_CS
//mx25片选
#define O_MX25_CS		((unsigned int)1<<12)	/* PB12 */
//------------------------------------------------------------

//PB13-26		Flash_SCK
//PB14-27		Flash_SO
//PB15-28		Flash_SI

//PC13-2		NULL
//PC14-3		NULL


//PC15-4		CON_DRV_POWER
//打印头驱动电源控制信号
#define O_POWER			((unsigned int)1<<15)	/* PC15 */
//------------------------------------------------------------

//*********************************************************
#define GET_KEY_LF		((GPIOA->IDR) & I_KEY_LF)
//------------------------------------------------------------
#define SET_H_BUSY_S	(GPIOA->BSRR = O_BUSY_S)
#define SET_L_BUSY_S	(GPIOA->BRR = O_BUSY_S)
#define GET_BUSY_S		(GPIOA->ODR & O_BUSY_S)
//------------------------------------------------------------
#define GET_LED_STA		(GPIOA->ODR & O_LED)
#define SET_H_LED		(GPIOA->BSRR = O_LED)
#define SET_L_LED		(GPIOA->BRR = O_LED)
//------------------------------------------------------------
#define SET_H_MX_CS		(GPIOB->BSRR = O_MX25_CS)
#define SET_L_MX_CS		(GPIOB->BRR = O_MX25_CS)
//------------------------------------------------------------
#define SET_H_LAT		(GPIOB->BSRR = O_LAT)
#define SET_L_LAT		(GPIOB->BRR = O_LAT)
//------------------------------------------------------------
#define SET_H_STB1		(GPIOB->BSRR = (O_STB1|O_STB3|O_STB5))
#define SET_L_STB1		(GPIOB->BRR = (O_STB1|O_STB3|O_STB5))

#define SET_H_STB2		(GPIOB->BSRR = (O_STB2|O_STB4|O_STB6))
#define SET_L_STB2		(GPIOB->BRR = (O_STB2|O_STB4|O_STB6))
//------------------------------------------------------------
#define SET_H_POWER		(GPIOC->BSRR = O_POWER)
#define SET_L_POWER		(GPIOC->BRR = O_POWER)
//------------------------------------------------------------
#define SET_H_VCC_CON	(GPIOB->BSRR = O_VCC_CON)
#define SET_L_VCC_CON	(GPIOB->BRR = O_VCC_CON)



#endif /* RD_OEM_BOARD_H */
