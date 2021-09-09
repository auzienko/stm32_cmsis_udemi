#include "main.h"

#define MPU6050_I2C_ADDRS (0x68 << 1)

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

  //Test CRC
  crc_reset();
  crc_accumulate(0x00110022);
  crc_accumulate(0x00330044);
  //Read
  printf("CRC is %08lX\n", crc_read());

  /* Loop forever */
  while(1)
  {

    rcc_msDelay(500);
  }
}

