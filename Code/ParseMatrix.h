#ifndef PARSEMATRIX_H
#define PARSEMATRIX_H
#include "ReceiveBufferPro.h"

void CharProcessDeg0(void);
void CharProcessDeg90(void);
void CharProcessDeg180(void);
void CharProcessDeg270(void);
void DotProcessDeg270(unsigned short Char_Type);
void DotProcessDeg90(unsigned short Char_Type);
void DotProcessDeg0(unsigned short Char_Type);
void DotProcessDeg180(unsigned short Char_Type);
void MAPDot24NOR(unsigned short MapLen);
void MAPDot24FX(unsigned short MapLen);
void MAPDot8NOR(unsigned short MapLen);
void MAPDot8FX(unsigned short MapLen);
void Dot12ProcessDeg0(unsigned short Char_Type);
void Dot12ProcessDeg90(unsigned short Char_Type);
void Dot12ProcessDeg180(unsigned short Char_Type);
void Dot12ProcessDeg270(unsigned short Char_Type);
void CharProDotPro(unsigned short t_Wide,unsigned short t_High,unsigned short t_Deg,unsigned int DotArry[32]);

#endif
