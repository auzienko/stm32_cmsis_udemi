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
  //CRC Configuration
  crc_enable();
  //Enable button Interrupt
  exti_PB_config();

  for (uint8_t i = 5; i > 0; i--)
  {
    printf("Entering stop mode in %d sec\n", i);
    rcc_msDelay(1000);
  }

  pwr_enter_stop_mode();

  //Re - configure System Clock
  rcc_HSE_config();
  rcc_SysTick_config(72000);
  printf("Woke up from stop mode\n");


  /* Loop forever */
  while(1)
  {
    gpio_LED_toggleGreen();
    rcc_msDelay(500);
  }
}

void EXTI0_IRQHandler(void)
{
  NVIC_ClearPendingIRQ(EXTI0_IRQn);

  //in Pending register (EXTI_PR)
  //PRx:Pending bit
  //This bit is set when the selected edge event arrives on the external interrupt line.
  //This bit is cleared by writing a ‘1’ into the bit.
  EXTI->PR = EXTI_PR_PR0;

  //Application...
}
