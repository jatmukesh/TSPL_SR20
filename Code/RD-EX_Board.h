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
#ifndef RD_EX_BOARD_H
#define RD_EX_BOARD_H
#include "stm32f10x_gpio.h"


//串口 忙信号
#define O_BUSY_S			((unsigned int)1<<2)	/* PB2 */
//拨码
#define I_SW1_S				((unsigned int)1<<8)	/* PA8*/
//串口发送
#define O_TXD1_S			((unsigned int)1<<9)	/* PA9 */
//串口接收
#define I_RXD1_S			((unsigned int)1<<10)	/* PA10 */

#define I_SW2_S				((unsigned int)1<<15)	/* PA15*/
#define I_SW3_S				((unsigned int)1<<14)	/* PA14 */
#define I_SW4_S				((unsigned int)1<<13)	/* PA13 */
#define I_SW5_S				((unsigned int)1<<12)	/* PA12 */
#define I_SW6_S				((unsigned int)1<<11)	/* PA11*/


#define I_D0_P				((unsigned int)1<<8)	/* PA8 */
#define I_D1_P				((unsigned int)1<<9)	/* PA9 */
#define I_D2_P				((unsigned int)1<<10)	/* PA10 */
#define I_D3_P				((unsigned int)1<<11)	/* PA11 */
#define I_D4_P				((unsigned int)1<<12)	/* PA12 */
#define I_D5_P				((unsigned int)1<<13)	/* PA13 */
#define I_D6_P				((unsigned int)1<<14)	/* PA14 */
#define I_D7_P				((unsigned int)1<<15)	/* PA15 */

//并口接收
#define O_ACK_P				((unsigned int)1<<4)	/* PA4 */

/*
模拟输入: I_LPS_BM|I_TM|I_GET_VOLTAGE
输入: I_D0_P|I_D1_P|I_D2_P|I_D3_P|I_D4_P|I_D5_P|I_D6_P|I_D7_P|
输出: O_SWITCH|O_ACK_P|O_LAT
*/

//并口 中断信号
#define I_IRQ_P				((unsigned int)1<<0)	/* PB0 */


//悬空脚作为PE信号
#define O_PE_P				((unsigned int)1<<13)	/* PC13 */
//并口忙信号
#define O_BUSY_P			((unsigned int)1<<14)	/* PC14 */

//USB ????
#define O_USBPOWER_U		((unsigned int)1<<15)	/* PA15 */
//--------------------------------------串口机器IO---------------------------------------
#ifdef RD_TYPE_SERIAL
//--S
//得到外部电压
#define I_GET_VOLTAGE		((unsigned int)1<<0)	/* PA0 */
//打印头加热信号
#define I_TM				((unsigned int)1<<1)	/* PA1 */
//缺纸检测
#define I_LPS_BM			((unsigned int)1<<2)	/* PA2 */
//按键
#define I_KEY_LF			((unsigned int)1<<3)	/* PA3 */
//LED
#define O_LED				((unsigned int)1<<4)	/* PA4 */
//打印头时钟
#define O_CLK				((unsigned int)1<<5)	/* PA5 */
//打印头锁存信号
#define O_LAT				((unsigned int)1<<6)	/* PA6 */
//打印头数据
#define O_DI				((unsigned int)1<<7)	/* PA7 */

/*
模拟输入: I_GET_VOLTAGE|I_TM|I_LPS_BM
逻辑输入:I_KEY_LF|I_SW1_S|I_SW2_S|I_SW3_S|I_SW4_S|I_SW5_S|I_SW6_S
输出:O_LED|O_LAT
*/

#define STB6				((unsigned int)1<<0)	/* PB0*/
#define STB5				((unsigned int)1<<1)	/* PB1*/


#define STB1				((unsigned int)1<<3)	/* PB3*/
#define STB2				((unsigned int)1<<4)	/* PB4*/
#define STB3				((unsigned int)1<<5)	/* PB5*/

//电机控制信号
#define O_PWM_MOTO1			((unsigned int)1<<6)	/* PB6 */
#define O_PWM_MOTO2			((unsigned int)1<<7)	/* PB7 */
#define O_PWM_MOTO3			((unsigned int)1<<8)	/* PB8 */
#define O_PWM_MOTO4			((unsigned int)1<<9)	/* PB9 */

#define O_SCL				((unsigned int)1<<10)	/* PB10 */
#define I_SDA				((unsigned int)1<<11)	/* PB11 */

//SPI MX字库
#define O_MX25_CS			((unsigned int)1<<12)	/* PB12 */
#define O_MX25_SCK			((unsigned int)1<<13)	/* PB13 */
#define O_MX25_SO			((unsigned int)1<<14)	/* PB14 */
#define I_MX25_SI			((unsigned int)1<<15)	/* PB15 */

/*
输出:STB6|STB5|O_BUSY_S|STB1|STB2|STB3|O_MX25_CS

*/

//驱动电源脉冲信号
#define O_SWITCH			((unsigned int)1<<13)	/* PC13 */
#define STB4				((unsigned int)1<<14)	/* PC14*/
//需设置为悬空
#define LPS				((unsigned int)1<<15)	/* PC15*/

/*
输出:O_SWITCH|STB4
悬空:LPS


*/



#define GET_LED_STA		(GPIOA->ODR & O_LED)
#define SET_H_LED		(GPIOA->BSRR = O_LED)
#define SET_L_LED		(GPIOA->BRR = O_LED)


#define SET_STB1		(GPIOB->BSRR = STB3)
#define SET_STB2		(GPIOB->BSRR = STB2)
#define SET_STB3		(GPIOB->BSRR = STB5)
#define SET_STB5		(GPIOB->BSRR = STB1)
#define SET_STB6		(GPIOB->BSRR = STB6)
#define SET_STB4		(GPIOC->BSRR = STB4)

#define CLR_STB1		(GPIOB->BRR = STB3)
#define CLR_STB2		(GPIOB->BRR = STB2)
#define CLR_STB3		(GPIOB->BRR = STB5)
#define CLR_STB5		(GPIOB->BRR = STB1)
#define CLR_STB6		(GPIOB->BRR = STB6)
#define CLR_STB4		(GPIOC->BRR = STB4)



#define SET_H_STB1		SET_STB1;SET_STB3;SET_STB5
#define SET_L_STB1		CLR_STB1;CLR_STB3;CLR_STB5

#define SET_H_STB2		SET_STB4;SET_STB2;SET_STB6
#define SET_L_STB2		CLR_STB4;CLR_STB2;CLR_STB6


#define GET_SWITCH_STA	(GPIOC->ODR & O_SWITCH)
#define SET_H_SWITCH	(GPIOC->BSRR = O_SWITCH)
#define SET_L_SWITCH	(GPIOC->BRR = O_SWITCH)


#define GET_KEY_LF		((GPIOA->IDR) & I_KEY_LF)

#endif
#define O_CTS_TTL			((unsigned int)1<<13)// PA13

#define GET_CTS2		(GPIOA->ODR & O_CTS_TTL)
#define SET_H_CTS2		(GPIOA->BSRR = O_CTS_TTL)
#define SET_L_CTS2		(GPIOA->BRR = O_CTS_TTL)
#ifdef RD_TYPE_PARALLEL
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--P
#define I_LPS_BM			((unsigned int)1<<0)	/* PA0 */
//打印头加热信号
#define I_TM				((unsigned int)1<<1)	/* PA1 */
//得到外部电压
#define I_GET_VOLTAGE		((unsigned int)1<<2)	/* PA2 */
//驱动电源脉冲信号
#define O_SWITCH			((unsigned int)1<<3)	/* PA3 */

//打印头时钟
#define O_CLK				((unsigned int)1<<5)	/* PA5 */
//打印头锁存信号
#define O_LAT				((unsigned int)1<<6)	/* PA6 */
//打印头数据
#define O_DI				((unsigned int)1<<7)	/* PA7 */
//并口数据

#define STB1				((unsigned int)1<<1)	/* PB1*/
#define STB2				((unsigned int)1<<2)	/* PB2*/

#define STB3				((unsigned int)1<<3)	/* PB3*/
#define STB4				((unsigned int)1<<4)	/* PB4*/
#define I_KEY_LF			((unsigned int)1<<5)	/* PB5 */

//电机控制信号
#define O_PWM_MOTO1			((unsigned int)1<<6)	/* PB6 */
#define O_PWM_MOTO2			((unsigned int)1<<7)	/* PB7 */
#define O_PWM_MOTO3			((unsigned int)1<<8)	/* PB8 */
#define O_PWM_MOTO4			((unsigned int)1<<9)	/* PB9 */

#define O_SCL				((unsigned int)1<<10)	/* PB10 */
#define I_SDA				((unsigned int)1<<11)	/* PB11 */

//SPI MX字库
#define O_MX25_CS			((unsigned int)1<<12)	/* PB12 */
#define O_MX25_SCK			((unsigned int)1<<13)	/* PB13 */
#define O_MX25_SO			((unsigned int)1<<14)	/* PB14 */
#define I_MX25_SI			((unsigned int)1<<15)	/* PB15 */


/*
输入:I_KEY_LF
输出:STB1|STB2|STB3|STB4|O_MX25_CS


*/




#define O_LED				((unsigned int)1<<15)	/* PC15 */
/*
输出:O_PE_P|O_BUSY_P|O_LED


*/




#define GET_LED_STA		(GPIOC->ODR & O_LED)
#define SET_H_LED		(GPIOC->BSRR = O_LED)
#define SET_L_LED		(GPIOC->BRR = O_LED)



#define SET_STB1		(GPIOB->BSRR = STB1)
#define SET_STB2		(GPIOB->BSRR = STB2)
#define SET_STB3		(GPIOB->BSRR = STB3)
#define SET_STB4		(GPIOB->BSRR = STB4)

#define CLR_STB1		(GPIOB->BRR = STB1)
#define CLR_STB2		(GPIOB->BRR = STB2)
#define CLR_STB3		(GPIOB->BRR = STB3)
#define CLR_STB4		(GPIOB->BRR = STB4)

#define SET_H_STB1		SET_STB1;SET_STB3
#define SET_L_STB1		CLR_STB1;CLR_STB3

#define SET_H_STB2		SET_STB2;SET_STB4
#define SET_L_STB2		CLR_STB2;CLR_STB4


#define GET_SWITCH_STA	(GPIOA->ODR & O_SWITCH)
#define SET_H_SWITCH	(GPIOA->BSRR = O_SWITCH)
#define SET_L_SWITCH	(GPIOA->BRR = O_SWITCH)


#define GET_KEY_LF				((GPIOB->IDR) & I_KEY_LF)

#define GET_IRQ_P				((GPIOB->IDR) & I_IRQ_P)
#endif


//************************************************************************
#define SET_H_ACK		(GPIOA->BSRR = O_ACK_P)
#define SET_L_ACK		(GPIOA->BRR = O_ACK_P)

#define SET_H_LAT		(GPIOA->BSRR = O_LAT)
#define SET_L_LAT		(GPIOA->BRR = O_LAT)


#define SET_H_BUSY_S	(GPIOB->BSRR = O_BUSY_S)
#define SET_L_BUSY_S	(GPIOB->BRR = O_BUSY_S)

#define GET_BUSY_S		(GPIOB->ODR & O_BUSY_S)


#define SET_H_BUSY_P	(GPIOC->BSRR = O_BUSY_P)
#define SET_L_BUSY_P	(GPIOC->BRR = O_BUSY_P)

#define GET_BUSY_P		(GPIOC->ODR & O_BUSY_P)


#define SET_H_PE_P		(GPIOC->BSRR = O_PE_P)
#define SET_L_PE_P		(GPIOC->BRR = O_PE_P)


#define SET_H_MX_CS		(GPIOB->BSRR = O_MX25_CS)
#define SET_L_MX_CS		(GPIOB->BRR = O_MX25_CS)



#endif /* Board_h */
