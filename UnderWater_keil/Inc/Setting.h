#ifndef _SETTING_H
#define _SETTING_H

//功能开启标志位
//对电池电量的检测
#define SWPowerScanner 0
//对姿态传感器的检测
#define SWAttitudeSensor 0
//视频叠加字符输出
#define SWDisplayWords 0
//多组装形式的支持
#define SWTransMode 0
//姿态控制的支持
#define SWAttitudeControl 1
//定向定深等自动运动的支持
#define SWAutoMove 0
//数据是否回传至上位机
#define SWPassback 0
//水深传感器的读取
#define SWWaterDeep 0

#endif // !_SETTING_H
