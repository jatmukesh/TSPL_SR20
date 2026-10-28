#if !defined( _MYTYPE_H )
#define _MYTYPE_H
#include "PDL.h"
#define SPI_Mode_Select      ((u16)0xF7FF)
#define CR1_SPE_Set          ((u16)0x0040)
void WREN_Check(void);
void WRSR(void);
void mySPI_Init(void);
unsigned char SPI_Send_word(unsigned char byte);
void AT91F_DataFlashADDRRead (unsigned int Addr,unsigned len, unsigned short *dataBuffer);
void MX25_PowerDown (void);
void MX25_Resume (void);
void WREN(void);
void WRDI(void);
void WR_OneByte(unsigned int Addr,unsigned char data);
void ERASE_32K(void);
void ERASE_4K(unsigned int Addr);
unsigned char RDSR (void);
unsigned short RD_ID(void);
void SST25V_DBSY(void);
unsigned char ReadByte (unsigned int Addr);
void AT91F_DataFlashPageWrite (unsigned int PageAddr,unsigned short *dataBuffer);
void AT91F_DataFlashPageWrite_NE (unsigned int PageAddr,unsigned short *dataBuffer);
void AT91F_WritenOrderBytes (unsigned int dataAddr,unsigned char *dataBuffer,unsigned short len);
unsigned short ReadWord (unsigned int Addr);
void ReadFlashAndDeg(unsigned int Addr,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32]);
void AT91F_WritenNBytes (unsigned int dataAddr,unsigned char *dataBuffer,unsigned short len);
void ReadNBytes (unsigned int Addr,unsigned char *bufferDatChar,unsigned short len);
unsigned short cal_crc (unsigned int Addr,unsigned int len);
void PowerDown (void);
void MX_Resume (void);
void mySPsI_Init(void);
void ReadIntFlashAndDeg(unsigned int Addr,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32],unsigned char codepage);
void ReadIntAndDeg917(unsigned int Addr,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32]);
unsigned int  ReadFlashAndDeg24x12(char mChar,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32]);
void ReadInFlashAndDeg(unsigned int m_offset,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32]);
void ReadIntAndDeg(unsigned int Addr,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32]);
void ReadIntAndDeg917(unsigned int Addr,unsigned short s_Wide,unsigned short s_High,unsigned int DotArry[32]);

#ifdef CHECKSUM
 //Address 0 to 64 KB is bootloader & after 64KB is the actual flash started
#define FLASH_START_ADDRESS     0x01010000UL   // Start at 64KB 
#define FLASH_END_ADDRESS       0x0102F496UL  // End at  
//#define CHECKSUM_FLASH_ADDRESS  0x01040000UL

//#define EXPECTED_CHECKSUM       
#define READ_AddrValue(addr)   (*(volatile uint32_t *)(addr))



extern void  Delay1ms(unsigned short mdelay);
uint32_t Flash_Checksum_Calculation(void);
void Flash_Checksum_Failed(void);
uint32_t Flash_ReadData(uint32_t Read_Address);
void Flash_WriteData(uint32_t Addr, uint32_t Data);




#endif



#endif
