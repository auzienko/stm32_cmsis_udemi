#include "main.h"
#include "fatfs.h"
#include "wav_recorder.h"

//FatFs variables
FRESULT   fresult;
FATFS     fs;
DWORD     freeClusters;
FATFS     *pFatFs;
FIL       fil;

int main(void)
{
  char    dataRd[75];
  UINT    readBytes;

  //Max clock of 72MHz
  rcc_HSE_config();
  rcc_SysTick_config(72000);
  //UART Configuration
  uart_UART1_GPIO_config();
  uart_UART1_config();
  //GPIO Configuration
  gpio_LED_config();
  gpio_LED_writeGreen(0);
  gpio_LED_writeRed(0);
  //GPIO Push button configuration
  gpio_PB_config();
  //GPIO switch configuration
  gpio_SW_config();
  //SD card initialization
  spi_SD_CS_config();
  spi_GPIO_config();
  spi_config();
  MX_FATFS_Init();
  rcc_msDelay(100);
  //MIC ADC
  adc_GPIO_config();
  adc_MIC_config();
  tim_TIM3_MIC_config();

  //Mount SD card
  fresult = f_mount(&fs, "", 1);
  if (fresult != FR_OK)
  {
    printf("Failed to mount SD card to FatFs\n");
  }
  else
  {
    printf("Successfully Mounted SD card\n");
  }

  printf("SD card result = %d\n", fresult);

  /* Loop forever */
  while(1)
  {
    gpio_LED_toggleGreen();
    rcc_msDelay(500);
    //WAV Recorder
    if(gpio_PB_read())
    {
      if(wav_recorder_fileCreate("Audio1.wav"))
      {
        printf("Audio file created Successfully\n");
        rcc_msDelay(1000);
        gpio_LED_writeRed(1);
        printf("Recording started...\n");
        wav_recorder_record();
        while(!wav_recorder_isFinished())
        {
          wav_recorder_process();
          if(gpio_PB_read())
          {
            wav_recorder_stop();
            wav_recorder_process();
            printf("Recording stopped\n");
          }
        }
        gpio_LED_writeRed(0);
        rcc_msDelay(1000);
      }
      else
      {
        printf("Audio file creation failed\n");
      }
    }
  }
}

//DMA1_Channel1 Interrupt callback
void DMA1_Channel1_IRQHandler(void)
{
  //Half Transfer
  if(DMA1->ISR & DMA_ISR_HTIF1)
  {
    DMA1->IFCR |= DMA_IFCR_CHTIF1;
    wavRecorder_halfTransfer_Callback();
  }
  else if(DMA1->ISR & DMA_ISR_TCIF1) //Full Transfer
  {
    DMA1->IFCR |= DMA_IFCR_CTCIF1;
    wavRecorder_fullTransfer_Callback();
  }
  NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
}

