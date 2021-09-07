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
  //I2C Configuration
  i2c_I2C1_GPIO_config();
  i2c_I2C1_config();

  //Test Slave Address
  if(i2c_I2C1_isSlaveAddressExist(MPU6050_I2C_ADDRS, 50))
  {
    printf("MPU6050 Detected\n");
  }
  else
  {
    printf("MPU6050 Not detected\n");
  }

  /* Loop forever */
  while(1)
  {
    rcc_msDelay(500);
  }
}

