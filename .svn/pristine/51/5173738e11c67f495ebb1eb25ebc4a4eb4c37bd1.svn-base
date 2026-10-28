#ifndef DRIVE_h
#define DRIVE_h
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                 /* STM32 Initialization              */
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "PrinterParType.h"

#ifdef RD_MICROPRINTER
#include "RD-MP_Board.h"
#endif

#ifdef RD_W2C1PRINTER
#include "RD-W2C1_Board.h"
#endif

#ifdef RD_OEMPRINTER
#include "RD-OEM_Board.h"
#endif

#ifdef RD_EXPRINTER
#include "RD-EX_Board.h"
#endif

#ifdef RD_MP3PRINTER
#include "RD-MP3_Board.h"
#endif

#ifdef RD_BMPRINTER  
#include "RD-BM_Board.h"
#endif
#ifdef RD_TSPRINTER  
#include "RD-TS_Board.h"
#endif

#ifdef RD_A100PRINTER  
#include "RD-A100_Board.h"
#endif
#ifdef RD_A200PRINTER  
#include "RD-A200_Board.h"
#endif



#define U8	unsigned  char
#define U16	unsigned  short
#define U32	unsigned  int
#define BYTE	unsigned  char
#define NULL  0
typedef unsigned short      WORD;
typedef BYTE        *LPBYTE;


//------------------------------------------------------------
#ifndef MOTO_STEP_FX

#define MOTO_RUN_HALF_1_CODR			(O_PWM_MOTO1|O_PWM_MOTO4)
#define MOTO_RUN_HALF_2_CODR			(O_PWM_MOTO4)
#define MOTO_RUN_HALF_3_CODR			(O_PWM_MOTO2|O_PWM_MOTO4)
#define MOTO_RUN_HALF_4_CODR			(O_PWM_MOTO2)
#define MOTO_RUN_HALF_5_CODR			(O_PWM_MOTO2|O_PWM_MOTO3)
#define MOTO_RUN_HALF_6_CODR			(O_PWM_MOTO3)
#define MOTO_RUN_HALF_7_CODR			(O_PWM_MOTO1|O_PWM_MOTO3)
#define MOTO_RUN_HALF_8_CODR			(O_PWM_MOTO1)

#endif

#ifdef MOTO_STEP_FX

#define MOTO_RUN_HALF_8_CODR			(O_PWM_MOTO1|O_PWM_MOTO4)
#define MOTO_RUN_HALF_7_CODR			(O_PWM_MOTO4)
#define MOTO_RUN_HALF_6_CODR			(O_PWM_MOTO2|O_PWM_MOTO4)
#define MOTO_RUN_HALF_5_CODR			(O_PWM_MOTO2)
#define MOTO_RUN_HALF_4_CODR			(O_PWM_MOTO2|O_PWM_MOTO3)
#define MOTO_RUN_HALF_3_CODR			(O_PWM_MOTO3)
#define MOTO_RUN_HALF_2_CODR			(O_PWM_MOTO1|O_PWM_MOTO3)
#define MOTO_RUN_HALF_1_CODR			(O_PWM_MOTO1)


#endif


//------------------------------------------------------------
//------------------------------------------------------------
/*
#define MOTO_RUN_HALF_1_CODR			(O_PWM_MOTO1|O_PWM_MOTO4)
#define MOTO_RUN_HALF_2_CODR			(O_PWM_MOTO2|O_PWM_MOTO4)
#define MOTO_RUN_HALF_3_CODR			(O_PWM_MOTO2|O_PWM_MOTO3)
#define MOTO_RUN_HALF_4_CODR			(O_PWM_MOTO1|O_PWM_MOTO3)
#define MOTO_RUN_HALF_5_CODR			(O_PWM_MOTO1|O_PWM_MOTO4)
#define MOTO_RUN_HALF_6_CODR			(O_PWM_MOTO2|O_PWM_MOTO4)
#define MOTO_RUN_HALF_7_CODR			(O_PWM_MOTO2|O_PWM_MOTO3)
#define MOTO_RUN_HALF_8_CODR			(O_PWM_MOTO1|O_PWM_MOTO3)
*/
//------------------------------------------------------------

void RCC_Configuration(void);
void SystemGPIOInit(void);
void PWM_Configuration(void);
void ADC1_Configuration(void);
void DMA_Configuration(void);
void Timer_Configuration(void);
int SendChar (int ch);
void USART1_InitConfig(unsigned short ch);
void USART2_InitConfig(unsigned short ch);
void USART3_InitConfig(unsigned short ch);
void Exit_Configuration(void);
void SetMotoPWM(unsigned int value,unsigned short ParTime);
void SetMotoPWM_A(unsigned int value);
unsigned char GetVolteV(void);
void WDG_Init(void);
unsigned short PrinterInterface(void);
unsigned short ReadSwStatus(void);
void MOTORUN(void);
void PrinterLPS(void);
void Mtrig(void);
unsigned int RevInt(unsigned int x);
int CalintCount(unsigned int x);
void Transpose32(unsigned int a[32]) ;
void TIM_ENABLE(TIM_TypeDef* TIMx,unsigned short TimeLen);
void TIM_DISABLE(TIM_TypeDef* TIMx);
unsigned char GetCurrentVol(void);
void ClearMoto(void);
void PrinterIdle(void);
void PrinterBusy(void);
unsigned char GetTM(void);
void JudgeSerialBaud(void);
unsigned char ReadKeyPlus(void);
void SysTick_Init(void);
void RunFeedLabPaper_order(void);
void AutoMOTORUN(void);
void MOTORUN_FX(unsigned short steplen);
#endif
