#include <FlexiTimer2.h>

//如要开启串口转发程序请取消注释掉下面的define,默认串口2
//#define SerialTransmit

#ifdef SerialTransmit

//转发串口波特率
#define TransSerialBaud 115200

#endif // SerialTransmit


//方便使用的定义
typedef unsigned char u8;
typedef unsigned short u16;
//模拟口数据采集定义
#define Portrait A0
#define Rotate A1
#define Vertical A2
#define ROver A3
#define Light A4
#define PTZ A5
#define Clip A6
#define Flip A7
//数字口数据采集定义
#define Transverse 30
#define Gesture 31
#define Orient 32
#define Deepkeeping 33
#define AutoclipSwitcher 34
#define LightSwitcher 35
#define DUndefined1 36
#define Pitch 37
#define Roll 38
#define PRMixed 39
#define SpeedMode 40
#define StandardMode 41
#define MaxSpeedMode 42
#define HandClip 43
#define SemiautoClip 44
#define AutoClip 45

//串口波特率
#define SerialBaud 115200

//抓手刷新频率 单位Hz
#define SendFreq 5

//自动模式数据调节
//翻转初始角度
#define InitFlipAngle 1500
//翻转角度
#define FlipAngle 1500
//翻转持续时间 单位毫秒
#define FlipTime 1000
//夹取初始/释放角度
#define InitClipAngle 1500
//抓取角度
#define ClipAngle 1500
//夹取持续时间 单位毫秒
#define ClipTime 1000
//夹取释放时间 单位毫秒
#define DisClipTime 1000

//PWM输出范围 若想反向直接把高低换个个儿
//前进后退与侧推
//前进后退
#define PortraitHigh 1925
#define PortraitLow 1075
//旋转
#define RotateHigh 1075
#define RotateLow 1925
//上下
#define VerticalHigh 1075
#define VerticalLow 1925
//横滚
#define ROverHigh 1925
#define ROverLow 1075
//舵机范围
//夹取舵机
#define ClipHigh 2500
#define ClipLow 500
//翻转舵机
#define FlipHigh 2500
#define FlipLow 500
//云台
#define PTZHigh 2500
#define PTZLow 500
//灯光
#define LightHigh 4999
#define LightLow 0

//以下为代码部分

//全局变量定义
//字符串指针
u8* SendString = 0;

u16* PPortrait = 0;
u16* PRotate = 0;
u16* PVertical = 0;
u16* PROver = 0;
u16* PLight = 0;
u16* PPTZ = 0;
u16* PClip = 0;
u16* PFlip = 0;

u8* PTransverse = 0;
u8* POrient = 0;
u8* PDeepkeeping = 0;
u8* PMode = 0;
u8* PGesture = 0;

//延时计算
const u16 Latency = 1000 / SendFreq;
const u16 DoClipTime = ClipTime / Latency;
const u16 DoFlipTime = FlipTime / Latency;
const u16 DoDisClipTime = DisClipTime / Latency;

//灯光开关标志位
u8 FLight = 0;
//姿态控制标志位
u8 FGesture = 0;
u8 FPitch = 0;
u8 FRoll = 0;
u8 FPRMixed = 0;
//推进器标志位
u8 FSpeedMode = 0;
u8 FStandardMode = 0;
u8 FMaxSpeedMode = 0;
//自动抓取标志位
u8 CFStart = 0;
u8 CFMode = 0;
u8 FHandClip = 0;
u8 FSemiautoClip = 0;
u8 FAutoClip = 0;
u16 FClipTime = 0;
u16 FFlipTime = 0;

//回传判定标志位
u8 CheckRec = 0;
//回传缓存位
u8 RecBuf = 0;

//回传延时标志位
u8 LateBuf = 0;

//机器人回传的数据
u8 Serial2Data[38] = { 0 };

//定时器中断
void TimerInterrupt()
{
	if (LateBuf > 4)
	{
		Serial.flush();
		delay(1);
		Serial.write(SendString, 23);
		Serial.flush();
		LateBuf = 0;
	}
	//自动夹取代码
	switch(CFMode)
	{
	case 2:
		*PClip = (u16)map(*PClip, 0, 1024, ClipLow, ClipHigh);
		switch (CFStart)
		{
		case 0:
			*PFlip = InitFlipAngle;
			break;
		case 1:
			*PFlip = FlipAngle;
			++FFlipTime;
			if (FFlipTime == DoFlipTime)
			{
				FFlipTime = 0;
				CFStart = 0;
			}
			break;
		}
		break;
	case 3:
		switch (CFStart)
		{
		case 0:
			*PClip = InitClipAngle;
			*PFlip = InitFlipAngle;
			break;
		case 1:
			*PClip = ClipAngle;
			*PFlip = InitFlipAngle;
			++FClipTime;
			if (FClipTime == DoClipTime)
			{
				FClipTime = 0;
				CFStart = 2;
			}
			break;
		case 2:
			*PClip = ClipAngle;
			*PFlip = FlipAngle;
			++FFlipTime;
			if (FFlipTime == DoFlipTime)
			{
				FFlipTime = 0;
				CFStart = 3;
			}
			break;
		case 3:
			*PClip = InitClipAngle;
			*PFlip = FlipAngle;
			++FClipTime;
			if (FClipTime == DoDisClipTime)
			{
				FClipTime = 0;
				CFStart = 0;
			}
			break;
		}
		break;
	default:
		CFStart = 0;
		break;
	}
	++LateBuf;
}
//IO初始化与指针的定向处理
void IOInit()
{
	//打开输入GPIO
	pinMode(Transverse, INPUT);
	pinMode(Gesture, INPUT);
	pinMode(Orient, INPUT);
	pinMode(Deepkeeping, INPUT);
	pinMode(AutoclipSwitcher, INPUT);
	pinMode(LightSwitcher, INPUT);
	pinMode(DUndefined1, INPUT);
	pinMode(Pitch, INPUT);
	pinMode(Roll, INPUT);
	pinMode(PRMixed, INPUT);
	pinMode(SpeedMode, INPUT);
	pinMode(StandardMode, INPUT);
	pinMode(MaxSpeedMode, INPUT);
	pinMode(HandClip, INPUT);
	pinMode(SemiautoClip, INPUT);
	pinMode(AutoClip, INPUT);
	Serial.begin(SerialBaud);
#ifdef SerialTransmit
	Serial2.begin(TransSerialBaud);
#endif // SerialTransmit

	//开辟字符串用内存空间
	SendString = (u8*)malloc(23);
	//指针定向
	SendString[0] = 0x25;
	SendString[22] = 0x21;
	PPortrait = (u16*)(SendString + 1);
	PRotate = (u16*)(SendString + 3);
	PVertical = (u16*)(SendString + 5);
	PROver = (u16*)(SendString + 7); 
	PLight = (u16*)(SendString + 9);
	PPTZ = (u16*)(SendString + 11);
	PClip = (u16*)(SendString + 13);
	PFlip = (u16*)(SendString + 15);
	PTransverse = SendString + 17;
	POrient = SendString + 18;
	PDeepkeeping = SendString + 19;
	PMode = SendString + 20;
	PGesture = SendString + 21;
	//初始化代码发送
	SendString[0] = 0x25;
	SendString[22] = 0x21;
	*PPortrait = (u16)1500;
	*PRotate = (u16)1500;
	*PVertical = (u16)1500;
	*PROver = (u16)1500;
	*PLight = (u16)0;
	*PPTZ = (u16)1500;
	*PClip = (u16)1500;
	*PFlip = (u16)1500;
	*PTransverse = 0;
	*POrient = 0;
	*PDeepkeeping = 0;
	*PMode = 1;
	*PGesture = 0;
	Serial.write(SendString, 23);
}
//读取函数
void InfTake()
{
	//模拟口读取
	u16 tPortrait = (u16)analogRead(Portrait);
	u16 tRotate = (u16)analogRead(Rotate);
	u16 tVertical = (u16)analogRead(Vertical);
	u16 tROver = (u16)analogRead(ROver);
	u16 tLight = (u16)analogRead(Light);
	u16 tPTZ = (u16)analogRead(PTZ);
	
	//数字口读取
	*PTransverse = (u8)digitalRead(Transverse);
	*POrient = (u8)digitalRead(Orient);
	*PDeepkeeping = (u8)digitalRead(Deepkeeping);
	FLight = digitalRead(LightSwitcher);
	FGesture = digitalRead(Gesture);
	FPitch = digitalRead(Pitch);
	FRoll = digitalRead(Roll);
	FPRMixed = digitalRead(PRMixed);
	FSpeedMode = digitalRead(SpeedMode);
	FStandardMode = digitalRead(StandardMode);
	FMaxSpeedMode = digitalRead(MaxSpeedMode);
	FHandClip = digitalRead(HandClip);
	FSemiautoClip = digitalRead(SemiautoClip);
	FAutoClip = digitalRead(AutoClip);
	CFMode = FHandClip + FSemiautoClip * 2 + FAutoClip * 3;
	
	//简单处理
	*PLight = (u16)map(((tLight)*FLight), 0, 1024, LightLow, LightHigh);
	*PGesture = (u8)(FGesture*(FPitch + 2*FRoll + 3*FPRMixed));
	*PPortrait = (u16)map(tPortrait, 0, 1024, PortraitLow, PortraitHigh);
	*PRotate = (u16)map(tRotate, 0, 1024, RotateLow, RotateHigh);
	*PVertical = (u16)map(tVertical, 0, 1024, VerticalLow, VerticalHigh);
	*PROver = (u16)map(tROver, 0, 1024, ROverHigh, ROverLow);
	*PPTZ = (u16)map(tPTZ, 0, 1024, PTZLow, PTZHigh);
	u16 tClip = 0;
	u16 tFlip = 0;
	switch (CFMode)
	{
	case 1:
		tClip = (u16)analogRead(Clip);
		tFlip = (u16)analogRead(Flip);
		*PClip = (u16)map(tClip, 0, 1024, ClipLow, ClipHigh);
		*PFlip = (u16)map(tFlip, 0, 1024, FlipLow, FlipHigh);
		break;
	case 2:
		tClip = (u16)analogRead(Clip);
		*PClip = (u16)map(tClip, 0, 1024, ClipLow, ClipHigh);
		if (!CFStart)
		{
			CFStart = digitalRead(AutoclipSwitcher);
			tFlip = (u16)analogRead(Flip);
			*PFlip = (u16)map(tFlip, 0, 1024, FlipLow, FlipHigh);
		}
		break;
	case 3:
		if (!CFStart)
		{
			tClip = (u16)analogRead(Clip);
			tFlip = (u16)analogRead(Flip);
			CFStart = digitalRead(AutoclipSwitcher);
			*PClip = (u16)map(tClip, 0, 1024, ClipLow, ClipHigh);
			*PFlip = (u16)map(tFlip, 0, 1024, FlipLow, FlipHigh);
		}
		break;
	}
}

void setup() 
{
	// put your setup code here, to run once:
	IOInit();
	delay(1000);
	InfTake();
	Serial.write(SendString, 23);
	//定时器中断设置
	FlexiTimer2::set(Latency, TimerInterrupt);
	FlexiTimer2::start();
}

void loop() 
{
	// put your main code here, to run repeatedly:
		if(Serial.available() > 0)
	{
#ifdef SerialTransmit
		Serial2Data = Serial.read();
#else
		Serial.read();
#endif // SerialTransmit
		++CheckRec;
		if (CheckRec == 37)
		{
			CheckRec = 0;
			LateBuf = 0;
			Serial.flush();
#ifdef SerialTransmit
			Serial2.flush();
#endif // SerialTransmit
			delay(1);
			Serial.write(SendString, 23);
#ifndef SerialTransmit
			Serial.flush();
#else
			Serial2.write(Send2Data, 37);
			Serial2.flush();
#endif // SerialTransmit
			InfTake();
		}
	}
	else
	{
		InfTake();
	}
	
}
