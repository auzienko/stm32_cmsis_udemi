#include "main.h"
#include "fatfs.h"

//FatFs variables
FRESULT   fresult;
FATFS     fs;
DWORD     freeClusters;
FATFS     *pFatFs;
uint32_t  total;
uint32_t  freeSpace;

int main(void)
{
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

  //Read SD card size and free space
  f_getfree("", &freeClusters, &pFatFs);
  total = (pFatFs->n_fatent - 2) * pFatFs->csize;
  freeSpace = freeClusters * pFatFs->csize;
  printf("Total size %lu KB\nAvailable size %lu KB\n", total/2, freeSpace/2);

  /* Loop forever */
  while(1)
  {
    gpio_LED_toggleGreen();
    rcc_msDelay(500);
  }
}
