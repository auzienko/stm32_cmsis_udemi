/*
 * i2c.h
 *
 *  Created on: Sep 7, 2021
 *      Author: aleksandruzienko
 */

#include "i2c.h"

/*
 * @brief I2C GPIO pins configuration
 * SCL PB8, SDA PB9, INT PB12
 */
void i2c_I2C1_GPIO_config(void)
{
  //Enable PB Clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //IOPBEN:IO port B clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //1: IO port B clock enabled

  //AFIO Enable
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //AFIOEN:Alternate function IO clock enable
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //1: Alternate Function IO clock enabled

  //Mode Output 10MHz;
  //in Port configuration register high (GPIOx_CRH) (x=A..G)
  //MODEy[1:0]:Port x mode bits (y=8..15)
  GPIOB->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_MODE9); //reset
  GPIOB->CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE9_0); //01: Output mode, max speed 10 MHz.

  //CNF Alternate Function Open Drain
  //in Port configuration register high (GPIOx_CRH) (x=A..G)
  //CNFy[1:0]:Port x configuration bits (y=8..15)
  GPIOB->CRH |= (GPIO_CRH_CNF8 | GPIO_CRH_CNF9); //11: Alternate function output Open-drain

  //Remap
  //in AF remap and debug I/O configuration register (AFIO_MAPR)
  //I2C1_REMAP:I2C1 remapping
  AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP; //1: Remap (SCL/PB8, SDA/PB9)
}

/*
 * @brief I2C Peripheral configuration
 */
void i2c_I2C1_config(void)
{
  //Enable I2C Clock
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //I2C1EN:I2C1 clock enable
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //1: I2C1 clock enabled

  //Tell I2C register APB1 Clock Frequency
  //in I2C Control register 2 (I2C_CR2)
  //FREQ[5:0]:Peripheral clock frequency
  I2C1->CR2 &= ~(I2C_CR2_FREQ); //clear;
  I2C1->CR2 |= (36Ul << I2C_CR2_FREQ_Pos); //set 36MHz;

  //Rise Time
  //I2C TRISE register (I2C_TRISE)
  //TRISE[5:0]:Maximum rise time in Fm/Sm mode(Master mode)
  //For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns.
  //If, in the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and TPCLK1 = 125 ns
  //therefore the TRISE[5:0] bits must be programmed with 09h.
  I2C1->TRISE &= ~(I2C_TRISE_TRISE); //clear
  I2C1->TRISE |= (37Ul << I2C_TRISE_TRISE_Pos); //36 + 1 = 37

  //I2C Speed (100kHz)
  //in I2C Clock control register (I2C_CCR)
  //CCR[11:0]:Clock control register in Fm/Sm mode(Master mode)
  //For instance: in Sm mode, to generate a 100 kHz SCL frequency:
  //If FREQ = 08, TPCLK1 = 125 ns so CCR must be programmed with 0x28 (0x28 <=> 40d x 125 ns = 5000 ns.)
  // 100kHz = 100000
  // Period = 1/100000 = 0.00001
  // Period in ms = 0.00001 * 1000 = 0.01 ms or 10 microseconds
  // Period = T high + T low; T high == T low; => T high = Period / 2 = 5 microseconds
  // 5 / (1/36) = 180 => CCR = 180 = 0xB4;
  I2C1->CCR = 180; //--> 100kHz SCL

  //Enable I2C peripheral
  //in I2C Control register 1 (I2C_CR1)
  //PE:Peripheral enable
  I2C1->CR1 |= I2C_CR1_PE; //1: Peripheral enable
}

/*
 * @brief I2C Check Address
 */
bool i2c_I2C1_isSlaveAddressExist(uint8_t addrs, uint32_t timeout)
{
  uint32_t      initialTicks;
  __IO uint16_t clearRead;

  initialTicks = rcc_msGetTicks();

  //Wait for busy
  //I2C Status register 2 (I2C_SR2)
  //BUSY:Bus busy
  while(I2C1->SR2 & I2C_SR2_BUSY) //1: Communication ongoing on the bus
  {
    if ((rcc_msGetTicks() - initialTicks) > timeout)
      return (false);
  }

  //Send Start condition
  //in I2C Control register 1 (I2C_CR1)
  //POS:Acknowledge/PEC Position (for data reception)
  I2C1->CR1 &= ~(I2C_CR1_POS);
  //START:Start generation
  I2C1->CR1 |= I2C_CR1_START; //1: Repeated start generation
  //Wait for Start condition generated
  //in I2C Status register 1 (I2C_SR1)
  //SB:Start bit (Master mode)
  while(!(I2C1->SR1 & I2C_SR1_SB)) //1: Start condition generated.
  {
    if ((rcc_msGetTicks() - initialTicks) > timeout)
      return (false);
  }

  //Send slave address
  //in I2C Data register (I2C_DR)
  //DR[7:0]8-bit data register
  I2C1->DR = addrs;

  //Wait for ACK
  //in I2C Status register 1 (I2C_SR1)
  //ADDR:Address sent (master mode)/matched(slave mode)
  while(!(I2C1->SR1 & I2C_SR1_ADDR)) //1: End of address transmission.
  {
    if ((rcc_msGetTicks() - initialTicks) > timeout)
      return (false);
  }

  //Generate Stop condition
  //in I2C Control register 1 (I2C_CR1)
  //STOP:Stop generation
  I2C1->CR1 |= I2C_CR1_STOP; //1: Stop generation after the current byte transfer or after the current Start condition is sent.

  //Clear ADDRS flag
  //clearing sequence (READ SR1 then READ SR2)
  clearRead = I2C1->SR1;
  clearRead = I2C1->SR2;
  (void)clearRead;

  //Wait for busy
  //I2C Status register 2 (I2C_SR2)
  //BUSY:Bus busy
  while(I2C1->SR2 & I2C_SR2_BUSY) //1: Communication ongoing on the bus
  {
    if ((rcc_msGetTicks() - initialTicks) > timeout)
      return (false);
  }
  return (true);
}

/*
 * @brief I2C Transmit (Master)
 */
bool i2c_I2C1_masterTransmit(uint8_t addrs, uint8_t *pData, uint16_t len, uint32_t timeout);

/*
 * @brief I2C Receive (Master)
 */
bool i2c_I2C1_masterReceive(uint8_t addrs, uint8_t *pData, uint16_t len, uint32_t timeout);
