#include "main.h"

#define MPU6050_I2C_ADDRS (0x68 << 1)

int main(void)
{
  uint8_t data;

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

  //Read Who Am I register
  I2C_Read(117, &data, 1);
  printf("Who Am I register value is %d\n", (int)data);

  //Read Accelerometer X, Y and Z values
  MPU_ConfigTypeDef myConfig;
  myConfig.Accel_Full_Scale = AFS_SEL_4g;
  myConfig.ClockSource = Internal_8MHz;
  myConfig.CONFIG_DLPF = DLPF_184A_188G_Hz;
  myConfig.Sleep_Mode_Bit = 0;
  myConfig.Gyro_Full_Scale = FS_SEL_500;
  MPU6050_Config(&myConfig);
  ScaledData_Def meAccess;

  /* Loop forever */
  while(1)
  {
    MPU6050_Get_Accel_Scale(&meAccess);
    printf("Accel: X: %.2f, Y: %.2f, Z: %.2f\n", meAccess.x, meAccess.y, meAccess.z);
    printf("\n-----------------------------\n");
    rcc_msDelay(500);
  }
}

