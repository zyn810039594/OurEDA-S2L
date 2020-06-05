#include "main.h"
#include "Display.h"
#include "Setting.h"
#include "BasicCtrl.h"

typedef unsigned char u8;
typedef unsigned short u16;

//数字字符
static u8 NumChar[11][2] = { { 0xA3, 0xB0 }, { 0xA3, 0xB1 }, { 0xA3, 0xB2 }, { 0xA3, 0xB3 }, { 0xA3, 0xB4 }, { 0xA3, 0xB5 }, { 0xA3, 0xB6 }, { 0xA3, 0xB7 }, { 0xA3, 0xB8 }, { 0xA3, 0xB9 }, { 0xA3, 0xAE } };

/*
//报警回复字符
u8 Warn1[14] = { 0xBC, 0xE0, 0xB2, 0xE2, 0xB5, 0xE3, 0xA3, 0xB1, 0xB4, 0xA6, 0xC2, 0xA9, 0xCB, 0xAE };
u8 Warn2[14] = { 0xBC, 0xE0, 0xB2, 0xE2, 0xB5, 0xE3, 0xA3, 0xB2, 0xB4, 0xA6, 0xC2, 0xA9, 0xCB, 0xAE };
u8 CleanWarn[14] = { 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1 };
//报警显示位置
u8 Warn1Position[4] = { POSX_WARN1_HIGH, POSX_WARN1_LOW, POSY_WARN1_HIGH, POSY_WARN1_LOW };
u8 Warn2Position[4] = { POSX_WARN2_HIGH, POSX_WARN2_LOW, POSY_WARN2_HIGH, POSY_WARN2_LOW };
*/
u8 WarnSet[2][23] =  
{ 
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//报警1显示位置
	POSX_WARN1_HIGH,
	POSX_WARN1_LOW,
	POSY_WARN1_HIGH,
	POSY_WARN1_LOW,
	//报警回复字符
	0xBC,
	0xE0,
	0xB2,
	0xE2,
	0xB5,
	0xE3,
	0xA3,
	0xB1,
	0xB4,
	0xA6,
	0xC2,
	0xA9,
	0xCB,
	0xAE,
	//校验位
	0x00
},
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//报警2回复位置
	POSX_WARN2_HIGH,
	POSX_WARN2_LOW,
	POSY_WARN2_HIGH,
	POSY_WARN2_LOW,
	//报警显示字符
	0xBC,
	0xE0,
	0xB2,
	0xE2,
	0xB5,
	0xE3,
	0xA3,
	0xB2,
	0xB4,
	0xA6,
	0xC2,
	0xA9,
	0xCB,
	0xAE,
	//校验位
	0x00
}
		
};
u8 WarnClean[2][23] =  
{ 
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//报警1显示位置
	POSX_WARN1_HIGH,
	POSX_WARN1_LOW,
	POSY_WARN1_HIGH,
	POSY_WARN1_LOW,
	//报警清空字符
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	//校验位
	0x00
},
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//报警2显示位置
	POSX_WARN2_HIGH,
	POSX_WARN2_LOW,
	POSY_WARN2_HIGH,
	POSY_WARN2_LOW,
	//报警清空字符
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	0xA1,
	//校验位
	0x00
}	
};
/*
//水深回复字符
u8 RDeep[6] = { 0xC9, 0xEE, 0xB6, 0xC8, 0xA3, 0xBA };
//水温回复字符
u8 RWT[6] = { 0xCB, 0xAE, 0xCE, 0xC2, 0xA3, 0xBA };
//水深显示位置（水温隔个空格）
u8 WaterDeepPosition[4] = { POSX_WATERDEEP_HIGH,POSX_WATERDEEP_LOW,POSY_WATERDEEP_HIGH,POSY_WATERDEEP_LOW};
*/
u8 WaterDeepS[45] =  
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//水深显示位置（水温在水深后隔个空格）
	POSX_WATERDEEP_HIGH,
	POSX_WATERDEEP_LOW,
	POSY_WATERDEEP_HIGH,
	POSY_WATERDEEP_LOW,
	//水深回复字符
	0xC9,
	0xEE,
	0xB6,
	0xC8,
	0xA3,
	0xBA,
	//水深数字位
	0x00,
	0x00, 
	0x00, 
	0x00,
	0x00,
	0x00,
	0x00,
	0x00, 
	0x00, 
	0x00,
	0x00,
	0x00,
	//空格
	0xA1,
	0xA1,
	//水温回复字符
	0xCB,
	0xAE,
	0xCE,
	0xC2,
	0xA3,
	0xBA,
	//水温数字位
	0x00,
	0x00,
	0x00, 
	0x00,
	0x00,
	0x00,
	0x00,
	0x00, 
	0x00,
	0x00,
	//校验位
	0x00
};
/*
//姿态显示字符
u8 RAttitude[8] = { 0xA3, 0xD9, 0xA3, 0xD0, 0xA3, 0xD2, 0xA3, 0xBA };
//姿态显示位置
u8 AttPosition[4] = { POSX_ATTITUDE_HIGH,POSX_ATTITUDE_LOW,POSY_ATTITUDE_HIGH,POSY_ATTITUDE_LOW };
*/
u8 AttitudeS[51] =
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//姿态显示位置
	POSX_ATTITUDE_HIGH,
	POSX_ATTITUDE_LOW,
	POSY_ATTITUDE_HIGH,
	POSY_ATTITUDE_LOW,
	//姿态显示字符
	0xA3,
	0xD9,
	0xA3,
	0xD0,
	0xA3,
	0xD2,
	0xA3,
	0xBA,
	//Yaw数字位
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	//空格
	0xA1,
	0xA1,
	//Pitch数字位
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	//空格
	0xA1,
	0xA1,
	//Row数字位
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	//校验位
	0x00
};
/*
//电池显示字符
u8 RPower[6] = { 0xB5, 0xE7, 0xB3, 0xD8, 0xA3, 0xBA };
//电池显示位置
u8 PowerPosition[4] = { POSX_POWER_HIGH, POSX_POWER_LOW, POSY_POWER_HIGH, POSY_POWER_LOW };
*/
u8 PowerS[23] =
{
	0x5A,
	0x5B,
	0x63,
	0xFF,
	//电池显示位置
	POSX_POWER_HIGH,
	POSX_POWER_LOW,
	POSY_POWER_HIGH,
	POSY_POWER_LOW,
	//电池显示字符
	0xB5,
	0xE7,
	0xB3,
	0xD8,
	0xA3,
	0xBA,
	//电池数字位
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	//电池伏特位
	0xA3, 
	0xD6,
	//校验位
	0x00
};


//报警函数
void DisWarning(UART_HandleTypeDef* DisUart,u8 WarningPlace)
{
	switch (WarningPlace)
	{
	case 0:
		WarnSet[0][22] = XorCaculate(WarnSet[0], 22);
		HAL_UART_Transmit_DMA(DisUart, WarnSet[0], 23);
		break;
	case 1:
		WarnSet[1][22] = XorCaculate(WarnSet[1], 22);
		HAL_UART_Transmit_DMA(DisUart, WarnSet[1], 23);
		break;
	case 3:
		WarnClean[0][22] = XorCaculate(WarnClean[0], 22);
		HAL_UART_Transmit_DMA(DisUart, WarnClean[0], 23);
		break;
	case 4:
		WarnClean[1][22] = XorCaculate(WarnClean[1], 22);
		HAL_UART_Transmit_DMA(DisUart, WarnClean[1], 23);
		break;
	}
}
//水深显示函数
void DisWaterDeep(UART_HandleTypeDef* DisUart, u8* WaterDeepNum, u8* WaterTempertureNum)
{
	u8 i;
	for (i = 0; i < 6; ++i)
	{
		WaterDeepS[14 + i + i] = NumChar[WaterDeepNum[i]][0];
		WaterDeepS[15 + i + i] = NumChar[WaterDeepNum[i]][1];
	}
	for (i = 0; i < 5; ++i)
	{
		WaterDeepS[34 + i + i] = NumChar[WaterTempertureNum[i]][0];
		WaterDeepS[35 + i + i] = NumChar[WaterTempertureNum[i]][1];
	}
	WaterDeepS[44] = XorCaculate(WaterDeepS, 44);
	HAL_UART_Transmit_DMA(DisUart, WaterDeepS, 45);
}
//姿态显示函数
void DisAttitude(UART_HandleTypeDef* DisUart, u16 YawNum, u16 PitchNum, u16 RollNum, u16 InsideTempertureNum)
{
	u8 i;
	u8 YN[5];
	u8 PN[5];
	u8 RN[5];

	YN[0] = YawNum / 1000;
	YawNum %= 1000;
	YN[1] = YawNum / 100;
	YawNum %= 100;
	YN[2] = YawNum / 10;
	YN[3] = 10;
	YN[4] = YawNum % 10;
	PN[0] = PitchNum / 1000;
	PitchNum %= 1000;
	PN[1] = PitchNum / 100;
	PitchNum %= 100;
	PN[2] = PitchNum / 10;
	PN[3] = 10;
	PN[4] = PitchNum % 10;
	RN[0] = RollNum / 1000;
	RollNum %= 1000;
	RN[1] = RollNum / 100;
	RollNum %= 100;
	RN[2] = RollNum / 10;
	RN[3] = 10;
	RN[4] = RollNum % 10;	
//	u8 ITN[5];
//	ITN[0] = InsideTempertureNum / 1000;
//	InsideTempertureNum %= 1000;
//	ITN[1] = InsideTempertureNum / 100;
//	InsideTempertureNum %= 100;
//	ITN[3] = 11;
//	ITN[2] = InsideTempertureNum / 10;
//	ITN[4] = InsideTempertureNum % 10;

	for(i = 0 ; i < 5 ; ++i)
	{
		AttitudeS[16 + i + i] = NumChar[YN[i]][0];
		AttitudeS[17 + i + i] = NumChar[YN[i]][1];
	}
	for (i = 0; i < 5; ++i)
	{
		AttitudeS[28 + i + i] = NumChar[PN[i]][0];
		AttitudeS[29 + i + i] = NumChar[PN[i]][1];
	}
	for (i = 0; i < 5; ++i)
	{
		AttitudeS[40 + i + i] = NumChar[PN[i]][0];
		AttitudeS[41 + i + i] = NumChar[PN[i]][1];
	}
	AttitudeS[50] = XorCaculate(AttitudeS, 50);
	HAL_UART_Transmit_DMA(DisUart, AttitudeS, 51);
}
//电量显示函数
void DisPower(UART_HandleTypeDef* DisUart, u8 PowerVolNum)
{
	u8 i;
	u8 PVN[4];
	PVN[0] = PowerVolNum / 100;
	PowerVolNum %= 100;
	PVN[1] = PowerVolNum / 10;
	PVN[2] = 10;
	PVN[3] = PowerVolNum % 10;
	for (i = 0; i < 4; ++i)
	{
		PowerS[14 + i + i] = NumChar[PVN[i]][0];
		PowerS[15 + i + i] = NumChar[PVN[i]][1];
	}
	PowerS[22] = XorCaculate(PowerS, 22);
	HAL_UART_Transmit_DMA(DisUart, PowerS, 23);
}
