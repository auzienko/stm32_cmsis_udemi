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
  //TIM2 - Delay
  //tim_TIM2_delay_config();
  //TIM2 Periodic
  tim_TIM2_periodic_config(300);

  /* Loop forever */
  while(1)
  {
    gpio_LED_toggleGreen();
    rcc_msDelay(1000);
  }
}

void TIM2_IRQHandler(void)
{
  //is TIM2 interrupt?
  //in TIMx status register (TIMx_SR)
  //UIF:Update interrupt flag
  if (TIM2->SR & TIM_SR_UIF)
  {
    //Clear flag
    TIM2->SR &= ~(TIM_SR_UIF);
    //Clear NVIC
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    //Indication
    gpio_LED_toggleRed();
  }
}
