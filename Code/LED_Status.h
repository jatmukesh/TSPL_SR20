#ifndef _LED_STATU_H
#define _LED_STATU_H
/*
//8v 以上100%
//7.5v 以上75%
//7.3v y以上50%
//7.1v y以上25%

*/
#define		BATTERY_POWER_100		8000
#define		BATTERY_POWER_75		7500
#define		BATTERY_POWER_50		7300
#define		BATTERY_POWER_25		7100
#define		BATTERY_POWER_00		7000
void SetChargeLedStatus(unsigned short mPowVol);

void LED_SetChargeStatus(unsigned char mleddat);

#endif


