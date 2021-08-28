#include "main.h"
#include <time.h>

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
  //RTC
  rtc_LSE_config();
  rtc_config();
  rtc_set_unix_time(1000);



  /* Loop forever */
  while(1)
  {
    printf("RTC Counter = %d\n\r", (int)rtc_get_unix_time());
    rcc_msDelay(1000);
  }
}
