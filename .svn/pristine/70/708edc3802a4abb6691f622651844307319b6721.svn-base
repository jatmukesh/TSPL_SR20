#include "ParseMatrix.h"
extern unsigned short	Buffer_Dataflash[36];
extern unsigned short*	p_ListDataDot;//链表中字符图形的存放位置
extern const unsigned char chInvTable16[] ;
extern unsigned short	g_mapCount;//图形数据计数器


void CharProDotPro(unsigned short t_Wide,unsigned short t_High,unsigned short t_Deg)
{
	unsigned int DotBuffer[32];//按照32个图块进行处理

	//Buffer_Dataflash[i];




}



/****************************************************************************
* 函数名：void CharProcessDeg0()
* 功  能：
*
* 说  明：
****************************************************************************/
void CharProcessDeg0(void)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned char i;
	unsigned int Byte_Src;
	Byte_Process1 = 0;
	Byte_Process2 = 0;
	Byte_Process3 = 0;
	Byte_Process4 = 0;
	Byte_Process5 = 0;
	Byte_Process6 = 0;
	Byte_Process7 = 0;
	Byte_Process8 = 0;
	for (i=0;i<3;i++)
	{
		Byte_Src = Buffer_Dataflash[i];
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>15) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>14) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>13) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>12) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>11) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>10) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>9) & 0x1);
		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>8) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>7) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>6) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>5) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>4) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>3) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>2) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>1) & 0x1);
		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src) & 0x1);
	}
	*p_ListDataDot++ = (unsigned short)Byte_Process1;
	*p_ListDataDot++ = (unsigned short)Byte_Process2;
	*p_ListDataDot++ = (unsigned short)Byte_Process3;
	*p_ListDataDot++ = (unsigned short)Byte_Process4;
	*p_ListDataDot++ = (unsigned short)Byte_Process5;
	*p_ListDataDot++ = (unsigned short)Byte_Process6;
	*p_ListDataDot++ = (unsigned short)Byte_Process7;
	*p_ListDataDot++ = (unsigned short)Byte_Process8;
}
/****************************************************************************
* 函数名：CharProcessDeg90(void)
* 功  能：
*
* 说  明：
****************************************************************************/
void CharProcessDeg90(void)
{
	*p_ListDataDot++ = Buffer_Dataflash[2] & 0xff;
	*p_ListDataDot++ = (Buffer_Dataflash[2] >> 8 ) & 0xff;
	*p_ListDataDot++ = Buffer_Dataflash[1] & 0xff;
	*p_ListDataDot++ = (Buffer_Dataflash[1] >> 8 ) & 0xff;
	*p_ListDataDot++ = Buffer_Dataflash[0] & 0xff;
	*p_ListDataDot++ = (Buffer_Dataflash[0] >> 8 ) & 0xff;
}
/****************************************************************************
* 函数名：void CharProcessDeg180(void)
* 功  能：
*
* 说  明：
****************************************************************************/
void CharProcessDeg180(void)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned char i;
	unsigned int Byte_Src;
	Byte_Process1 = 0;
	Byte_Process2 = 0;
	Byte_Process3 = 0;
	Byte_Process4 = 0;
	Byte_Process5 = 0;
	Byte_Process6 = 0;
	Byte_Process7 = 0;
	Byte_Process8 = 0;
	for (i=3;i>0;i--)
	{
		Byte_Src = Buffer_Dataflash[i-1];
		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>7) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>6) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>5) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>4) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>3) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>2) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>1) & 0x1);
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src) & 0x1);


		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>15) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>14) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>13) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>12) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>11) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>10) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>9) & 0x1);
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>8) & 0x1);
	}
	*p_ListDataDot++ = (unsigned short)Byte_Process1;
	*p_ListDataDot++ = (unsigned short)Byte_Process2;
	*p_ListDataDot++ = (unsigned short)Byte_Process3;
	*p_ListDataDot++ = (unsigned short)Byte_Process4;
	*p_ListDataDot++ = (unsigned short)Byte_Process5;
	*p_ListDataDot++ = (unsigned short)Byte_Process6;
	*p_ListDataDot++ = (unsigned short)Byte_Process7;
	*p_ListDataDot++ = (unsigned short)Byte_Process8;
}
/****************************************************************************
* 函数名：void CharProcessDeg270(void)
* 功  能：
*
* 说  明：
****************************************************************************/
void CharProcessDeg270(void)
{
	unsigned int Byte_Process3;
	Byte_Process3 = ((Buffer_Dataflash[0] >> 8) & 0xff);
	*p_ListDataDot++ = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
	Byte_Process3 = ((Buffer_Dataflash[0] ) & 0xff);
	*p_ListDataDot++ = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
	Byte_Process3 = ((Buffer_Dataflash[1] >> 8) & 0xff);
	*p_ListDataDot++ = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
	Byte_Process3 = ((Buffer_Dataflash[1] ) & 0xff);
	*p_ListDataDot++ = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
	Byte_Process3 = ((Buffer_Dataflash[2] >> 8) & 0xff);
	*p_ListDataDot++ = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
	Byte_Process3 = ((Buffer_Dataflash[2] ) & 0xff);
	*p_ListDataDot++ = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);

}
/****************************************************************************
* 函数名：void DotProcessDegZoom(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void DotProcessDeg0(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned char chwide,chhigh,i,j,k;
	unsigned int Byte_Src;
	chhigh = Char_Type >> 11;//高度按字节算
	chwide = (Char_Type &0xff)>> 2 ;//宽度除4
	i=0;
	for (k =chhigh;k >0;k-- )
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;

		for (j = chwide;j > 0;j--)
		{
			Byte_Src = Buffer_Dataflash[i++];
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>15) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>14) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>13) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>12) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>11) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>10) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>9) & 0x1);
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>8) & 0x1);

			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>7) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>6) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>5) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>4) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>3) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>2) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>1) & 0x1);
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src) & 0x1);

			Byte_Src = Buffer_Dataflash[i++];

			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>15) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>14) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>13) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>12) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>11) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>10) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>9) & 0x1);
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>8) & 0x1);

			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>7) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>6) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>5) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>4) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>3) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>2) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>1) & 0x1);
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src) & 0x1);
			//得到每行的4个点
		}
		//加入数据到链表
		if(chwide > 4)
		{
			*p_ListDataDot++ = Byte_Process1 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process1;

			*p_ListDataDot++ = Byte_Process2 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;

			*p_ListDataDot++ = Byte_Process3 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process3;

			*p_ListDataDot++ = Byte_Process4 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process4;

			*p_ListDataDot++ = Byte_Process5 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process5;

			*p_ListDataDot++ = Byte_Process6 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process6;

			*p_ListDataDot++ = Byte_Process7 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process7;

			*p_ListDataDot++ = Byte_Process8 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process8;
		}
		else
		{

			*p_ListDataDot++ = (unsigned short)Byte_Process1;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;
			*p_ListDataDot++ = (unsigned short)Byte_Process3;
			*p_ListDataDot++ = (unsigned short)Byte_Process4;
			*p_ListDataDot++ = (unsigned short)Byte_Process5;
			*p_ListDataDot++ = (unsigned short)Byte_Process6;
			*p_ListDataDot++ = (unsigned short)Byte_Process7;
			*p_ListDataDot++ = (unsigned short)Byte_Process8;
		}
	}
}
/****************************************************************************
* 函数名：void DotProcessDeg180(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void DotProcessDeg180(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned char chwide,chhigh,i,j,k;
	unsigned int Byte_Src;

	chhigh = Char_Type >> 11;//高度按字节算

	chwide = (Char_Type ) & 0xff;//宽度

	i =( chhigh * chwide ) >> 1;//得到字长
	i -= 1;
	chwide = (Char_Type >> 2) & 0x3f;//宽度除4
	for (k =chhigh;k >0;k-- )
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		for (j = chwide;j > 0;j--)
		{
			Byte_Src = Buffer_Dataflash[i--];
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>7) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>6) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>5) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>4) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>3) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>2) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>1) & 0x1);
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src) & 0x1);


			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>15) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>14) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>13) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>12) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>11) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>10) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>9) & 0x1);
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>8) & 0x1);

			Byte_Src = Buffer_Dataflash[i--];

			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>7) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>6) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>5) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>4) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>3) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>2) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>1) & 0x1);
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src) & 0x1);


			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>15) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>14) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>13) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>12) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>11) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>10) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>9) & 0x1);
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>8) & 0x1);
			//得到每行的4个点
		}
		//加入数据到链表
		if(chwide > 4)
		{
			*p_ListDataDot++ = Byte_Process1 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process1;

			*p_ListDataDot++ = Byte_Process2 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;

			*p_ListDataDot++ = Byte_Process3 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process3;

			*p_ListDataDot++ = Byte_Process4 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process4;

			*p_ListDataDot++ = Byte_Process5 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process5;

			*p_ListDataDot++ = Byte_Process6 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process6;

			*p_ListDataDot++ = Byte_Process7 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process7;

			*p_ListDataDot++ = Byte_Process8 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process8;
		}
		else
		{
			*p_ListDataDot++ = (unsigned short)Byte_Process1;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;
			*p_ListDataDot++ = (unsigned short)Byte_Process3;
			*p_ListDataDot++ = (unsigned short)Byte_Process4;
			*p_ListDataDot++ = (unsigned short)Byte_Process5;
			*p_ListDataDot++ = (unsigned short)Byte_Process6;
			*p_ListDataDot++ = (unsigned short)Byte_Process7;
			*p_ListDataDot++ = (unsigned short)Byte_Process8;
		}
	}
}

/****************************************************************************
* 函数名：void DotProcessDeg90(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void DotProcessDeg90(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Src;

	unsigned char chwide,chhigh,i,j,k;
	chhigh = Char_Type >> 11;//高度按字节算
	chwide = (Char_Type &0xff)>> 1;//宽度除2
	for (k =chwide;k >0;k-- )
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		i = k -1;
		for (j = chhigh;j > 0;j--)
		{
			Byte_Src = Buffer_Dataflash[i];
			Byte_Process2 = (Byte_Process2<<8)|((Byte_Src >> 8) & 0xff);
			Byte_Process1 = (Byte_Process1<<8)|(Byte_Src & 0xff);
			i += chwide;
		}
		//加入数据到链表
		if(chhigh > 2)
		{
			*p_ListDataDot++ = Byte_Process1 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process1;

			*p_ListDataDot++ = Byte_Process2 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;
		}
		else
		{
			*p_ListDataDot++ = (unsigned short)Byte_Process1;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;
		}
	}
}
/****************************************************************************
* 函数名：void DotProcessDeg270(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void DotProcessDeg270(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;

	unsigned int Byte_Src;

	unsigned char chwide,chhigh,i,j,k;

	chhigh = Char_Type >> 11;//高度按字节算
	chwide = (Char_Type &0xff)>> 1;//宽度除2
	i = 0;
	for (k = 0 ;k < chwide;k++ )
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		i = (chhigh - 1) * chwide +k;
		for (j = chhigh;j > 0;j--)
		{
			Byte_Src = Buffer_Dataflash[i];
			Byte_Process3 = ((Byte_Src >> 8) & 0xff);

			Byte_Process4 = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
			Byte_Process1 = (Byte_Process1<<8)|Byte_Process4;

			Byte_Process3 = Byte_Src & 0xff;
			Byte_Process4 = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
			Byte_Process2 = (Byte_Process2<<8)|Byte_Process4;
			i -= chwide;
		}
		//加入数据到链表
		if(chhigh > 2)
		{
			*p_ListDataDot++ = Byte_Process1 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process1;

			*p_ListDataDot++ = Byte_Process2 >> 16;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;
		}
		else
		{
			*p_ListDataDot++ = (unsigned short)Byte_Process1;
			*p_ListDataDot++ = (unsigned short)Byte_Process2;
		}
	}
}


/****************************************************************************
* 函数名：MAPDot24NOR(unsigned short Char_Type)
* 功  能：
*
* 说  明：
***********************************************************************/
void MAPDot24NOR(unsigned short MapLen)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned int Byte_Process9;
	unsigned int Byte_Process10;
	unsigned int Byte_Process11;
	unsigned int Byte_Process12;
	unsigned int Byte_Process13;
	unsigned int Byte_Process14;
	unsigned int Byte_Process15;
	unsigned int Byte_Process16;
	unsigned int Byte_Process17;
	unsigned int Byte_Process18;
	unsigned int Byte_Process19;
	unsigned int Byte_Process20;
	unsigned int Byte_Process21;
	unsigned int Byte_Process22;
	unsigned int Byte_Process23;
	unsigned int Byte_Process24;
	unsigned int chwide,j,k;
	unsigned int Byte_Src;
	chwide = MapLen >> 4;
	if((MapLen & 0xf) != 0)
	{
		chwide++;
	}
	for (k = MapLen;k >0;)
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		Byte_Process9 = 0;
		Byte_Process10 = 0;
		Byte_Process11 = 0;
		Byte_Process12 = 0;
		Byte_Process13 = 0;
		Byte_Process14 = 0;
		Byte_Process15 = 0;
		Byte_Process16 = 0;
		Byte_Process17 = 0;
		Byte_Process18 = 0;
		Byte_Process19 = 0;
		Byte_Process20 = 0;
		Byte_Process21 = 0;
		Byte_Process22 = 0;
		Byte_Process23 = 0;
		Byte_Process24 = 0;

		for (j = 16;j > 0;j--)
		{
			Byte_Src = 0;
			Byte_Src = PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>23) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>22) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>21) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>20) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>19) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>18) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>17) & 0x1);
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>16) & 0x1);
			Byte_Process9 = (Byte_Process9<<1)|((Byte_Src>>15) & 0x1);
			Byte_Process10 = (Byte_Process10<<1)|((Byte_Src>>14) & 0x1);
			Byte_Process11 = (Byte_Process11<<1)|((Byte_Src>>13) & 0x1);
			Byte_Process12 = (Byte_Process12<<1)|((Byte_Src>>12) & 0x1);
			Byte_Process13 = (Byte_Process13<<1)|((Byte_Src>>11) & 0x1);
			Byte_Process14 = (Byte_Process14<<1)|((Byte_Src>>10) & 0x1);
			Byte_Process15 = (Byte_Process15<<1)|((Byte_Src>>9) & 0x1);
			Byte_Process16 = (Byte_Process16<<1)|((Byte_Src>>8) & 0x1);
			Byte_Process17 = (Byte_Process17<<1)|((Byte_Src>>7) & 0x1);
			Byte_Process18 = (Byte_Process18<<1)|((Byte_Src>>6) & 0x1);
			Byte_Process19 = (Byte_Process19<<1)|((Byte_Src>>5) & 0x1);
			Byte_Process20 = (Byte_Process20<<1)|((Byte_Src>>4) & 0x1);
			Byte_Process21 = (Byte_Process21<<1)|((Byte_Src>>3) & 0x1);
			Byte_Process22 = (Byte_Process22<<1)|((Byte_Src>>2) & 0x1);
			Byte_Process23 = (Byte_Process23<<1)|((Byte_Src>>1) & 0x1);
			Byte_Process24 = (Byte_Process24<<1)|((Byte_Src) & 0x1);
			k--;
			g_mapCount -= 3;
			if (k == 0)
			{
				break;
			}
		}
		//加入数据到链表
		*p_ListDataDot = (unsigned short)Byte_Process1;
		*(p_ListDataDot+chwide*1) = (unsigned short)Byte_Process2;
		*(p_ListDataDot+chwide*2) = (unsigned short)Byte_Process3;
		*(p_ListDataDot+chwide*3) = (unsigned short)Byte_Process4;
		*(p_ListDataDot+chwide*4) = (unsigned short)Byte_Process5;
		*(p_ListDataDot+chwide*5) = (unsigned short)Byte_Process6;
		*(p_ListDataDot+chwide*6) = (unsigned short)Byte_Process7;
		*(p_ListDataDot+chwide*7) = (unsigned short)Byte_Process8;
		*(p_ListDataDot+chwide*8) = (unsigned short)Byte_Process9;
		*(p_ListDataDot+chwide*9) = (unsigned short)Byte_Process10;
		*(p_ListDataDot+chwide*10) = (unsigned short)Byte_Process11;
		*(p_ListDataDot+chwide*11) = (unsigned short)Byte_Process12;
		*(p_ListDataDot+chwide*12) = (unsigned short)Byte_Process13;
		*(p_ListDataDot+chwide*13) = (unsigned short)Byte_Process14;
		*(p_ListDataDot+chwide*14) = (unsigned short)Byte_Process15;
		*(p_ListDataDot+chwide*15) = (unsigned short)Byte_Process16;
		*(p_ListDataDot+chwide*16) = (unsigned short)Byte_Process17;
		*(p_ListDataDot+chwide*17) = (unsigned short)Byte_Process18;
		*(p_ListDataDot+chwide*18) = (unsigned short)Byte_Process19;
		*(p_ListDataDot+chwide*19) = (unsigned short)Byte_Process20;
		*(p_ListDataDot+chwide*20) = (unsigned short)Byte_Process21;
		*(p_ListDataDot+chwide*21) = (unsigned short)Byte_Process22;
		*(p_ListDataDot+chwide*22) = (unsigned short)Byte_Process23;
		*(p_ListDataDot+chwide*23) = (unsigned short)Byte_Process24;

		p_ListDataDot++;
	}
	p_ListDataDot += chwide *23;
}
/****************************************************************************
* 函数名：MAPDot24FX(unsigned short Char_Type)
* 功  能：
*
* 说  明：
***********************************************************************/
void MAPDot24FX(unsigned short MapLen)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned int Byte_Process9;
	unsigned int Byte_Process10;
	unsigned int Byte_Process11;
	unsigned int Byte_Process12;
	unsigned int Byte_Process13;
	unsigned int Byte_Process14;
	unsigned int Byte_Process15;
	unsigned int Byte_Process16;
	unsigned int Byte_Process17;
	unsigned int Byte_Process18;
	unsigned int Byte_Process19;
	unsigned int Byte_Process20;
	unsigned int Byte_Process21;
	unsigned int Byte_Process22;
	unsigned int Byte_Process23;
	unsigned int Byte_Process24;
	unsigned int chwide,j,k,m;
	unsigned int Byte_Src;
	unsigned short*	 addseave;
	chwide = MapLen >> 4;
	m = MapLen & 0xf;
	if(m != 0)
	{
		chwide++;
	}
	else
	{
		m = 16;
	}
	k = MapLen;
	addseave = p_ListDataDot;
	addseave += (24 * chwide );
	p_ListDataDot += (chwide - 1);

	if (m < 16)
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		Byte_Process9 = 0;
		Byte_Process10 = 0;
		Byte_Process11 = 0;
		Byte_Process12 = 0;
		Byte_Process13 = 0;
		Byte_Process14 = 0;
		Byte_Process15 = 0;
		Byte_Process16 = 0;
		Byte_Process17 = 0;
		Byte_Process18 = 0;
		Byte_Process19 = 0;
		Byte_Process20 = 0;
		Byte_Process21 = 0;
		Byte_Process22 = 0;
		Byte_Process23 = 0;
		Byte_Process24 = 0;

		for (j = m;j > 0;j--)
		{
			Byte_Src = 0;
			Byte_Src = PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Process1 = (Byte_Process1>>1)|((Byte_Src>>8) & 0x8000);
			Byte_Process2 = (Byte_Process2>>1)|((Byte_Src>>7) & 0x8000);
			Byte_Process3 = (Byte_Process3>>1)|((Byte_Src>>6) & 0x8000);
			Byte_Process4 = (Byte_Process4>>1)|((Byte_Src>>5) & 0x8000);
			Byte_Process5 = (Byte_Process5>>1)|((Byte_Src>>4) & 0x8000);
			Byte_Process6 = (Byte_Process6>>1)|((Byte_Src>>3) & 0x8000);
			Byte_Process7 = (Byte_Process7>>1)|((Byte_Src>>2) & 0x8000);
			Byte_Process8 = (Byte_Process8>>1)|((Byte_Src>>1) & 0x8000);
			Byte_Process9 = (Byte_Process9>>1)|((Byte_Src) & 0x8000);
			Byte_Process10 = (Byte_Process10>>1)|((Byte_Src<<1) & 0x8000);
			Byte_Process11 = (Byte_Process11>>1)|((Byte_Src<<2) & 0x8000);
			Byte_Process12 = (Byte_Process12>>1)|((Byte_Src<<3) & 0x8000);
			Byte_Process13 = (Byte_Process13>>1)|((Byte_Src<<4) & 0x8000);
			Byte_Process14 = (Byte_Process14>>1)|((Byte_Src<<5) & 0x8000);
			Byte_Process15 = (Byte_Process15>>1)|((Byte_Src<<6) & 0x8000);
			Byte_Process16 = (Byte_Process16>>1)|((Byte_Src<<7) & 0x8000);
			Byte_Process17 = (Byte_Process17>>1)|((Byte_Src<<8) & 0x8000);
			Byte_Process18 = (Byte_Process18>>1)|((Byte_Src<<9) & 0x8000);
			Byte_Process19 = (Byte_Process19>>1)|((Byte_Src<<10) & 0x8000);
			Byte_Process20 = (Byte_Process20>>1)|((Byte_Src<<11) & 0x8000);
			Byte_Process21 = (Byte_Process21>>1)|((Byte_Src<<12) & 0x8000);
			Byte_Process22 = (Byte_Process22>>1)|((Byte_Src<<13) & 0x8000);
			Byte_Process23 = (Byte_Process23>>1)|((Byte_Src<<14) & 0x8000);
			Byte_Process24 = (Byte_Process24>>1)|((Byte_Src<<15) & 0x8000);

			k--;
			g_mapCount -= 3;
			if (k == 0)
			{
				break;
			}
		}
		//加入数据到链表
		*(p_ListDataDot+chwide*23)= (unsigned short)(Byte_Process1 >> (16-m));
		*(p_ListDataDot+chwide*22) = (unsigned short)(Byte_Process2  >> (16-m));
		*(p_ListDataDot+chwide*21) = (unsigned short)(Byte_Process3 >> (16-m));
		*(p_ListDataDot+chwide*20) = (unsigned short)(Byte_Process4 >> (16-m));
		*(p_ListDataDot+chwide*19) = (unsigned short)(Byte_Process5 >> (16-m));
		*(p_ListDataDot+chwide*18) = (unsigned short)(Byte_Process6 >> (16-m));
		*(p_ListDataDot+chwide*17) = (unsigned short)(Byte_Process7 >> (16-m));
		*(p_ListDataDot+chwide*16) = (unsigned short)(Byte_Process8 >> (16-m));
		*(p_ListDataDot+chwide*15) = (unsigned short)(Byte_Process9 >> (16-m));
		*(p_ListDataDot+chwide*14) = (unsigned short)(Byte_Process10 >> (16-m));
		*(p_ListDataDot+chwide*13) = (unsigned short)(Byte_Process11 >> (16-m));
		*(p_ListDataDot+chwide*12) = (unsigned short)(Byte_Process12 >> (16-m));
		*(p_ListDataDot+chwide*11) = (unsigned short)(Byte_Process13 >> (16-m));
		*(p_ListDataDot+chwide*10) = (unsigned short)(Byte_Process14 >> (16-m));
		*(p_ListDataDot+chwide*9) = (unsigned short)(Byte_Process15 >> (16-m));
		*(p_ListDataDot+chwide*8) = (unsigned short)(Byte_Process16 >> (16-m));
		*(p_ListDataDot+chwide*7) = (unsigned short)(Byte_Process17 >> (16-m));
		*(p_ListDataDot+chwide*6) = (unsigned short)(Byte_Process18 >> (16-m));
		*(p_ListDataDot+chwide*5) = (unsigned short)(Byte_Process19 >> (16-m));
		*(p_ListDataDot+chwide*4) = (unsigned short)(Byte_Process20 >> (16-m));
		*(p_ListDataDot+chwide*3) = (unsigned short)(Byte_Process21 >> (16-m));
		*(p_ListDataDot+chwide*2) = (unsigned short)(Byte_Process22 >> (16-m));
		*(p_ListDataDot+chwide)  = (unsigned short)(Byte_Process23 >> (16-m));
		*(p_ListDataDot)		 = (unsigned short)(Byte_Process24 >> (16-m));
		p_ListDataDot--;
	}
	for (;k >0;)
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		Byte_Process9 = 0;
		Byte_Process10 = 0;
		Byte_Process11 = 0;
		Byte_Process12 = 0;
		Byte_Process13 = 0;
		Byte_Process14 = 0;
		Byte_Process15 = 0;
		Byte_Process16 = 0;
		Byte_Process17 = 0;
		Byte_Process18 = 0;
		Byte_Process19 = 0;
		Byte_Process20 = 0;
		Byte_Process21 = 0;
		Byte_Process22 = 0;
		Byte_Process23 = 0;
		Byte_Process24 = 0;

		for (j = 16;j > 0;j--)
		{
			Byte_Src = 0;
			Byte_Src = PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Process1 = (Byte_Process1>>1)|((Byte_Src>>8) & 0x8000);
			Byte_Process2 = (Byte_Process2>>1)|((Byte_Src>>7) & 0x8000);
			Byte_Process3 = (Byte_Process3>>1)|((Byte_Src>>6) & 0x8000);
			Byte_Process4 = (Byte_Process4>>1)|((Byte_Src>>5) & 0x8000);
			Byte_Process5 = (Byte_Process5>>1)|((Byte_Src>>4) & 0x8000);
			Byte_Process6 = (Byte_Process6>>1)|((Byte_Src>>3) & 0x8000);
			Byte_Process7 = (Byte_Process7>>1)|((Byte_Src>>2) & 0x8000);
			Byte_Process8 = (Byte_Process8>>1)|((Byte_Src>>1) & 0x8000);
			Byte_Process9 = (Byte_Process9>>1)|((Byte_Src) & 0x8000);
			Byte_Process10 = (Byte_Process10>>1)|((Byte_Src<<1) & 0x8000);
			Byte_Process11 = (Byte_Process11>>1)|((Byte_Src<<2) & 0x8000);
			Byte_Process12 = (Byte_Process12>>1)|((Byte_Src<<3) & 0x8000);
			Byte_Process13 = (Byte_Process13>>1)|((Byte_Src<<4) & 0x8000);
			Byte_Process14 = (Byte_Process14>>1)|((Byte_Src<<5) & 0x8000);
			Byte_Process15 = (Byte_Process15>>1)|((Byte_Src<<6) & 0x8000);
			Byte_Process16 = (Byte_Process16>>1)|((Byte_Src<<7) & 0x8000);
			Byte_Process17 = (Byte_Process17>>1)|((Byte_Src<<8) & 0x8000);
			Byte_Process18 = (Byte_Process18>>1)|((Byte_Src<<9) & 0x8000);
			Byte_Process19 = (Byte_Process19>>1)|((Byte_Src<<10) & 0x8000);
			Byte_Process20 = (Byte_Process20>>1)|((Byte_Src<<11) & 0x8000);
			Byte_Process21 = (Byte_Process21>>1)|((Byte_Src<<12) & 0x8000);
			Byte_Process22 = (Byte_Process22>>1)|((Byte_Src<<13) & 0x8000);
			Byte_Process23 = (Byte_Process23>>1)|((Byte_Src<<14) & 0x8000);
			Byte_Process24 = (Byte_Process24>>1)|((Byte_Src<<15) & 0x8000);

			k--;
			g_mapCount -= 3;
			if (k == 0)
			{
				break;
			}
		}
		//加入数据到链表
		*(p_ListDataDot+chwide*23)= (unsigned short)Byte_Process1;
		*(p_ListDataDot+chwide*22) = (unsigned short)Byte_Process2 ;
		*(p_ListDataDot+chwide*21) = (unsigned short)Byte_Process3;
		*(p_ListDataDot+chwide*20) = (unsigned short)Byte_Process4;
		*(p_ListDataDot+chwide*19) = (unsigned short)Byte_Process5;
		*(p_ListDataDot+chwide*18) = (unsigned short)Byte_Process6;
		*(p_ListDataDot+chwide*17) = (unsigned short)Byte_Process7;
		*(p_ListDataDot+chwide*16) = (unsigned short)Byte_Process8;
		*(p_ListDataDot+chwide*15) = (unsigned short)Byte_Process9;
		*(p_ListDataDot+chwide*14) = (unsigned short)Byte_Process10;
		*(p_ListDataDot+chwide*13) = (unsigned short)Byte_Process11;
		*(p_ListDataDot+chwide*12) = (unsigned short)Byte_Process12;
		*(p_ListDataDot+chwide*11) = (unsigned short)Byte_Process13;
		*(p_ListDataDot+chwide*10) = (unsigned short)Byte_Process14;
		*(p_ListDataDot+chwide*9) = (unsigned short)Byte_Process15;
		*(p_ListDataDot+chwide*8) = (unsigned short)Byte_Process16;
		*(p_ListDataDot+chwide*7) = (unsigned short)Byte_Process17;
		*(p_ListDataDot+chwide*6) = (unsigned short)Byte_Process18;
		*(p_ListDataDot+chwide*5) = (unsigned short)Byte_Process19;
		*(p_ListDataDot+chwide*4) = (unsigned short)Byte_Process20;
		*(p_ListDataDot+chwide*3) = (unsigned short)Byte_Process21;
		*(p_ListDataDot+chwide*2) = (unsigned short)Byte_Process22;
		*(p_ListDataDot+chwide) = (unsigned short)Byte_Process23;
		*(p_ListDataDot) = (unsigned short)Byte_Process24;
		p_ListDataDot--;
	}
	p_ListDataDot = addseave ;
}
/****************************************************************************
* 函数名：MAPDot24NOR(unsigned short Char_Type)
* 功  能：
*
* 说  明：
***********************************************************************/
void MAPDot8NOR(unsigned short MapLen)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned int chwide,j,k;
	unsigned int Byte_Src;
	chwide = MapLen >> 4;
	if((MapLen & 0xf) != 0)
	{
		chwide++;
	}
	for (k = MapLen;k >0;)
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		for (j = 16;j > 0;j--)
		{
			Byte_Src = PrinterCore();
			Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>7) & 0x1);
			Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>6) & 0x1);
			Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>5) & 0x1);
			Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>4) & 0x1);
			Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>3) & 0x1);
			Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>2) & 0x1);
			Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>1) & 0x1);
			Byte_Process8 = (Byte_Process8<<1)|((Byte_Src) & 0x1);
			k--;
			g_mapCount --;
			if (k == 0)
			{
				break;
			}
		}
		//加入数据到链表
		*p_ListDataDot = (unsigned short)Byte_Process1;
		*(p_ListDataDot+chwide*1) = (unsigned short)Byte_Process2;
		*(p_ListDataDot+chwide*2) = (unsigned short)Byte_Process3;
		*(p_ListDataDot+chwide*3) = (unsigned short)Byte_Process4;
		*(p_ListDataDot+chwide*4) = (unsigned short)Byte_Process5;
		*(p_ListDataDot+chwide*5) = (unsigned short)Byte_Process6;
		*(p_ListDataDot+chwide*6) = (unsigned short)Byte_Process7;
		*(p_ListDataDot+chwide*7) = (unsigned short)Byte_Process8;
		p_ListDataDot++;
	}
	p_ListDataDot += chwide *7;
}


/****************************************************************************
* 函数名：MAPDot8FX(unsigned short Char_Type)
* 功  能：
*
* 说  明：
***********************************************************************/
void MAPDot8FX(unsigned short MapLen)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned int chwide,j,k,m;
	unsigned int Byte_Src;
	unsigned short*	 addseave;
	chwide = MapLen >> 4;
	m = MapLen & 0xf;
	if(m != 0)
	{
		chwide++;
	}
	else
	{
		m = 16;
	}
	k = MapLen;
	addseave = p_ListDataDot;
	addseave += (8 * chwide );
	p_ListDataDot += (chwide - 1);

	if (m < 16)
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		for (j = m;j > 0;j--)
		{

			Byte_Src = PrinterCore();
			Byte_Process1 = (Byte_Process1>>1)|((Byte_Src<<8) & 0x8000);
			Byte_Process2 = (Byte_Process2>>1)|((Byte_Src<<9) & 0x8000);
			Byte_Process3 = (Byte_Process3>>1)|((Byte_Src<<10) & 0x8000);
			Byte_Process4 = (Byte_Process4>>1)|((Byte_Src<<11) & 0x8000);
			Byte_Process5 = (Byte_Process5>>1)|((Byte_Src<<12) & 0x8000);
			Byte_Process6 = (Byte_Process6>>1)|((Byte_Src<<13) & 0x8000);
			Byte_Process7 = (Byte_Process7>>1)|((Byte_Src<<14) & 0x8000);
			Byte_Process8 = (Byte_Process8>>1)|((Byte_Src<<15) & 0x8000);
			k--;
			g_mapCount --;
			if (k == 0)
			{
				break;
			}
		}
		//加入数据到链表
		*(p_ListDataDot+chwide*7)= (unsigned short)(Byte_Process1 >> (16-m));
		*(p_ListDataDot+chwide*6) = (unsigned short)(Byte_Process2  >> (16-m));
		*(p_ListDataDot+chwide*5) = (unsigned short)(Byte_Process3 >> (16-m));
		*(p_ListDataDot+chwide*4) = (unsigned short)(Byte_Process4 >> (16-m));
		*(p_ListDataDot+chwide*3) = (unsigned short)(Byte_Process5 >> (16-m));
		*(p_ListDataDot+chwide*2) = (unsigned short)(Byte_Process6 >> (16-m));
		*(p_ListDataDot+chwide) = (unsigned short)(Byte_Process7 >> (16-m));
		*(p_ListDataDot) = (unsigned short)(Byte_Process8 >> (16-m));

		p_ListDataDot--;
	}
	for (;k >0;)
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		Byte_Process3 = 0;
		Byte_Process4 = 0;
		Byte_Process5 = 0;
		Byte_Process6 = 0;
		Byte_Process7 = 0;
		Byte_Process8 = 0;
		for (j = 16;j > 0;j--)
		{

			Byte_Src = PrinterCore();
			Byte_Process1 = (Byte_Process1>>1)|((Byte_Src<<8) & 0x8000);
			Byte_Process2 = (Byte_Process2>>1)|((Byte_Src<<9) & 0x8000);
			Byte_Process3 = (Byte_Process3>>1)|((Byte_Src<<10) & 0x8000);
			Byte_Process4 = (Byte_Process4>>1)|((Byte_Src<<11) & 0x8000);
			Byte_Process5 = (Byte_Process5>>1)|((Byte_Src<<12) & 0x8000);
			Byte_Process6 = (Byte_Process6>>1)|((Byte_Src<<13) & 0x8000);
			Byte_Process7 = (Byte_Process7>>1)|((Byte_Src<<14) & 0x8000);
			Byte_Process8 = (Byte_Process8>>1)|((Byte_Src<<15) & 0x8000);
			k--;
			g_mapCount --;
			if (k == 0)
			{
				break;
			}
		}
		//加入数据到链表
		*(p_ListDataDot+chwide*7)= (unsigned short)Byte_Process1;
		*(p_ListDataDot+chwide*6) = (unsigned short)Byte_Process2;
		*(p_ListDataDot+chwide*5) = (unsigned short)Byte_Process3;
		*(p_ListDataDot+chwide*4) = (unsigned short)Byte_Process4;
		*(p_ListDataDot+chwide*3) = (unsigned short)Byte_Process5;
		*(p_ListDataDot+chwide*2) = (unsigned short)Byte_Process6;
		*(p_ListDataDot+chwide) = (unsigned short)Byte_Process7;
		*(p_ListDataDot) = (unsigned short)Byte_Process8;

		p_ListDataDot--;
	}
	p_ListDataDot = addseave ;
}

/****************************************************************************
* 函数名：void Dot12ProcessDeg0(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void Dot12ProcessDeg0(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned char chwide,i,j;
	unsigned int Byte_Src;

	chwide = (Char_Type &0xff) >> 1;//宽度除2 数据是按照字存储的
	i=0;

	Byte_Process1 = 0;
	Byte_Process2 = 0;
	Byte_Process3 = 0;
	Byte_Process4 = 0;
	Byte_Process5 = 0;
	Byte_Process6 = 0;
	Byte_Process7 = 0;
	Byte_Process8 = 0;

	for (j = chwide;j > 0;j--)
	{
		Byte_Src = Buffer_Dataflash[i++];
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>15) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>14) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>13) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>12) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>11) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>10) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>9) & 0x1);
		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>8) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>7) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>6) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>5) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>4) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>3) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>2) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>1) & 0x1);
		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src) & 0x1);
	}
	//加入数据到链表
	*p_ListDataDot++ = (unsigned short)Byte_Process1;
	*p_ListDataDot++ = (unsigned short)Byte_Process2;
	*p_ListDataDot++ = (unsigned short)Byte_Process3;
	*p_ListDataDot++ = (unsigned short)Byte_Process4;
	*p_ListDataDot++ = (unsigned short)Byte_Process5;
	*p_ListDataDot++ = (unsigned short)Byte_Process6;
	*p_ListDataDot++ = (unsigned short)Byte_Process7;
	*p_ListDataDot++ = (unsigned short)Byte_Process8;
	//下面处理半个字节的高度
	Byte_Process1 = 0;
	Byte_Process2 = 0;
	Byte_Process3 = 0;
	Byte_Process4 = 0;
	chwide = chwide >> 1;//半字节再除2
	for (j = chwide;j > 0;j--)
	{
		Byte_Src = Buffer_Dataflash[i++];
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>15) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>14) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>13) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>12) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>11) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>10) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>9) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>8) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>7) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>6) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>5) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>4) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>3) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>2) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>1) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>0) & 0x1);
	}
	*p_ListDataDot++ = (unsigned short)Byte_Process1;
	*p_ListDataDot++ = (unsigned short)Byte_Process2;
	*p_ListDataDot++ = (unsigned short)Byte_Process3;
	*p_ListDataDot++ = (unsigned short)Byte_Process4;		

}

/****************************************************************************
* 函数名：void Dot12ProcessDeg90(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void Dot12ProcessDeg90(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Src;

	unsigned char chwide,i,k;

	
	chwide = (Char_Type &0xff)>> 1;//宽度除2
	
	for (k =chwide;k >0;k-- )
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		i = k -1;

		Byte_Src = Buffer_Dataflash[i];
		Byte_Process2 = (Byte_Process2<<8)|((Byte_Src >> 8) & 0xff);
		Byte_Process1 = (Byte_Process1<<8)|(Byte_Src & 0xff);
		
		i = chwide + ( i >> 1);
		Byte_Src = Buffer_Dataflash[i];
		i = k -1;
		if(i & 1)
		{
			Byte_Process2 = (Byte_Process2<<4)|((Byte_Src >> 4) & 0xf);
			Byte_Process1 = (Byte_Process1<<4)|(Byte_Src & 0xf);		
		
		}
		else
		{
			Byte_Process2 = (Byte_Process2<<4)|((Byte_Src >> 12) & 0xf);
			Byte_Process1 = (Byte_Process1<<4)|((Byte_Src >> 8) & 0xf);		
		}
		//加入数据到链表
		*p_ListDataDot++ = (unsigned short)Byte_Process1;
		*p_ListDataDot++ = (unsigned short)Byte_Process2;
	}
}
/****************************************************************************
* 函数名：void Dot12ProcessDeg180(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void Dot12ProcessDeg180(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;
	unsigned int Byte_Process5;
	unsigned int Byte_Process6;
	unsigned int Byte_Process7;
	unsigned int Byte_Process8;
	unsigned char chwide,i,j,k;
	unsigned int Byte_Src;

	chwide = (Char_Type ) & 0xff;//宽度
	
	j = chwide >> 1;
	k = chwide >> 2;
	j += k;
	i = j-1;
	Byte_Process1 = 0;
	Byte_Process2 = 0;
	Byte_Process3 = 0;
	Byte_Process4 = 0;
	for(;k>0;k--)
	{
		Byte_Src = Buffer_Dataflash[i--];
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>1) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>2) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>3) & 0x1);


		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>4) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>5) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>6) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>7) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>8) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>9) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>10) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>11) & 0x1);

		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>12) & 0x1);	
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>13) & 0x1);	
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>14) & 0x1);	
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>15) & 0x1);	
	}
	*p_ListDataDot++ = (unsigned short)Byte_Process1;
	*p_ListDataDot++ = (unsigned short)Byte_Process2;
	*p_ListDataDot++ = (unsigned short)Byte_Process3;
	*p_ListDataDot++ = (unsigned short)Byte_Process4;
	
	Byte_Process1 = 0;
	Byte_Process2 = 0;
	Byte_Process3 = 0;
	Byte_Process4 = 0;
	Byte_Process5 = 0;
	Byte_Process6 = 0;
	Byte_Process7 = 0;
	Byte_Process8 = 0;
	
	j = chwide >> 1;
	for (;j > 0;j--)
	{
		Byte_Src = Buffer_Dataflash[i--];
		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>7) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>6) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>5) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>4) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>3) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>2) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>1) & 0x1);
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src) & 0x1);

		Byte_Process8 = (Byte_Process8<<1)|((Byte_Src>>15) & 0x1);
		Byte_Process7 = (Byte_Process7<<1)|((Byte_Src>>14) & 0x1);
		Byte_Process6 = (Byte_Process6<<1)|((Byte_Src>>13) & 0x1);
		Byte_Process5 = (Byte_Process5<<1)|((Byte_Src>>12) & 0x1);
		Byte_Process4 = (Byte_Process4<<1)|((Byte_Src>>11) & 0x1);
		Byte_Process3 = (Byte_Process3<<1)|((Byte_Src>>10) & 0x1);
		Byte_Process2 = (Byte_Process2<<1)|((Byte_Src>>9) & 0x1);
		Byte_Process1 = (Byte_Process1<<1)|((Byte_Src>>8) & 0x1);

	}
	//加入数据到链表
	*p_ListDataDot++ = (unsigned short)Byte_Process1;
	*p_ListDataDot++ = (unsigned short)Byte_Process2;
	*p_ListDataDot++ = (unsigned short)Byte_Process3;
	*p_ListDataDot++ = (unsigned short)Byte_Process4;
	*p_ListDataDot++ = (unsigned short)Byte_Process5;
	*p_ListDataDot++ = (unsigned short)Byte_Process6;
	*p_ListDataDot++ = (unsigned short)Byte_Process7;
	*p_ListDataDot++ = (unsigned short)Byte_Process8;	

}
/****************************************************************************
* 函数名：void Dot12ProcessDeg270(unsigned short Char_Type)
* 功  能：
*
* 说  明：
****************************************************************************/
void Dot12ProcessDeg270(unsigned short Char_Type)
{
	unsigned int Byte_Process1;
	unsigned int Byte_Process2;
	unsigned int Byte_Process3;
	unsigned int Byte_Process4;

	unsigned int Byte_Src;

	unsigned char chwide,i,j,k;

	chwide = (Char_Type &0xff)>> 1;//宽度除2
	i = 0;
	for (k = 0 ;k < chwide;k++ )
	{
		Byte_Process1 = 0;
		Byte_Process2 = 0;
		
		j = k >> 1;

		j += chwide;//得到低4位的地址

		Byte_Src = Buffer_Dataflash[j];
		
		if(k & 1)
		{

			Byte_Process3 = ((Byte_Src >> 4) & 0xf);
			Byte_Process4 = chInvTable16[Byte_Process3];
			Byte_Process1 = Byte_Process4 ;

			Byte_Process3 = ((Byte_Src ) & 0xf);
			Byte_Process4 = chInvTable16[Byte_Process3];
			Byte_Process2 = Byte_Process4;
		}
		else
		{
			Byte_Process3 = ((Byte_Src >> 12) & 0xf);
			Byte_Process4 = chInvTable16[Byte_Process3];
			Byte_Process1 = Byte_Process4;
			
			
			Byte_Process3 = ((Byte_Src >> 8) & 0xf);
			Byte_Process4 = chInvTable16[Byte_Process3];
			Byte_Process2 = Byte_Process4;
			
		}

		Byte_Src = Buffer_Dataflash[i++];

		Byte_Process3 = ((Byte_Src >> 8) & 0xff);

		Byte_Process4 = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
		Byte_Process1 = (Byte_Process1<<8)|Byte_Process4;

		Byte_Process3 = Byte_Src & 0xff;
		Byte_Process4 = (chInvTable16[Byte_Process3 & 0xf] << 4)|(chInvTable16[(Byte_Process3 >> 4) & 0xf]);
		Byte_Process2 = (Byte_Process2<<8)|Byte_Process4;
		//加入数据到链表
		*p_ListDataDot++ = (unsigned short)Byte_Process1;
		*p_ListDataDot++ = (unsigned short)Byte_Process2;
	}
}
