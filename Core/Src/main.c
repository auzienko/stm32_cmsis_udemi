#include "main.h"

bool  uartDmaRxFlag = false;
bool  uartDmaTxFlag = false;

int main(void)
{
  uint8_t rxBuf[10];

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
  //DMA UART1 configuration
  uart_UART1_DMA_config();

  printf("Write 5 bytes to terminal:\n");
  //Open DMA Receive
  uart_UART1_DMA_receive(rxBuf, 5);

  /* Loop forever */
  while(1)
  {
    if(uartDmaRxFlag)
    {
      uartDmaRxFlag = false;
      uart_UART1_DMA_transmit(rxBuf, 5);
      while (!uartDmaTxFlag);
      uartDmaTxFlag = false;
      //Open DMA Receive
      uart_UART1_DMA_receive(rxBuf, 5);
    }

    //DMA Counter
    //from DMA channel x number of data register (DMA_CNDTRx) (x = 1..7, where x = channel number)
    printf("DMA Receive Counter = %d\n", (int)DMA1_Channel5->CNDTR);
    gpio_LED_toggleGreen();
    rcc_msDelay(500);
  }
}

//UART DMA - RX
void  DMA1_Channel5_IRQHandler(void)
{
  if(DMA1->ISR & DMA_ISR_TCIF5) //TCIFx:Channelx transfer complete flag(x=1..7)
  {
    //> Disable DMA Channel 5
    //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
    //> EN:Channel enable
    DMA1_Channel5->CCR &= ~(DMA_CCR_EN); //0: Channel disabled

    //Clear flags
    //in DMA interrupt flag clear register (DMA_IFCR)
    //CTCIFx:Channelx transfer complete clear(x=1..7)
    //CGIFx:Channelx global interrupt clear(x=1..7)
    DMA1->IFCR |= DMA_IFCR_CTCIF5; //1: Clears the corresponding TCIF flag in the DMA_ISR register
    DMA1->IFCR |= DMA_IFCR_CGIF5; //1: Clears the GIF, TEIF, HTIF and TCIF flags in the DMA_ISR register

    //Notify application
    uartDmaRxFlag = true;
  }
  //Transfer error handling
  if(DMA1->ISR & DMA_ISR_TEIF5) //TEIFx:Channelx transfer error flag(x=1..7)
  {
    //Clear flags
    //in DMA interrupt flag clear register (DMA_IFCR)
    //CGIFx:Channelx global interrupt clear(x=1..7)
    //CTEIFx:Channelx transfer error clear(x=1..7)
    DMA1->IFCR |= DMA_IFCR_CGIF5; //1: Clears the GIF, TEIF, HTIF and TCIF flags in the DMA_ISR register
    DMA1->IFCR |= DMA_IFCR_CTEIF5; //1: Clears the corresponding TEIF flag in the DMA_ISR register
  }
}

//UART DMA - TX
void  DMA1_Channel4_IRQHandler(void)
{
  if(DMA1->ISR & DMA_ISR_TCIF4) //TCIFx:Channelx transfer complete flag(x=1..7)
  {
    //> Disable DMA Channel 4
    //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
    //> EN:Channel enable
    DMA1_Channel4->CCR &= ~(DMA_CCR_EN); //0: Channel disabled

    //Clear flags
    //in DMA interrupt flag clear register (DMA_IFCR)
    //CTCIFx:Channelx transfer complete clear(x=1..7)
    //CGIFx:Channelx global interrupt clear(x=1..7)
    DMA1->IFCR |= DMA_IFCR_CTCIF4; //1: Clears the corresponding TCIF flag in the DMA_ISR register
    DMA1->IFCR |= DMA_IFCR_CGIF4; //1: Clears the GIF, TEIF, HTIF and TCIF flags in the DMA_ISR register

    //Notify application
    uartDmaTxFlag = true;
  }
  //Transfer error handling
  if(DMA1->ISR & DMA_ISR_TEIF4) //TEIFx:Channelx transfer error flag(x=1..7)
  {
    //Clear flags
    //in DMA interrupt flag clear register (DMA_IFCR)
    //CGIFx:Channelx global interrupt clear(x=1..7)
    //CTEIFx:Channelx transfer error clear(x=1..7)
    DMA1->IFCR |= DMA_IFCR_CGIF4; //1: Clears the GIF, TEIF, HTIF and TCIF flags in the DMA_ISR register
    DMA1->IFCR |= DMA_IFCR_CTEIF4; //1: Clears the corresponding TEIF flag in the DMA_ISR register
  }
}
