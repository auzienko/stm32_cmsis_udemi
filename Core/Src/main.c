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
  //Encoder
  tim_TIM4_Enconder_GPIO_config();
  tim_TIM4_Encoder_config();


  /* Loop forever */
  while(1)
  {
    printf("Encoder Counter = %d\n\r", (int)TIM4->CNT);
    printf("Direction: %s\n\r", ((TIM4->CR1) & TIM_CR1_DIR) == 0 ? "CW":"CCW");
    gpio_LED_toggleGreen();
    rcc_msDelay(250);
  }
}
