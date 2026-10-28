#ifndef RD_MP_IICTIMER_H
#define RD_MP_IICTIMER_H
#ifdef TIME_PRINTER

void I2C_delay(void) ;
void Delay(unsigned short DelayCount);	
void R8025_IICStart(void);
void R8025_IICStop(void);
void R8025_IICNoAck(void);
void R8025_IICAck(void);

unsigned char R8025_IICRecAck(void) ;
void R8025_IICSendByte(unsigned char SendByte);
unsigned char R8025_IICReceiveByte(void) ;

void Set_R8025(unsigned char timetype,unsigned char value);

void CleanAlarm(void);
void Set_timer(unsigned char data[]);
unsigned char ReadPCF8563(unsigned char m_Space);
void ReturnTime(unsigned char data[]);


#endif



#endif


