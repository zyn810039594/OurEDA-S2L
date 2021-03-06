/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Attitude.h"
#include "Setting.h"
#include "Auto.h"
#include "Display.h"
#include "BasicCtrl.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef unsigned char u8;
typedef unsigned short u16;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;

osThreadId ControlTaskHandle;
osThreadId DisplayTaskHandle;
osThreadId WaterDeepTaskHandle;
osThreadId AttitudeTaskHandle;
osThreadId EmptyTaskHandle;
osThreadId AutoMoveTaskHandle;
osThreadId InitialTaskHandle;
osThreadId SwitcherTaskHandle;
/* USER CODE BEGIN PV */
//闁诡剚妲掔换妯肩矙鐎ｎ亝鍎欓柛鏂诲姀椤曨剙效閸屾稓鍨奸煫鍥ㄣ仦缂??
volatile u8 TaskBeingChangeFlag = 0;
//濞戞挾绮敮鑸垫交濞戞埃鏌ら柛姘煎灠婵晝鎷犻柨瀣勾闁哄秴娲ょ换鏃?鎷??
volatile u8 ControlTaskFlag = 0;
//闁哄嫬澧介妵姘交濞戞埃鏌ら柛姘煎灠婵晝鎷犻柨瀣勾闁哄秴娲ょ换鏃?鎷??
volatile u8 DisplayTaskFlag = 0;
//婵ê鐡ㄧ换浣肝??婵炴潙顑堢换妯肩矙鐎ｎ亝鍎欓柛鏂诲姀椤曨剙效閸屾稓鍨奸煫鍥ㄣ仦缂??
volatile u8 WaterDeepTaskFlag = 0;
//濞戞梹绻嗛柊杈╂嫚鐠囨彃绲块弶鈺傜〒閳诲ジ宕ラ姘楅悹鍥敱閻即寮介崶褏绠跺ù??
volatile u8 AttitudeTaskFlag = 0;
//闁煎浜滄慨鈺傛交閹邦剙袟閺夆晜绋撻埢濂稿触椤栨艾袟閻犲洭鏀遍惇浼村冀閸パ呯濞???
volatile u8 AutoMoveTaskFlag = 0;

//闁规亽鍔忕换姗?宕抽埉鎵篗闁告瑱绲介悾鐐▕?
volatile uint32_t* MoveCCR[8] = { &(TIM1->CCR1), &(TIM1->CCR2), &(TIM1->CCR3), &(TIM1->CCR4), &(TIM2->CCR1), &(TIM2->CCR2), &(TIM2->CCR3), &(TIM2->CCR4) };
//濞戞挻褰冭ぐ?1缂傚倹鎸搁悺?
u8 UART1RXCache[UART1RXLen];
//濞戞挻褰冭ぐ?3缂傚倹鎸搁悺?
u8 UART3RXCache[UART3RXLen];
//濞戞挻褰冭ぐ?4缂傚倹鎸搁悺?
u8 UART4RXCache[UART4RXLen];
//濞戞挻褰冭ぐ?4閻犲洩顕цぐ鍥箰閸ヮ剚瀚?
u8* UART4RXPosition = 0;
//缂侇垵宕电划娲触椤栨艾袟闁绘鍩??娴ｅ湱鍨奸煫鍥ㄣ仦缂??
volatile u8 SystemBegin = 0;

//婵☆垪鍓濈?氭瑥螞?婵炴潙顑囩槐锔????
uint32_t ADCCache[6] = { 0 };

//婵ê鐡ㄧ换渚?寮悧鍫濈ウ
u16 WaterDepth = 0;
//婵ê鐡ㄦ穱顖炲极閻楀牆绁?
u16 WaterTemperture = 0;
//婵ê鐡ㄧ换渚?骞愭径澶岀Т闁轰胶澧楀畵?
u8 WaterDepthNum[6];
//婵ê鐡ㄦ穱顖炲箰婢跺绉撮柡浣哄瀹??
u8 WaterTempertureNum[5];

//闁告梻濞??閻斿嘲顔婇柛妯煎枎椤劙寮悧鍫濈ウ
u16 AccNum[3] = { 0 };
//閻熸瑦甯??閻斿嘲顔婇柛妯煎枎椤劙寮悧鍫濈ウ
u16 RotNum[3] = { 0 };
//Roll Pitch Yaw
u16 EulNum[3] = { 0 };
//缁惧彞绀佹慨蹇曟喆閹烘垵鏂у┑顔碱儐閺嗙喖骞??
u16 MagNUM[3] = { 0 };
//濞戞梹绻嗛柊閬嶅级閹稿簼鍒婇幖杈剧畱鐢偅鎱ㄧ?ｎ偅娈堕柟??
u16 InsTemNum;

//婵炲柌鍕?欓悷娆愬笧缁劑寮稿鍕闁???
u16 EulResNum[3] = { 0 };


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM7_Init(void);
void CtrlTask(void const * argument);
void DisTask(void const * argument);
void WDTask(void const * argument);
void AttTask(void const * argument);
void EmpTask(void const * argument);
void AutoTask(void const * argument);
void InitTask(void const * argument);
void SWTask(void const * argument);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_IWDG_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM14_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	
	BasicPoint(UART1RXCache);
	HAL_IWDG_Refresh(&hiwdg);
	
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ControlTask */
  osThreadDef(ControlTask, CtrlTask, osPriorityHigh, 0, 128);
  ControlTaskHandle = osThreadCreate(osThread(ControlTask), NULL);

  /* definition and creation of DisplayTask */
  osThreadDef(DisplayTask, DisTask, osPriorityAboveNormal, 0, 128);
  DisplayTaskHandle = osThreadCreate(osThread(DisplayTask), NULL);

  /* definition and creation of WaterDeepTask */
  osThreadDef(WaterDeepTask, WDTask, osPriorityNormal, 0, 128);
  WaterDeepTaskHandle = osThreadCreate(osThread(WaterDeepTask), NULL);

  /* definition and creation of AttitudeTask */
  osThreadDef(AttitudeTask, AttTask, osPriorityNormal, 0, 128);
  AttitudeTaskHandle = osThreadCreate(osThread(AttitudeTask), NULL);

  /* definition and creation of EmptyTask */
  osThreadDef(EmptyTask, EmpTask, osPriorityIdle, 0, 128);
  EmptyTaskHandle = osThreadCreate(osThread(EmptyTask), NULL);

  /* definition and creation of AutoMoveTask */
  osThreadDef(AutoMoveTask, AutoTask, osPriorityNormal, 0, 128);
  AutoMoveTaskHandle = osThreadCreate(osThread(AutoMoveTask), NULL);

  /* definition and creation of InitialTask */
  osThreadDef(InitialTask, InitTask, osPriorityRealtime, 0, 128);
  InitialTaskHandle = osThreadCreate(osThread(InitialTask), NULL);

  /* definition and creation of SwitcherTask */
  osThreadDef(SwitcherTask, SWTask, osPriorityHigh, 0, 128);
  SwitcherTaskHandle = osThreadCreate(osThread(SwitcherTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */
  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */
  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 6;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */
  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */
  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Reload = 2047;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */
  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */
  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */
  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 191;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 50000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */
  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 95;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */
  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */
  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 95;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 50000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 3839;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 25000;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 47;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 5000;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */
  HAL_TIM_MspPostInit(&htim14);

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */
  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */
  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */
	
  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */
  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */
  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */
  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */
  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */
  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */
  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */
  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */
  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */
  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */
  /* USER CODE END USART6_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/* USER CODE BEGIN Header_CtrlTask */
/**
  * @brief  Function implementing the ControlTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_CtrlTask */
void CtrlTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  u8 WarnNum = 0;
	  u8 PowerNum = ADCCache[0] / 4096 * 3.3 * 50;
	  HAL_IWDG_Refresh(&hiwdg);
	  BasicControl(MoveCCR, &TIM3->CCR1, &TIM3->CCR3, &TIM3->CCR4, &TIM14->CCR1);
	  
#ifdef SWPassback
	  if (ADCCache[1] > 3100)
	  {
		  WarnNum += 1;
	  }
	  if (ADCCache[2] > 3100)
	  {
		  WarnNum += 2;
	  }
	  PassBack(&huart1, PowerNum, WarnNum, AccNum, RotNum, EulNum, MagNUM, InsTemNum, WaterTemperture, WaterDepth);
#endif // SWPassback

	  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	  HAL_UART_Receive_DMA(&huart1, UART1RXCache, UART1RXLen);
	  
	  vTaskSuspend(ControlTaskHandle);
    osDelay(5);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_DisTask */
/**
* @brief Function implementing the DisplayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DisTask */
void DisTask(void const * argument)
{
  /* USER CODE BEGIN DisTask */
	static u8 PowerNum=0;
  /* Infinite loop */
  for(;;)
  {
#ifdef HardWare_QL800

#ifdef SWPowerScanner
	  PowerNum = ADCCache[0] / 4096 * 3.3 * 50;
	  DisPower(&huart4, PowerNum);
	  if (ADCCache[1] > 3100)
	  {
		  DisWarning(&huart2, 0);
		  osDelay(5);
	  }
	  else
	  {
		  DisWarning(&huart2, 3);
		  osDelay(5);
	  }
	  HAL_IWDG_Refresh(&hiwdg);
	  if (ADCCache[2] > 3100)
	  {
		  DisWarning(&huart2, 1);
		  osDelay(5);
	  }
	  else
	  {
		  DisWarning(&huart2, 4);
		  osDelay(5);
	  }
	  HAL_IWDG_Refresh(&hiwdg);
#endif // SWPowerScanner

#ifdef SWWaterDeep
	  DisWaterDeep(&huart2, WaterDepthNum, WaterTempertureNum);
	  osDelay(5);
	  HAL_IWDG_Refresh(&hiwdg);	  
#endif // SWWaterDeep

#ifdef SWAttitudeSensor
	  DisAttitude(&huart2, EulResNum[0], EulResNum[1], EulResNum[2], InsTemNum);
	  osDelay(5);
	  HAL_IWDG_Refresh(&hiwdg);
#endif // SWAttitudeSensor
				  
#endif // HardWare_QL800
	  
#ifdef HardWare_QL504
#ifdef SWPowerScanner
	  PowerNum = ADCCache[0] * 0.040283203125;
	  static u8 WarningState[2] = { 0, 0 };
	  if (ADCCache[1] > 2500)
	  {
		  if (WarningState[0] == 0)
		  {
			  DisWarning_QL504(&huart2, hiwdg, 0);
			  WarningState[0] = 1;
		  }
	  }
	  else
	  {
		  if (WarningState[0] == 1)
		  {
			  DisWarning_QL504(&huart2, hiwdg, 3);
			  WarningState[0] = 0;
		  }
	  }
	  HAL_IWDG_Refresh(&hiwdg);
	  if (ADCCache[2] > 2500)
	  {
		  if (WarningState[1] == 0)
		  {
			  DisWarning_QL504(&huart2, hiwdg, 1);
			  WarningState[1] = 1;
		  }
	  }
	  else
	  {
		  if (WarningState[1]==1)
		  {
			  DisWarning_QL504(&huart2, hiwdg, 4);
			  WarningState[1] = 0;
		  }
		  
	  }
	  HAL_IWDG_Refresh(&hiwdg);
#endif // SWPowerScanner

	  DisData_QL504(&huart2, hiwdg, WaterDepthNum, WaterTempertureNum, EulResNum[2], EulResNum[0], PowerNum);
	  
#endif // HardWare_QL504

	  
	  HAL_IWDG_Refresh(&hiwdg);
    osDelay(1);
  }
  /* USER CODE END DisTask */
}

/* USER CODE BEGIN Header_WDTask */
/**
* @brief Function implementing the WaterDeepTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WDTask */
void WDTask(void const * argument)
{
  /* USER CODE BEGIN WDTask */
  /* Infinite loop */
  for(;;)
  {
	  DeepAnalyze(UART4RXPosition, &WaterDepth, &WaterTemperture, WaterDepthNum, WaterTempertureNum);
	  HAL_UART_Receive_DMA(&huart4, UART4RXCache, UART4RXLen);
	  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	  HAL_IWDG_Refresh(&hiwdg);
	  vTaskSuspend(WaterDeepTaskHandle);
    osDelay(1);
  }
  /* USER CODE END WDTask */
}

/* USER CODE BEGIN Header_AttTask */
/**
* @brief Function implementing the AttitudeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AttTask */
void AttTask(void const * argument)
{
  /* USER CODE BEGIN AttTask */
  /* Infinite loop */
  for(;;)
  {
	  AttitudeTake(EulResNum, AccNum, RotNum, EulNum, MagNUM, &InsTemNum);
	  HAL_UART_Receive_DMA(&huart3, UART3RXCache, UART3RXLen);
	  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	  
	  HAL_IWDG_Refresh(&hiwdg);
	  vTaskSuspend(AttitudeTaskHandle);
    osDelay(2);
  }
  /* USER CODE END AttTask */
}

/* USER CODE BEGIN Header_EmpTask */
/**
* @brief Function implementing the EmptyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_EmpTask */
void EmpTask(void const * argument)
{
  /* USER CODE BEGIN EmpTask */
	
  /* Infinite loop */
  for(;;)
  {
	  HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END EmpTask */
}

/* USER CODE BEGIN Header_AutoTask */
/**
* @brief Function implementing the AutoMoveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AutoTask */
void AutoTask(void const * argument)
{
  /* USER CODE BEGIN AutoTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_IWDG_Refresh(&hiwdg);
    osDelay(1);
  }
  /* USER CODE END AutoTask */
}

/* USER CODE BEGIN Header_InitTask */
/**
* @brief Function implementing the InitialTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_InitTask */
void InitTask(void const * argument)
{
  /* USER CODE BEGIN InitTask */
	vTaskSuspend(ControlTaskHandle);
	vTaskSuspend(AttitudeTaskHandle);
	vTaskSuspend(WaterDeepTaskHandle);
	vTaskSuspend(DisplayTaskHandle);
	vTaskSuspend(AutoMoveTaskHandle);
	HAL_UART_Receive_DMA(&huart1, UART1RXCache, UART1RXLen);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	
	TIM1->CCR1 = 1500;
	TIM1->CCR2 = 1500;
	TIM1->CCR3 = 1500;
	TIM1->CCR4 = 1500;
	TIM2->CCR1 = 1500;
	TIM2->CCR2 = 1500;
	TIM2->CCR3 = 1500;
	TIM2->CCR4 = 1500;
	TIM3->CCR1 = 1500;
	TIM3->CCR3 = 1500;
	TIM3->CCR4 = 1500;
	TIM14->CCR1 = 0;
	HAL_TIM_Base_Start_IT(&htim7);
#ifdef Debug_Mode
	SystemBegin = 1;
#endif // SWDebug

	while (!SystemBegin)
	{
		HAL_IWDG_Refresh(&hiwdg);
	}
	
#ifdef SWDisplayWords
	vTaskResume(DisplayTaskHandle);
#endif // SWDisplayWords

#ifdef SWAutoMove
	vTaskResume(AutoMoveTaskHandle);		  
#endif // SWAutoMove

#ifdef SWPowerScanner
	
	HAL_ADC_Start_DMA(&hadc1, ADCCache, 6);
	
#endif // SWPowerScanner
	
#ifdef SWWaterDeep
	HAL_UART_Receive_DMA(&huart4, UART4RXCache, UART4RXLen);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
#endif // SWWaterDeep

	
#ifdef SWAttitudeSensor
	AttitudePoint(UART3RXCache);
	HAL_UART_Receive_DMA(&huart3, UART3RXCache, UART3RXLen);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
#endif // SWAttitudeSensor
	
	HAL_IWDG_Refresh(&hiwdg);
	vTaskSuspend(InitialTaskHandle);
  /* Infinite loop */
  for(;;)
  {
    osDelay(10000);
  }
  /* USER CODE END InitTask */
}

/* USER CODE BEGIN Header_SWTask */
/**
* @brief Function implementing the SwitcherTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SWTask */
void SWTask(void const * argument)
{
  /* USER CODE BEGIN SWTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_IWDG_Refresh(&hiwdg);
	  if (TaskBeingChangeFlag)
	  {
		  TaskBeingChangeFlag = 0;
		  
		  if (ControlTaskFlag)
		  {
			  ControlTaskFlag = 0;
			  vTaskResume(ControlTaskHandle);
			  osDelay(2);
		  }
		  
#ifdef SWWaterDeep
		  if (WaterDeepTaskFlag)
		  {
			  WaterDeepTaskFlag = 0;
			  vTaskResume(WaterDeepTaskHandle);
			  osDelay(2);
		  }				  
#endif // SWWaterDeep


		  
#ifdef SWDisplayWords
		  if (DisplayTaskFlag)
		  {
			  DisplayTaskFlag=0;
			  vTaskResume(DisplayTaskHandle);osDelay(2);
		  }
#endif

#ifdef SWAttitudeControl
		  if (AttitudeTaskFlag)
		  {
			  AttitudeTaskFlag = 0;
			  vTaskResume(AttitudeTaskHandle);
			  osDelay(2);
		  }
#endif // SWAttitudeControl
		  
#ifdef SWAutoMove
		  if (AutoMoveTaskFlag)
		  {
			  AutoMoveTaskFlag = 0;
			  vTaskResume(AutoMoveTaskHandle);
			  osDelay(2);
		  }
#endif // SWAutoMove


	  }
	  else
	  {
		  osDelay(1);
	  }
  }
  /* USER CODE END SWTask */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
