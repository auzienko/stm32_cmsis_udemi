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
  //Enable PortA Clock in APB2 peripheral clock enable register (RCC_APB2ENR)
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //Mode to AF (UART1) in Port configuration register high (GPIOx_CRH)
  GPIOA->CRH &= ~(GPIO_CRH_CNF9);//clear
  GPIOA->CRH |= GPIO_CRH_CNF9_1; //10: Alternate function output Push-pull
  GPIOA->CRH &= ~(GPIO_CRH_CNF10);//clear
  GPIOA->CRH |= GPIO_CRH_CNF10_0; //01: Floating input (reset state)
  //Output max 10Mhz in Port configuration register high (GPIOx_CRH)
  GPIOA->CRH &= ~(GPIO_CRH_MODE9);//clear
  GPIOA->CRH |= GPIO_CRH_MODE9_0;//01: Output mode, max speed 10 MHz.
  GPIOA->CRH &= ~(GPIO_CRH_MODE10);//clear & set 00: Input mode (reset state)
  //Map PA9, PA10 to UART1 in APB2 peripheral clock enable register (RCC_APB2ENR)
  //and in AF remap and debug I/O configuration register (AFIO_MAPR) USART1_REMAP
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP); //clear and set 0: No remap (TX/PA9, RX/PA10)
}

/*
 * @brief UART1 Peripheral Configuration
 */
void  uart_UART1_config(void)
{
  //Enable UART1 Clock in APB2 peripheral clock enable register (RCC_APB2ENR) USART1EN
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  //Enable Transmit in Control register 1 (USART_CR1) TE (Transmitter enable)
  USART1->CR1 |= USART_CR1_TE;
  //Parity to Even in Control register 1 (USART_CR1) PS (Parity selection)
  USART1->CR1 &= ~(USART_CR1_PS); // 0: Even parity
  //Parity control enabled in Control register 1 (USART_CR1) PCE (Parity control enable)
  USART1->CR1 &= ~(USART_CR1_PCE);
  //Word length to 8-bits in Control register 1 (USART_CR1) M (Word length)
  USART1->CR1 &= ~(USART_CR1_M);
  //Stop bits to 1 in Control register 2 (USART_CR2) STOP (STOP bits)
  USART1->CR2 &= ~(USART_CR2_STOP);
  //Disable HW Flow Control in Control register 3 (USART_CR3) CTSE (CTS enable) & RTSE (RTS enable)
  USART1->CR3 &= ~(USART_CR3_CTSE);
  USART1->CR3 &= ~(USART_CR3_RTSE);
  //Set Baud rate to 115200 in Baud rate register (USART_BRR)
  USART1->BRR = 0;
  USART1->BRR |= (39Ul << 4Ul); //set DIV_Fraction
  USART1->BRR |= (1Ul << 0Ul); //set DIV_Mantissa
  //Clear some flag and enable
  USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
  USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
  //Enable UART in Control register 1 (USART_CR1) UE (USART enable)
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
