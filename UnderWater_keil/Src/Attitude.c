#include "main.h"
#include "Attitude.h"
#include "Setting.h"

typedef unsigned char u8;
typedef unsigned short u16;


static u16* PAccPos[3];
static u16* PRotPos[3];
static u16* PEulPos[3];
static u16* PMagPos[3];
static u16* PInsTemPos[4];




//姿态控制代码（只写了4推模式）
//正常模式代码
void NormalMode(u8 Trans, volatile uint32_t* MoveThruster[8], u16 VerticalNum, u16 ROverNum)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*MoveThruster[4] = VerticalNum;
		*MoveThruster[5] = VerticalNum;
		*MoveThruster[6] = VerticalNum;
		*MoveThruster[7] = VerticalNum;
		break;
	case 2:
		
		break;
	}
	return;
}
//俯仰模式代码
void PitchMode(u8 Trans, volatile uint32_t* MoveThruster[8], u16 VerticalNum, u16 ROverNum)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*MoveThruster[4] = 3000 - VerticalNum;
		*MoveThruster[5] = VerticalNum;
		*MoveThruster[6] = VerticalNum;
		*MoveThruster[7] = 3000 - VerticalNum;
		break;
	case 2:
		
		break;
	}
	return;
}
//横滚模式代码
void RowMode(u8 Trans, volatile uint32_t* MoveThruster[8], u16 VerticalNum, u16 ROverNum)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:
		*MoveThruster[4] = 3000 - ROverNum;
		*MoveThruster[5] = 3000 - ROverNum;
		*MoveThruster[6] = ROverNum;
		*MoveThruster[7] = ROverNum;
		break;
	case 2:
		
		break;
	}
	return;
}
//混合模式代码（未编辑）
void MixMode(u8 Trans, volatile uint32_t* MoveThruster[8], u16 VerticalNum, u16 ROverNum)
{
	switch (Trans)
	{
	case 0:
		
		break;
	case 1:

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
	PDN[3] = 10;
	PWTN[2] = 10;
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
//姿态指针定向
void AttitudePoint(u8* SerialData)
{
	PAccPos[0] = (u16*)(SerialData + 2);
	PAccPos[1] = (u16*)(SerialData + 4);
	PAccPos[2] = (u16*)(SerialData + 6);
	PInsTemPos[0] = (u16*)(SerialData + 8);
	PRotPos[0] = (u16*)(SerialData + 13);
	PRotPos[1] = (u16*)(SerialData + 15);
	PRotPos[2] = (u16*)(SerialData + 17);
	PInsTemPos[1] = (u16*)(SerialData + 19);
	PEulPos[0] = (u16*)(SerialData + 24);
	PEulPos[1] = (u16*)(SerialData + 26);
	PEulPos[2] = (u16*)(SerialData + 28);
	PInsTemPos[2] = (u16*)(SerialData + 30);
	PMagPos[0] = (u16*)(SerialData + 35);
	PMagPos[1] = (u16*)(SerialData + 37);
	PMagPos[2] = (u16*)(SerialData + 39);
	PInsTemPos[2] = (u16*)(SerialData + 41);
}
//姿态数据取出
void AttitudeTake(u16* AttitudeNum,u16* AccelerationNum,u16* RotSpeedNum,u16* EulerAngleNum,u16* MagnetismNum,u16* InsideTempertureNum)
{
	
	AccelerationNum[0] = *PAccPos[0];
	AccelerationNum[1] = *PAccPos[1];
	AccelerationNum[2] = *PAccPos[2];
	RotSpeedNum[0] = *PRotPos[0];
	RotSpeedNum[1] = *PRotPos[1];
	RotSpeedNum[2] = *PRotPos[2];
	EulerAngleNum[0] = *PEulPos[0];
	EulerAngleNum[1] = *PEulPos[1];
	EulerAngleNum[2] = *PEulPos[2];
	MagnetismNum[0] = *PMagPos[0];
	MagnetismNum[1] = *PMagPos[1];
	MagnetismNum[2] = *PMagPos[2];
	*InsideTempertureNum = (*PInsTemPos[0] + *PInsTemPos[1] + *PInsTemPos[2] + *PInsTemPos[3]) / 4;
	AttitudeNum[0] = (u16)(((float)EulerAngleNum[0] / 32768) * 1800);
	AttitudeNum[1] = (u16)(((float)EulerAngleNum[1] / 32768) * 1800);
	AttitudeNum[2] = (u16)(((float)EulerAngleNum[2] / 32768) * 1800);
}