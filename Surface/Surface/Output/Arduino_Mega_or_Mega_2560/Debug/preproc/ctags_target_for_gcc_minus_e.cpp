# 1 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
# 1 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
# 2 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino" 2
//方便使用的定义


//模拟口数据采集定义
# 14 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
//数字口数据采集定义
# 32 "F:\\Emb\\OurEDA-S2L\\Surface\\Surface\\sketches\\Surface.ino"
//串口波特率


//发送频率 单位Hz
const unsigned short SendFreq = 25;

//自动模式数据调节
//翻转初始角度 0~1023
const unsigned short InitFlipAngle = 512;
//翻转角度 0~1023
const unsigned short FlipAngle = 512;
//翻转持续时间 单位毫秒
const unsigned short FlipTime = 1000;
//夹取初始/释放角度 0~1023
const unsigned short InitClipAngle = 512;
//抓取角度 0~1023
const unsigned short ClipAngle = 512;
//夹取持续时间 单位毫秒
const unsigned short ClipTime = 1000;
//夹取释放时间 单位毫秒
const unsigned short DisClipTime = 1000;

//前进后退与侧推反向
//前进后退反向 0正向 1反向
const unsigned char PortraitSide = 0;
//旋转反向 0正向 1反向
const unsigned char RotateSide = 0;
//上下反向 0正向 1反向
const unsigned char VerticalSide = 0;

//舵机反向
//夹取舵机反向 0正向 1反向
const unsigned char ClipSide = 0;
//翻转舵机反向 0正向 1反向
const unsigned char FlipSide = 0;
//云台反向 0正向 1反向
const unsigned char PTZSide = 0;

//以下为代码部分

//全局变量定义
//字符串指针
unsigned char* SendString = 0;

unsigned short* PPortrait = 0;
unsigned short* PRotate = 0;
unsigned short* PVertical = 0;
unsigned short* PAUndefined1 = 0;
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
const unsigned short Latency = 1000 / SendFreq;
const unsigned short DoClipTime = ClipTime / Latency;
const unsigned short DoFlipTime = FlipTime / Latency;
const unsigned short DoDisClipTime = DisClipTime / Latency;

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

//定时器中断
void TimerInterrupt()
{
 //简单处理
 CFMode = CFStart*(FHandClip + FSemiautoClip * 2 + FAutoClip * 3);
 *PLight = (unsigned short)((*PLight)*FLight);
 *PGesture = (unsigned char)(FGesture*(FPitch + 2*FRoll + 3*FPRMixed));
 *PPortrait = (unsigned short)*PPortrait + PortraitSide*(1024 - 2*(*PPortrait));
 *PRotate = (unsigned short)*PRotate + RotateSide*(1024 - 2*(*PRotate));
 *PVertical = (unsigned short)*PVertical + VerticalSide*(1024 - 2*(*PVertical));
 *PPTZ = (unsigned short)*PPTZ + PTZSide*(1024 - 2*(*PPTZ));
 //自动夹取代码
 switch (CFMode)
 {
 case 1:
  *PClip = (unsigned short)*PClip + ClipSide*(1024 - (*PClip));
  *PFlip = (unsigned short)*PFlip + FlipSide*(1024 - 2*(*PFlip));
  break;
 case 2:
  *PClip = (unsigned short)*PClip + ClipSide*(1024 - (*PClip));
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
 }
 Serial.write(SendString, 21);
}

void setup()
{
 // put your setup code here, to run once:
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
 pinMode(44,0x0);
 pinMode(45, 0x0);
 Serial.begin(115200);
 //开辟字符串用内存空间
 SendString = (unsigned char*)malloc(21);
 //指针定向
 PPortrait = (unsigned short*)(SendString + 1);
 PRotate = (unsigned short*)(SendString + 3);
 PVertical = (unsigned short*)(SendString + 5);
 //PAUndefined1=0;
 PLight = (unsigned short*)(SendString + 7);
 PPTZ = (unsigned short*)(SendString + 9);
 PClip = (unsigned short*)(SendString + 11);
 PFlip = (unsigned short*)(SendString + 13);
 PTransverse = SendString + 15;
 POrient = SendString + 16;
 PDeepkeeping = SendString + 17;
 PMode = SendString + 18;
 PGesture = SendString + 19;
 delay(4000);
 //机器人初始化指令发送
 SendString[0] = 0x25;
 for (int i = 1; i < 20; ++i)
 {
  SendString[i] = 0xFF;
 }
 SendString[20] = 0x21;
 Serial.write(SendString, 21);
 //开启定时器中断，开始运行
 FlexiTimer2::set(Latency, TimerInterrupt);
 FlexiTimer2::start();
}

void loop()
{
 // put your main code here, to run repeatedly:
 //模拟口读取
 *PPortrait = (unsigned short)analogRead(A0);
 *PRotate = (unsigned short)analogRead(A1);
 *PVertical = (unsigned short)analogRead(A2);
 //*PAUndefined1=(u16)analogRead(AUndefined1);
 *PLight = (unsigned short)analogRead(A4);
 *PPTZ = (unsigned short)analogRead(A5);
 *PClip = (unsigned short)analogRead(A6);
 *PFlip = (unsigned short)analogRead(A7);
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
 if (CFStart == 0)
 {
  CFStart = digitalRead(34);
 }
}
