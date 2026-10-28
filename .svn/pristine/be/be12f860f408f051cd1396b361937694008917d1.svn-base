// QR_Encode.h : CQR_Encode 类声明和接口定义
// Date 2006/05/17	Ver. 1.22	Psytec Inc.
#ifndef AFX_QR_ENCODE_H
#define AFX_QR_ENCODE_H

#ifdef QR_ENCODE_ENABLE
#include "PDL.h"
// #include  <stdlib.h>
typedef unsigned char      BYTE;
typedef unsigned short      WORD;
//float abs(float __x);
/////////////////////////////////////////////////////////////////////////////
// 常数

// 误差修正水平
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// 数据模式
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// 版（模型）组
#define QR_VRESION_S	0 // 1 - 9
#define QR_VRESION_M	1 // 10 - 26
#define QR_VRESION_L	2 // 27 - 40

#define MAX_ALLCODEWORD	 1085 // 最高总数码字版本40 为 3706最大  版本20 为 1085
#define MAX_DATACODEWORD  861 // 最大数据编码字（版本40 - L）的版本40 为 2956最大 版本20 为 861
#define MAX_CODEBLOCK	  153 // 最大数量的Burokkudetakodowado（含遥感码字）
#define MAX_MODULESIZE	  97 // 最大数量的模块一边版本40 为 177最大 版本20 为 97

//位图绘
#define QR_MARGIN	4

/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// 块的RS
	int ncAllCodeWord;	// 块中的数目字码
	int ncDataCodeWord;	// 数据代码字数（代码字数 - RS码字数）

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR码版本（型号）的信息

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // 版本（型号）号码（1-40）
	int ncAllCodeWord; //

	// (0 = L, 1 = M, 2 = Q, 3 = H)
	int ncDataCodeWord[4];	//

	int ncAlignPoint;	//
	int nAlignPoint[6];	//

	RS_BLOCKINFO RS_BlockInfo1[4]; //
	RS_BLOCKINFO RS_BlockInfo2[4]; //

} QR_VERSIONINFO, *LPQR_VERSIONINFO;
void Setm_byModuleData(int x, int y,unsigned char data);
unsigned char Getm_byModuleData(int x, int y);
int CountPenalty(void);
void SetFormatInfoPattern(int nPatternNo);
void SetMaskingPattern(int nPatternNo);
void SetCodeWordPattern(void);
void SetVersionPattern(void);
void SetAlignmentPattern(int x, int y);
void SetFinderPattern(int x, int y);
void SetFunctionModule(void);
void FormatModule(void);
unsigned char Get_byRSExp(int x, int y);
void GetRSCodeWord(unsigned char* lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);
unsigned short KanjiToBinaly(unsigned short wc);
unsigned char AlphabetToBinaly(unsigned char c);
unsigned char IsKanjiData(unsigned char c1, unsigned char c2);
unsigned char IsAlphabetData(unsigned char c);
unsigned char IsNumeralData(unsigned char c);
int SetBitStream(int nIndex, unsigned short wData, int ncData);
int GetBitLength(unsigned char nMode, int ncData, int nVerGroup);
unsigned char EncodeSourceData(char* lpsSource, int ncLength, int nVerGroup);
int GetEncodeVersion(int nVersion, char* lpsSource, int ncLength);
unsigned char EncodeData(int nLevel, int nVersion, unsigned char bAutoExtent, int nMaskingNo, char* lpsSource, int ncSource);
unsigned char GetModuleData(int x, int y);
//
/////////////////////////////////////////////////////////////////////////////
#endif 

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
