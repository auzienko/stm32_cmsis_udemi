#include "main.h"

int main(void)
{
  uint8_t cardId[4];
  uint8_t myCardId[4] ={0x1C, 0xC4, 0x33, 0x31};

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
  //RFID Module
  rc522_init();

  //Simple RFID Card Reader
  printf("Place your card...\n");

  /* Loop forever */
  while(1)
  {
    if(rc522_checkCard(cardId))
    {
      printf("Card is detected with ID:\n");
      for(uint8_t i = 0; i < 4; i++)
      {
        printf("[%d]: 0x%02X\n", i, (unsigned int)cardId[i]);
      }
      if(rc522_compareIds(cardId, myCardId))
      {
        printf("Welcome!\n");
        gpio_LED_writeGreen(1);
      }
      else
      {
        printf("Error :: Access denied.\n");
        gpio_LED_writeRed(1);
      }
      printf("*************************\n");
      rcc_msDelay(2000);
      gpio_LED_writeRed(0);
      gpio_LED_writeGreen(0);
    }
    rcc_msDelay(500);
  }
}

