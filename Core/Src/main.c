#include "main.h"

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
  myTimeDate.hour = 20;
  myTimeDate.min = 45;
  myTimeDate.sec = 0;
  myTimeDate.month = 8;
  myTimeDate.mday = 30;
  myTimeDate.year = 2021;
  rtc_set_TimeDate(&myTimeDate);

  /* Loop forever */
  while(1)
  {
    printf("RTC Counter = %d ::", (int)rtc_get_unix_time());
    rtc_get_TimeDate(&myTimeDate);
    printf("Time & Date %d:%d:%d %d.%d.%d %s\n\r",
        (int)myTimeDate.hour,
        (int)myTimeDate.min,
        (int)myTimeDate.sec,
        (int)myTimeDate.mday,
        (int)myTimeDate.month,
        (int)myTimeDate.year,
        daysOfWeekString[(int)myTimeDate.wday - 1]);
    rcc_msDelay(1000);
  }
}
