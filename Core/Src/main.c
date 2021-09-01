#include "main.h"

bool alarmFlag = false;

int main(void)
{
  RTC_TimeDate_t myTimeDate;

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
  //RTC
  rtc_LSE_config();
  rtc_config();
  //Backup registers 1 - 10
  rtc_backup_registers_enable();
  BKP->DR1 = 0x1144; //test data for save in BKP

  /* Loop forever */
  while(1)
  {
    printf("BKP->DR1 = %d ::\n\r", (int)BKP->DR1);
    rcc_msDelay(1000);
  }
}
