#include "main.h"
#include "fatfs.h"
#include "wav_recorder.h"

#include "cmsis_os.h"

//Threat1
osThreadId  Thread1Handle;

void Thread1Handler(void const *argument);

//Threat2
osThreadId  Thread2Handle;

void Thread2Handler(void const *argument);

int main(void)
{
  //Max clock of 72MHz
  rcc_HSE_config();
  rcc_SysTick_config(72000);
  tim_TIM2_initTick();
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

  //Thread1 Initialization
  osThreadDef(Thread1, Thread1Handler, osPriorityNormal, 0, 128);
  Thread1Handle = osThreadCreate(osThread(Thread1), NULL);

  //Thread2 Initialization
  osThreadDef(Thread2, Thread2Handler, osPriorityNormal, 0, 128);
  Thread1Handle = osThreadCreate(osThread(Thread2), NULL);

  //Start Schedular
  osKernelStart();

  /* Loop forever */
  while(1)
  {

  }
}


void Thread1Handler(void const *argument)
{
  while(1)
  {
    gpio_LED_toggleGreen();
    osDelay(500);
  }
}

void Thread2Handler(void const *argument)
{
  while(1)
  {
    gpio_LED_toggleRed();
    osDelay(750);
  }
}
