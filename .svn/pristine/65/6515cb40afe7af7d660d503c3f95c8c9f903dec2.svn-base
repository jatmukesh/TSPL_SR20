 H文件内容如下:
 
 
 
 ＃i nclude "stm32f10x.h"
 #ifndef _TWI_H_
 #define _TWI_H_
 
 
 
 //条件编译 1:使用软件模拟I2C
 #define TWI_ENABLE 1
 
 
 
 #define TWI_SCL_0 GPIOB->BRR=GPIO_Pin_8
 #define TWI_SCL_1 GPIOB->BSRR=GPIO_Pin_8
 #define TWI_SDA_0 GPIOB->BRR=GPIO_Pin_9
 #define TWI_SDA_1 GPIOB->BSRR=GPIO_Pin_9
 #define TWI_SDA_STATE (GPIOB->IDR&GPIO_Pin_9)
 
 
 
 enum ENUM_TWI_REPLY
 {
 TWI_NACK=0 
 ,TWI_ACK=1
 };
 
 
 
 enum ENUM_TWI_BUS_STATE
 {
 TWI_READY=0
 ,TWI_BUS_BUSY=1
 ,TWI_BUS_ERROR=2
 };
 
 
 
 #define TWI_RETRY_COUNT 3 //重试次数
 
 
 
 extern void Delay_mS(u32 n);
 #define DELAY Delay_mS(40)
 #define RETRY_DELAY Delay_mS(50) 
 
 
 
 void TWI_Initialize(void);
 u8 TWI_START(void);
 void TWI_STOP(void);
 u8  TWI_SendByte(u8 Data);
 u8 TWI_ReceiveByte(void);
 void TWI_SendACK(void);
 void TWI_SendNACK(void);
 #endif
 
 
 
 C文件如下:
 
 
 
 ＃i nclude "TWI.h"
 ＃i nclude "Global.h"
 #define TWI_NOP TWI_Delay()
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_Delay                                                                     
  * 描    述:延时函数                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:无                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 void TWI_Delay(void)
 {
  u32 i=5;
  while(i--); 
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_Initialize                                                                     
  * 描    述:I2C初始化函数                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:无                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 void TWI_Initialize(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
 
 
 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
   
   TWI_SDA_1;
  TWI_SCL_1;  
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   
   //////DebugPrint("Software TWI Initializing...\n"); 
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_START                                                                     
  * 描    述:发送启动                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:无                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 u8 TWI_START(void)
 { 
  TWI_SDA_1; 
  TWI_NOP;
    
  TWI_SCL_1; 
  TWI_NOP;    
 
 
 
  if(!TWI_SDA_STATE)
  {
   ////DebugPrint("TWI_START:BUSY\n");
   return TWI_BUS_BUSY;
  }
  TWI_SDA_0;
  TWI_NOP;
   
  TWI_SCL_0;  
  TWI_NOP; 
 
 
 
  if(TWI_SDA_STATE)
  {
   ////DebugPrint("TWI_START:BUS ERROR\n");
   return TWI_BUS_ERROR;
  } 
  
  return TWI_READY;
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_STOP                                                                     
  * 描    述:发送停止位                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:无                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 void TWI_STOP(void)
 {
  TWI_SDA_0; 
  TWI_NOP;
    
  TWI_SCL_1; 
  TWI_NOP;    
 
 
 
  TWI_SDA_1;
  TWI_NOP;
   
 // TWI_SCL_0;  
 // TWI_NOP;  
 
 
 
  //////DebugPrint("TWI_STOP\n");   
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_SendACK                                                                     
  * 描    述:收到数据,发送ACK                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:无                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 void TWI_SendACK(void)
 {
  TWI_SDA_0;
  TWI_NOP;
  TWI_SCL_1;
  TWI_NOP;
  TWI_SCL_0; 
  TWI_NOP; 
  //////DebugPrint("TWI_SendACK\n");   
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_SendNACK                                                                     
  * 描    述:收到数据,发送NACK                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:无                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 void TWI_SendNACK(void)
 {
  TWI_SDA_1;
  TWI_NOP;
  TWI_SCL_1;
  TWI_NOP;
  TWI_SCL_0; 
  TWI_NOP;
  //////DebugPrint("TWI_SendNACK\n");    
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_SendByte                                                                     
  * 描    述:发送一个字节                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:TWI_ACK/TWI_NACK                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 u8 TWI_SendByte(u8 Data)
 {
  u8 i;
  TWI_SCL_0;
  for(i=0;i<8;i++)
  {  
   //---------数据建立----------
   if(Data&0x80)
   {
    TWI_SDA_1;
   }
   else
   {
    TWI_SDA_0;
   } 
   Data<<=1;
   TWI_NOP;
   //---数据建立保持一定延时----
   
   //----产生一个上升沿[正脉冲] 
   TWI_SCL_1;
   TWI_NOP;
   TWI_SCL_0;
   TWI_NOP;//延时,防止SCL还没变成低时改变SDA,从而产生START/STOP信号
   //---------------------------   
  }
  //接收从机的应答 
  TWI_SDA_1; 
  TWI_NOP;
  TWI_SCL_1;
  TWI_NOP;   
  if(TWI_SDA_STATE)
  {
   TWI_SCL_0;
   //////DebugPrint("TWI_NACK!\n");
   return TWI_NACK;
  }
  else
  {
   TWI_SCL_0;
   //////DebugPrint("TWI_ACK!\n");
   return TWI_ACK;  
  }    
 }
 
 
 
 /*******************************************************************************
  * 函数名称:TWI_ReceiveByte                                                                     
  * 描    述:接收一个字节                                                                     
  *                                                                               
  * 输    入:无                                                                     
  * 输    出:无                                                                     
  * 返    回:返回接收到的数据                                                                     
  * 作    者:梅川酷子                                                                     
  * 修改日期:2010年6月8日                                                                    
  *******************************************************************************/
 u8 TWI_ReceiveByte(void)
 {
  u8 i,Dat;
  TWI_SDA_1;
  TWI_SCL_0; 
  Dat=0;
  for(i=0;i<8;i++)
  {
   TWI_SCL_1;//产生时钟上升沿[正脉冲],让从机准备好数据 
   TWI_NOP; 
   Dat<<=1;
   if(TWI_SDA_STATE) //读引脚状态
   {
    Dat|=0x01; 
   }   
   TWI_SCL_0;//准备好再次接收数据  
   TWI_NOP;//等待数据准备好         
  }
  //////DebugPrint("TWI_Dat:%x\n",Dat);
  return Dat;
 }
 




代码就不打包了，直接贴上来了
=================================
I2C的引脚配置：

/* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
=================================
/*******************************************************************************
* File Name          : i2c_fram.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Header for i2c_ee.c module
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_FRAM_H
#define __I2C_FRAM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


bool I2C_FRAM_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
bool I2C_FRAM_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);

#endif /* __I2C_FRAM_H */

=================================
/*******************************************************************************
* File Name          : i2c_fram.c
* Author             : Appcat
* Version            : V0.0.1
* Date               : 07/11/2009
* Description        : This file provides a set of functions needed to manage the
*                      communication between I2C peripheral and I2C FM24CL16 FRAM.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "i2c_fram.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           256

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu8 FRAM_ADDRESS;

/* Private function prototypes -----------------------------------------------*/

/**/
void I2C_delay(void)
{        
   u8 i=150; //这里可以优化速度        ，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   } 
}

bool I2C_Start(void)
{
        SDA_H;
        SCL_H;
        I2C_delay();
        if(!SDA_read)return FALSE;        //SDA线为低电平则总线忙,退出
        SDA_L;
        I2C_delay();
        if(SDA_read) return FALSE;        //SDA线为高电平则总线出错,退出
        SDA_L;
        I2C_delay();
        return TRUE;
}

void I2C_Stop(void)
{
        SCL_L;
        I2C_delay();
        SDA_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        SDA_H;
        I2C_delay();
}

void I2C_Ack(void)
{        
        SCL_L;
        I2C_delay();
        SDA_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        SCL_L;
        I2C_delay();
}

void I2C_NoAck(void)
{        
        SCL_L;
        I2C_delay();
        SDA_H;
        I2C_delay();
        SCL_H;
        I2C_delay();
        SCL_L;
        I2C_delay();
}

bool I2C_WaitAck(void)          //返回为:=1有ACK,=0无ACK
{
        SCL_L;
        I2C_delay();
        SDA_H;                        
        I2C_delay();
        SCL_H;
        I2C_delay();
        if(SDA_read)
        {
      SCL_L;
      return FALSE;
        }
        SCL_L;
        return TRUE;
}

void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
                SCL_H;
        I2C_delay();
    }
    SCL_L;
}

u8 I2C_ReceiveByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;                                
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
          SCL_H;
      I2C_delay();        
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

bool I2C_FRAM_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
        u8 Addr = 0, count = 0;

        Addr = WriteAddr / I2C_PageSize;

        count = WriteAddr % I2C_PageSize;

        Addr = Addr << 1;

        Addr = Addr & 0x0F;  

        FRAM_ADDRESS = I2C1_SLAVE_ADDRESS7 | Addr;

    if (!I2C_Start()) return FALSE;
    I2C_SendByte(FRAM_ADDRESS);//设置器件地址+段地址 
    if (!I2C_WaitAck())
        {
                I2C_Stop(); 
                return FALSE;
        }
    I2C_SendByte(count);   //设置段内地址      
        I2C_WaitAck();        
          
        while(NumByteToWrite--)
        {
          I2C_SendByte(* pBuffer);
          I2C_WaitAck();
      pBuffer++;
        }
        I2C_Stop();
          //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
          //Systick_Delay_1ms(10);
        return TRUE;
}


//读出1串数据         
bool I2C_FRAM_BufferRead(u8* pBuffer, u16 WriteAddr, u16 NumByteToRead)
{                
        u8 Addr = 0, count = 0;

    Addr = WriteAddr / I2C_PageSize;

        count = WriteAddr % I2C_PageSize;

        Addr = Addr << 1;

        Addr = Addr & 0x0F;  

        FRAM_ADDRESS = I2C1_SLAVE_ADDRESS7 | Addr;
        
        if (!I2C_Start()) return FALSE;

    I2C_SendByte(FRAM_ADDRESS);//设置器件地址+段地址 

    if (!I2C_WaitAck()) 
        {
                I2C_Stop(); 
                return FALSE;
        }

    I2C_SendByte(count);   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(FRAM_ADDRESS | 0x01);
    I2C_WaitAck();
    while(NumByteToRead)
    {
      *pBuffer = I2C_ReceiveByte();
      if(NumByteToRead == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      NumByteToRead--;
    }
    I2C_Stop();
    return TRUE;
}

================================= 
 
 
