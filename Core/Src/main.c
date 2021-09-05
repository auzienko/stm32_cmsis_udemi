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
  //SPI Configuration;
  spi_RFID_CS_config();
  spi_GPIO_config();
  spi_config();
  //Read RFID registers using SPI
  for(uint8_t i = 0; i < 0x40; i++)
  {
    printf("Register[0x%02X] = 0x%02X\n", i,  rc522_regRead8(i));
    rcc_msDelay(50);
  }

  /* Loop forever */
  while(1)
  {

    rcc_msDelay(500);
  }
}

