#include "PDL.h"
extern _PDL		PRINTER_PDL ;


#ifdef TIME_PRINTER

/* Private function prototypes -----------------------------------------------
//设置时间
1C 53 08 06 25 19 23 56 4 
//返回时间
1C 55
//打印时间
1C 54 8
20
1C 54 7
20
1C 54 5
20
1C 54 4
20
1C 54 3
20
1C 54 2
0D
1C 54 08 20 1C 54 07 20 1C 54 05 20 1C 54 04 20 1C 54 03 20 1C 54 02 0D
*/

/**/
#define IIC_DEL_TIME	20
void I2C_delay(void)
{        
   unsigned short i; //这里可以优化速度        ，经测试最低到5还能写入
   i=IIC_DEL_TIME;
		PRINTER_PDL.SYS._DelayNus(i);
}
void Delay(unsigned short DelayCount)					//1us
{

	
}

void R8025_IICStart(void)
{
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	Delay(IIC_DEL_TIME);
	//SDA_H;
	PRINTER_PDL.TIMER._SDA_H();
	Delay(IIC_DEL_TIME);
	//SCL_H;
	PRINTER_PDL.TIMER._SCL_H();
	I2C_delay();
	//SDA_L;
	PRINTER_PDL.TIMER._SDA_L();
	I2C_delay();
	//SCL_L;
	PRINTER_PDL.TIMER._SCL_L();
	Delay(IIC_DEL_TIME);

}

void R8025_IICStop(void)
{
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	//SCL_L;
	PRINTER_PDL.TIMER._SCL_L();
	Delay(IIC_DEL_TIME);
	//SDA_L;
	PRINTER_PDL.TIMER._SDA_L();
	Delay(IIC_DEL_TIME);
	//SCL_H;
	PRINTER_PDL.TIMER._SCL_H();
	I2C_delay();
	//SDA_H;
	PRINTER_PDL.TIMER._SDA_H();
	I2C_delay();
}

void R8025_IICAck(void)
{       
PRINTER_PDL.SYS._WDG_ReloadCounter();	
	//SDA_L;
	PRINTER_PDL.TIMER._SCL_L();
	PRINTER_PDL.TIMER._SDA_L();	
	Delay(IIC_DEL_TIME);
	//SCL_H;
	PRINTER_PDL.TIMER._SCL_H();
	I2C_delay();
	//SCL_L;
	PRINTER_PDL.TIMER._SCL_L();
	I2C_delay();
}

void R8025_IICNoAck(void)
{        
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	//SDA_H;
	PRINTER_PDL.TIMER._SCL_L();	
	PRINTER_PDL.TIMER._SDA_H();
	Delay(IIC_DEL_TIME);
	//SCL_H;
	PRINTER_PDL.TIMER._SCL_H();
	I2C_delay();
	//SCL_L;
	PRINTER_PDL.TIMER._SCL_L();
}
unsigned char R8025_IICRecAck(void)          //返回为:=0有ACK,=1无ACK
{
	unsigned short mcoutn;
	mcoutn = 0;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	PRINTER_PDL.TIMER._SDA_InputEn();
	Delay(IIC_DEL_TIME);
	//SCL_H;
	PRINTER_PDL.TIMER._SCL_H();
	I2C_delay();
	
	while(PRINTER_PDL.TIMER.SDA_read())
	{
		mcoutn++;
		if(mcoutn > 1000)
		{
			R8025_IICStop();
			 return 1;
		}
	}

	PRINTER_PDL.TIMER._SCL_L();
	return 0;
}

void R8025_IICSendByte(unsigned char SendByte) //数据从高位到低位//
{
    unsigned char i,m_SendByte;
	m_SendByte = SendByte;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
    i=8;
    //SCL_L;
		PRINTER_PDL.TIMER._SCL_L();
    while(i--)
    {
			I2C_delay();
			if(m_SendByte&0x80)
				//SDA_H;  
				PRINTER_PDL.TIMER._SDA_H();
			else 
				//SDA_L;   
				PRINTER_PDL.TIMER._SDA_L();
			m_SendByte = (m_SendByte<< 1);
			I2C_delay();
			//SCL_H;
			PRINTER_PDL.TIMER._SCL_H();
			I2C_delay();
			//SCL_L;
			PRINTER_PDL.TIMER._SCL_L();
    }

}

unsigned char R8025_IICReceiveByte(void)  //数据从高位到低位//
{ 
    unsigned char i;
    unsigned char ReceiveByte;
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	i=8;
	ReceiveByte=0;
	I2C_delay();
  //SDA_H;                                
	PRINTER_PDL.TIMER._SDA_H();
	PRINTER_PDL.TIMER._SDA_InputEn();
	while(i--)
	{
		I2C_delay();
		//SCL_H;
		PRINTER_PDL.TIMER._SCL_H();
		Delay(IIC_DEL_TIME);
		ReceiveByte = (ReceiveByte<< 1);      
		//if(SDA_read)
		if(PRINTER_PDL.TIMER.SDA_read())
		{
			ReceiveByte|=0x01;
		}
		I2C_delay();
		//SCL_L;
		PRINTER_PDL.TIMER._SCL_L();
	}
    return ReceiveByte;
}

void Set_R8025(unsigned char timetype,unsigned char value)
{
  unsigned short  i;
	timetype = timetype<<4;
	i = 1000;
	while(i--)
	{
		R8025_IICStart();

		R8025_IICSendByte(0x64);
		if(R8025_IICRecAck())
			continue;

		R8025_IICSendByte(timetype);
		if(R8025_IICRecAck())
			continue;

		R8025_IICSendByte(value);
		if(R8025_IICRecAck())
		{
			continue;
		}
		break;
	}

	R8025_IICStop();
	Delay(3000);
	Delay(3000);
	Delay(3000);
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	Delay(3000);
	Delay(3000);
	PRINTER_PDL.SYS._WDG_ReloadCounter();
	Delay(3000);
	Delay(3000);
	Delay(3000);
	
		PRINTER_PDL.SYS._WDG_ReloadCounter();
}
void CleanAlarm(void)
{

	Set_R8025(0X08,0x00);
	Set_R8025(0x09,0x00);
	Set_R8025(0x0A,0x00);
	Set_R8025(0x0B,0x00);
	Set_R8025(0x0C,0x00);
}

void Set_timer(unsigned char data[])
{

//时钟启动	
	Set_R8025(0x0E,0x20);			//timer run
	Set_R8025(0x0F,0);			//timer run
//----秒
	Set_R8025(0,data[5]);//
//----分
	Set_R8025(1,data[4]);//
//----时
	Set_R8025(2,data[3]);//
//----星期
	Set_R8025(3,data[6]);//
//----日
	Set_R8025(4,data[2]);//
//----月
	Set_R8025(5,data[1]);//
//----年
	Set_R8025(6,data[0]);//
	
	Set_R8025(0x07,0x00);	//	

	
	
}
unsigned char ReadPCF8563(unsigned char m_Space)
{
	  unsigned char data;
	
	if(m_Space == 3)//星期
	{
		data &= 0x7;
	}
	else if(m_Space == 5)//月份
	{
		data &= 0x1F;
	}
	else if(m_Space == 4)//日
	{
		data &= 0x3F;
	}
	else if(m_Space == 2)//时
	{
		data &= 0x3F;
	}
	else
	{
		data &= 0x7F;
	}
	return data;
}
void ReturnTime(unsigned char data[])
{
	unsigned int j,i;
	i=100;
	while(i--)
	{

		R8025_IICStart();
		R8025_IICSendByte(0x64);
		if(R8025_IICRecAck())
			continue;

		R8025_IICSendByte(0x00);
		if(R8025_IICRecAck())
			continue;

		R8025_IICStart();
		R8025_IICSendByte(0x65);
		if(R8025_IICRecAck())
			continue;

		for(j=0;j<6;j++)
		{
			data[j]=R8025_IICReceiveByte();
			R8025_IICAck();
			
		}

		data[6]=R8025_IICReceiveByte();        //read last byte data
		R8025_IICNoAck();
		break;
	}
	R8025_IICStop();					  
	data[0] &=0x7F;		//秒
	data[1] &=0x7F;		//分
	data[2] &=0x3F;		//时
	data[3] &=0x7;       //星期
	data[4] &=0x3F;       //日
	data[5] &=0x1F;		//月
	data[6] &=0xFF;	   		//年	

}


#endif
