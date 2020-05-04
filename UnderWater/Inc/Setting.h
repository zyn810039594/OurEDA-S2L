#ifndef _SETTING_H
#define _SETTING_H



//功能开启标志位
//对电池电量的检测
#define SWPowerScanner 1
//对姿态传感器的检测
#define SWAttitudeSensor 1
//视频叠加字符输出
#define SWDisplayWords 1
//多组装形式的支持
#define SWTransMode 0
//姿态控制的支持
#define SWAttitudeControl 1
//定向定深等自动运动的支持
#define SWAutoMove 0
//数据是否回传至上位机
#define SWPassback 1
//水深传感器的读取
#define SWWaterDeep 0
//超过20s没指令后的动作
//舵机范围
#define PTZHighMove 1250
#define PTZLowMove 1750
//灯光动作
#define LightHighMove 4999
#define LightLowMove 0

#endif // !_SETTING_H
