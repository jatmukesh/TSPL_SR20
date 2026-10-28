
/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "PDL.h"
#include <stdio.h>
#include "HP12832_01_SSD1306_I2C.h"
#include "macros.h"

extern _PDL		PRINTER_PDL ;
extern void ServiceIRQ_A_Handler(void);
extern int Handler_main(void);
extern void ProcessMotoLineData(void);
extern void OneLinePrintDataSend(void);
extern void SysTickProFun(void);
extern unsigned short PUSB_ReceiveNBytes (unsigned char *dataBuffer,unsigned short len) ;
extern unsigned short BT_ReceiveNBytes (unsigned char *dataBuffer,unsigned short len) ;
extern void RS232_USART_Handler(void);
extern void TTL_USART_Handler(void); 
extern unsigned short g_PowerFlag;

extern unsigned int g_PowerOffCount;
extern unsigned int g_totaldatanum;

#ifdef CHECKSUM
uint32_t g_Flash_Calculated_Checksum;	//Runtime calculated 32-bit checksum value computed across Flash.
extern const uint32_t golden_checksum;
volatile uint32_t g_Stored_Checksum;	//Volatile variable holding runtime copy of golden_checksum
extern uint8_t g_Checksum_Flag;
int temp;
extern void Flash_Checksum_Failed(void);
extern uint32_t Flash_Checksum_Calculation(void);
#endif

 /**
		* @FuncName  main
		* @param     void
		* @retval    int 		(@review: Not Returing Int type)
		* @LocalVar  None
		* @GlobalVar PRINTER_PDL, g_PowerOffCount, g_PowerFlag, g_totaldatanum,
		*            g_Stored_Checksum, g_Flash_Calculated_Checksum, g_Checksum_Flag,
								 golden_checksum
		* @Description Firmware entry point. Configures system clock, links IRQ callbacks to the
										PDL layer, initializes system peripherals (Bluetooth, Motor, Thermal Print Head, RFID,
		*								SPI Flash, USB, ADC),
		*               verifies Flash checksum, and transfers control to Handler_main().
		*/
int main (void)
{ 

	//重新映射中断 // Remap interrupt
	//-重新映射打印机中断函数 // - Remap printer interrupt function
	SYSCTRL_HCLKConfig(SYSCTRL_HCLK_Div_None);
	g_PowerOffCount = 0;
	PRINTER_PDL.SYS._SystemSysTickIRQHandler = SysTickProFun;	//interrupt for system timers and delays
	PRINTER_PDL.SYS._SystemPrintIRQHandler = ProcessMotoLineData; // Stepper motor interrupt for managing motor steps and line feeding
	PRINTER_PDL.SYS._SystemPrintLineDatIRQHandler = OneLinePrintDataSend; // Printing of one dot-line 

	PRINTER_PDL.SERVICE._ServiceIRQ_A_Handler = ServiceIRQ_A_Handler;	// interrupt for strobe control and motor timing 
	
	
	PRINTER_PDL.RS232._RxInterruptIRQHandler = RS232_USART_Handler; //serial receive interrupt handler
	PRINTER_PDL.TTL._RxInterruptIRQHandler = TTL_USART_Handler;	//UART receive interrupt handler to process incoming communication
	g_PowerFlag = 0;
	PRINTER_PDL.USB._ReceiveNBytes = PUSB_ReceiveNBytes;  // Receiver function to process data packets over USB
	
#ifndef EMS_TEST_CODE	   //(Modification for EMS Vendor test code: Kajal 02 April 2026)	
	PRINTER_PDL.BT._ReceiveNBytes = BT_ReceiveNBytes;
#endif
	
	
	//初始化系统模块	// Initialize system modules
	PRINTER_PDL.SYS._SystemInit();
	//初始化系统IO		// Initialize system I/O

	PRINTER_PDL.SYS._SystemIOInit();
	
#ifdef CHECKSUM
	g_Stored_Checksum = golden_checksum;
	g_Flash_Calculated_Checksum = Flash_Checksum_Calculation();
//	g_Flash_Calculated_Checksum+=1;  //To create faluty condition of checksum calculation (Test)
	if(g_Flash_Calculated_Checksum == g_Stored_Checksum)
			g_Checksum_Flag = 1;


#endif	

		__RESETPRIMASK();
	//ADC功能模块初始化	// Initialize ADC function module
	//BT功能模块初始化		// Initialize Bluetooth function module
	PRINTER_PDL.BT._Init_Fun(0);
		enable_gpio_32();
//	PRINTER_PDL.ADC._GPIO_Init_Fun(4);	
	//打印中断初始化			// Initialize print interrupt
#ifdef MY_DEBUG_ENABLE	
	printf("1...\r\n");	
#endif	
	PRINTER_PDL.SYS._SystemPrintIRQInit();
	//处理一行打印数据中断初始化
	// Initialize interrupt for handling one line of print data
	PRINTER_PDL.SYS._SystemPrintLineDatInit();
	
	PRINTER_PDL.SERVICE._Init_Fun();
	//初始化系统时钟	// Initialize system clock
	PRINTER_PDL.SYS._SystemSysTickInit();


	//电机功能初始化 // Initialize motor functions
	PRINTER_PDL.MOTO._Init_Fun();
	//蓝牙串口初始化	// Initialize Bluetooth serial port
//	PRINTER_PDL.RS232._Init_Fun(0x06);
	//BT功能模块初始化	// Initialize Bluetooth function module
//	PRINTER_PDL.BT._Init_Fun(0);
	//初始化打印头数据发送	// Initialize printhead data transmission


	PRINTER_PDL.SDTH._Init_Fun();
	//RFID模块功能初始化	// Initialize RFID function module
	PRINTER_PDL.RFID._Init_Fun();

	//flash spi读取功能初始化	// Initialize flash SPI read function
	PRINTER_PDL.SPI._Init_Fun();
	 

	//USB 功能模块		// Initialize USB function module
	PRINTER_PDL.USB._Init_Fun();	

//	PRINTER_PDL.SYS._POWER_EN();//开启电源	// Power ON
	PRINTER_PDL.ADC._GPIO_Init_Fun(4);
	g_totaldatanum = 0;
#ifdef MY_DEBUG_ENABLE	
	printf("App Star...\r\n");
#endif

#ifdef CHECKSUM
	if(g_Checksum_Flag == 1)
	{
			Handler_main();
	}
	else
	{
//		// MyPrintf("\r\nChecksum Failed\r\n");
    g_PowerFlag = 0;
    Flash_Checksum_Failed();
	}
#else
	Handler_main();
#endif	
}

	//
//	PRINTER_PDL.SYS._POWER_EN();//开启电源
//	while(1)
//	{
//		PRINTER_PDL.SYS._LED_Charge_ON();
//		PRINTER_PDL.SYS._LED_Stauts_ON();		
//		PRINTER_PDL.SYS._WDG_ReloadCounter();  // reload the watchdog 重装载看门狗定时器
//	}

/** // Code Comment Format
  * @FuncName  
  * @param  
  * @retval 
	* @LocalVar
	* @GlobalVar
	* @Description
  */