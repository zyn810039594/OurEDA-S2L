#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "stm32f4xx_hal.h"

//警告1位置
#define POSX_WARN1_HIGH 0x00
#define POSX_WARN1_LOW 0x00
#define POSY_WARN1_HIGH 0x00
#define POSY_WARN1_LOW 0x00
//警告2位置
#define POSX_WARN2_HIGH 0x00
#define POSX_WARN2_LOW 0x00
#define POSY_WARN2_HIGH 0x00
#define POSY_WARN2_LOW 0x00
//水深位置
#define POSX_WATERDEEP_HIGH 0x00
#define POSX_WATERDEEP_LOW 0x00
#define POSY_WATERDEEP_HIGH 0x00
#define POSY_WATERDEEP_LOW 0x00
//姿态位置
#define POSX_ATTITUDE_HIGH 0x00
#define POSX_ATTITUDE_LOW 0x00
#define POSY_ATTITUDE_HIGH 0x00
#define POSY_ATTITUDE_LOW 0x00
//电池电压位置
#define POSX_POWER_HIGH 0x00
#define POSX_POWER_LOW 0x00
#define POSY_POWER_HIGH 0x00
#define POSY_POWER_LOW 0x00

//函数定义
void DisWarning(UART_HandleTypeDef* DisUart, unsigned char WarningPlace);
void DisWaterDeep(UART_HandleTypeDef* DisUart, unsigned char* WaterDeepNum, unsigned char* WaterTempertureNum);
void DisAttitude(UART_HandleTypeDef* DisUart, unsigned short YawNum, unsigned short PitchNum, unsigned short RollNum, unsigned short InsideTempertureNum);
void DisPower(UART_HandleTypeDef* DisUart, unsigned char PowerVolNum);

void DisWarning_QL504(UART_HandleTypeDef* DisUart, IWDG_HandleTypeDef IWD_Flash,unsigned char WarningPlace);
void DisData_QL504(UART_HandleTypeDef* DisUart, IWDG_HandleTypeDef IWD_Flash, unsigned char* WaterDeepNum, unsigned char* WaterTempertureNum, unsigned short YawNum, unsigned short PitchNum, unsigned char PowerVolNum);


#endif // !_DISPLAY_H
