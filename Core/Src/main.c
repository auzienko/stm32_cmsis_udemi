#include "main.h"
#include "fatfs.h"

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

  //Open file for Read/Write
  fresult = f_open(&fil, "TestZ.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
  if (fresult != FR_OK)
  {
    printf("Failed to Open file\n");
  }
  else
  {
    printf("Successfully Opened file\n");
    //Write to the file
    f_puts("test string\nSTM32 tutorial", &fil);
    f_lseek(&fil, 0);
    readBytes = 0;
    fresult = f_read(&fil, dataRd, 75, &readBytes);
    if (fresult != FR_OK)
    {
      printf("Failed to Read file\n");
    }
    else
    {
      dataRd[readBytes] = '\0';
      printf("String :%s\n", dataRd);
    }
    f_close(&fil);
  }

  /* Loop forever */
  while(1)
  {
    gpio_LED_toggleGreen();
    rcc_msDelay(500);
  }
}
