#include "main.h"
#include "Display.h"
#include "Setting.h"
#include "BasicCtrl.h"
#include "cmsis_os.h"

typedef unsigned char u8;
typedef unsigned short u16;

static u8 String_WaterOn[2][27] = {
	{ 0x55, 0x07, 0x06, 0x01, 0x00, 0x55, 0x0A, 0x00, 0x00, 0x00, 0x55, 0x31, 0x06, 0x01, 0x00, 0xB2, 0xD5, 0xC4, 0xDA, 0xA3, 0xB1, 0xB5, 0xE3, 0xC2, 0xA9, 0xCB, 0xAE },
	{ 0x55, 0x07, 0x07, 0x01, 0x00, 0x55, 0x0A, 0x00, 0x00, 0x00, 0x55, 0x31, 0x06, 0x01, 0x00, 0xB2, 0xD5, 0xC4, 0xDA, 0xA3, 0xB2, 0xB5, 0xE3, 0xC2, 0xA9, 0xCB, 0xAE }
};
static u8 String_WaterOff[2][27] = {
	{ 0x55, 0x07, 0x05, 0x01, 0x00, 0x55, 0x0A, 0x00, 0x00, 0x00, 0x55, 0x31, 0x06, 0x01, 0x00, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1 },
	{ 0x55, 0x07, 0x06, 0x01, 0x00, 0x55, 0x0A, 0x00, 0x00, 0x00, 0x55, 0x31, 0x06, 0x01, 0x00, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1 }
};

static u8 Num[11][2] = 
{
	{ 0xa3, 0xb0 },
	{ 0xa3, 0xb1 },
	{ 0xa3, 0xb2 },
	{ 0xa3, 0xb3 },
	{ 0xa3, 0xb4 },
	{ 0xa3, 0xb5 },
	{ 0xa3, 0xb6 },
	{ 0xa3, 0xb7 },
	{ 0xa3, 0xb8 },
	{ 0xa3, 0xb9 },
	{ 0xa3, 0xae }
};
static void SendNum_QL504(UART_HandleTypeDef* DisUart, IWDG_HandleTypeDef IWD_Flash, u8 Row, u8 Column, u8 Number);



static void SendNum_QL504(UART_HandleTypeDef* DisUart, IWDG_HandleTypeDef IWD_Flash, u8 Row, u8 Column, u8 Number)
{
	static u8 SendTempBuffer[15] = { 0x55, 0x07, 0x00, 0x00, 0x00, 0x55, 0x0A, 0x00, 0x00, 0x00 ,0x55, 0x08, 0x00, 0x00, 0x00 };
	SendTempBuffer[2] = Row+1;
	SendTempBuffer[3] = Column;
	SendTempBuffer[12] = Num[Number][0];
	SendTempBuffer[13] = Num[Number][1];
	HAL_UART_Transmit_DMA(DisUart, SendTempBuffer, 15);
	HAL_IWDG_Refresh(&IWD_Flash);
	osDelay(15);
}

void DisWarning_QL504(UART_HandleTypeDef* DisUart, IWDG_HandleTypeDef IWD_Flash, u8 WarningPlace)
{
	switch (WarningPlace)
	{
	case 0:
		HAL_UART_Transmit_DMA(DisUart, String_WaterOn[0], 27);
		break;
	case 1:
		HAL_UART_Transmit_DMA(DisUart, String_WaterOn[1], 27);
		break;
	case 3:
		HAL_UART_Transmit_DMA(DisUart, String_WaterOff[0], 27);
		break;
	case 4:
		HAL_UART_Transmit_DMA(DisUart, String_WaterOff[1], 27);
		break;
	}
	HAL_IWDG_Refresh(&IWD_Flash);
	osDelay(50);
	
}

void DisData_QL504(UART_HandleTypeDef* DisUart, IWDG_HandleTypeDef IWD_Flash, u8* WaterDeepNum, u8* WaterTempertureNum, u16 YawNum, u16 PitchNum, u8 PowerVolNum)
{
	static int i = 0;
	static int j = 0;
	static int ScreenTime = 0;
	static u8 DisProcess = 0;
	
	u8 PVN[4];
	PVN[0] = PowerVolNum / 100;
	PowerVolNum %= 100;
	PVN[1] = PowerVolNum / 10;
	PVN[2] = 10;
	PVN[3] = PowerVolNum % 10;
	
	u8 YN[5];
	u8 PN[5];

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
	DisProcess = 1;
	while (DisProcess)
	{
		switch (ScreenTime)
		{
		case 0:
			if (j == 6)
			{
				i = 0;
				j = 0;
				ScreenTime = 1;
			}
			else
			{
				SendNum_QL504(DisUart,IWD_Flash, 1, 4 + j, WaterDeepNum[i]);
				++i;
				++j;
				osDelay(5);
			}
			break;
		case 1:
			if (j == 5)
			{
				i = 0;
				j = 0;
				ScreenTime = 2;
			}
			else
			{
				SendNum_QL504(DisUart, IWD_Flash, 2, 4 + j, WaterTempertureNum[i]);
				++i;
				++j;
				osDelay(5);
			}
			break;
		case 2:
			if (j == 5)
			{
				i = 0;
				j = 0;
				ScreenTime = 3;
			}
			else
			{
				SendNum_QL504(DisUart, IWD_Flash, 1, 16 + j, YN[i]);
				++i;
				++j;
				osDelay(5);
			}
			break;
		case 3:
			if (j == 5)
			{
				i = 0;
				j = 0;
				ScreenTime = 4;
			}
			else
			{
				SendNum_QL504(DisUart, IWD_Flash, 8, 1 + j, PN[i]);
				++j;
				++i;
				osDelay(5);
			}
			break;
		case 4:
			if (j == 4)
			{
				i = 0;
				j = 0;
				ScreenTime = 5;
			}
			else
			{
				SendNum_QL504(DisUart, IWD_Flash, 3, 4 + j, PVN[i]);
				++i;
				++j;
				osDelay(5);
			}
			break;
		case 5:
			ScreenTime = 0;
			DisProcess = 0;
			break;
		}
	}
	
}
