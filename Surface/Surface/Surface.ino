#include <FlexiTimer2.h>
//方便使用的定义
#define u8 unsigned char
#define u16 unsigned short
//模拟口数据采集定义
#define Portrait A0
#define Rotate A1
#define Vertical A2
#define AUndefined1 A3
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

//发送频率 单位Hz
const u16 SendFreq=25;

//自动模式数据调节
//翻转初始角度 0~1023
const u16 InitFlipAngle=512;
//翻转角度 0~1023
const u16 FlipAngle=512;
//翻转持续时间 单位毫秒
const u16 FlipTime=1000;
//夹取初始角度 0~1023
const u16 InitClipAngle=512;
//抓取角度 0~1023
const u16 ClipAngle=512;
//夹取持续时间 单位毫秒
const u16 ClipTime=1000;

//前进后退与侧推反向
//前进后退反向 0正向 1反向
const u8 PortraitSide=0;
//旋转反向 0正向 1反向
const u8 RotateSide=0;
//上下反向 0正向 1反向
const u8 VerticalSide=0;

//舵机反向
//夹取舵机反向 0正向 1反向
const u8 ClipSide=0;
//翻转舵机反向 0正向 1反向
const u8 FlipSide=0;
//云台反向 0正向 1反向
const u8 PTZSide=0;

//以下为代码部分

//全局变量定义
//字符串指针
u8* SendString=0;

u16* PPortrait=0;
u16* PRotate=0;
u16* PVertical=0;
u16* PAUndefined1=0;
u16* PLight=0;
u16* PPTZ=0;
u16* PClip=0;
u16* PFlip=0;

u8* PTransverse=0;
u8* POrient=0;
u8* PDeepkeeping=0;
u8* PMode=0;
u8* PGesture=0;

//延时计算
const u16 Latency=1000/SendFreq;
const u16 DoClipTime=Cliptime/Latency;
const u16 DoFlipTime=Fliptime/Latency;

//灯光开关标志位
u8 FLight=0;
//姿态控制标志位
u8 FGesture=0;
u8 FPitch=0;
u8 FRoll=0;
u8 FPRMixed=0;
//推进器标志位
u8 FSpeedMode=0;
u8 FStandardMode=0;
u8 FMaxSpeedMode=0;
//自动抓取标志位
u8 CFStart=0;
u8 CFMode=0;
u8 FHandClip=0;
u8 FSemiautoClip=0;
u8 FAutoClip=0;
u8 FClipTime=0;
u8 FFlipTime=0;

//定时器中断
void TimerInterrupt()
{
  //简单处理
  CFMode=CFStart*(FHandClip+FSemiautoClip*2+FAutoClip*3);
  *PLight=(u16)((*PLight)*Flight);
  *PGesture=(u8)(FGesture*(FPitch+2*FRoll+3*FPRMixed));
  *PPortrait=(u16)*PPortrait+PortraitSide*(1024-2*(*PPortrait));
  *PRotate=(u16)*PRotate+RotateSide*(1024-2*(*PRotate));
  *PVertical=(u16)*PVertical+VerticalSide*(1024-2*(*PVertical));
  *PPTZ=(u16)*PPTZ+PTZSide*(1024-2*(*PPTZ)));
  
  switch(CFMode)
  {
    case 0:
    *PClip=(u16)*PClip+ClipSide*(1024-(*PClip));
    *PFlip=(u16)*PFlip+FlipSide*(1024-2*(*PFlip));
    break;
    case 1:
    if(CFStart)
    {
      
    }
    break;
    
  }
}

void setup() 
{
  // put your setup code here, to run once:
  //打开输入GPIO
  pinMode(Transverse,INPUT);
  pinMode(Gesture,INPUT);
  pinMode(Orient,INPUT);
  pinMode(Deepkeeping,INPUT);
  pinMode(AutoclipSwitcher,INPUT);
  pinMode(LightSwicher,INPUT);
  pinMode(DUndefined1,INPUT);
  pinMode(Pitch,INPUT);
  pinMode(Roll,INPUT);
  pinMode(PRMixed,INPUT);
  pinMode(SpeedMode,INPUT);
  pinMode(StandardMode,INPUT);
  pinMode(MaxSpeedMode,INPUT);
  pinMode(HandClip,INPUT);
  pinMode(SemiautoClip.INPUT);
  pinMode(AutoClip,INPUT);
  Serial.begin(SerialBaud);
  //开辟字符串用内存空间
  SendString=(u8*)malloc(21);
  //指针定向
  PPortrait=(u16*)(SendString+1);
  PRotate=(u16*)(SendString+3);
  PVertical=(u16*)(SendString+5);
  //PAUndefined1=0;
  PLight=(u16*)(SendString+7);
  PPTZ=(u16*)(SendString+9);
  PClip=(u16*)(SendString+11);
  PFlip=(u16*)(SendString+13);
  PTransverse=SendString+15;
  POrient=SendString+16;
  PDeepkeeping=SendString+17;
  PMode=SendString+18;
  PGesture=SendString+19;
  delay(4000);
  //机器人初始化指令发送
  SendString[0]=0x25;
  for(int i=1;i<20;++i)
  {
    SendString[i]=0xFF;
  }
  SendString[20]=0x21;
  Serial.write(SendString,21);
  //开启定时器中断，开始运行
  FlexiTimer2::set(Latency, TimerInterrupt);
  FlexiTimer2::start();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //模拟口读取
  *PPortrait=(u16)analogRead(Portrait);
  *PRotate=(u16)analogRead(Rotate);
  *PVertical=(u16)analogRead(Vertical);
  //*PAUndefined1=(u16)analogRead(AUndefined1);
  *PLight=(u16)analogRead(Light);
  *PPTZ=(u16)analogRead(PTZ);
  *PClip=(u16)analogRead(Clip);
  *PFlip=(u16)analogRead(Flip);
  //数字口读取
  *PTransverse=(u8)digitalRead(Transverse);
  *POrient=(u8)digitalRead(Orient);
  *PDeepkeeping=(u8)digitalRead(Deepkeeping);
  FLight=digitalRead(LightSwitcher);
  FGesture=digitalRead(Gesture);
  FPitch=digitalRead(Pitch);
  FRoll=digitalRead(Roll)
  FPRMixed=digitalRead(PRMixed);
  FSpeedMode=digitalRead(SpeedMode);
  FStandardMode=digitalRead(StandardMode);
  FMaxSpeedMode=digitalRead(MaxSpeedMode);
  FHandClip=digitalRead(HandClip);
  FSemiautoClip=digitalRead(SemiautoClip);
  FAutoClip=digitalRead(AutoClip);
  if(CFStart==0)
  {
    CFStart=digitalRead(AutoclipSwitcher);
  }
}
