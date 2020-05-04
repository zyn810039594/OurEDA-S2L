#ifndef _ATTITUDE_H
#define _ATTITUDE_H

#include "stm32f4xx_hal.h"

void NormalMode(unsigned char Trans, volatile uint32_t* MoveThruster[8], unsigned short RotateNum, unsigned short ROverNum);
void PitchMode(unsigned char Trans, volatile uint32_t* MoveThruster[8], unsigned short RotateNum, unsigned short ROverNum);
void RowMode(unsigned char Trans, volatile uint32_t* MoveThruster[8], unsigned short RotateNum, unsigned short ROverNum);
void MixMode(unsigned char Trans, volatile uint32_t* MoveThruster[8], unsigned short RotateNum, unsigned short ROverNum);

void DeepAnalyze(unsigned char* DataPosition, unsigned short* PDepth, unsigned short *PWaterTemperture, unsigned char* PDN, unsigned char* PWTN);
void AttitudePoint(unsigned char* SerialData);
void AttitudeTake(unsigned short* AttitudeNum, unsigned short* AccelerationNum, unsigned short* RotSpeedNum, unsigned short* EulerAngleNum, unsigned short* MagnetismNum, unsigned short* InsideTempertureNum);



#endif // !_ATTITUDE_H
