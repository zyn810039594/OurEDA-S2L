#ifndef _ATTITUDE_H
#define _ATTITUDE_H

void NormalMode(unsigned char Trans);
void PitchMode(unsigned char Trans);
void RowMode(unsigned char Trans);
void MixMode(unsigned char Trans);

void DeepAnalyze(unsigned char* DataPosition, unsigned short* PDepth, unsigned short*PWaterTemperture, unsigned char* PDN, unsigned char* PWTN);
void AttitudeAnalyze(unsigned short* PRollL, unsigned short* AttitudeNum);


#endif // !_ATTITUDE_H
