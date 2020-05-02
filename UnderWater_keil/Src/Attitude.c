#include "main.h"
#include "cmsis_os.h"
#include "Attitude.h"
#include "Setting.h"

typedef unsigned char u8;
typedef unsigned short u16;

static volatile uint32_t *VerticalCCR[4] = { &(TIM2->CCR1), &(TIM2->CCR2), &(TIM2->CCR3), &(TIM2->CCR4) };


extern u16* PRotate;
extern u16* PROver;


//姿态控制代码（只写了4推模式）
//正常模式代码
void NormalMode(u8 Trans)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*VerticalCCR[0] = *PRotate;
		*VerticalCCR[1] = *PRotate;
		*VerticalCCR[2] = *PRotate;
		*VerticalCCR[3] = *PRotate;
		break;
	case 2:
		
		break;
	}
	return;
}
//俯仰模式代码
void PitchMode(u8 Trans)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*VerticalCCR[0] = 2500-*PRotate;
		*VerticalCCR[1] = *PRotate;
		*VerticalCCR[2] = *PRotate;
		*VerticalCCR[3] = 2500-*PRotate;
		break;
	case 2:
		
		break;
	}
	return;
}
//横滚模式代码
void RowMode(u8 Trans)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*VerticalCCR[0] = 2500-*PROver;
		*VerticalCCR[1] = 2500-*PROver;
		*VerticalCCR[2] = *PROver;
		*VerticalCCR[3] = *PROver;
		break;
	case 2:
		
		break;
	}
	return;
}
//混合模式代码（未编辑）
void MixMode(u8 Trans)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*VerticalCCR[0] = *PRotate;
		*VerticalCCR[1] = *PRotate;
		*VerticalCCR[2] = *PRotate;
		*VerticalCCR[3] = *PRotate;
		break;
	case 2:
		
		break;
	}
	return;
}
//姿态数据分析代码
//水深数据分析
void DeepAnalyze(u8* DataPosition,u16* PDepth,u16*PWaterTemperture,u8* PDN,u8* PWTN)
{
	u8 i = 0;
	PDN[3] = 11;
	PWTN[2] = 11;
	if (DataPosition[4] == '.')
	{
		PWTN[0] = (DataPosition[2] - '0');
		PWTN[1] = (DataPosition[3] - '0');
		PWTN[3] = (DataPosition[5] - '0');
		PWTN[4] = (DataPosition[6] - '0');
		*PWaterTemperture = PWTN[0] * 1000 + PWTN[1] * 100 + PWTN[3] * 10 + PWTN[4];
		if (DataPosition[9] == '-')
		{
			*PDepth = 0;
			PDN[0] = 0;
			PDN[1] = 0;
			PDN[2] = 0;
			PDN[4] = 0;
			PDN[5] = 0;
		}
		else
		{
			if (DataPosition[10] == '.')
			{
				PDN[0] = 0;
				PDN[1] = 0;
				PDN[2] = DataPosition[9] - '0';
				PDN[4] = DataPosition[11] - '0';
				PDN[5] = DataPosition[12] - '0';
				*PDepth = PDN[2] * 100 + PDN[4] * 10 + PDN[5];
			}
			else if (DataPosition[11] == '.')
			{
				PDN[0] = 0;
				PDN[1] = DataPosition[9] - '0';
				PDN[2] = DataPosition[10] - '0';
				PDN[4] = DataPosition[12] - '0';
				PDN[5] = DataPosition[13] - '0';
				*PDepth = PDN[1] * 1000 + PDN[2] * 100 + PDN[4] * 10 + PDN[5];
			}
			else if (DataPosition[12] == '.')
			{
				PDN[0] = DataPosition[9] - '0';
				PDN[1] = DataPosition[10] - '0';
				PDN[2] = DataPosition[12] - '0';
				PDN[4] = DataPosition[13] - '0';
				PDN[5] = DataPosition[14] - '0';
				*PDepth = PDN[0] * 10000 + PDN[1] * 1000 + PDN[2] * 100 + PDN[4] * 10 + PDN[5];
			}
		}
	}
	else if (DataPosition[3] == '.')
	{
		PWTN[0] = 0;
		PWTN[1] = (DataPosition[2] - '0');
		PWTN[3] = (DataPosition[4] - '0');
		PWTN[4] = (DataPosition[5] - '0');
		*PWaterTemperture = PWTN[1] * 100 + PWTN[3] * 10 + PWTN[4];
		if (DataPosition[8] == '-')
		{
			*PDepth = 0;
			PDN[0] = 0;
			PDN[1] = 0;
			PDN[2] = 0;
			PDN[4] = 0;
			PDN[5] = 0;
		}
		else
		{
			if (DataPosition[9] == '.')
			{
				PDN[0] = 0;
				PDN[1] = 0;
				PDN[2] = DataPosition[8] - '0';
				PDN[4] = DataPosition[10] - '0';
				PDN[5] = DataPosition[11] - '0';
				*PDepth = PDN[2] * 100 + PDN[4] * 10 + PDN[5];

			}
			else if (DataPosition[10] == '.')
			{
				PDN[0] = 0;
				PDN[1] = DataPosition[8] - '0';
				PDN[2] = DataPosition[9] - '0';
				PDN[4] = DataPosition[11] - '0';
				PDN[5] = DataPosition[12] - '0';
				*PDepth = PDN[1] * 1000 + PDN[2] * 100 + PDN[4] * 10 + PDN[5];

			}
			else if (DataPosition[11] == '.')
			{
				PDN[0] = DataPosition[8] - '0';
				PDN[1] = DataPosition[9] - '0';
				PDN[2] = DataPosition[10] - '0';
				PDN[4] = DataPosition[12] - '0';
				PDN[5] = DataPosition[13] - '0';
				*PDepth = PDN[0] * 10000 + PDN[1] * 1000 + PDN[2] * 100 + PDN[4] * 10 + PDN[5];
			}
		}
	}
}
//姿态数据分析
void AttitudeAnalyze(u16* PRollL,u16* AttitudeNum)
{
	AttitudeNum[0] = PRollL[0] * 32768 * 1800;
	AttitudeNum[1] = PRollL[1] * 32768 * 1800;
	AttitudeNum[2] = PRollL[2] * 32768 * 1800;
}