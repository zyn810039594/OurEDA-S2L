#ifndef _SETTING_H
#define _SETTING_H



//功能开启标志位
//对电池电量的检测
#define SWPowerScanner
//对姿态传感器的检测
#define SWAttitudeSensor
//视频叠加字符输出
#define SWDisplayWords
//多组装形式的支持
#define SWTransMode
//姿态控制的支持
#define SWAttitudeControl
//定向定深等自动运动的支持
//#define SWAutoMove
//数据是否回传至上位机
#define SWPassback
//水深传感器的读取
#define SWWaterDeep
//超过20s没指令后的动作
//舵机范围
#define PTZHighMove 1250
#define PTZLowMove 1750
//灯光动作
#define LightHighMove 4999
#define LightLowMove 0

//叠加板的选择
//#define HardWare_QL800
#define HardWare_QL504

#endif // !_SETTING_H
