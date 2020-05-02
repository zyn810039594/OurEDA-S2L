#include "main.h"
#include "cmsis_os.h"
#include "BasicCtrl.h"
#include "Attitude.h"
#include "Setting.h"
#include "Auto.h"
#include "Display.h"
#include "BasicCtrl.h"
#include <stdlib.h>



typedef unsigned char u8;
typedef unsigned short u16;

static u8 Zero = 0;
static u8 One = 1;
static u8 NMoveMode;


static volatile uint32_t* MoveCCR[4] = { &(TIM1->CCR1), &(TIM1->CCR2), &(TIM1->CCR3), &(TIM1->CCR4) };

u16* PPortrait = 0;
u16* PRotate = 0;
u16* PVertical = 0;
u16* PROver = 0;
u16* PLight = 0;
u16* PPTZ = 0;
u16* PClip = 0;
u16* PFlip = 0;

u8* PTransverse = 0;
u8* POrient = &Zero;
u8* PDeepkeeping = &Zero;
u8* PMode = &One;
u8* PAttitude = &Zero;

u8* PassBackString = 0;
u8* PBackPower = 0;
u8* PBackWarn = 0;
u16* PBackA[3] = { 0 };
u16* PBackW[3] = { 0 };
u16* PBackE[3] = { 0 };
u16* PBackH[3] = { 0 };
u16* PBackIT = 0;
u16* PBackWT = 0;
u16* PBackWD = 0;
u8* PBackCheck = 0;

u8 XorCaculate(u8* CalString, u8 CalStringSize);
void MoveDataAnalyze();
void BasicPoint();
void BasicControl();
void FFMove();
void SixMove();
void EightMove();
void(*BasicMove[3])() = { SixMove, FFMove, EightMove };
void(*AttitudeMove[4])(u8) = { NormalMode, PitchMode, RowMode, MixMode };
void(*OrientMove[3])() = { SOrientMove, FFOrientMove, EOrientMove };
void(*DeepkeepingMove[3])() = { SDeepkeepingMove, FFDeepkeepingMove, EDeepkeepingMove };


extern u8 UART1RXCache[UART1RXLen];

//指针定向函数
void BasicPoint()
{
	PPortrait = (u16*)(UART1RXCache + 1);
	PRotate = (u16*)(UART1RXCache + 3);
	PVertical = (u16*)(UART1RXCache + 5);
	PROver = (u16*)(UART1RXCache + 7); 
	PLight = (u16*)(UART1RXCache + 9);
	PPTZ = (u16*)(UART1RXCache + 11);
	PClip = (u16*)(UART1RXCache + 13);
	PFlip = (u16*)(UART1RXCache + 15);
	PTransverse = UART1RXCache + 17;
	if (SWAutoMove)
	{
		POrient = UART1RXCache + 18;
		PDeepkeeping = UART1RXCache + 19;
	}
	
	if (SWTransMode)
	{
		PMode = UART1RXCache + 20;
	}
	if (SWAttitudeControl)
	{
		PAttitude = UART1RXCache + 21;
	}
	if (SWPassback)
	{
		PassBackString = (u8*)malloc(37);
		PassBackString[0] = 0x25;
		PassBackString[1] = 0xee;
		PBackPower = PassBackString + 2;
		PBackWarn = PassBackString + 3;
		PBackA[0] = (u16*)(PassBackString + 4);
		PBackA[1] = (u16*)(PassBackString + 6);
		PBackA[2] = (u16*)(PassBackString + 8);
		PBackW[0] = (u16*)(PassBackString + 10);
		PBackW[1] = (u16*)(PassBackString + 12);
		PBackW[2] = (u16*)(PassBackString + 14);
		PBackE[0] = (u16*)(PassBackString + 16);
		PBackE[1] = (u16*)(PassBackString + 18);
		PBackE[2] = (u16*)(PassBackString + 20);
		PBackH[0] = (u16*)(PassBackString + 22);
		PBackH[1] = (u16*)(PassBackString + 24);
		PBackH[2] = (u16*)(PassBackString + 26);
		PBackIT = (u16*)(PassBackString + 28);
		PBackWT = (u16*)(PassBackString + 30);
		PBackWD = (u16*)(PassBackString + 32);
		PBackCheck = PassBackString + 34;
		PassBackString[35] = 0xFF;
		PassBackString[36] = 0xFF;
	}
	return;
}

//基本控制程序
void BasicControl()
{
	//先对灯光、舵机等不受模式影响的值进行操作
	TIM3->CCR1 = *PClip;
	TIM3->CCR3 = *PPTZ;
	TIM3->CCR4 = *PFlip;
	TIM14->CCR1 = *PLight;
	//运动数据分析
	MoveDataAnalyze();
	//运动模式判断
	switch ((*POrient)+(*PDeepkeeping*2))
	{
	case 0:
		BasicMove[*PMode]();
		AttitudeMove[*PAttitude](*PMode);
		break;
	case 1:
		OrientMove[*PMode]();
		AttitudeMove[*PAttitude](*PMode);
		break;
	case 2:
		BasicMove[*PMode]();
		DeepkeepingMove[*PMode]();
		break;
	case 3:
		OrientMove[*PMode]();
		DeepkeepingMove[*PMode]();
		break;
	}
}
void FFMove()
{
	switch (NMoveMode)
	{
	case 1:
		*MoveCCR[0] = *PRotate;
		*MoveCCR[1] = *PRotate;
		*MoveCCR[2] = (1500 - *PRotate + *PPortrait);
		*MoveCCR[3] = (1500 - *PRotate + *PPortrait);
		break;
	case 2:
		*MoveCCR[0] = *PPortrait;
		*MoveCCR[1] = *PPortrait;
		*MoveCCR[2] = (1500 - *PRotate + *PPortrait);
		*MoveCCR[3] = (1500 - *PRotate + *PPortrait);
		break;
	case 3:
		*MoveCCR[0] = (*PRotate + *PPortrait - 1500);
		*MoveCCR[1] = (*PRotate + *PPortrait - 1500);
		*MoveCCR[2] = *PPortrait;
		*MoveCCR[3] = *PPortrait;
		break;
	case 4:
		*MoveCCR[0] = (*PRotate + *PPortrait - 1500);
		*MoveCCR[1] = (*PRotate + *PPortrait - 1500);
		*MoveCCR[2] = (2500 - *PRotate);
		*MoveCCR[3] = (2500 - *PRotate);
		break;
	case 5:
		*MoveCCR[0] = *PRotate;
		*MoveCCR[1] = (2500 - *PRotate);
		*MoveCCR[2] = *PRotate;
		*MoveCCR[3] = 2500 - *PRotate;
		break;
	}
}
void SixMove()
{
	
}
void EightMove()
{
	
}
void MoveDataAnalyze()
{
	if ((*PRotate > 1500 - RegFloating)&&(*PRotate < 1500 + RegFloating))
	{
		*PRotate = 1500;
	}
	if ((*PPortrait > 1500 - RegFloating)&&(*PPortrait < 1500 + RegFloating))
	{
		*PPortrait = 1500;
	}
	if ((*PVertical > 1500 - RegFloating)&&(*PVertical < 1500 + RegFloating))
	{
		*PVertical = 1500;
	}
	if (*PTransverse)
	{
		NMoveMode = 5;
		return;
	}
	u8 AFlag = (*PRotate > *PPortrait);
	u8 BFlag = ((*PRotate + *PPortrait) > 3000);
	u8 CFlag = (*PRotate > 1500);
	u8 DFlag = (*PPortrait > 1500);
	u8 SFlag = AFlag * 8 + BFlag * 4 + CFlag * 2 + DFlag;
	switch (SFlag)
	{
	case 0:case 15:
		NMoveMode = 1;
		break;
	case 7:case 8:
		NMoveMode = 2;
		break;
	case 5:case 10:
		NMoveMode = 3;
		break;
	case 1:case 14:
		NMoveMode = 4;
		break;
	}
	return;
}
void PassBack(UART_HandleTypeDef* PassBackUart,u8 PowerNum, u8 WarnNum, u16 ANum[3], u16 WNum[3], u16 ENum[3], u16 HNum[3], u16 ITNum, u16 WTNum, u16 WDNum)
{
	*PBackPower = PowerNum;
	*PBackWarn = WarnNum;
	*PBackA[0] = ANum[0];
	*PBackA[1] = ANum[1];
	*PBackA[2] = ANum[2];
	*PBackW[0] = WNum[0];
	*PBackW[1] = WNum[1];
	*PBackW[2] = WNum[2];
	*PBackE[0] = ENum[0];
	*PBackE[1] = ENum[1];
	*PBackE[2] = ENum[2];
	*PBackH[0] = HNum[0];
	*PBackH[1] = HNum[1];
	*PBackH[2] = HNum[2];
	*PBackIT = ITNum;
	*PBackWT = WTNum;
	*PBackWD = WDNum;
	*PBackCheck = XorCaculate(PassBackString, 34);
	HAL_UART_Transmit_DMA(PassBackUart, PassBackString, 37);
}
//异或运算位计算
u8 XorCaculate(u8* CacString, u8 CacStringSize)
{
	u8 CacResult = CacString[0];
	for (u8 i = 1; i < CacStringSize; ++i)
	{
		CacResult ^= CacString[i];
	}
	return CacResult;
}