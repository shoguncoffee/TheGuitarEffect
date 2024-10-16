#include "main.h"
#include <stdio.h>

/* Since SysTick is set to 1ms (unless to set it quicker) */
/* to run up to 48khz, a buffer around 1000 (or more) is requested */
/* to run up to 96khz, a buffer around 2000 (or more) is requested */
#define AUDIO_BUFFER_SIZE       2048
#define AUDIO_DEFAULT_VOLUME    70

/* Audio file size and start address are defined here since the audio file is
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SIZE               524288
#define AUDIO_START_OFFSET_ADDRESS    0            /* Offset relative to audio file header size */
#define AUDIO_FILE_ADDRESS            0x08080000   /* Audio file address */

typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,
  AUDIO_STATE_PLAYING,
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

typedef struct {
  uint8_t buff[AUDIO_BUFFER_SIZE];
  uint32_t fptr;
  BUFFER_StateTypeDef state;
}AUDIO_BufferTypeDef;


ALIGN_32BYTES (static AUDIO_BufferTypeDef  buffer_ctl);
static AUDIO_PLAYBACK_StateTypeDef  audio_state;
static uint32_t  AudioStartAddress;
static uint32_t  AudioFileSize;
__IO uint32_t uwVolume = 20;
__IO uint32_t uwPauseEnabledStatus = 0;


uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData) {
  uint8_t *lptr = pdata;
  uint32_t ReadDataNbr;

  ReadDataNbr = 0;
  while(((offset + ReadDataNbr) < AudioFileSize) && (ReadDataNbr < NbrOfData)) {
    pbuf[ReadDataNbr]= lptr [offset + ReadDataNbr];
    ReadDataNbr++;
  }
  return ReadDataNbr;
}

AUDIO_ErrorTypeDef AUDIO_Start(uint32_t audio_start_address, uint32_t audio_file_size) {
  uint32_t bytesread;

  buffer_ctl.state = BUFFER_OFFSET_NONE;
  AudioStartAddress = audio_start_address;
  AudioFileSize = audio_file_size;
  bytesread = GetData(
	  (void *)AudioStartAddress,
	  0,
	  &buffer_ctl.buff[0],
	  AUDIO_BUFFER_SIZE
  );
  if (bytesread > 0) {
    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE/2);

    BSP_AUDIO_OUT_Play((uint16_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
    audio_state = AUDIO_STATE_PLAYING;
    buffer_ctl.fptr = bytesread;
    return AUDIO_ERROR_NONE;
  }
  return AUDIO_ERROR_IO;
}


uint8_t AUDIO_Process() {
  uint32_t bytesread;
  AUDIO_ErrorTypeDef error_state = AUDIO_ERROR_NONE;

  switch(audio_state) {
	  case AUDIO_STATE_PLAYING:

		if (buffer_ctl.fptr >= AudioFileSize) {
		  /* Play audio sample again ... */
		  buffer_ctl.fptr = 0;
		  error_state = AUDIO_ERROR_EOF;
		}

		/* 1st half buffer played; so fill it and continue playing from bottom*/
		if (buffer_ctl.state == BUFFER_OFFSET_HALF) {
		  bytesread = GetData(
			  (void *) AudioStartAddress,
			  buffer_ctl.fptr,
			  &buffer_ctl.buff[0],
			  AUDIO_BUFFER_SIZE /2
		  );

		  if (bytesread > 0) {
			buffer_ctl.state = BUFFER_OFFSET_NONE;
			buffer_ctl.fptr += bytesread;

			/* Clean Data Cache to update the content of the SRAM */
			SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE/2);
		  }
		}

		/* 2nd half buffer played; so fill it and continue playing from top */
		if (buffer_ctl.state == BUFFER_OFFSET_FULL) {
		  bytesread = GetData(
			  (void *) AudioStartAddress,
			  buffer_ctl.fptr,
			  &buffer_ctl.buff[AUDIO_BUFFER_SIZE /2],
			  AUDIO_BUFFER_SIZE /2
		  );
		  if (bytesread > 0) {
			buffer_ctl.state = BUFFER_OFFSET_NONE;
			buffer_ctl.fptr += bytesread;

			/* Clean Data Cache to update the content of the SRAM */
			SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);
		  }
		}
		break;

	  default:
		error_state = AUDIO_ERROR_NOTREADY;
		break;
  }
  return (uint8_t) error_state;
}

void BSP_AUDIO_OUT_TransferComplete_CallBack() {
  if(audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 2nd part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_FULL;
  }
}


void BSP_AUDIO_OUT_HalfTransfer_CallBack() {
  if(audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 1st part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_HALF;
  }
}


void BSP_AUDIO_OUT_Error_CallBack() {
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"       DMA  ERROR     ");

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_KEY) != RESET)
  {
    return;
  }

  /* could also generate a system reset to recover from the error */
  /* .... */
}
