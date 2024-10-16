#include "main.h"
#include <stdio.h>
#include "string.h"

typedef enum {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF = 1,
    BUFFER_OFFSET_FULL = 2,
} BUFFER_StateTypeDef;

#define AUDIO_BLOCK_SIZE   ((uint32_t) 512)
#define AUDIO_BUFFER_IN    AUDIO_REC_START_ADDR     /* In SDRAM */
#define AUDIO_BUFFER_OUT   (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */


extern uint32_t audio_rec_buffer_state;


void AudioLoopback_SetHint() {
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO LOOPBACK EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User button for next menu", CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);
}

void AudioLoopback_demo () {
  AudioLoopback_SetHint();

  if (BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_DIGITAL_MICROPHONE_2, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK) {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Microphones sound streamed to headphones", CENTER_MODE);

  /* Initialize SDRAM buffers */
  memset((uint16_t*) AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE * 2);
  memset((uint16_t*) AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE * 2);
  audio_rec_buffer_state = BUFFER_OFFSET_NONE;

  /* Start Recording */
  BSP_AUDIO_IN_Record((uint16_t*) AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
  BSP_AUDIO_OUT_Play((uint16_t*) AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE * 2);

  BSP_AUDIO_OUT_SetVolume(50);

  while (1) {
    while(audio_rec_buffer_state != BUFFER_OFFSET_HALF) {
      if (CheckForUserInput() > 0) {
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        return;
      }
    }
    audio_rec_buffer_state = BUFFER_OFFSET_NONE;

    memcpy((uint16_t *)(AUDIO_BUFFER_OUT),
           (uint16_t *)(AUDIO_BUFFER_IN),
           AUDIO_BLOCK_SIZE);

    while(audio_rec_buffer_state != BUFFER_OFFSET_FULL) {
      if (CheckForUserInput() > 0) {
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        return;
      }
    }
    audio_rec_buffer_state = BUFFER_OFFSET_NONE;

    memcpy((uint16_t *)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE)),
           (uint16_t *)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)),
           AUDIO_BLOCK_SIZE);

    if (CheckForUserInput() > 0) {
      BSP_AUDIO_IN_Stop(CODEC_PDWN_SW);
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
      return;
    }
  }
}
