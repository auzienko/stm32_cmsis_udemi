#include "main.h"

int main(void)
{
  uint16_t wdata[3] = {0x1122, 0x3344, 0x5566};
  uint16_t rdata[3] = {0};

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

  //Flash -- 0x08007C00
  //Write
  flash_unlock();
  flash_erase(0x08007C00);
  flash_write16(0x08007C00, wdata, 3);
  flash_lock();

  //Read
  flash_read16(0x08007C00, rdata, 3);
  for (uint8_t i = 0; i < 3; i++)
  {
    printf("rdata[%u] = %X\n", i, rdata[i]);
  }

  /* Loop forever */
  while(1)
  {
    gpio_LED_toggleGreen();
    rcc_msDelay(500);
  }
}
