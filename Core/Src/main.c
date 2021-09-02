#include "main.h"

int main(void)
{
  uint8_t data[10];


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

  printf("Write 5 bytes to terminal:\n\r");
  /* Loop forever */
  while(1)
  {
    if(uart_UART1_receive(data, 5, 2000))
    {
      uart_UART1_transmit(data, 5, 1000);
    }
  }
}
