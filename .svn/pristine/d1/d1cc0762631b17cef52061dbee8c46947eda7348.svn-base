#include "LabelPro.h"
#include "PrinterParType.h"
#include "RD_V2_DeviceBSP.h"
//#include <stdio.h>

//// 定义最大数据点数量
//#define MAX_DATA_POINTS 100

// 定义数据点结构体
//typedef struct {
//    short x;
//    short y;
//} DataPoint;


//// 定义线性回归函数
//void linearRegression(DataPoint* data, int n, double* a, double* b) 
//{
//    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
//    for (int i = 0; i < n; i++) {
//        sumX += data[i].x;
//        sumY += data[i].y;
//        sumXY += data[i].x * data[i].y;
//        sumX2 += data[i].x * data[i].x;
//    }
//    double denominator = n * sumX2 - sumX * sumX;
//    *a = (n * sumXY - sumX * sumY) / denominator;
//    *b = (sumY * sumX2 - sumX * sumXY) / denominator;
//}

static unsigned short g_DataPoint[64];
static unsigned short g_offset = 0;
static unsigned short g_Getoffset = 0;
//static float sumX,sumY,sumXY;//,sumX2 ;
static unsigned int sumX,sumY,sumXY;//,sumX2 ;



void testcal(void)
{
	#if 0
	double  a;
	unsigned  int i;
	for(i=0;i<64;i++)
	{
		g_DataPoint[i] = 1200;
	}
	i= 1;
	while(i)
	{
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);
	SET_L_O_BEEP;
	linearRegression_0_30();
	SET_H_O_BEEP;
	linearRegression_32(&a);	
	WDG_ResetCounter();
	}
	#endif
}


void SetFreValueF(unsigned  short mdat)
{
	
	g_DataPoint[g_offset & 0x1F] = mdat;
	g_offset++;
	g_offset &= 0x1F;
}
void SetFreValue(unsigned  short mdat)
{
	
	g_DataPoint[g_offset & 0x1F] = mdat;
	g_offset++;
	g_offset &= 0x1F;
}
void linearRegression_0_30(void) 
{		
	short i;
	sumY = 0;
	sumXY = 0;
	i = 0;	
	
	g_Getoffset = (g_offset+1) & 0x1F;//当前的g_offset指向第32个数据9

	//-0
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-1
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-2
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-3
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-4
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-5
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-6
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-7
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-8
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-9
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-10
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-11
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-12
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-13
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-14
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-15
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-16
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-17
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-18
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-19
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-20
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-21
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-22
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-23
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-24
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-25
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-26
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-27
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-28
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-29
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;
	//-30
	sumY += g_DataPoint[g_Getoffset];
	sumXY += i * g_DataPoint[g_Getoffset++];
	g_Getoffset &= 0x1F;
	i++;

}
void linearRegression_32(double * a) 
{		
	unsigned short k;
	double denominator;
	double sbu;
	k = g_offset - 1;;//得到当前的数据，g_offset是指向下个存放数据的位置
	k &= 0x1F;
	sumY += g_DataPoint[k];
	sumXY += 31 * g_DataPoint[k];//第32数据的位置是x=31
	
	sumX = 496;
//	sumX2 = 10416;

	denominator = 87296;//32 * sumX2 - 246016;
	

	sbu = 32 * sumXY ;
	*a = (sbu - sumX * sumY) / denominator;
//printf("%.2f ",*a);
	//*b = (sumY * sumX2 - sumX * sumXY) / denominator;
}

//===============






