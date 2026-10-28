
 #ifdef PRINTER_AND_USB 
/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#define EP0_PACKET_SIZE    8
#define USB_IDVENDOR_LOW        0xC5                                    /*  USB设备的厂商ID号           */
#define USB_IDVENDOR_HIGH       0x08

#define USB_IDPRODUCT_LOW       0x06                                    /*  USB设备的产品ID号           */
#define USB_IDPRODUCT_HIGH      0x03

/* USB Standard Device Descriptor */
#if 0
const u8 Virtual_Printer_DeviceDescriptor[] =
  {
     0x12,                                      /*  设备描述符长度,= 12H        */
    USB_DEVICE_DESCRIPTOR_TYPE,                                         /*  设备描述符类型,= 01H        */
    0x00,0x01,                                                          /*  协议版本,= 1.10             */
    0,                                   /*  测试设备类型, = 0DCH        */
    0, 0,                                                               /*  设备子类,设备协议           */
    EP0_PACKET_SIZE,                                                    /*  端点0最大数据包大小,= 40H   */
    USB_IDVENDOR_LOW,
    USB_IDVENDOR_HIGH,                                                  /*  公司的设备ID         */
    USB_IDPRODUCT_LOW,
    USB_IDPRODUCT_HIGH,                                                 /*  设备制造商定的产品ID        */
    0x00,0x02,                                                          /*  设备系列号                  */
    1, 2, 3,                                                            /*  索引                        */
    1                                                                   /*  可能的配置数                */
  };
	#endif
const u8 Virtual_Printer_DeviceDescriptor[] =
  {
     0x12,                                      /*  设备描述符长度,= 12H        */
    USB_DEVICE_DESCRIPTOR_TYPE,                                         /*  设备描述符类型,= 01H        */
    0x00,0x02,                                                          /*  协议版本,= 1.10             */
    0,                                   /*  测试设备类型, = 0DCH        */
    0, 0,                                                               /*  设备子类,设备协议           */
    EP0_PACKET_SIZE,                                                    /*  端点0最大数据包大小,= 40H   */
    0xC5,
    0x08,                                                  /*  公司的设备ID         */
    0x06,
    0x03,                                                 /*  设备制造商定的产品ID        */
    0x00,0x02,                                                          /*  设备系列号                  */
    1, 2, 3,                                                            /*  索引                        */
    1                                                                   /*  可能的配置数                */
  };
const u8 Virtual_Printer_ConfigDescriptor[] =
  {
#if 0 
		0x09,                               /*  配置描述符长度,= 09H        */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,                                  /*  配置描述符类型,= 02H        */
    VIRTUAL_PRINTER_SIZ_CONFIG_DESC,0x00,                                      /*  描述符总长度, = 004EH       */
    1,                                                                  /*  只支持1个接口               */
    1,                                                                  /*  配置值                      */
    0,                                                                  /*  字符串描述符指针(无)        */
    0x80,                                                               /*  自供电,支持远程唤醒         */
    0x32 , 

 
 //interface descriptor       9                                           /*  接口描述符                  */
//interface 0,Alternate Setting 0
    9,                                   /*  接口描述符长度,= 09H        */
    USB_INTERFACE_DESCRIPTOR_TYPE,                                      /*  接口描述符类型,= 04H        */
    0,                                                                  /*  接口数,只有1个              */
    0,                                                                  /*  可选配置,只有1个            */
    1,                                                      			/*  除端点0的端点索引数目,=04H  */
    7,                                   								/*  测试设备类型,= 0DCH         */
    1,                               									/*  子类代码,= 0A0H             */
    1,                               									/*  协议代码,= 0B0H             */
    0,                                                                   /*  字符串描述符索引            */
//Bulk Out endpoint descriptor, logic endpoint 1 OUT  7                  /*  逻辑端点 1 输出             */
    7,                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x1,                                                                /*  端点1 OUT                   */
    0x02,                                       							 /*  中断传输,= 03H              */
    0x40,0x00,                                              			 /*  端点最大包的大小,= 0x40    */
    0,                                                                   /*  传输间隔时间,= 10ms         */
//interface 0,Alternate Setting 1 9
    9,                                   /*  接口描述符长度,= 09H        */
    USB_INTERFACE_DESCRIPTOR_TYPE,                                      /*  接口描述符类型,= 04H        */
    0,                                                                  /*  接口数,只有1个              */
    1,                                                                  /*  可选配置,只有1个            */
    2,                                                      			/*  除端点0的端点索引数目,=04H  */
    7,                                   								/*  测试设备类型,= 0DCH         */
    1,                               									/*  子类代码,= 0A0H             */
    2,                               									/*  协议代码,= 0B0H             */
    0 ,                                                                  /*  字符串描述符索引            */
//Bulk Out endpoint descriptor, logic endpoint 1 OUT     7               /*  逻辑端点 1 输出             */
    7,                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x1,                                                                /*  端点1 OUT                   */
    0x02,                                        /*  中断传输,= 03H              */
    0x40,0x00,                                               /*  端点最大包的大小,= 0010H    */
    0x0,                                                                   /*  传输间隔时间,= 10ms         */

//endpoint descriptor, logic endpoint 2 IN        7                      /*  逻辑端点 2 输入             */

    7,                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x82,                                                               /*  端点2 IN                    */
    0x02,                                             					/*  批量传输,= 02H              */
    0x40,0x00,                                              			 /*  端点最大包的大小,= 0040H    */
    0x0,                                                                  /*  批量传输时该值无效          */

//interface 0,Alternate Setting 2 9
   9,                                   /*  接口描述符长度,= 09H        */
    USB_INTERFACE_DESCRIPTOR_TYPE,                                      /*  接口描述符类型,= 04H        */
    0,                                                                  /*  接口数,只有1个              */
    2,                                                                  /*  可选配置,只有1个            */
    3,                                                      			/*  除端点0的端点索引数目,=04H  */
    0xFF,                                   								/*  测试设备类型,= 0DCH         */
    0,                               									/*  子类代码,= 0A0H             */
    0xFF,                               									/*  协议代码,= 0B0H             */
    0,                                                                  /*  字符串描述符索引            */

//Bulk Out endpoint descriptor, logic endpoint 1 OUT       7             /*  逻辑端点 1 输出             */
    7,                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x1,                                                                /*  端点1 OUT                   */
    0x02,                                        /*  中断传输,= 03H              */
    0x40,0x00,                                               /*  端点最大包的大小,= 0010H    */
    0,                                                                   /*  传输间隔时间,= 10ms         */
//endpoint descriptor, logic endpoint 2 IN           7                   /*  逻辑端点 2 输入             */
    7,                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x82,                                                               /*  端点2 IN                    */
    0x02,                                             					/*  批量传输,= 02H              */
    0x40,0x00,                                              			 /*  端点最大包的大小,= 0040H    */
    0,                                                                  /*  批量传输时该值无效          */
//endpoint descriptor, logic endpoint 3 IN            7                  /*  逻辑端点 2 输入             */
    7,                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x83,                                                               /*  端点2 IN                    */
    3,                                             					/*  批量传输,= 02H              */
    0x4,0x00,                                              			 /*  端点最大包的大小,= 0040H    */
    1,                                                                  /*  批量传输时该值无效          */
		
#endif		
0x09,0x02,0x20,0x00,0x01,0x01,0x00,0xc0,
0x00,0x09,0x04,0x00,0x00,0x02,0x07,0x01,
0x02,0x00,0x07,0x05,0x01,0x02,0x40,0x00,
0x00,0x07,0x05,0x81,0x02,0x40,0x00,0x00,		
		
  };

/* USB String Descriptors */
//const u8 Virtual_Printer_StringLangID[VIRTUAL_PRINTER_SIZ_STRING_LANGID] =
const u8 Virtual_Printer_StringLangID[VIRTUAL_PRINTER_SIZ_STRING_LANGID] =	
  {
    VIRTUAL_PRINTER_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const u8 Virtual_Printer_StringVendor[VIRTUAL_PRINTER_SIZ_STRING_VENDOR] =
  {
    VIRTUAL_PRINTER_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
'r', 0x00, 
'd', 0x00, 
'c', 0x00, 
'x', 0x00,
'R', 0x00, 
'O', 0x00, 
'S', 0x00, 
' ', 0x00, 
' ', 0x00, 
'T', 0x00, 
'e', 0x00,
'c', 0x00, 
'h', 0x00, 
'n', 0x00, 
'o', 0x00, 
'l', 0x00,
'o', 0x00,
'g', 0x00, 
'y', 0x00, 
' ', 0x00, 
'I', 0x00,
'n', 0x00,
'c', 0x00,
'.',0x00 ,
  };

const u8 Virtual_Printer_StringProduct[VIRTUAL_PRINTER_SIZ_STRING_PRODUCT] =
  {
    VIRTUAL_PRINTER_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "STM32 Virtual COM Port" */
'R', 0x00,
'D', 0x00,
'P', 0x00,
'r', 0x00,
'i', 0x00,
'n', 0x00,
't', 0x00,
'e', 0x00,
'r', 0x00,

  };
#if 0
u8 Virtual_Printer_StringSerial[VIRTUAL_PRINTER_SIZ_STRING_SERIAL] =
  {
    VIRTUAL_PRINTER_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
	'R', 0x00,
	'D', 0x00,
	'V', 0x00,
	'2', 0x00,
	'-', 0x00,
	'U', 0x00,
	'B', 0x00,
	'1', 0x00,
	'2', 0x00,
	'3', 0x00,
	'4', 0x00,
	'5', 0x00,
  }
	
#endif	
	
	

u8 Virtual_Printer_StringSerial[VIRTUAL_PRINTER_SIZ_STRING_SERIAL] =
  {
		0x32,0x03,0x32,0x00,0x35,0x00,0x36,0x00,0x38,0x00,0x39,0x00,0x35,0x00,0x30,0x00,
		0x30,0x00,0x30,0x00,0x30,0x00,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00
  }	
  ;/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


u8 Printer_GetDeviceID_Str[44] =
  {
		0x00,0x22,0x4d,0x46,0x47,0x3a,'R','D','S','T',0x20,0x3b,0x43,0x4d,0x44,0x3a,//."MFG:SPRT ;CMD:         5.2.0        
		0x45,0x50,0x53,0x4f,0x4e,0x3b,0x43,0x4c,0x53,0x3a,0x50,0x52,0x49,0x4e,0x54,0x45, // EPSON;CLS:PRINTE         5.2.16   
		0x52,0x3b,0x4d,0x44,0x4c,0x3a,'R','D','M','P','x','x',              // R;MDL:SP-T9;             5.2.32     		
  }
  ;/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

		
	
	
	#endif
