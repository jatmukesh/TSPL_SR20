#include "Drive.h"
unsigned short AD_Value[3];
extern unsigned short g_LPSStatus,g_PE_Standard;//缺纸检测器状态
extern unsigned short	g_PaperEnd;//缺纸
extern PrintrConfig	s_PrintrConfig;//打印机配置参数
extern PrinterStruct	s_PrintParameters;//打印机参数
extern PrintControl	s_PrinterMOTOPar;//打印机步进电机参数
extern unsigned int	g_CurrentValue;//当前的电压值
extern const U32 PrintMotoHalfStep[8];
extern unsigned short	g_MotoAll;//电机走步索引
extern unsigned int FlowFlag;//波特率
unsigned short key_Cont;
unsigned short g_ConfigRCC;
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{

	ErrorStatus HSEStartUpStatus;

 //-------------------------------

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);//使能外部时钟

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待外部时钟就绪

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer 启用预取缓冲器 */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state FLASH设置1个等待周期*/
    FLASH_SetLatency(FLASH_Latency_1);
    /* HCLK = SYSCLK 设置系统设置*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK PCLK2时钟=主时钟*/
     RCC_PCLK2Config(RCC_HCLK_Div1);
   // RCC_PCLK2Config(RCC_HCLK_Div8);
    /* PCLK1 = HCLK/2 PCLK1时钟为主时钟1/2*/
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 12MHz * 6 = 72 MHz 设置时钟为24M*/
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_3);

    /* Enable PLL 使能PLL*/
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready 等待PLL工作稳定*/
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source 选择PLL做为系统时钟源*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source 准备就绪，开始干活*/
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

   //启动AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


}
/****************************************************************************
* 函数名：SystemGPIOInit()
* 功  能：初始化IO
*
* 说  明：

****************************************************************************/
void SystemGPIOInit(void)
{

 	GPIO_InitTypeDef GPIO_InitStructure;
 	//*********************************配置GPIOA**************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	/*
	O_PWM_MOTO1 O_PWM_MOTO2 O_PWM_MOTO3 O_PWM_MOTO4 配置为PWM
	I_Get_Voltage I_LPS_BM 配置为AD
	O_CLK 配置为SPI
	*/
	//-------------------------配置输出------------------------------
	//电机驱动 忙信号
	//配置端口PA8--PA12 为输出推挽输出
	GPIO_InitStructure.GPIO_Pin = O_BUSY_S | O_LED | O_PWM_MOTO1 | O_PWM_MOTO2 | O_PWM_MOTO3 | O_PWM_MOTO4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最大输出速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;////通用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 //设置GPIO为输出

	//-------------------------配置输入------------------------------
	//配置AD输入
	//配置端口
	GPIO_InitStructure.GPIO_Pin = I_GET_VOLTAGE|I_BL_Mark;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//悬浮模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I_KEY_LF;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//悬浮模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//*******************************配置端口 GPIOB**************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
	//配置AD输入
	//配置 热敏头温度检测
	GPIO_InitStructure.GPIO_Pin = I_LPS|I_TM;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//悬浮模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//-------------------------配置输出------------------------------
	//打印头数据锁存信号 STB 加热信号 逻辑电源 VCC_CON MX25_CS
	//配置端口PB3--PB9 为输出推挽输出 PB11 VCC_CON
	GPIO_InitStructure.GPIO_Pin = O_LAT|O_MX25_CS|O_STB1|O_STB2|O_STB3|O_STB4|O_STB5|O_STB6|O_VCC_CON;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最大输出速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;////通用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);			 //设置GPIO为输出
	//-------------------------配置输入------------------------------

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);


	//*******************************配置端口 GPIOC**************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOC时钟
	//配置打印头驱动电源 POWER
	//配置端口 PC15
	//-------------------------配置输出------------------------------
	GPIO_InitStructure.GPIO_Pin = O_POWER;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最大输出速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;////通用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);			 //设置GPIO为输出




	SET_L_STB1;SET_L_STB2;
	SET_H_LAT;
}
unsigned short PrinterInterface(void)
{
	return 0;
}
void PWM_Configuration(void)
{


    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	//-------PWM------------------------
	//Step1.开启TIM和相应端口时钟
	//启动GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	//启动AFIO
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//启动TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//Step2. GPIO做相应设置，为AF输出
	//PA.8/9口设置为TIM1的OC1输出口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    //TIM1基本计数器设置（设置PWM频率）
    //频率=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 1000;
    TIM_BaseInitStructure.TIM_Prescaler = 1;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = 0;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //启用ARR的影子寄存器（直到产生更新事件才更改设置）
    TIM_ARRPreloadConfig(TIM1, ENABLE);


    //TIM1_OC1模块设置（设置1通道占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //启用CCR1寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    //TIM1_OC2模块设置（设置2通道占空比）
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    //启用CCR2寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

     //TIM1_OC3模块设置（设置3通道占空比）
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    //启用CCR2寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);


       //TIM1_OC4模块设置（设置4通道占空比）
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 200;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    //启用CCR2寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);


    //死区设置
 	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	TIM1->CCR4 = 0;
    //TIM1开启
    TIM_Cmd(TIM1, ENABLE);
    //TIM1_OC通道输出PWM（一定要加）
    TIM_CtrlPWMOutputs(TIM1, ENABLE);



}


//常规转换序列1：通道10  对应电位器
//常规转换序列2：通道16（内部温度传感器）
//==============================================================
void ADC1_Configuration(void)
{
	int i;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div4) ;

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描方式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换开启
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发禁止
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐方式
	ADC_InitStructure.ADC_NbrOfChannel = 3;      //设置转换序列长度为2
	ADC_Init(ADC1, &ADC_InitStructure);//调用函数，初始化ADC1

	//ADC内置温度传感器使能（要使用片内温度传感器，切忌要开启它）
	// ADC_TempSensorVrefintCmd(ENABLE);

	//常规转换序列1：通道4 外部输入电压检测
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	//常规转换序列2：通道6 缺纸检测
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
	//常规转换序列3：通道9 打印头温度检测
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_55Cycles5);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);
	for(i=0;i<0xffff;i++);
	// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_DMACmd(ADC1, ENABLE);
	for(i=0;i<0xffff;i++);
	// 下面是ADC自动校准，开机后需执行一次，保证精度
	// Enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);
	// Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));
	// Start ADC1 calibaration
	ADC_StartCalibration(ADC1);
	// Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));
	// ADC自动校准结束---------------
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}
/*******************************************************************************
* Function Name : DMA_Configuration
* Description     : DMA设置：从ADC模块自动读转换结果至内存
* Input           : None
* Output          : None
* Return          : None
*******************************************************************************/
void DMA_Configuration(void)
{
     DMA_InitTypeDef DMA_InitStructure;

      //启动DMA时钟
   	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
     DMA_DeInit(DMA1_Channel1);
     DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&ADC1->DR;//外设地址
     DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)&AD_Value;//内存地址/*自己开辟的数组*/
     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA单向传输方式
     //BufferSize=2，因为ADC转换序列有2个通道
    //如此设置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]
     DMA_InitStructure.DMA_BufferSize = 3;
     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA外设递增模式
     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //设置DMA内存递增模式
     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设字长
     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存地址
     //循环模式开启，Buffer写满后，自动回到初始地址开始传输
     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 设置传输模式连续不断的循环模式
     DMA_InitStructure.DMA_Priority = DMA_Priority_High;//设置DMA优先级
     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 设置DMA的2个memory中的变量互相访问
     DMA_Init(DMA1_Channel1, &DMA_InitStructure);
     //配置完成后，启动DMA通道
     DMA_Cmd(DMA1_Channel1, ENABLE);
}
void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	//-----------------定时器2配置-------------------------------------------
	//打开定时器的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//------------------------------------------------------------
	//TIMx寄存器重设为缺省值
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=2000;  //定时器初始值
	TIM_TimeBaseStructure.TIM_Prescaler=36;         //时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  // 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);   //初始化定时器的值
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);               //清除定时器中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  //使能中断
	//TIM_Cmd(TIM2,ENABLE);            //开启时钟
  //------------------------------------------------------------
  //配置TIM2中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//-----------------定时器3配置-------------------------------------------
	//打开定时器的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//------------------------------------------------------------
	//TIMx寄存器重设为缺省值
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=600;  //定时器初始值
	TIM_TimeBaseStructure.TIM_Prescaler=0;         //时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  // 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);   //初始化定时器的值
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);               //清除定时器中断标志
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  //使能中断
	//TIM_Cmd(TIM3,ENABLE);            //开启时钟
  //------------------------------------------------------------
  //配置TIM2中断
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


#if 0
	//-----------------定时器4配置-------------------------------------------
	//打开定时器的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	//------------------------------------------------------------
	//TIMx寄存器重设为缺省值
	TIM_DeInit(TIM6);
	TIM_TimeBaseStructure.TIM_Period=2000;  //定时器初始值
	TIM_TimeBaseStructure.TIM_Prescaler=3600;         //时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  // 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);   //初始化定时器的值
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);               //清除定时器中断标志
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);  //使能中断
	//TIM_Cmd(TIM2,ENABLE);            //开启时钟
  //------------------------------------------------------------
  //配置TIM2中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

}
void SysTick_Init(void)
{

  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
  //设置系统定时器中断时间为1ms
  SysTick_SetReload(4500);
  /* Enable SysTick interrupt */
  //允许系统定时器中断
  SysTick_ITConfig(ENABLE);
  /* Enable the SysTick Counter */
  //使能系统定时器
  SysTick_CounterCmd(SysTick_Counter_Enable);
}
void TIM_ENABLE(TIM_TypeDef* TIMx,unsigned short TimeLen)
{
	TIMx->SR = (u16)~TIM_IT_Update;//清除中断标志
	TIMx->ARR = TimeLen;//加热时间
	TIMx->CR1|=0x01;//定时器2时钟使能
}
void TIM_DISABLE(TIM_TypeDef* TIMx)
{
	TIMx->SR = (u16)~TIM_IT_Update;//清除中断标志
	TIMx->CR1 &= 0xfffffffe;//关闭定时器//停止计数器时钟
	TIMx->CNT = 0;
}

int SendChar (int ch)
{
	int i;
	//等待发送结束
	i = 0x3000;
	while (!(USART2->SR & USART_FLAG_TXE))
	{
		i--;
		if(i < 10)
		{
			break;
		}
	}
	//将数据放入发送寄存器
	USART2->DR = (ch & 0xFF);
		i = 0x3000;
	while (!(USART2->SR & USART_FLAG_TC))
	{
		i--;
		if(i < 10)
		{
			break;
		}
	}		
	return (ch);
}
//
/*----------------------------------------------------------------------------
 Define  Baudrate setting (BRR) for USART2
 *----------------------------------------------------------------------------*/
/*******************************************************************************
* 函数名：USART1_InitConfig()
* 功  能：初始化串口1
*
* 说  明：
*******************************************************************************/
#ifdef RD_USART2_EN
void USART2_InitConfig(unsigned short ch)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;

	switch(FlowFlag & 0x0F)
	{
	case 0:
				USART_InitStructure.USART_BaudRate = 150;
			break;
		case 1:
				USART_InitStructure.USART_BaudRate = 300;
			break;
		case 2:
				USART_InitStructure.USART_BaudRate = 600;
			break;
		case 3:
				USART_InitStructure.USART_BaudRate = 1200;
			break;
		case 4:
				USART_InitStructure.USART_BaudRate = 2400;
			break;
		case 5:
				USART_InitStructure.USART_BaudRate = 4800;
			break;
		case 6:
				USART_InitStructure.USART_BaudRate = 9600;
			break;
		case 7:
				USART_InitStructure.USART_BaudRate = 19200;
			break;
		case 8:
				USART_InitStructure.USART_BaudRate = 38400;
			break;
		case 9:
				USART_InitStructure.USART_BaudRate = 57600;
			break;
		case 10:
				USART_InitStructure.USART_BaudRate = 115200;
			break;
		default:
			USART_InitStructure.USART_BaudRate = 9600; // set baudrate
	}
	if(ch == 0xAAAA)
	{
		USART_InitStructure.USART_BaudRate = 115200;
	}


	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
									 //USART_WordLength_8b; //8 位数据
									 //USART_WordLength_9b; //9 位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
								   //USART_StopBits_1  ;//在帧结尾传输 1 个停止位
								   //USART_StopBits_0.5;//在帧结尾传输 0.5 个停止位
								   //USART_StopBits_2  ;//在帧结尾传输 2 个停止位
								   //USART_StopBits_1.5;//在帧结尾传输 1.5 个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;
								 //USART_Parity_No  ;//奇偶失能
								 //USART_Parity_Even;//偶模式
								 //USART_Parity_Odd ;//奇模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
											  //USART_HardwareFlowControl_None;   //硬件流控制失能
											  //USART_HardwareFlowControl_RTS;    //发送请求 RTS使能
											  //USART_HardwareFlowControl_CTS;    //清除发送 CTS使能
											  //USART_HardwareFlowControl_RTS_CTS;//RTS和 CTS使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
							   //USART_Mode_Tx;//发送使能
							   //USART_Mode_Rx;//接收使能
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
									 //USART_Clock_Enable ;//时钟高电平活动
									 //USART_Clock_Disable;//时钟低电平活动
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
									//USART_CPOL_High;//时钟高电平
									//USART_CPOL_Low ;//时钟低电平
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
									//USART_CPHA_1Edge;//时钟第一个边沿进行数据捕获
									//USART_CPHA_2Edge;//时钟第二个边沿进行数据捕获
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
									   //USART_LastBit_Disable;//最后一位数据的时钟脉冲不从 SCLK输出
									   //USART_LastBit_Enable ;//最后一位数据的时钟脉冲从 SCLK输出
	USART_Init(USART2, &USART_InitStructure);//初始化外设 USARTx 寄存器
	USART_Cmd(USART2, ENABLE);
				//ENABLE-DISABLE//使能或者失能 USART 外设
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//ENABLE-DISABLE//使能或者失能指定的 USART 中断
					//USART_IT_PE  //奇偶错误中断
					//USART_IT_TXE //发送中断
					//USART_IT_TC  //传输完成中断
					//USART_IT_RXNE//接收中断
					//USART_IT_IDLE//空闲总线中断
					//USART_IT_LBD //LIN中断检测中断
					//USART_IT_CTS //CTS中断
					//USART_IT_ERR //错误中断


	USART_ClockInit(USART2, &USART_ClockInitStructure);



	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif
#ifdef RD_TYPE_PARALLEL
void Exit_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//外部中断使用的RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//使能GPIO时钟

	//外部中断使用的GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect EXTI Line12 to PC.12 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

	/* Configure EXTI Line12 to generate an interrupt on falling edge */
	//配置外部中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Generate software interrupt: simulate a falling edge applied on EXTI line 12 */
	//这里是软件模拟产生中断
	EXTI_GenerateSWInterrupt(EXTI_Line0);

	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		/* Clear the EXTI line 12 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

	//外部中断使用的NVIC配置
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable the EXTI15_10 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif

void SetMotoPWM(unsigned int value,unsigned short ParTime)
{
	if(value & O_PWM_MOTO1)
	{
		TIM1->CCR1 = 1000;
	}
	else
	{
		TIM1->CCR1 = ParTime;// 500;
		//TIM2->CCR1 = ParTime * 60;
	}
	if(value & O_PWM_MOTO2)
	{
		TIM1->CCR2 = 1000;
	}
	else
	{
		TIM1->CCR2 = ParTime;//500;
		//TIM2->CCR2 = ParTime * 60; ;
	}
	if(value & O_PWM_MOTO3)
	{
		TIM1->CCR3 =1000;
	}
	else
	{
		TIM1->CCR3 =ParTime;//500;
		//TIM2->CCR3 = ParTime * 60; ;
	}
	if(value & O_PWM_MOTO4)
	{
		TIM1->CCR4 = 1000;
	}
	else
	{
		TIM1->CCR4 = ParTime;//500;
		//TIM2->CCR4 = ParTime * 60; ;
	}
}

void SetMotoPWM_A(unsigned int value)
{
	if(value & O_PWM_MOTO1)
	{
		TIM1->CCR1 = 1000;
	}
	else
	{
		TIM1->CCR1 = 10;
	}
	if(value & O_PWM_MOTO2)
	{
		TIM1->CCR2 = 1000;
	}
	else
	{
		TIM1->CCR2 = 10;
	}
	if(value & O_PWM_MOTO3)
	{
		TIM1->CCR3 =1000;
	}
	else
	{
		TIM1->CCR3 = 10;
	}
	if(value & O_PWM_MOTO4)
	{
		TIM1->CCR4 = 1000;
	}
	else
	{
		TIM1->CCR4 = 10;
	}
}
/*
unsigned char GetVolteV(void)
{
	unsigned sum,i,k;
	sum = 0;
	for(i=0;i<64;i++)
	{
		for(k=0;k<500;k++);
		sum += AD_Value[0]>>4;
	}
	sum = sum >> 6;
	return sum;
}


*/

//对输入电压进行AD采样后中值滤波
#define N 11
unsigned char GetVolteV(void)
{
	unsigned char value_buf[N];
	unsigned short count,i,j,temp;
	for ( count=0;count<N;count++)
	{
		value_buf[count] = ((AD_Value[0]>>4) & 0xFF);
		for(j=0;j<1000;j++);

		IWDG_ReloadCounter();
	}
	for (j=0;j<N-1;j++)
	{
		IWDG_ReloadCounter();
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
	return value_buf[(N-1)/2];
}
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
unsigned short VolDetStand[]= {112,120,128,136,145,150,159,167,175,182,190,197,205,212,220,228,235,243,250,2000};
unsigned char GetCurrentVol(void)
{
	unsigned short data,k;
	data = GetVolteV();
	k = 0;
	while(1)
	{
		if(data < VolDetStand[k])
		{
			break;
		}
		k++;
	}
	return k;
}



void WDG_Init(void)
{
	/* Check if the system has resumed from IWDG reset -------------------------*/
	//检查看门狗复位标志
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* IWDGRST flag set */
		/* Turn on led connected to PC.08 */
		//点亮LED
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);

		/* Clear reset flags */
		//清除复位标志
		RCC_ClearFlag();
	}
	else
	{
		/* IWDGRST flag is not set */
		/* Turn off led connected to PC.08 */
		//熄灭LED
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
	}

	/* IWDG timeout equal to 280 ms (the timeout may varies due to LSI frequency
	  dispersion) -------------------------------------------------------------*/
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	//允许IWDG被修改
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
	//设置IWDG时钟
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	/* Set counter reload value to 349 */
	//设置定时器重载值
	IWDG_SetReload(700);

	/* Reload IWDG counter */
	//重载IWDG计数器
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	//使能IWDG
	IWDG_Enable();

	IWDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器

}
 unsigned short ReadSwStatus(void)
{
	return 0;
}
void ClearMoto(void)
{
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	TIM1->CCR4 = 0;
}
void MOTORUN(void)
{
	int i,j;
	SET_H_POWER;//使能驱动电源
	for(i=0;i<100;i++)
	{
		ClearMoto();

		IWDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
		SetMotoPWM(PrintMotoHalfStep[g_MotoAll  & 7],100);
		g_MotoAll++;
		Mtrig();
		for(j=0;j<0x2000;j++);
	//	SendChar(AD_Value[0] >> 4);
	//	SendChar(AD_Value[1] >> 4);
	//	SendChar(AD_Value[2] >> 4);

	}
	ClearMoto();
	SET_L_POWER;//关闭驱动电源
}
void Mtrig(void)
{
	IWDG_ReloadCounter(); //reload the watchdog 重装载看门狗定时器
}

//设置打印机空闲
void PrinterIdle(void)
{
	SET_L_BUSY_S;
}
//设置打印机忙碌
void PrinterBusy(void)
{
	SET_H_BUSY_S;
}

const unsigned char  TM_Index[]={232,218,200,177,153,127,103,82,65};
unsigned char GetTM(void)
{
	unsigned i,k,data;


	i=0;
	data = AD_Value[2] >> 4;
	for(k = 0;k<9;k++)
	{
		if(data < TM_Index[k])
		{
			i++;
		}
	}
	return i;
}

 /****************************************************************************
 * 函数名：RAMAIC_EndISR()
 * 功  能：反转一个字
  ****************************************************************************/
unsigned int RevInt(unsigned int x) {
   x = (x & 0x55555555) <<  1 | (x >>  1) & 0x55555555;
   x = (x & 0x33333333) <<  2 | (x >>  2) & 0x33333333;
   x = (x & 0x0F0F0F0F) <<  4 | (x >>  4) & 0x0F0F0F0F;
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
 * 函数名：void KeyRead( void )
 * 功  能：按键程序
 不按键 Trg = 0 Cont = 0
 按下一次
 Trg ＝ 0x01;Trg只会在这个时候对应位的值为1，其它时候都为0
 Cont ＝ 0x01;
 （3）长按键）的情况
 ReadData ＝ 0x01；这个不会变，因为按键没有松开
 Trg ＝ ReadData & (ReadData ^ Cont) ＝ 0x01 & (0x01 ^ 0x01) = 0 ，只要按键没有松开，这个Trg值永远为 0 ！！！
Cont ＝ 0x01；只要按键没有松开，这个值永远是0x01！！
  ****************************************************************************/
/*
unsigned char Trg;
unsigned char Cont;
void KeyRead( void )
{
    unsigned char ReadData = PINB^0xff;// 1
    Trg = ReadData & (ReadData ^ Cont);// 2
    Cont = ReadData;// 3
}
*/
 /****************************************************************************
 * 函数名：unsigned char ReadKeyPlus(void)
 * 功  能：判断按键是否按下
  ****************************************************************************/

unsigned char ReadKeyPlus(void)
{
	unsigned short i,temp,key_Trg;
star_key:

	key_Trg = GET_KEY_LF;
	for(i=0;i<800;i++)
	{
		IWDG_ReloadCounter();
	}
	if(GET_KEY_LF != key_Trg)
	{
		goto star_key;
	}
	for(i=0;i<1000;i++)
	{
		IWDG_ReloadCounter();
	}
	if(GET_KEY_LF != key_Trg)
	{

		goto star_key;
	}

	temp = key_Trg^0xFFFF;// 1
	key_Trg = temp & (temp ^ key_Cont);// 2
	key_Cont = temp;// 3


	if(key_Trg & I_KEY_LF)
	{
		return 1;
	}
	return 0;
}
