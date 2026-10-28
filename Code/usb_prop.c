
  #ifdef PRINTER_AND_USB 
/* Includes ------------------------------------------------------------------*/
//USB应用程序专门定义的属性
#include "usb_lib.h"
#include "usb_conf.h"//USB配置文件
#include "usb_prop.h"  //USB应用程序专门定义的属性
#include "usb_desc.h"//USB描述符
#include "usb_pwr.h" //USB电源管理模块
#include "hw_config.h"
extern int SendChar (int ch)  ;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Request = 0;

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
  };

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Virtual_Printer_init,
    Virtual_Printer_Reset,
    Virtual_Printer_Status_In,
    Virtual_Printer_Status_Out,
    Virtual_Printer_Data_Setup,
    Virtual_Printer_NoData_Setup,
    Virtual_Printer_Get_Interface_Setting,
    Virtual_Printer_GetDeviceDescriptor,
    Virtual_Printer_GetConfigDescriptor,
    Virtual_Printer_GetStringDescriptor,
    0,
    8 /*0x40MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Virtual_Printer_GetConfiguration,
    Virtual_Printer_SetConfiguration,
    Virtual_Printer_GetInterface,
    Virtual_Printer_SetInterface,
    Virtual_Printer_GetStatus,
    Virtual_Printer_ClearFeature,
    Virtual_Printer_SetEndPointFeature,
    Virtual_Printer_SetDeviceFeature,
    Virtual_Printer_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (u8*)Virtual_Printer_DeviceDescriptor,
    VIRTUAL_PRINTER_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (u8*)Virtual_Printer_ConfigDescriptor,
    VIRTUAL_PRINTER_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[5] =
  {
    {(u8*)Virtual_Printer_StringLangID, VIRTUAL_PRINTER_SIZ_STRING_LANGID},
    {(u8*)Virtual_Printer_StringVendor, VIRTUAL_PRINTER_SIZ_STRING_VENDOR},
    {(u8*)Virtual_Printer_StringProduct, VIRTUAL_PRINTER_SIZ_STRING_PRODUCT},
    {(u8*)Virtual_Printer_StringSerial, VIRTUAL_PRINTER_SIZ_STRING_SERIAL},
		 {(u8*)Printer_GetDeviceID_Str, 44}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Virtual_Printer_init.
* Description    : Virtual COM Port Mouse init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Printer_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  IWDG_ReloadCounter();
  Get_SerialNum();

  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();
  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);
  wInterrupt_Mask = IMR_MSK;
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : Virtual_Printer_Reset
* Description    : Virtual_Printer Mouse reset routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Printer_Reset(void)
{
	IWDG_ReloadCounter();
  /* Set Virtual_Printer DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = Virtual_Printer_ConfigDescriptor[7];

  /* Set Virtual_Printer DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;
  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_STALL);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
	SetEPType(ENDP1, EP_BULK);
	SetEPRxStatus(ENDP1, EP_RX_VALID);
	SetEPTxStatus(ENDP1, EP_TX_VALID);

	SetEPRxAddr(ENDP1, ENDP1_RXADDR);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
//	Clear_Status_Out(ENDP1);
	SetEPRxCount(ENDP1, VIRTUAL_PRINTER_DATA_SIZE);
	SetEPTxCount(ENDP1, VIRTUAL_PRINTER_DATA_SIZE);	

  /* Initialize Endpoint 2 */
	SetEPType(ENDP2, EP_BULK);
	SetEPRxStatus(ENDP2, EP_RX_DIS);
	SetEPTxStatus(ENDP2, EP_TX_VALID);

	SetEPRxAddr(ENDP2, ENDP2_RXADDR);
	SetEPTxAddr(ENDP2, ENDP2_TXADDR);
//	Clear_Status_Out(ENDP2);
	SetEPRxCount(ENDP2, VIRTUAL_PRINTER_DATA_SIZE);
	SetEPTxCount(ENDP2, VIRTUAL_PRINTER_DATA_SIZE);

  /* Initialize Endpoint 3 */
	SetEPType(ENDP3, EP_INTERRUPT);
	SetEPRxStatus(ENDP3, EP_RX_DIS);
	SetEPTxStatus(ENDP3, EP_TX_VALID);
	SetEPTxAddr(ENDP3, ENDP3_TXADDR);
//	Clear_Status_Out(ENDP3);
	SetEPRxCount(ENDP3, 4);
	SetEPTxCount(ENDP3, 4);
  /* Set this device to response on default address */
  SetDeviceAddress(0);

  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Virtual_Printer_SetConfiguration.
* Description    : Udpade the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Printer_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

   IWDG_ReloadCounter();
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;

  }
}
u8 *Virtual_Printer_GetReportDescriptor(u16 Length)
{
	 return USB_SUCCESS;
//  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Printer_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Printer_SetDeviceAddress (void)
{
 IWDG_ReloadCounter();
 bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Virtual_Printer_Status_In.
* Description    : Virtual COM Port Status In Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Printer_Status_In(void)
{
}

/*******************************************************************************
* Function Name  : Virtual_Printer_Status_Out
* Description    : Virtual COM Port Status OUT Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Printer_Status_Out(void)
{


}

/*******************************************************************************
* Function Name  : Virtual_Printer_Data_Setup
* Description    : handle the data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Printer_Data_Setup(u8 RequestNo)
{
  u8    *(*CopyRoutine)(u16);
  CopyRoutine = NULL;
	IWDG_ReloadCounter();
  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
  {

    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = Virtual_Printer_GetReportDescriptor;
    }


  } /* End of GET_DESCRIPTOR */

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Printer_NoData_Setup.
* Description    : handle the no data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Printer_NoData_Setup(u8 RequestNo)
{
/*
  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    if (RequestNo == SET_COMM_FEATURE)
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)
    {
      return USB_SUCCESS;
    }
  }
*/	IWDG_ReloadCounter();
  return USB_SUCCESS ;//USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Virtual_Printer_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Virtual_Printer_GetDeviceDescriptor(u16 Length)
{

  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Printer_GetConfigDescriptor.
* Description    : get the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *Virtual_Printer_GetConfigDescriptor(u16 Length)
{
  IWDG_ReloadCounter();
 return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Printer_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *Virtual_Printer_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  IWDG_ReloadCounter();
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }

}
/*******************************************************************************
* Function Name  : Virtual_Printer_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *Virtual_Printer_GetDeviceID(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
	wValue0 = 3;
  IWDG_ReloadCounter();

    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);


}

/*******************************************************************************
* Function Name  : Virtual_Printer_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 * Printer_GetDeviceID(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
	wValue0 = 4;
  IWDG_ReloadCounter();

    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);


}


extern unsigned short g_LPSStatus,g_PE_Standard;//缺纸检测器状态
unsigned char g_PrintStatus[2];
u8 * Printer_GePrinterStatus(u16 Length)
{
	u8 wValue0 = pInformation->USBwValue0;
	wValue0 = 4;
	IWDG_ReloadCounter();

	if(g_LPSStatus > g_PE_Standard)
	{
		g_PrintStatus[0] = 0x18;
	}	
	else
	{
		g_PrintStatus[0] = 0x38;
	}
			
	 pInformation->Ctrl_Info.Usb_wLength = 1;
	
	return g_PrintStatus;


}

/*******************************************************************************
* Function Name  : Virtual_Printer_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.
* Input1         : u8: Interface : interface number.
* Input2         : u8: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Virtual_Printer_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
 return USB_SUCCESS;
}



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

#endif
