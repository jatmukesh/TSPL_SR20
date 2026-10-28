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
MOTO1(T2-PWM)
MOTO2(T2-PWM)
MOTO3(T2-PWM)
MOTO4(T2-PWM)
Collect-VPP
CLK
LPS-BM
DI
Timer_SCL
BUSY/TXD1
ACK/RXD1
PE/USB_DM
NULL/USB_DP
STB2
STB1
LAT
IRQ/CTS
TM
Mtrig(Power)
D0/SW6
D1/SW5
D2/SW4
D3/SW2
D4/SW3
D5/SW1
D6/USB_DET
D7/USB_PPWR
Timer_SDA
Flash_CS
Flash_SCK
Flash_SO
Flash_SI
SEL
LF
LED

*/
#ifndef RD_MP_BOARD_H
#define RD_MP_BOARD_H
#include "stm32f10x_gpio.h"

//电机控制信号
#define O_PWM_MOTO1		((unsigned int)1<<0)	/* PA0 ++*/
#define O_PWM_MOTO2		((unsigned int)1<<1)	/* PA1 ++*/
#define O_PWM_MOTO3		((unsigned int)1<<2)	/* PA2 ++*/
#define O_PWM_MOTO4		((unsigned int)1<<3)	/* PA3 ++*/

//=====================================================
//缺纸检测
#define I_PS_BM			((unsigned int)1<<6)	/* PA6++ */

//对射
#define I_LPS			((unsigned int)1<<4)	/* PA4++ */

//打印头加热信号
#define I_TM				((unsigned int)1<<1)	/* PB1 ++*/


//=====================================================
//打印头时钟
#define O_CLK				((unsigned int)1<<5)	/* PA5 ++*/

//打印头数据
#define O_DI				((unsigned int)1<<7)	/* PA7 ++*/

//打印头锁存信号
#define O_LAT				((unsigned int)1<<4)	/* PB4++ */

#define SET_H_LAT		(GPIOB->BSRR = O_LAT)
#define SET_L_LAT		(GPIOB->BRR = O_LAT)


//蓝牙状态
#define I_BT_STATUS			((unsigned int)1<<14)	/* PC14 ++ */
//胶辊到位开关
#define I_HS						((unsigned int)1<<15)	/* PC15 ++ */

#define GET_BT_STATUS		(GPIOC->IDR & I_BT_STATUS)



//串口发送
#define O_TXD1_S			((unsigned int)1<<9)	/* PA9++ */
//串口接收
#define I_RXD1_S			((unsigned int)1<<10)	/* PA10 ++*/

//USB DM
#define O_USBDM_U			((unsigned int)1<<11)	/* PA11 ++*/
//USB DP
#define O_USBDP_U			((unsigned int)1<<12)	/* PA12 ++*/
//USB 上拉输出
#define O_USBPOWER_U		((unsigned int)1<<13)	/* PC13 ++ */
//USB 检测信号
#define I_USB_DET				((unsigned int)1<<5)	/* PB5 ++*/

//加热信号 STB
#define O_STB1				((unsigned int)1<<0)	/* PB0 ++ */

#define O_STB2				((unsigned int)1<<8)	/* PB8 ++ */

#define SET_H_STB1		(GPIOB->BSRR = O_STB1)
#define SET_L_STB1		(GPIOB->BRR = O_STB1)

#define SET_H_STB2		(GPIOB->BSRR = O_STB2)
#define SET_L_STB2		(GPIOB->BRR = O_STB2)





#define SET_H_USBPOWER_U		(GPIOC->BSRR = O_USBPOWER_U)
#define SET_L_USBPOWER_U		(GPIOC->BRR = O_USBPOWER_U)

//蓝牙 忙信号
#define O_CTS				((unsigned int)1<<13)	/* PA13++ */

#define GET_BUSY_S		(GPIOA->ODR & O_CTS)
#define SET_H_BUSY_S	(GPIOA->BSRR = O_CTS)
#define SET_L_BUSY_S	(GPIOA->BRR = O_CTS)



//WIFI串口发送
#define O_TXD3_S			((unsigned int)1<<10)	/* PB10 ++ */
//WIFI串口接收
#define I_RXD3_S			((unsigned int)1<<11)	/* PB11 ++ */
//WIFI CTS
#define O_WIFI_CTS			((unsigned int)1<<6)	/* PB6++ */
//WIFI STATUS 连接状态
#define I_WIFI_STATUS			((unsigned int)1<<7)	/* PB7++ */
//WIFI RLOAD 恢复默认值
#define O_WIFI_RLOAD			((unsigned int)0)	/*  */

//WIFI RST
#define O_WIFI_RST			((unsigned int)1<<9)	/* PB9++ */



#define GET_CTS2		(GPIOB->ODR & O_WIFI_CTS)
#define SET_H_CTS2	(GPIOB->BSRR = O_WIFI_CTS)
#define SET_L_CTS2	(GPIOB->BRR = O_WIFI_CTS)


#define GET_WIFI_STATUS		((GPIOB->IDR) & I_WIFI_STATUS)


#define SET_H_WIFI_RLOAD	(GPIOB->BSRR = O_WIFI_RLOAD)
#define SET_L_WIFI_RLOAD	(GPIOB->BRR = O_WIFI_RLOAD)

#define SET_H_WIFI_RST		(GPIOB->BSRR = O_WIFI_RST)
#define SET_L_WIFI_RST		(GPIOB->BRR = O_WIFI_RST)


//驱动电源脉冲信号
#define O_SWITCH			((unsigned int)1<<2)	/* PB2 ++ */

#define GET_SWITCH_STA	(GPIOB->ODR & O_SWITCH)
#define SET_H_SWITCH	(GPIOB->BSRR = O_SWITCH)
#define SET_L_SWITCH	(GPIOB->BRR = O_SWITCH)


#define SET_H_POWER	(GPIOB->BSRR = O_SWITCH)
#define SET_L_POWER	(GPIOB->BRR = O_SWITCH)





//SPI MX字库
#define O_MX25_CS			((unsigned int)1<<12)	/* PB12 ++*/
#define O_MX25_SCK			((unsigned int)1<<13)	/* PB13 ++*/
#define O_MX25_SO			((unsigned int)1<<14)	/* PB14 ++*/
#define I_MX25_SI			((unsigned int)1<<15)	/* PB15 ++*/

#define SET_H_MX_CS		(GPIOB->BSRR = O_MX25_CS)
#define SET_L_MX_CS		(GPIOB->BRR = O_MX25_CS)

//按键 LED

#define I_KEY_LF			((unsigned int)1<<8)	/* PA8 ++*/
#define O_WIFI_LED				((unsigned int)1<<15)	/* PA15++ */
#define O_BT_LED				((unsigned int)1<<14)	/* PA14++ */

#define O_STATUS_LED				((unsigned int)1<<3)	/* PB3++ */


#define GET_KEY_LF				((GPIOA->IDR) & I_KEY_LF)

#define GET_WIFI_LED_STA		(GPIOA->ODR & O_WIFI_LED)
#define SET_H_WIFI_LED			(GPIOA->BSRR = O_WIFI_LED)
#define SET_L_WIFI_LED			(GPIOA->BRR = O_WIFI_LED)


#define GET_BT_LED_STA		(GPIOA->ODR & O_BT_LED)
#define SET_H_BT_LED			(GPIOA->BSRR = O_BT_LED)
#define SET_L_BT_LED			(GPIOA->BRR = O_BT_LED)


#define GET_LED_STA		(GPIOB->ODR & O_STATUS_LED)
#define SET_H_LED			(GPIOB->BSRR = O_STATUS_LED)
#define SET_L_LED			(GPIOB->BRR = O_STATUS_LED)














#endif /* Board_h */
