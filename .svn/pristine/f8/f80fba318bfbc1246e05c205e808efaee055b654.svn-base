#include "LED_Status.h"
#include "PDL.h"
extern _PDL		PRINTER_PDL ;

//#define		BATTERY_POWER_100		3950
//#define		BATTERY_POWER_70		3740
//#define		BATTERY_POWER_30		3620
//#define		BATTERY_POWER_00		3440

//PRINTER_PDL.SYS._LED_BT_Link_OFF();
//PRINTER_PDL.SYS._LED_Stauts_OFF();
//PRINTER_PDL.SYS._LED_Power_OFF();			
	
unsigned int g_ChargeLedcount=0;
unsigned int g_SavVol=0;

extern unsigned int g_BatDisCount;
void SetChargeLedStatus(unsigned short mPowVol)
{
	g_ChargeLedcount++;
	
	if(g_SavVol > (mPowVol +80) )
	{
		g_SavVol = mPowVol;
	}
	else if(mPowVol > (g_SavVol +80) )
	{
		g_SavVol = mPowVol;
	}
	if(g_SavVol > BATTERY_POWER_00)
	{
		if(g_BatDisCount > 5000)
		{
			g_BatDisCount = 0xF0000000;
			PRINTER_PDL.SYS._Charge_LED1_OFF();
			PRINTER_PDL.SYS._Charge_LED2_OFF();
			PRINTER_PDL.SYS._Charge_LED3_OFF();		
			return;			
		}
		
	}
	
	if(g_SavVol > BATTERY_POWER_100)
	{//100%

		PRINTER_PDL.SYS._Charge_LED1_ON();
		PRINTER_PDL.SYS._Charge_LED2_ON();
		PRINTER_PDL.SYS._Charge_LED3_ON();
	}
	else if(g_SavVol > BATTERY_POWER_70)
	{//60%

		PRINTER_PDL.SYS._Charge_LED1_ON();
		PRINTER_PDL.SYS._Charge_LED2_ON();
		PRINTER_PDL.SYS._Charge_LED3_OFF();
	}		
	else if(g_SavVol > BATTERY_POWER_30)
	{//30%

		PRINTER_PDL.SYS._Charge_LED1_ON();
		PRINTER_PDL.SYS._Charge_LED2_OFF();
		PRINTER_PDL.SYS._Charge_LED3_OFF();
	}		
	else if(g_SavVol > BATTERY_POWER_00)
	{//闪烁
		if(g_ChargeLedcount > 300)
		{
			g_ChargeLedcount = 0;
			if(PRINTER_PDL.SYS._GET_Charge_LED1())
			{
				PRINTER_PDL.SYS._Charge_LED1_ON();
				PRINTER_PDL.SYS._Charge_LED2_OFF();
				PRINTER_PDL.SYS._Charge_LED3_OFF();
			}
			else
			{
				PRINTER_PDL.SYS._Charge_LED1_OFF();
				PRINTER_PDL.SYS._Charge_LED2_OFF();
				PRINTER_PDL.SYS._Charge_LED3_OFF();
			}
		}
	}		
	else
	{//自动关机
		PRINTER_PDL.SYS._Charge_LED1_OFF();
		PRINTER_PDL.SYS._Charge_LED2_OFF();
		PRINTER_PDL.SYS._Charge_LED3_OFF();
	}		
	
	
}

void LED_SetChargeStatus(unsigned char mleddat)
{

	if(mleddat&1)
	{
		PRINTER_PDL.SYS._Charge_LED3_ON();
	}
	else
	{
		PRINTER_PDL.SYS._Charge_LED3_OFF();
	}		
	if(mleddat&2)
	{
		PRINTER_PDL.SYS._Charge_LED2_ON();
	}
	else
	{
		PRINTER_PDL.SYS._Charge_LED2_OFF();
	}				
	if(mleddat&4)
	{
		PRINTER_PDL.SYS._Charge_LED1_ON();
	}
	else
	{
		PRINTER_PDL.SYS._Charge_LED1_OFF();
	}	
}



