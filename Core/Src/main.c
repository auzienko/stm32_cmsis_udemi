#include "main.h"

bool      adcEoc = false;
bool      adcAwd = false;
uint16_t  adcArray[3] = {0};

int main(void)
{
  uint16_t injAdcValue;

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
  //ADC
  adc_GPIO_config();
  adc_multiChannel_config();
  adc_multiChannel_DMA_config(adcArray);
  tim_TIM3_config();
  adc_AWD_config(2, 100, 2000);
  adc_start();
  adc_INJ_config(1);
  injAdcValue = 0;

  /* Loop forever */
  while(1)
  {
    if (adcEoc)
    {
      adcEoc = false;
      for (uint8_t i = 0; i < 3; i++)
      {
        printf("ADC Channel[%d] = %d\n\r", (int)(i + 1), (int)adcArray[i]);
      }
      printf("-----------------\n\r");
      if (adcAwd)
      {
        adcAwd = false;
        printf("AWD is Triggered!\n\r");
      }
    }
    if(gpio_PB_read())
    {
      adc_INJ_start();
      if (adc_INJ_pollForConversion(100))
      {
        injAdcValue = ADC1->JDR1;
        printf("INJ ADC Value = %d\n\r", (int)(injAdcValue));
      }
      rcc_msDelay(100);
    }
  }
}

//DMA Transfer Complete Interrupt
void DMA1_Channel1_IRQHandler(void)
{
  //Check Interrupt
  //in DMA interrupt status register (DMA_ISR)
  //TCIFx: Channelx transfer complete flag(x=1..7)
  if (DMA1->ISR & DMA_ISR_TCIF1)
  {
    //Clear Interrupt
    NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);

    // Clear
    //in DMA interrupt flag clear register (DMA_IFCR)
    //CTCIFx: Channelx transfer complete clear(x=1..7)
    DMA1->IFCR |= DMA_IFCR_CTCIF1;

    //Signal to application
    gpio_LED_toggleRed();
    adcEoc = true;
  }
}

//ADC AWD Interrupt
void ADC1_2_IRQHandler(void)
{
  //Is AWD?
  //in ADC status register (ADC_SR)
  //AWD:Analog watch dog flag
  if (ADC1->SR & ADC_SR_AWD)
  {
    //Clear AWD flag
    //in ADC status register (ADC_SR)
    //AWD:Analog watch dog flag
    ADC1->SR &= ~(ADC_SR_AWD);
    adcAwd = true;
  }
  //Clear Interrupt
  NVIC_ClearPendingIRQ(ADC1_2_IRQn);
}
