/*
 * uart.c
 *
 *  Created on: Aug 16, 2021
 *      Author: aleksandruzienko
 */

#include "uart.h"

/*
 * @brief UART1 GPIO Configuration
 */
void  uart_UART1_GPIO_config(void)
{
  //PA9 - Tx (transmit), PA10 - Rx (receive)
  //Enable PortA Clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  //Mode to AF (UART1)
  //in Port configuration register high (GPIOx_CRH)
  GPIOA->CRH &= ~(GPIO_CRH_CNF9);//clear
  GPIOA->CRH |= GPIO_CRH_CNF9_1; //10: Alternate function output Push-pull
  GPIOA->CRH &= ~(GPIO_CRH_CNF10);//clear
  GPIOA->CRH |= GPIO_CRH_CNF10_0; //01: Floating input (reset state)

  //Output max 10Mhz
  //in Port configuration register high (GPIOx_CRH)
  GPIOA->CRH &= ~(GPIO_CRH_MODE9);//clear
  GPIOA->CRH |= GPIO_CRH_MODE9_0;//01: Output mode, max speed 10 MHz.
  GPIOA->CRH &= ~(GPIO_CRH_MODE10);//clear & set 00: Input mode (reset state)

  //Map PA9, PA10 to UART1
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //and in AF remap and debug I/O configuration register (AFIO_MAPR) USART1_REMAP
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP); //clear and set 0: No remap (TX/PA9, RX/PA10)
}

/*
 * @brief UART1 Peripheral Configuration
 */
void  uart_UART1_config(void)
{
  //Enable UART1 Clock
  //in APB2 peripheral clock enable register
  //(RCC_APB2ENR) USART1EN
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  //Enable Transmit
  //in Control register 1 (USART_CR1)
  //TE (Transmitter enable)
  USART1->CR1 |= USART_CR1_TE; //1: Transmitter is enabled

  //Enable Receive
  //in Control register 1 (USART_CR1)
  //RE: Receiver enable
  USART1->CR1 |= USART_CR1_RE; //1: Receiver is enabled and begins searching for a start bit

  //Parity to Even
  //in Control register 1 (USART_CR1)
  //PS (Parity selection)
  USART1->CR1 &= ~(USART_CR1_PS); // 0: Even parity

  //Parity control enabled
  //in Control register 1 (USART_CR1)
  //PCE (Parity control enable)
  USART1->CR1 &= ~(USART_CR1_PCE);

  //Word length to 8-bits
  //in Control register 1 (USART_CR1)
  //M (Word length)
  USART1->CR1 &= ~(USART_CR1_M);

  //Stop bits to 1
  //in Control register 2 (USART_CR2)
  //STOP (STOP bits)
  USART1->CR2 &= ~(USART_CR2_STOP);

  //Disable HW Flow Control
  //in Control register 3 (USART_CR3)
  //CTSE (CTS enable) & RTSE (RTS enable)
  USART1->CR3 &= ~(USART_CR3_CTSE);
  USART1->CR3 &= ~(USART_CR3_RTSE);

  //Set Baud rate to 115200
  //in Baud rate register (USART_BRR)
  USART1->BRR = 0;
  USART1->BRR |= (39Ul << 4Ul); //set DIV_Fraction
  USART1->BRR |= (1Ul << 0Ul); //set DIV_Mantissa

  //Clear some flag and enable
  USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
  USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

  //Enable UART
  //in Control register 1 (USART_CR1)
  //UE (USART enable)
  USART1->CR1 |= USART_CR1_UE;
}

/*
 * @brief UART1 Transmit function
 */
bool  uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout)
{
  //Wait on TXE to start transmit
  //Write to DR as TXE flag is HIGH (Tx buffer Empty)
  uint8_t dataIdx;
  uint32_t  startTick;

  dataIdx = 0;
  startTick = rcc_msGetTicks();
  while (dataIdx < len)
  {
    if (USART1->SR & USART_SR_TXE) //Tx buffer empty
    {
      USART1->DR = data[dataIdx];
      dataIdx++;
    }
    else
    {
      if((rcc_msGetTicks() - startTick >= timeout))
        return (false);
    }}
  //wait for busy flag
  while (USART1->SR & USART_SR_TC)
  {
    if((rcc_msGetTicks() - startTick >= timeout))
      return (false);
  }
  return (true);
}

/*
 * @brief UART1 Transmit Receive
 */
bool  uart_UART1_receive(uint8_t *data, uint8_t len, uint32_t timeout)
{
  uint8_t dataRemain;
  uint32_t  startTick;

  dataRemain = len;
  startTick = rcc_msGetTicks();
  while (dataRemain > 0)
  {
    if (USART1->SR & USART_SR_RXNE) //RXNE:Read data register not empty
    {
      *data++ = (uint8_t)(USART1->DR & 0xFF); //Read data from Data Register
      dataRemain --;
    }
    else
    {
      if((rcc_msGetTicks() - startTick >= timeout))
        return (false);
    }
  }
  return (true);
}

/*
 * @brief UART1 DMA Configuration
 */
void uart_UART1_DMA_config(void)
{
  //Enable UART DMA
  //In Control register 3 (USART_CR3)
  //DMAT:DMA enable transmitter
  //DMAR:DMA enable receiver
  USART1->CR3 |= USART_CR3_DMAT; //1: DMA mode is enabled for transmission
  USART1->CR3 |= USART_CR3_DMAR; //1: DMA mode is enabled for reception

  //Enable DMA1 Clock
  //in AHB peripheral clock enable register (RCC_AHBENR)
  //DMA1EN: DMA1 clock enable
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;

  /* Table 78. Summary of DMA1 requests for each channel (ref man page 282)
   * USART1_TX - Channel 4, USART1_RX - Channel 5
   */

  //Clear DMA1 - Channel 4 status flags & Channel 5 status flags
  //in DMA interrupt flag clear register (DMA_IFCR)
  DMA1->IFCR |= 0x0F << 12; //clear all 4 flags Channel 4
  DMA1->IFCR |= 0x0F << 16; //clear all 4 flags Channel 5

  //TX Channel DMA Configuration

  //> Peripheral address - UART_DR
  //> in DMA channel x peripheral address register (DMA_CPARx) (x = 1..7, where x = channel number)
  DMA1_Channel4->CPAR = (uint32_t)(&(USART1->DR));

  //> Mode - Normal/Circuit
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> CIRC: Circular mode
  DMA1_Channel4->CCR &= ~(DMA_CCR_CIRC); // 0: Circular mode disabled

  //> Memory/Peripheral Increment
  //> Enable Memory increment
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> MINC: Memory increment mode
  DMA1_Channel4->CCR |= DMA_CCR_MINC; //1: Memory increment mode enabled
  //> Disable Peripheral increment
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> PINC: Peripheral increment mode
  DMA1_Channel4->CCR &= ~(DMA_CCR_PINC); //0: Peripheral increment mode disabled

  //> Set data size 8 bits <-> 8 bits
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> MSIZE[1:0]:Memory size
  //> PSIZE[1:0]:Peripheral size
  DMA1_Channel4->CCR &= ~(DMA_CCR_MSIZE); //00: 8-bits
  DMA1_Channel4->CCR &= ~(DMA_CCR_PSIZE); //00: 8-bits

  //> Direction Channel 4
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> DIR: Data transfer direction
  DMA1_Channel4->CCR |= DMA_CCR_DIR; //1: Read from memory

  //> Disable DMA Channel 4
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> EN:Channel enable
  DMA1_Channel4->CCR &= ~(DMA_CCR_EN); //0: Channel disabled

  //> Transfer complete Interrupt enable Channel 4
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> TCIE:Transfer complete interrupt enable
  DMA1_Channel4->CCR |= DMA_CCR_TCIE; //1: TC interrupt enabled

  //RX Channel DMA Configuration

  //> Peripheral address - UART_DR
  //> in DMA channel x peripheral address register (DMA_CPARx) (x = 1..7, where x = channel number)
  DMA1_Channel5->CPAR = (uint32_t)(&(USART1->DR));

  //> Mode - Normal/Circuit
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> CIRC: Circular mode
  DMA1_Channel5->CCR &= ~(DMA_CCR_CIRC); // 0: Circular mode disabled

  //> Memory/Peripheral Increment
  //> Enable Memory increment
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> MINC: Memory increment mode
  DMA1_Channel5->CCR |= DMA_CCR_MINC; //1: Memory increment mode enabled
  //> Disable Peripheral increment
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> PINC: Peripheral increment mode
  DMA1_Channel5->CCR &= ~(DMA_CCR_PINC); //0: Peripheral increment mode disabled

  //> Set data size 8 bits <-> 8 bits
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> MSIZE[1:0]:Memory size
  //> PSIZE[1:0]:Peripheral size
  DMA1_Channel5->CCR &= ~(DMA_CCR_MSIZE); //00: 8-bits
  DMA1_Channel5->CCR &= ~(DMA_CCR_PSIZE); //00: 8-bits

  //> Direction Channel 5
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> DIR: Data transfer direction
  DMA1_Channel5->CCR &= ~(DMA_CCR_DIR); //0: Read from peripheral

  //> Disable DMA Channel 5
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> EN:Channel enable
  DMA1_Channel5->CCR &= ~(DMA_CCR_EN); //0: Channel disabled

  //> Transfer complete Interrupt enable Channel 5
  //> in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //> TCIE:Transfer complete interrupt enable
  DMA1_Channel5->CCR |= DMA_CCR_TCIE; //1: TC interrupt enabled

  //Enable DMA Interrupt
  NVIC_SetPriority(DMA1_Channel4_IRQn, 6);
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  NVIC_SetPriority(DMA1_Channel5_IRQn, 6);
  NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

/*
 * @brief UART1 DMA Transmit
 */
void uart_UART1_DMA_transmit(uint8_t *data, uint8_t len)
{
  //Clear DMA1 - Channel 4 status flags
  //in DMA interrupt flag clear register (DMA_IFCR)
  DMA1->IFCR |= 0x0F << 12; //clear all 4 flags Channel 4

  //Set Memory Address
  //in DMA channel x memory address register (DMA_CMARx) (x = 1..7, where x = channel number)
  DMA1_Channel4->CMAR = (uint32_t)data;

  //Set Number of transfers
  //in DMA channel x number of data register (DMA_CNDTRx) (x = 1..7, where x = channel number)
  DMA1_Channel4->CNDTR = len;

  //Enable DMA Channel 4
  DMA1_Channel4->CCR |= DMA_CCR_EN; //1: Channel enabled
}

/*
 * @brief UART1 DMA Receive
 */
void uart_UART1_DMA_receive(uint8_t *data, uint8_t len)
{
  __IO uint32_t readTmp;

  //Clear overrun error, if we have any
  //in Status register (USART_SR)
  //ORE:Overrun error
  //It is cleared by a software sequence (an read to the USART_SR register followed by a read to the USART_DR register).
  readTmp = USART1->SR;
  readTmp = USART1->DR;
  (void)readTmp;

  //Clear DMA1 - Channel 5 status flags
  //in DMA interrupt flag clear register (DMA_IFCR)
  DMA1->IFCR |= 0x0F << 16; //clear all 4 flags Channel 5

  //Set Memory Address
  //in DMA channel x memory address register (DMA_CMARx) (x = 1..7, where x = channel number)
  DMA1_Channel5->CMAR = (uint32_t)data;

  //Set Number of transfers
  //in DMA channel x number of data register (DMA_CNDTRx) (x = 1..7, where x = channel number)
  DMA1_Channel5->CNDTR = len;

  //Enable DMA Channel 5
  DMA1_Channel5->CCR |= DMA_CCR_EN; //1: Channel enabled
}
