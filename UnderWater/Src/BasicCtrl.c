#include "main.h"
#include "cmsis_os.h"
#include "BasicCtrl.h"
#include "Attitude.h"
#include "Setting.h"
#include "Auto.h"
#include "Display.h"
#include "BasicCtrl.h"



typedef unsigned char u8;
typedef unsigned short u16;

static u8 Zero = 0;
static u8 One = 1;
static u8 NMoveMode;

static volatile uint32_t* MoveCCR[4] = { &(TIM1->CCR1), &(TIM1->CCR2), &(TIM1->CCR3), &(TIM1->CCR4) };

u16* PPortrait = 0;
u16* PRotate = 0;
u16* PVertical = 0;
u16* PAUndefined1 = 0;
u16* PLight = 0;
u16* PPTZ = 0;
u16* PClip = 0;
u16* PFlip = 0;

u8* PTransverse = 0;
u8* POrient = &Zero;
u8* PDeepkeeping = &Zero;
u8* PMode = &One;
u8* PAttitude = &Zero;







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
	PLight = (u16*)(UART1RXCache + 7);
	PPTZ = (u16*)(UART1RXCache + 9);
	PClip = (u16*)(UART1RXCache + 11);
	PFlip = (u16*)(UART1RXCache + 13);
	PTransverse = UART1RXCache + 15;
	if (SWAutoMove)
	{
		POrient = UART1RXCache + 16;
		PDeepkeeping = UART1RXCache + 17;
	}
	
	if (SWTransMode)
	{
		PMode = UART1RXCache + 18;
	}
	if (SWAttitudeControl)
	{
		PAttitude = UART1RXCache + 19;
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
	u8 AFlag = (*PRotate > *PPortrait);
	u8 BFlag = ((*PRotate + *PPortrait) > 3000);
	u8 CFlag = (*PRotate > 1500);
	u8 DFlag = (*PPortrait > 1500);
	switch (*PTransverse)
	{
	case 0:
		if (BFlag == 1)
		{
			if (CFlag == 0)
			{
				NMoveMode = 3;
			}
			else if (DFlag == 0)
			{
				NMoveMode = 4;
			}
			else
			{
				if (AFlag == 1)
				{
					NMoveMode = 1;
				}
				else
				{
					NMoveMode = 2;
				}
			}
		}
		else
		{
			if (CFlag == 1)
			{
				NMoveMode = 3;
			}
			else if (DFlag == 1)
			{
				NMoveMode = 4;
			}
			else
			{
				if (AFlag == 0)
				{
					NMoveMode = 1;
				}
				else
				{
					NMoveMode = 2;
				}
			}
		}
		break;
	case 1:
		NMoveMode = 5;
		break;
	}
	return;
}