#ifndef _BASICCTRL_H
#define _BASICCTRL_H



#define RegFloating 5

void BasicPoint();
void BasicControl();
void PassBack(UART_HandleTypeDef* PassBackUart, unsigned char PowerNum, unsigned char WarnNum, unsigned short ANum[3], unsigned short WNum[3], unsigned short ENum[3], unsigned short HNum[3], unsigned short ITNum, unsigned short WTNum, unsigned short WDNum);
unsigned char XorCaculate(unsigned char* CacString, unsigned char CacStringSize);


#endif // !_BASICCTRL_H
