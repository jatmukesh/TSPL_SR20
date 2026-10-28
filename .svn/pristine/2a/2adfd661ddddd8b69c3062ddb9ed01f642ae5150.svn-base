
  #ifdef PRINTER_AND_USB 
/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
   #include "PrinterParType.h"
	 #include "Drive.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 buffer_out[VIRTUAL_PRINTER_DATA_SIZE];
u32 count_out = 0;
u32 count_in = 0;
	extern int SendChar (int ch)  ;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP3_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
extern unsigned short *p_REAndSAcount;//保存数据个数的指针
extern  unsigned short *p_SaveDrREcount;//存储数据到缓冲区的偏移量
extern unsigned char  receive_bufer[1024*2];
unsigned short g_ReceiveBusyFlag;//保存数据个数的指针
unsigned short g_reflag;
extern unsigned short g_LPSStatus,g_PE_Standard;//缺纸检测器状态
extern PrintControl	s_PrinterMOTOPar;//打印机步进电机参数
extern unsigned short g_ReturnBusy;
void EP1_OUT_Callback(void)
{
	int i;
	unsigned char data2;
	unsigned char data_buffer[5];
	count_out = GetEPRxCount(ENDP1);
	PMAToUserBufferCopy(buffer_out, ENDP1_RXADDR, count_out);
	if(g_ReceiveBusyFlag == 0xAAAA)
	{
		return;
	}

	for(i=0;i<count_out;i++)
	{
		//SendChar(buffer_out[i]);
		(*p_REAndSAcount)++;//缓冲区得到的数据个数
		g_reflag = g_reflag << 8;
		g_reflag |= buffer_out[i];
		if(g_reflag == 0x1B76)
		{
			data2 = 0;
			if(g_LPSStatus < g_PE_Standard)
			{
				data2 |= 1; //等于1表示缺纸
			}
			if(s_PrinterMOTOPar.m_PrintEnd != ENABLE_VAR)
			{
				data2 |= 2;//=2表示打印机正在运行
			}
			data_buffer[0] = data2 | g_ReturnBusy;			
			
			UserToPMABufferCopy(data_buffer, ENDP1_TXADDR, 1);
			SetEPTxCount(ENDP1, 1);
			SetEPTxValid(ENDP1);		
			buffer_out[i] = 0;			
		}		
		receive_bufer[(*p_SaveDrREcount)++] = buffer_out[i];
		(*p_SaveDrREcount) = (*p_SaveDrREcount) & MASK_DATA;

	}
	if ((*p_REAndSAcount) > PRINTER_RECEIVE_L )
	{
		g_ReceiveBusyFlag = 0xAAAA;

	}
	else
	{
		SetEPRxValid(ENDP1);
		
	}	
}
unsigned int sendcountfl;
unsigned char UsbBufer[2];
void returnstatus(unsigned char flag)
{
	return;
	#ifdef PRINTER_AND_USB
	if(flag)
	{
		if(sendcountfl & 0x200)
		{
			sendcountfl = 0;
		}
		else
		{
			return;
		}
	}
	UsbBufer[0] = 0;
	if(g_LPSStatus < g_PE_Standard)
	{
		UsbBufer[0] |= 1; //等于1表示缺纸
	}
	if(s_PrinterMOTOPar.m_PrintEnd != ENABLE_VAR)
	{
		UsbBufer[0] |= 2;//=2表示打印机正在运行
	}	
	#ifdef RD_W2C1PRINTER
	if(GET_TAKE_PAPER)
	{
		UsbBufer[0] |= 8;
	}		
	#endif
	if ((*p_REAndSAcount) > PRINTER_RECEIVE_L )
	{
		UsbBufer[0] |= 4;
	}

	UserToPMABufferCopy(UsbBufer, ENDP1_TXADDR, 1);
	SetEPTxCount(ENDP1, 1);
	SetEPTxValid(ENDP1);	
	#endif
}

void EP2_OUT_Callback(void)
{
  count_out = GetEPRxCount(ENDP2);
  PMAToUserBufferCopy(buffer_out, ENDP2_RXADDR, count_out);
  SetEPRxValid(ENDP2);
}
void EP3_OUT_Callback(void)
{
  count_out = GetEPRxCount(ENDP3);
  PMAToUserBufferCopy(buffer_out, ENDP3_RXADDR, count_out);
  SetEPRxValid(ENDP3);
}
/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
  count_in = 0;
}

void EP2_IN_Callback(void)
{
  count_in = 0;
}
void EP3_IN_Callback(void)
{
  count_in = 0;
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/*

void USB_Send_Data(u8* data_buffer)
{
  count_in=60;
  UserToPMABufferCopy(data_buffer, ENDP1_TXADDR, count_in);
  SetEPTxCount(ENDP1, count_in);
  SetEPTxValid(ENDP1);

}  
*/
#endif
