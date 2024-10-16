/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "i2c.h"
#include "i2s.h"
#include "ltdc.h"
#include "sai.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f7xx_hal_adc.h"
#include "string.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_COLOR_RUBYRED		((uint32_t)0xFFB22222)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int EffectIndex = 0;
int ParameterIndex = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t CheckForUserInput() {
    if (BSP_PB_GetState(BUTTON_KEY) != RESET) {
        HAL_Delay(10);

        while (BSP_PB_GetState(BUTTON_KEY) != RESET);

        return 1 ;
    }
    return 0;
}


extern EffectTypedef effects[] = {
    {"DISTORTION", 2, 0.0, 0.0, 0.0, 0.0}, 	// ?"           ", 1"Gain", 2"Threshold", ?"    ", ?"        ", ?"   ", ?"           "
    {"FUZZ", 3, 0.0, 0.0, 0.0, 0.0}, 		// 1"Depth      ", 3"Gain", 2"Threshold", ?"    ", ?"        ", ?"   ", ?"           "
    {"FLANGER", 4, 0.0, 0.0, 0.0, 0.0}, 		// 1"Delay-Depth", ?"    ", ?"         ", 2"Rate", 3"Feedback", 4"Mix", ?"           "
    {"ROTARY", 2, 0.0, 0.0, 0.0, 0.0},		// 1"Depth      ", ?"    ", ?"         ", 2"Rate", ?"        ", ?"   ", ?"           "
    {"REVERB", 2, 0.0, 0.0, 0.0, 0.0},		// ?"           ", ?"    ", ?"         ", ?"    ", ?"        ", 2"Mix", 1"Reverb-Time"
    {"DELAY", 3, 0.0, 0.0, 0.0, 0.0}, 		// 1"Delay-Depth", ?"    ", ?"         ", ?"    ", 3"Feedback", 2"Mix", ?"         "
};

// just set it as global, don't pass in. It doesn't matter
char effect_names[6][4][15] = {
    {"Gain", "Threshold"},
    {"Depth", "Threshold", "Gain"},
    {"Delay-Depth", "Rate", "Feedback", "Mix"},
    {"Depth", "Rate"},
    {"Reverb-Time", "Mix"},
    {"Delay-Depth", "Mix", "Feedback"}
};


//float depthMax[6] = {0.0, 2.4, 6.0, 1.6, 0.0, 900.0};
//float gainMax[6] = {2.4, 3.0, 0.0, 0.0, 0.0, 0.0};
//float thresholdMax[6] = {50000, 30000, 0.0, 0.0, 0.0, 0.0};
//float rateMax[6] = {0.0, 0.0, 1.0, 8.0, 0.0, 0.0};
//float feedbackMax[6] = {0.0, 0.0, 0.6, 0.0, 0.0, 0.8};
//float mixMax[6] = {0.0, 0.0, 1.2, 0.0, 1.4, 1.2};
//float reverbTimeMax[6] = {0.0, 0.0, 0.0, 0.0, 1000.0, 0.0};

// display code
// min = 0, max = 4096 -> find percentage -> put in the middle
// try drawing many circles on top of each other
// radius should be static
static void Effect_Circle_Display(uint16_t center_x, uint16_t center_y, uint8_t percentage, char* val_name, uint8_t isActive);
static void Display_Detail(char* mode_name, char* state_name);
static int average_8(float x);
static int average_16(float x);
static uint8_t convertToPercentage(int x);
//static void update_percentage_display(uint16_t center_x, uint16_t center_y, uint8_t percentage, uint8_t isActive);
static void update_guage_value(uint16_t center_x, uint16_t center_y, uint8_t start, uint8_t stop, uint8_t percentage, uint8_t isActive);

// extern function for button interrupts
extern void change_effect_display();
extern void change_guage_display(uint8_t isActive);
extern void update_guage_value_display();

//uint8_t prev_percentage = 0;
//uint8_t can_update_parameter = 1;
//uint8_t ext_intrpt_enable = 1;
uint8_t prevADCValue = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC3_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SAI2_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_I2S1_Init();
  /* USER CODE BEGIN 2 */

  // HAL_ADC_Start_DMA(&hadc3, );

	BSP_LED_Init(LED1);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FRAME_BUFFER);

  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // change_effect_display();
  // change_guage_display(1);

	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2 + 15, BSP_LCD_GetXSize(), 60);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 30, (uint8_t *)"Press User Button to start :", CENTER_MODE);
	BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 45, (uint8_t *)"AUDIO RECORD example", CENTER_MODE);

  while (1) {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

	  if (BSP_PB_GetState(BUTTON_KEY) != RESET) {
        HAL_Delay(10);
        while (BSP_PB_GetState(BUTTON_KEY) != RESET);

        // set to inactive all
        change_guage_ddisplay(0);

        // disable all interrupts
  //      __disable_irq();

        // change to disable just exti15 and exti0
        HAL_NVIC_DisableIRQ(EXTI0_IRQn);

        // this line doesn't seem to work at all
        HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

        AudioRec_demo();

        // so this is quite easy
        // we want to update the thing according to parameter index
        // we check which parameter index it is
        // we check if the value is
        // current adjustable value
        uint8_t prev_percentage = 0;
        int adc_read = average_8(uhADCxConvertedValue);

        switch (ParameterIndex) {
            case 0:
                prev_percentage = convertToPercentage(effects[EffectIndex].Parameter1);
                effects[EffectIndex].Parameter1 = adc_read;
                break;
            case 1:
                prev_percentage = convertToPercentage(effects[EffectIndex].Parameter2);
                effects[EffectIndex].Parameter2 = adc_read;
                break;
            case 2:
                prev_percentage = convertToPercentage(effects[EffectIndex].Parameter3);
                effects[EffectIndex].Parameter3 = adc_read;
                break;
            case 3:
                prev_percentage = convertToPercentage(effects[EffectIndex].Parameter4);
                effects[EffectIndex].Parameter4 = adc_read;
                break;
            default:
                break;
        }

        // update data
        uint8_t cur_percentage = convertToPercentage(adc_read);

        if (cur_percentage != prev_percentage) {
            update_guage_value_display();
            prev_percentage = cur_percentage;
        }
        HAL_Delay(10);
     }
  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_SAI2;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  static uint32_t debounce_time = 0;

  if (GPIO_Pin == KEY_BUTTON_PIN) {
    /* Prevent debounce effect for user key */
    if ((HAL_GetTick() - debounce_time) > 50) {
      debounce_time = HAL_GetTick();
    }
  }
  else if (GPIO_Pin == AUDIO_IN_INT_GPIO_PIN) {
    /* Audio IN interrupt */
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
  /* Turn LED1 on: Transfer process is correct */
	BSP_LED_Toggle(LED1);
	HAL_Delay(1000);
}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.SubRegionDisable = 0x0;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0xA0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8KB;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
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
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
