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
#define O_PWM_MOTO1		((unsigned int)1<<0)	/* PA0 */
#define O_PWM_MOTO2		((unsigned int)1<<1)	/* PA1 */
#define O_PWM_MOTO3		((unsigned int)1<<2)	/* PA2 */
#define O_PWM_MOTO4		((unsigned int)1<<3)	/* PA3 */
//得到外部电压
#define I_GET_VOLTAGE		((unsigned int)1<<4)	/* PA4 */
//打印头时钟
#define O_CLK				((unsigned int)1<<5)	/* PA5 */
//缺纸检测
#define I_LPS_BM			((unsigned int)1<<6)	/* PA6 */
//打印头数据
#define O_DI				((unsigned int)1<<7)	/* PA7 */
//时钟芯片时钟
#define O_SCL				((unsigned int)1<<8)	/* PA8 */
//串口发送
#define O_TXD1_S			((unsigned int)1<<9)	/* PA9 */
//串口接收
#define I_RXD1_S			((unsigned int)1<<10)	/* PA10 */
//并口忙信号
#define O_BUSY_P			((unsigned int)1<<9)	/* PA9 */
//并口接收
#define O_ACK_P				((unsigned int)1<<10)	/* PA10 */
//USB DM
#define O_USBDM_U			((unsigned int)1<<11)	/* PA11 */
//USB DP
#define O_USBDP_U			((unsigned int)1<<12)	/* PA12 */
//并口 缺纸
#define O_PE_P				((unsigned int)1<<11)	/* PA11 */

//加热信号 STB
#define O_STB2				((unsigned int)1<<13)	/* PA13 */
//加热信号 STB
#define O_STB1				((unsigned int)1<<14)	/* PA14 */

//打印头锁存信号
#define O_LAT				((unsigned int)1<<15)	/* PA15 */

//并口 中断信号
#define I_IRQ_P				((unsigned int)1<<0)	/* PB0 */

//串口 忙信号
#define O_BUSY_S			((unsigned int)1<<0)	/* PB0 */

//打印头加热信号
#define I_TM				((unsigned int)1<<1)	/* PB1 */

//驱动电源脉冲信号
#define O_SWITCH			((unsigned int)1<<2)	/* PB2 */
//串口拨码
#ifdef SWITCH_BAUD
#define I_SW1_S				((unsigned int)1<<8)	/* PB8 */
#define I_SW2_S				((unsigned int)1<<7)	/* PB7 */
#define I_SW3_S				((unsigned int)1<<6)	/* PB6 */
#define I_SW4_S				((unsigned int)1<<5)	/* PB5 */
#define I_SW5_S				((unsigned int)1<<4)	/* PB4 */
#define I_SW6_S				((unsigned int)1<<3)	/* PB3 */
#endif

//并口数据
#define I_D0_P				((unsigned int)1<<3)	/* PB3 */
#define I_D1_P				((unsigned int)1<<4)	/* PB4 */
#define I_D2_P				((unsigned int)1<<5)	/* PB5 */
#define I_D3_P				((unsigned int)1<<6)	/* PB6 */
#define I_D4_P				((unsigned int)1<<7)	/* PB7 */
#define I_D5_P				((unsigned int)1<<8)	/* PB8 */
#define I_D6_P				((unsigned int)1<<9)	/* PB9 */
#define I_D7_P				((unsigned int)1<<10)	/* PB10 */

//USB连接检测
#define I_USBDET_U			((unsigned int)1<<9)	/* PB9 */

#ifdef RD_TYPE_SERIAL
#ifdef RD_TH 
//USB 上拉输出
#define O_USBPOWER_U		((unsigned int)1<<10)	/* PB10 */
#endif
#ifndef RD_TH 
//USB 上拉输出
#define O_USBPOWER_U		((unsigned int)1<<3)	/* PB3 */
#endif

//时钟芯片 数据


#endif

#ifdef TIME_PRINTER 
#ifndef DP_DG_PRINTER
//时钟芯片 数据
#define O_SDA				((unsigned int)1<<11)	/* PB11 */
#endif

#ifdef DP_DG_PRINTER
//时钟芯片 数据
#define O_SDA				((unsigned int)1<<8)	/* PB8 */
#endif
#endif

#ifdef RD_TYPE_PARALLEL
//USB 上拉输出
#define O_USBPOWER_U		((unsigned int)0<<0)	/* PB10 */





#endif

// #define O_CTS_TTL			((unsigned int)1<<7)	/* PB7 */

#define O_CTS_TTL			(((unsigned int)1<<7)|((unsigned int)1<<5))	/* PB7 PB5*/
//SPI MX字库
#define O_MX25_CS			((unsigned int)1<<12)	/* PB12 */
#define O_MX25_SCK			((unsigned int)1<<13)	/* PB13 */
#define O_MX25_SO			((unsigned int)1<<14)	/* PB14 */
#define I_MX25_SI			((unsigned int)1<<15)	/* PB15 */


//按键 LED
#define I_KEY_SEL			((unsigned int)1<<13)	/* PC13 */
#define I_KEY_LF			((unsigned int)1<<14)	/* PC14 */
#define O_LED				((unsigned int)1<<15)	/* PC15 */



#define GET_LED_STA		(GPIOC->ODR & O_LED)
#define SET_H_LED		(GPIOC->BSRR = O_LED)
#define SET_L_LED		(GPIOC->BRR = O_LED)

#define SET_H_LAT		(GPIOA->BSRR = O_LAT)
#define SET_L_LAT		(GPIOA->BRR = O_LAT)

#define SET_H_ACK		(GPIOA->BSRR = O_ACK_P)
#define SET_L_ACK		(GPIOA->BRR = O_ACK_P)

#define SET_H_STB1		(GPIOA->BSRR = O_STB1)
#define SET_L_STB1		(GPIOA->BRR = O_STB1)

#define SET_H_STB2		(GPIOA->BSRR = O_STB2)
#define SET_L_STB2		(GPIOA->BRR = O_STB2)

#define SET_H_BUSY_S	(GPIOB->BSRR = O_BUSY_S)
#define SET_L_BUSY_S	(GPIOB->BRR = O_BUSY_S)

#define GET_BUSY_S		(GPIOB->ODR & O_BUSY_S)


#define SET_H_BUSY_P	(GPIOA->BSRR = O_BUSY_P)
#define SET_L_BUSY_P	(GPIOA->BRR = O_BUSY_P)

#define GET_BUSY_P		(GPIOA->ODR & O_BUSY_P)


#define SET_H_PE_P		(GPIOA->BSRR = O_PE_P)
#define SET_L_PE_P		(GPIOA->BRR = O_PE_P)

#define GET_SWITCH_STA	(GPIOB->ODR & O_SWITCH)
#define SET_H_SWITCH	(GPIOB->BSRR = O_SWITCH)
#define SET_L_SWITCH	(GPIOB->BRR = O_SWITCH)

#define SET_H_MX_CS		(GPIOB->BSRR = O_MX25_CS)
#define SET_L_MX_CS		(GPIOB->BRR = O_MX25_CS)

#define GET_CTS2		(GPIOB->ODR & O_CTS_TTL)
#define SET_H_CTS2		(GPIOB->BSRR = O_CTS_TTL)
#define SET_L_CTS2		(GPIOB->BRR = O_CTS_TTL)

#define SET_H_USBPOWER_U		(GPIOB->BSRR = O_USBPOWER_U)
#define SET_L_USBPOWER_U		(GPIOB->BRR = O_USBPOWER_U)

#define GET_USBDET_U			((GPIOB->IDR) & I_USBDET_U)

#define GET_KEY_SEL				((GPIOC->IDR) & I_KEY_SEL)
#define GET_KEY_LF				((GPIOC->IDR) & I_KEY_LF)

#define GET_IRQ_P				((GPIOB->IDR) & I_IRQ_P)


#define SCL_H         GPIOA->BSRR = O_SCL
#define SCL_L         GPIOA->BRR  = O_SCL

#define SDA_H         GPIOB->BSRR = O_SDA
#define SDA_L         GPIOB->BRR  = O_SDA

#define SCL_read      GPIOA->IDR  & O_SCL
#define SDA_read      GPIOB->IDR  & O_SDA


#endif /* Board_h */
