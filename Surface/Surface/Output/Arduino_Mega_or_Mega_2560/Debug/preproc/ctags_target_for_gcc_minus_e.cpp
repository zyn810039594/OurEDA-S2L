# 1 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
# 2 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino" 2
//方便使用的定义
typedef unsigned char u8;
typedef unsigned short u16;
//模拟口数据采集定义
# 14 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
//数字口数据采集定义
# 32 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
//串口波特率


//抓手刷新频率 单位Hz


//自动模式数据调节
//翻转初始角度

//翻转角度

//翻转持续时间 单位毫秒

//夹取初始/释放角度

//抓取角度

//夹取持续时间 单位毫秒

//夹取释放时间 单位毫秒


//PWM输出范围 若想反向直接把高低换个个儿
//前进后退与侧推
//前进后退


//旋转


//上下


//横滚


//舵机范围
//夹取舵机


//翻转舵机


//云台


//灯光



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
const u16 Latency = 1000 / 5;
const u16 DoClipTime = 1000 / Latency;
const u16 DoFlipTime = 1000 / Latency;
const u16 DoDisClipTime = 1000 / Latency;

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

//定时器中断
void TimerInterrupt()
{
 if (LateBuf==3)
 {
  Serial.write(SendString, 23);
  LateBuf = 0;
 }
 //自动夹取代码
 switch(CFMode)
 {
 case 2:
  *PClip = (u16)map(*PClip, 0, 1024, 500, 2500);
  switch (CFStart)
  {
  case 0:
   *PFlip = 1500;
   break;
  case 1:
   *PFlip = 1500;
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
   *PClip = 1500;
   *PFlip = 1500;
   break;
  case 1:
   *PClip = 1500;
   *PFlip = 1500;
   ++FClipTime;
   if (FClipTime == DoClipTime)
   {
    FClipTime = 0;
    CFStart = 2;
   }
   break;
  case 2:
   *PClip = 1500;
   *PFlip = 1500;
   ++FFlipTime;
   if (FFlipTime == DoFlipTime)
   {
    FFlipTime = 0;
    CFStart = 3;
   }
   break;
  case 3:
   *PClip = 1500;
   *PFlip = 1500;
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
 pinMode(30, 0x0);
 pinMode(31, 0x0);
 pinMode(32, 0x0);
 pinMode(33, 0x0);
 pinMode(34, 0x0);
 pinMode(35, 0x0);
 pinMode(36, 0x0);
 pinMode(37, 0x0);
 pinMode(38, 0x0);
 pinMode(39, 0x0);
 pinMode(40, 0x0);
 pinMode(41, 0x0);
 pinMode(42, 0x0);
 pinMode(43, 0x0);
 pinMode(44, 0x0);
 pinMode(45, 0x0);
 Serial.begin(115200);
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
 LateBuf = 0;
 //模拟口读取
 *PPortrait = (u16)analogRead(A0);
 *PRotate = (u16)analogRead(A1);
 *PVertical = (u16)analogRead(A2);
 *PROver = (u16)analogRead(A3);
 *PLight = (u16)analogRead(A4);
 *PPTZ = (u16)analogRead(A5);

 //数字口读取
 *PTransverse = (u8)digitalRead(30);
 *POrient = (u8)digitalRead(32);
 *PDeepkeeping = (u8)digitalRead(33);
 FLight = digitalRead(35);
 FGesture = digitalRead(31);
 FPitch = digitalRead(37);
 FRoll = digitalRead(38);
 FPRMixed = digitalRead(39);
 FSpeedMode = digitalRead(40);
 FStandardMode = digitalRead(41);
 FMaxSpeedMode = digitalRead(42);
 FHandClip = digitalRead(43);
 FSemiautoClip = digitalRead(44);
 FAutoClip = digitalRead(45);
 CFMode = FHandClip + FSemiautoClip * 2 + FAutoClip * 3;

 //简单处理
 *PLight = (u16)map(((*PLight)*FLight), 0, 1024, 0, 4999);
 *PGesture = (u8)(FGesture*(FPitch + 2*FRoll + 3*FPRMixed));
 *PPortrait = (u16)map(*PPortrait, 0, 1024, 1075, 1925);
 *PRotate = (u16)map(*PRotate, 0, 1024, 1925, 1075);
 *PVertical = (u16)map(*PVertical, 0, 1024, 1925, 1075);
 *PROver = (u16)map(*PROver, 0, 1024, 1925, 1075);
 *PPTZ = (u16)map(*PPTZ, 0, 1024, 500, 2500);
 switch (CFMode)
 {
 case 1:
  *PClip = (u16)analogRead(A6);
  *PFlip = (u16)analogRead(A7);
  *PClip = (u16)map(*PClip, 0, 1024, 500, 2500);
  *PFlip = (u16)map(*PFlip, 0, 1024, 500, 2500);
  break;
 case 2:
  *PClip = (u16)analogRead(A6);
  *PClip = (u16)map(*PClip, 0, 1024, 500, 2500);
  if (!CFStart)
  {
   CFStart = digitalRead(34);
   *PFlip = (u16)analogRead(A7);
   *PFlip = (u16)map(*PFlip, 0, 1024, 500, 2500);
  }
  break;
 case 3:
  if (!CFStart)
  {
   *PClip = (u16)analogRead(A6);
   *PFlip = (u16)analogRead(A7);
   CFStart = digitalRead(34);
   *PClip = (u16)map(*PClip, 0, 1024, 500, 2500);
   *PFlip = (u16)map(*PFlip, 0, 1024, 500, 2500);
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
  Serial.read();
  ++CheckRec;
  if (CheckRec==37)
  {
   CheckRec = 0;
   Serial.write(SendString, 23);
   InfTake();
  }
 }
 else
 {
  InfTake();
 }

}
