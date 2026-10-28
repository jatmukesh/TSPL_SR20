#include "ParseMatrix.h"
extern unsigned short	Buffer_Dataflash[36];
extern unsigned short*	p_ListDataDot;//链表中字符图形的存放位置
extern const unsigned char chInvTable16[] ;
extern unsigned short	g_mapCount;//图形数据计数器
unsigned int DotArry[32];

void SaveProDotPro(unsigned int Bsr_dat,unsigned short p_Wide)
{
		if(p_Wide > 16)
		{
			*p_ListDataDot++ = Bsr_dat >> 16;
			*p_ListDataDot++ = (unsigned short)Bsr_dat;
		}
		else
		{
			*p_ListDataDot++ = (unsigned short)Bsr_dat;
		}
}


void CharProDotPro(unsigned short t_Wide,unsigned short t_High,unsigned short t_Deg)
{
	unsigned short i,m;
	unsigned int Byte_Src;
	
	if(t_Deg == 0)
	{
		;//0度处理行列转换
		m = 32-t_Wide;
		Transpose32(DotArry);
		for(i=0;i<t_High;i++)
		{
			Byte_Src = DotArry[i] >> m;
			SaveProDotPro(Byte_Src,t_Wide);
			
		}
	}
	else if(t_Deg == 1)
	{
		//90度
		m = 32 - t_High;
		for(i = t_Wide;i>0;i--)
		{
			Byte_Src = DotArry[i-1] >> m;
			SaveProDotPro(Byte_Src,t_High);
		}
	}
	else if(t_Deg == 2)
	{//180度
		Transpose32(DotArry);
		for(i=t_High;i>0;i--)
		{
			Byte_Src = RevInt(DotArry[i-1]);
			SaveProDotPro(Byte_Src,t_Wide);		
		}
	
	}
	else
	{
		//270度
	//	for(i = t_Wide;i>0;i--)
		for(i=0;i<t_Wide;i++)
		{
			Byte_Src =  RevInt(DotArry[i]);
			SaveProDotPro(Byte_Src,t_High);
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

	unsigned int chwide,j,k,m,i;
	unsigned int Byte_Src;
	chwide = MapLen >> 4;
	if((MapLen & 0xf) != 0)
	{
		chwide++;
	}
	k = 0;
	for(j=0;j<MapLen;j++)
	{
		Byte_Src = PrinterCore();
		Byte_Src = (Byte_Src << 8) |PrinterCore();
		Byte_Src = (Byte_Src << 8) |PrinterCore();
		DotArry[k++] = Byte_Src;
		g_mapCount -= 3;
		if( k > 31)
		{
			Transpose32(DotArry);//转换数组
			for(i=0;i<24;i++)
			{
				*(p_ListDataDot+chwide*i) = (unsigned short)(DotArry[i+8] >> 16);
				*(p_ListDataDot+chwide*i + 1) = (unsigned short)(DotArry[i+8]);
			}
			p_ListDataDot += 2;
			k = 0;
		}
	}
	Transpose32(DotArry);//转换数组
	m = 32 - k;
	if( k > 16)
	{
		for(i=0;i<24;i++)
		{
			*(p_ListDataDot+chwide*i) = (unsigned short)(DotArry[i+8] >> 16);
			*(p_ListDataDot+chwide*i + 1) = (unsigned short)(DotArry[i+8]) >> m;
		}
		p_ListDataDot += 2;
		k = 0;
	}
	if( k > 0)
	{
		for(i=0;i<24;i++)
		{
			*(p_ListDataDot+chwide*i) = (unsigned short)(DotArry[i+8]) >> m;
		}
		p_ListDataDot++;
		k = 0;
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

	unsigned int chwide,j,k,m,i;
	unsigned int Byte_Src;
	unsigned short*	 addseave;
	chwide = MapLen >> 4;
	if((MapLen & 0xf) != 0)
	{
		chwide++;
	}
	m = MapLen & 0xf;
	addseave = p_ListDataDot;
	addseave += (24 * chwide );
	p_ListDataDot += (chwide - 1);
	k = 0;
	if( m > 0)
	{// 大于0表示数据有小于一个半字的位图
		for(j=0;j<m;j++)
		{
			Byte_Src = PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			Byte_Src = (Byte_Src << 8) |PrinterCore();
			DotArry[k++] = Byte_Src;
			g_mapCount -= 3;
		}
		Transpose32(DotArry);//转换数组
		for(i=0;i<24;i++)
		{
			Byte_Src = RevInt(DotArry[31 - i]);
			*(p_ListDataDot+chwide*i) = (unsigned short)Byte_Src;
		}
		p_ListDataDot--;
		MapLen -= m;
	}

	k = 0;
	for(j=0;j<MapLen;j++)
	{
		Byte_Src = PrinterCore();
		Byte_Src = (Byte_Src << 8) |PrinterCore();
		Byte_Src = (Byte_Src << 8) |PrinterCore();
		DotArry[k++] = Byte_Src;
		g_mapCount -= 3;
		if( k > 31)
		{
			Transpose32(DotArry);//转换数组
			for(i=0;i<24;i++)
			{
				Byte_Src = RevInt(DotArry[31 -i]);
				*(p_ListDataDot+chwide*i) = (unsigned short)(Byte_Src >> 16);
				*(p_ListDataDot+chwide*i - 1) = (unsigned short)(Byte_Src);
			}
			p_ListDataDot -= 2;
			k = 0;
		}
	}
	Transpose32(DotArry);//转换数组
	if( k > 0)
	{
		for(i=0;i<24;i++)
		{
			Byte_Src = RevInt(DotArry[31 -i]);
			*(p_ListDataDot+chwide*i) = (unsigned short)Byte_Src;
		}
		p_ListDataDot--;
		k = 0;
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
	unsigned short chwide,j,k,m,i,n,t_Reg1,t_Reg2,t_Reg3,t_Reg4;
	chwide = MapLen >> 4;
	if((MapLen & 0xf) != 0)
	{
		chwide++;
	}
	for(i = 0;i<32;i++)
	{
		DotArry[i] = 0;
	}
	for(j=0;j<MapLen;j++)
	{
		m = j >> 5;
		n = j & 0x1F;
		
		DotArry[n] |= (PrinterCore() << (24 - m * 8));

		g_mapCount --;
		k++;
		if( k > 127)
		{
			Transpose32(DotArry);//转换数组
			for(i=0;i<8;i++)
			{
				t_Reg2 = i * 4;//i/2 * 8
				t_Reg3 = i ^ 1;
				for(t_Reg1=0;t_Reg1 < 8;t_Reg1++)
				{
					*(p_ListDataDot+chwide*t_Reg1) = (unsigned short)(DotArry[t_Reg2*8 + t_Reg1] >> (16 * t_Reg3));
				}
				p_ListDataDot++;
			}
			k = 0;
			for(i = 0;i<32;i++)
			{
				DotArry[i] = 0;
			}			
		}
	}
	Transpose32(DotArry);//转换数组
	if(k > 0)
	{
		m = k >> 4;//判断有多少个半字可以取
		n = k & 0xF;
		if(n)
		{
			m++;
		}
		i = 0;
		if(m > 0)
		{
			for(i=0;i<m;i++)
			{
				t_Reg2 = i * 4;//i/2 * 8
				t_Reg3 = i ^ 1;
				t_Reg4 = 0;
				if(i == (m - 1))
				{
					if(n)
					{
						t_Reg4 = 16 - n; 					
					}
				}
				for(t_Reg1=0;t_Reg1 < 8;t_Reg1++)
				{
					*(p_ListDataDot+chwide*t_Reg1) = (unsigned short)(DotArry[t_Reg2*8 + t_Reg1] >> (16 * t_Reg3 + t_Reg4));
				}
				p_ListDataDot++;
			}
		}
	}
	p_ListDataDot += chwide *7;
}
/****************************************************************************
* 函数名：MAPDot24NOR(unsigned short Char_Type)
* 功  能：
*
* 说  明：
***********************************************************************/

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
