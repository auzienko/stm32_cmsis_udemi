#include "main.h"

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
  //TIM3 PWM LEDs
  tim_TIM3_PWM_config();
  tim_TIM3_GPIO_config();


  /* Loop forever */
  while(1)
  {
    //Red
    tim_PWM_setRGB(255, 0, 0);
    rcc_msDelay(2000);
    //Green
    tim_PWM_setRGB(0, 255, 0);
    rcc_msDelay(2000);
    //Blue
    tim_PWM_setRGB(0, 0, 255);
    rcc_msDelay(2000);
  }
}
