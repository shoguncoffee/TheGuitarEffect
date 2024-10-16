/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

extern DMA_HandleTypeDef   hdma;

/*DMA status declared in "sdram_dma.c" file */
extern uint32_t uwDMA_Transfer_Complete;

/* SAI handler declared in "stm32746g_discovery_audio.c" file */
extern SAI_HandleTypeDef haudio_out_sai;

/* SAI handler declared in "stm32746g_discovery_audio.c" file */
extern SAI_HandleTypeDef haudio_in_sai;

/* SDRAM handler declared in "stm32746g_discovery_sdram.c" file */
extern SDRAM_HandleTypeDef sdramHandle;


extern ADC_HandleTypeDef    AdcHandle;
extern int EffectIndex;
extern int ParameterIndex;
extern Effect  effect[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc3;
extern ADC_HandleTypeDef hadc3;
extern DMA2D_HandleTypeDef hdma2d;
extern SDRAM_HandleTypeDef hsdram1;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc3);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles FMC global interrupt.
  */
void FMC_IRQHandler(void)
{
  /* USER CODE BEGIN FMC_IRQn 0 */

  /* USER CODE END FMC_IRQn 0 */
  HAL_SDRAM_IRQHandler(&hsdram1);
  /* USER CODE BEGIN FMC_IRQn 1 */

  /* USER CODE END FMC_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */

  /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2D_IRQn 0 */

  /* USER CODE END DMA2D_IRQn 0 */
  HAL_DMA2D_IRQHandler(&hdma2d);
  /* USER CODE BEGIN DMA2D_IRQn 1 */

  // BSP_LCD_DMA2D_IRQHandler();

  /* USER CODE END DMA2D_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void AUDIO_IN_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
}

void BSP_SDRAM_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(sdramHandle.hdma);
}

void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

// void EXTI0_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(WAKEUP_BUTTON_PIN);

//     // Check if the interrupt is triggered by EXTI Line0
//     static uint32_t debounce_time = 0;

//     if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
//     {
//         // Prevent debounce effect for user key
//         if ((HAL_GetTick() - debounce_time) > 150)
//         {
//             // Clear the EXTI line pending bit
//             __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

//             // stop updating the guage
// //            can_update_parameter = 0;

//             // Add your code here, e.g., toggle an LED
//             EffectIndex++;
// //            if (EffectIndex >= 6)
//             if (EffectIndex >= 6)
// 			{
// 				EffectIndex = 0;
// 			}

//             // change the whole display to account for
//             change_effect_display();
//             // set paramIndex to 0
//             ParameterIndex = 0;
//             change_guage_display(1);

// //            can_update_parameter = 1;

//             // Update the debounce time
//             debounce_time = HAL_GetTick();
//         }
//         else
//         {
//             // If the time is not sufficient, clear the interrupt to prevent further triggers
//             __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
//         }
//     }
// }

// void EXTI2_IRQHandler(void)
// {
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
// }

// void ADCx_DMA_IRQHandler(void)
// {
//   HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
// }

// void EXTI15_10_IRQHandler(void)
// {
//   /* Interrupt handler shared between SD_DETECT pin, USER_KEY button and touch screen interrupt */
//   if (__HAL_GPIO_EXTI_GET_IT(SD_DETECT_PIN) != RESET)
//   {
//     HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN | TS_INT_PIN | AUDIO_IN_INT_GPIO_PIN);   /* SD detect event or touch screen interrupt */
//   }
//   else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
//   {

// 	  // Check if the interrupt is triggered by EXTI Line0
// 	  static uint32_t debounce_time = 0;
// 	  // Prevent debounce effect for user key
// 	  if ((HAL_GetTick() - debounce_time) > 125)
// 	  {
// 		  // Clear the EXTI line pending bit
// 		  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);

// 		  // call change change_guage_display to update the former parameter to inactive
// 		  change_guage_display(0);

// 		  // Add your code here, e.g., toggle an LED
// 		  ParameterIndex++;
// 		  if (ParameterIndex >= effects[EffectIndex].ParametersNum)
// 		  {
// 			  ParameterIndex = 0;
// 		  }
// 		  // call change change_guage_display to update the current parameter to active
// 		  change_guage_display(1);

// 		  // Update the debounce time
// 		  debounce_time = HAL_GetTick();
// 	  }
// 	  else
// 	  {
// 		  // If the time is not sufficient, clear the interrupt to prevent further triggers
// 		  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
// 	  }
//   }
//   else
//   {     /* User button event or Touch screen interrupt */
//     HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
//   }
// }

/* USER CODE END 1 */
