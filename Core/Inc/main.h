/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "stdio.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_audio.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
} AUDIO_ErrorTypeDef;

typedef struct {
//  void   (*DemoFunc)(void);
  uint8_t DemoName[50];
  uint32_t DemoIndex;
} Effect;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

uint8_t AUDIO_Process();
uint8_t CheckForUserInput();
void AudioRec_demo();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_B0_Pin GPIO_PIN_4
#define LCD_B0_GPIO_Port GPIOE
#define FMC_NBL1_Pin GPIO_PIN_1
#define FMC_NBL1_GPIO_Port GPIOE
#define FMC_NBL0_Pin GPIO_PIN_0
#define FMC_NBL0_GPIO_Port GPIOE
#define AR_D3_Pin GPIO_PIN_4
#define AR_D3_GPIO_Port GPIOB
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_7
#define VCP_RX_GPIO_Port GPIOB
#define FMC_SDNCAS_Pin GPIO_PIN_15
#define FMC_SDNCAS_GPIO_Port GPIOG
#define LCD_B1_Pin GPIO_PIN_13
#define LCD_B1_GPIO_Port GPIOJ
#define OTG_FS_VBUS_Pin GPIO_PIN_12
#define OTG_FS_VBUS_GPIO_Port GPIOJ
#define Audio_INT_Pin GPIO_PIN_6
#define Audio_INT_GPIO_Port GPIOD
#define FMC_D2_Pin GPIO_PIN_0
#define FMC_D2_GPIO_Port GPIOD
#define NC1_Pin GPIO_PIN_8
#define NC1_GPIO_Port GPIOI
#define SAI2_MCLKA_Pin GPIO_PIN_4
#define SAI2_MCLKA_GPIO_Port GPIOI
#define LCD_DE_Pin GPIO_PIN_7
#define LCD_DE_GPIO_Port GPIOK
#define LCD_B7_Pin GPIO_PIN_6
#define LCD_B7_GPIO_Port GPIOK
#define LCD_B6_Pin GPIO_PIN_5
#define LCD_B6_GPIO_Port GPIOK
#define LCD_B4_Pin GPIO_PIN_12
#define LCD_B4_GPIO_Port GPIOG
#define SAI2_SDB_Pin GPIO_PIN_10
#define SAI2_SDB_GPIO_Port GPIOG
#define LCD_B2_Pin GPIO_PIN_14
#define LCD_B2_GPIO_Port GPIOJ
#define FMC_D3_Pin GPIO_PIN_1
#define FMC_D3_GPIO_Port GPIOD
#define ARDUINO_D7_Pin GPIO_PIN_3
#define ARDUINO_D7_GPIO_Port GPIOI
#define ARDUINO_D8_Pin GPIO_PIN_2
#define ARDUINO_D8_GPIO_Port GPIOI
#define FMC_A0_Pin GPIO_PIN_0
#define FMC_A0_GPIO_Port GPIOF
#define SAI2_SCKA_Pin GPIO_PIN_5
#define SAI2_SCKA_GPIO_Port GPIOI
#define SAI2_FSA_Pin GPIO_PIN_7
#define SAI2_FSA_GPIO_Port GPIOI
#define LCD_HSYNC_Pin GPIO_PIN_10
#define LCD_HSYNC_GPIO_Port GPIOI
#define SAI2_SDA_Pin GPIO_PIN_6
#define SAI2_SDA_GPIO_Port GPIOI
#define LCD_B5_Pin GPIO_PIN_4
#define LCD_B5_GPIO_Port GPIOK
#define LCD_BL_CTRL_Pin GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_Port GPIOK
#define LCD_B3_Pin GPIO_PIN_15
#define LCD_B3_GPIO_Port GPIOJ
#define TP3_Pin GPIO_PIN_15
#define TP3_GPIO_Port GPIOH
#define RCC_OSC32_IN_Pin GPIO_PIN_14
#define RCC_OSC32_IN_GPIO_Port GPIOC
#define FMC_A1_Pin GPIO_PIN_1
#define FMC_A1_GPIO_Port GPIOF
#define LCD_DISP_Pin GPIO_PIN_12
#define LCD_DISP_GPIO_Port GPIOI
#define LCD_VSYNC_Pin GPIO_PIN_9
#define LCD_VSYNC_GPIO_Port GPIOI
#define AR_D5_Pin GPIO_PIN_0
#define AR_D5_GPIO_Port GPIOI
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_TX_GPIO_Port GPIOA
#define RCC_OSC32_OUT_Pin GPIO_PIN_15
#define RCC_OSC32_OUT_GPIO_Port GPIOC
#define LCD_G6_Pin GPIO_PIN_1
#define LCD_G6_GPIO_Port GPIOK
#define LCD_G7_Pin GPIO_PIN_2
#define LCD_G7_GPIO_Port GPIOK
#define OSC_25M_Pin GPIO_PIN_0
#define OSC_25M_GPIO_Port GPIOH
#define FMC_A2_Pin GPIO_PIN_2
#define FMC_A2_GPIO_Port GPIOF
#define LCD_INT_Pin GPIO_PIN_13
#define LCD_INT_GPIO_Port GPIOI
#define LCD_R0_Pin GPIO_PIN_15
#define LCD_R0_GPIO_Port GPIOI
#define LCD_G4_Pin GPIO_PIN_11
#define LCD_G4_GPIO_Port GPIOJ
#define LCD_G5_Pin GPIO_PIN_0
#define LCD_G5_GPIO_Port GPIOK
#define AR_D0_Pin GPIO_PIN_7
#define AR_D0_GPIO_Port GPIOC
#define FMC_A3_Pin GPIO_PIN_3
#define FMC_A3_GPIO_Port GPIOF
#define LCD_CLK_Pin GPIO_PIN_14
#define LCD_CLK_GPIO_Port GPIOI
#define LCD_G1_Pin GPIO_PIN_8
#define LCD_G1_GPIO_Port GPIOJ
#define LCD_G3_Pin GPIO_PIN_10
#define LCD_G3_GPIO_Port GPIOJ
#define FMC_SDCLK_Pin GPIO_PIN_8
#define FMC_SDCLK_GPIO_Port GPIOG
#define AR_D1_Pin GPIO_PIN_6
#define AR_D1_GPIO_Port GPIOC
#define FMC_A4_Pin GPIO_PIN_4
#define FMC_A4_GPIO_Port GPIOF
#define FMC_SDNME_Pin GPIO_PIN_5
#define FMC_SDNME_GPIO_Port GPIOH
#define FMC_SDNE0_Pin GPIO_PIN_3
#define FMC_SDNE0_GPIO_Port GPIOH
#define LCD_G0_Pin GPIO_PIN_7
#define LCD_G0_GPIO_Port GPIOJ
#define LCD_G2_Pin GPIO_PIN_9
#define LCD_G2_GPIO_Port GPIOJ
#define AR_D4_Pin GPIO_PIN_7
#define AR_D4_GPIO_Port GPIOG
#define AR_D2_Pin GPIO_PIN_6
#define AR_D2_GPIO_Port GPIOG
#define ARDUINO_A4_Pin GPIO_PIN_7
#define ARDUINO_A4_GPIO_Port GPIOF
#define ARDUINO_A5_Pin GPIO_PIN_6
#define ARDUINO_A5_GPIO_Port GPIOF
#define FMC_A5_Pin GPIO_PIN_5
#define FMC_A5_GPIO_Port GPIOF
#define NC2_Pin GPIO_PIN_2
#define NC2_GPIO_Port GPIOH
#define LCD_R7_Pin GPIO_PIN_6
#define LCD_R7_GPIO_Port GPIOJ
#define FMC_D1_Pin GPIO_PIN_15
#define FMC_D1_GPIO_Port GPIOD
#define FMC_D15_Pin GPIO_PIN_10
#define FMC_D15_GPIO_Port GPIOD
#define ARDUINO_A1_Pin GPIO_PIN_10
#define ARDUINO_A1_GPIO_Port GPIOF
#define ARDUINO_A2_Pin GPIO_PIN_9
#define ARDUINO_A2_GPIO_Port GPIOF
#define ARDUINO_A3_Pin GPIO_PIN_8
#define ARDUINO_A3_GPIO_Port GPIOF
#define FMC_SDCKE0_Pin GPIO_PIN_3
#define FMC_SDCKE0_GPIO_Port GPIOC
#define FMC_D0_Pin GPIO_PIN_14
#define FMC_D0_GPIO_Port GPIOD
#define FMC_D14_Pin GPIO_PIN_9
#define FMC_D14_GPIO_Port GPIOD
#define FMC_D13_Pin GPIO_PIN_8
#define FMC_D13_GPIO_Port GPIOD
#define FMC_A6_Pin GPIO_PIN_12
#define FMC_A6_GPIO_Port GPIOF
#define FMC_A11_Pin GPIO_PIN_1
#define FMC_A11_GPIO_Port GPIOG
#define FMC_A9_Pin GPIO_PIN_15
#define FMC_A9_GPIO_Port GPIOF
#define LCD_R5_Pin GPIO_PIN_4
#define LCD_R5_GPIO_Port GPIOJ
#define EXT_RST_Pin GPIO_PIN_3
#define EXT_RST_GPIO_Port GPIOG
#define RMII_RXER_Pin GPIO_PIN_2
#define RMII_RXER_GPIO_Port GPIOG
#define LCD_R6_Pin GPIO_PIN_5
#define LCD_R6_GPIO_Port GPIOJ
#define FMC_A7_Pin GPIO_PIN_13
#define FMC_A7_GPIO_Port GPIOF
#define FMC_A10_Pin GPIO_PIN_0
#define FMC_A10_GPIO_Port GPIOG
#define LCD_R4_Pin GPIO_PIN_3
#define LCD_R4_GPIO_Port GPIOJ
#define FMC_D5_Pin GPIO_PIN_8
#define FMC_D5_GPIO_Port GPIOE
#define FMC_BA1_Pin GPIO_PIN_5
#define FMC_BA1_GPIO_Port GPIOG
#define FMC_BA0_Pin GPIO_PIN_4
#define FMC_BA0_GPIO_Port GPIOG
#define LCD_SCL_Pin GPIO_PIN_7
#define LCD_SCL_GPIO_Port GPIOH
#define FMC_A8_Pin GPIO_PIN_14
#define FMC_A8_GPIO_Port GPIOF
#define LCD_R3_Pin GPIO_PIN_2
#define LCD_R3_GPIO_Port GPIOJ
#define FMC_SDNRAS_Pin GPIO_PIN_11
#define FMC_SDNRAS_GPIO_Port GPIOF
#define FMC_D6_Pin GPIO_PIN_9
#define FMC_D6_GPIO_Port GPIOE
#define FMC_D8_Pin GPIO_PIN_11
#define FMC_D8_GPIO_Port GPIOE
#define FMC_D11_Pin GPIO_PIN_14
#define FMC_D11_GPIO_Port GPIOE
#define LCD_SDA_Pin GPIO_PIN_8
#define LCD_SDA_GPIO_Port GPIOH
#define LCD_R1_Pin GPIO_PIN_0
#define LCD_R1_GPIO_Port GPIOJ
#define LCD_R2_Pin GPIO_PIN_1
#define LCD_R2_GPIO_Port GPIOJ
#define FMC_D4_Pin GPIO_PIN_7
#define FMC_D4_GPIO_Port GPIOE
#define FMC_D7_Pin GPIO_PIN_10
#define FMC_D7_GPIO_Port GPIOE
#define FMC_D9_Pin GPIO_PIN_12
#define FMC_D9_GPIO_Port GPIOE
#define FMC_D12_Pin GPIO_PIN_15
#define FMC_D12_GPIO_Port GPIOE
#define FMC_D10_Pin GPIO_PIN_13
#define FMC_D10_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

#define RGB565_BYTE_PER_PIXEL     		2
#define ARBG8888_BYTE_PER_PIXEL   		4

/* Camera have a max resolution of VGA : 640x480 */
#define CAMERA_RES_MAX_X          		640
#define CAMERA_RES_MAX_Y          		480

/**
  * @brief  LCD FB_StartAddress
  * LCD Frame buffer start address : starts at beginning of SDRAM
  */
#define LCD_FRAME_BUFFER          		SDRAM_DEVICE_ADDR

/**
  * @brief  Camera frame buffer start address
  * Assuming LCD frame buffer is of size 480x800 and format ARGB8888 (32 bits per pixel).
  */
#define CAMERA_FRAME_BUFFER       		((uint32_t)(LCD_FRAME_BUFFER + (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * ARBG8888_BYTE_PER_PIXEL)))

/**
  * @brief  SDRAM Write read buffer start address after CAM Frame buffer
  * Assuming Camera frame buffer is of size 640x480 and format RGB565 (16 bits per pixel).
  */
#define SDRAM_WRITE_READ_ADDR        	((uint32_t)(CAMERA_FRAME_BUFFER + (CAMERA_RES_MAX_X * CAMERA_RES_MAX_Y * RGB565_BYTE_PER_PIXEL)))

#define SDRAM_WRITE_READ_ADDR_OFFSET 	((uint32_t)0x0800)
#define SRAM_WRITE_READ_ADDR_OFFSET  	SDRAM_WRITE_READ_ADDR_OFFSET

#define AUDIO_REC_START_ADDR         	SDRAM_WRITE_READ_ADDR

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
