# 1 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
# 2 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino" 2
//方便使用的定义


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
unsigned char* SendString = 0;

unsigned short* PPortrait = 0;
unsigned short* PRotate = 0;
unsigned short* PVertical = 0;
unsigned short* PROver = 0;
unsigned short* PLight = 0;
unsigned short* PPTZ = 0;
unsigned short* PClip = 0;
unsigned short* PFlip = 0;

unsigned char* PTransverse = 0;
unsigned char* POrient = 0;
unsigned char* PDeepkeeping = 0;
unsigned char* PMode = 0;
unsigned char* PGesture = 0;

//延时计算
const unsigned short Latency = 1000 / 10;
const unsigned short DoClipTime = 1000 / Latency;
const unsigned short DoFlipTime = 1000 / Latency;
const unsigned short DoDisClipTime = 1000 / Latency;

//灯光开关标志位
unsigned char FLight = 0;
//姿态控制标志位
unsigned char FGesture = 0;
unsigned char FPitch = 0;
unsigned char FRoll = 0;
unsigned char FPRMixed = 0;
//推进器标志位
unsigned char FSpeedMode = 0;
unsigned char FStandardMode = 0;
unsigned char FMaxSpeedMode = 0;
//自动抓取标志位
unsigned char CFStart = 0;
unsigned char CFMode = 0;
unsigned char FHandClip = 0;
unsigned char FSemiautoClip = 0;
unsigned char FAutoClip = 0;
unsigned short FClipTime = 0;
unsigned short FFlipTime = 0;

//回传判定标志位
unsigned char CheckRec = 0;
//回传缓存位
unsigned char RecBuf = 0;

//定时器中断
void TimerInterrupt()
{
 //自动夹取代码
 switch(CFMode)
 {
 case 2:
  *PClip = (unsigned short)map(*PClip, 0, 1024, 500, 2500);
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
 SendString = (unsigned char*)malloc(23);
 //指针定向
 PPortrait = (unsigned short*)(SendString + 1);
 PRotate = (unsigned short*)(SendString + 3);
 PVertical = (unsigned short*)(SendString + 5);
 PROver = (unsigned short*)(SendString + 7);
 PLight = (unsigned short*)(SendString + 9);
 PPTZ = (unsigned short*)(SendString + 11);
 PClip = (unsigned short*)(SendString + 13);
 PFlip = (unsigned short*)(SendString + 15);
 PTransverse = SendString + 17;
 POrient = SendString + 18;
 PDeepkeeping = SendString + 19;
 PMode = SendString + 20;
 PGesture = SendString + 21;
}
//读取函数
void InfTake()
{
 //模拟口读取
 *PPortrait = (unsigned short)analogRead(A0);
 *PRotate = (unsigned short)analogRead(A1);
 *PVertical = (unsigned short)analogRead(A2);
 *PROver = (unsigned short)analogRead(A3);
 *PLight = (unsigned short)analogRead(A4);
 *PPTZ = (unsigned short)analogRead(A5);

 //数字口读取
 *PTransverse = (unsigned char)digitalRead(30);
 *POrient = (unsigned char)digitalRead(32);
 *PDeepkeeping = (unsigned char)digitalRead(33);
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
 *PLight = (unsigned short)map(((*PLight)*FLight), 0, 1024, 0, 4999);
 *PGesture = (unsigned char)(FGesture*(FPitch + 2*FRoll + 3*FPRMixed));
 *PPortrait = (unsigned short)map(*PPortrait, 0, 1024, 1075, 1925);
 *PRotate = (unsigned short)map(*PRotate, 0, 1024, 1075, 1925);
 *PVertical = (unsigned short)map(*PVertical, 0, 1024, 1075, 1925);
 *PROver = (unsigned short)map(*PROver, 0, 1024, 1925, 1075);
 *PPTZ = (unsigned short)map(*PPTZ, 0, 1024, 500, 2500);
 switch (CFMode)
 {
 case 1:
  *PClip = (unsigned short)analogRead(A6);
  *PFlip = (unsigned short)analogRead(A7);
  *PClip = (unsigned short)map(*PClip, 0, 1024, 500, 2500);
  *PFlip = (unsigned short)map(*PFlip, 0, 1024, 500, 2500);
  break;
 case 2:
  *PClip = (unsigned short)analogRead(A6);
  *PClip = (unsigned short)map(*PClip, 0, 1024, 500, 2500);
  if (!CFStart)
  {
   CFStart = digitalRead(34);
   *PFlip = (unsigned short)analogRead(A7);
   *PFlip = (unsigned short)map(*PFlip, 0, 1024, 500, 2500);
  }
  break;
 case 3:
  if (!CFStart)
  {
   *PClip = (unsigned short)analogRead(A6);
   *PFlip = (unsigned short)analogRead(A7);
   CFStart = digitalRead(34);
   *PClip = (unsigned short)map(*PClip, 0, 1024, 500, 2500);
   *PFlip = (unsigned short)map(*PFlip, 0, 1024, 500, 2500);
  }
  break;
 }
}

void setup()
{
 // put your setup code here, to run once:
 IOInit();
 delay(4000);
 //定时器中断设置
 FlexiTimer2::set(Latency, TimerInterrupt);
 FlexiTimer2::start();
}

void loop()
{
 // put your main code here, to run repeatedly:
 if(Serial.available() > 0)
 {
  RecBuf = Serial.read();
  if (RecBuf == 0xFF)
  {
   ++CheckRec;
   if (CheckRec == 2)
   {
    CheckRec = 0;
    Serial.write(SendString, 23);
   }
  }
 }
 else
 {
  InfTake();
 }

}
