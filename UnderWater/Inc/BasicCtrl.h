#ifndef _BASICCTRL_H
#define _BASICCTRL_H

#include "stm32f4xx_hal.h"

#define RegFloating 60

void BasicPoint(unsigned char* SerialData);
void BasicControl(volatile uint32_t* MoveThruster[8], volatile uint32_t* ClipSteering, volatile uint32_t* PTZSteering, volatile uint32_t* FlipSteering, volatile uint32_t* LightSteering);
void PassBack(UART_HandleTypeDef* PassBackUart, unsigned char PowerNum, unsigned char WarnNum, unsigned short ANum[3], unsigned short WNum[3], unsigned short ENum[3], unsigned short HNum[3], unsigned short ITNum, unsigned short WTNum, unsigned short WDNum);
unsigned char XorCaculate(unsigned char* CacString, unsigned char CacStringSize);


#endif // !_BASICCTRL_H
