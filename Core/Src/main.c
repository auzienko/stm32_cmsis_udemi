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
  //TIM1-CH1N -->PB13 Output Compare
  tim_TIM1_CH1N_GPIO_config();
  tim_TIM1_OC_config(300);

  /* Loop forever */
  while(1)
  {
    printf("Output Compare is Active, TIM1 Counter = %d\n\r", (int)(TIM1->CNT));
    rcc_msDelay(1000);
  }
}
