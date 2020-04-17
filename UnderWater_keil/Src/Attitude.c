#include "main.h"
#include "cmsis_os.h"
#include "Attitude.h"
#include "Setting.h"

typedef unsigned char u8;
typedef unsigned short u16;

static volatile uint32_t *VerticalCCR[4] = { &(TIM2->CCR1), &(TIM2->CCR2), &(TIM2->CCR3), &(TIM2->CCR4) };


extern u16* PRotate;

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
void RowMode(u8 Trans)
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