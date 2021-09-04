/*
 * spi.c
 *
 *  Created on: Sep 4, 2021
 *      Author: aleksandruzienko
 */

#include "spi.h"

/*
 * @brief SPI GPIO pins setup
 * CS - PA4, SCK - PA5, MOSI - PA7, MISO - PA6
 */
void spi_GPIO_config(void)
{
  //Remap
  //check in SPI1 alternate function remapping (page 181)

  //Enable PA clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //IOPAEN:IO port A clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //1: IO port A clock enabled

  //PA5 (SCK) - output
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits(y=0..7
  //CNFy[1:0]:Portx configuration bits(y=0..7)
  GPIOA->CRL &= ~(GPIO_CRL_MODE5); //00: Input mode (reset state)
  GPIOA->CRL |= GPIO_CRL_MODE5_0; //01: Output mode, max speed 10 MHz.
  GPIOA->CRL &= ~(GPIO_CRL_CNF5); //00: General purpose output push-pull
  GPIOA->CRL |= GPIO_CRL_CNF5_1; //10: Alternate function output Push-pull

  //PA7 (MOSI) - output
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits(y=0..7
  //CNFy[1:0]:Portx configuration bits(y=0..7)
  GPIOA->CRL &= ~(GPIO_CRL_MODE7); //00: Input mode (reset state)
  GPIOA->CRL |= GPIO_CRL_MODE7_0; //01: Output mode, max speed 10 MHz.
  GPIOA->CRL &= ~(GPIO_CRL_CNF7); //00: General purpose output push-pull
  GPIOA->CRL |= GPIO_CRL_CNF7_1; //10: Alternate function output Push-pull

  //PA6 (MISO) - input
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits(y=0..7
  //CNFy[1:0]:Portx configuration bits(y=0..7)
  GPIOA->CRL &= ~(GPIO_CRL_MODE6); //00: Input mode (reset state)
  GPIOA->CRL &= ~(GPIO_CRL_CNF6); //clear
  GPIOA->CRL |= GPIO_CRL_CNF6_0; //01: Floating input (reset state)
}

/*
 * @brief SPI Peripheral configuration
 */
void spi_config(void)
{
  __IO uint32_t tempRd;

  //Enable SPI clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //SPI1EN:SPI1 clock enable
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //1: SPI1 clock enabled

  //Clock Phase
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //CPHA:Clock phase
  SPI1->CR1 |= SPI_CR1_CPHA; //1: The second clock transition is the first data capture edge

  //Clock polarity
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //CPOL:Clock polarity
  SPI1->CR1 |= SPI_CR1_CPOL; //1: CK to 1 when idle

  //Mode Master/Slave
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //MSTR:Master selection
  SPI1->CR1 |= SPI_CR1_MSTR; //1: Master configuration

  //Baud rate
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //BR[2:0]:Baud rate control
  SPI1->CR1 &= ~(SPI_CR1_BR); //clear
  SPI1->CR1 |= (SPI_CR1_BR_2 | SPI_CR1_BR_1); //110: fPCLK/128

  //MSB first
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //LSBFIRST:Frame format
  SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); //0: MSB transmitted first

  //Duplex
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //RXONLY:Receive only
  SPI1->CR1 &= ~(SPI_CR1_RXONLY); //0: Full duplex (Transmit and receive)

  //Data format 8/16
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //DFF:Data frame format
  SPI1->CR1 &= ~(SPI_CR1_DFF); //0: 8-bit data frame format is selected for transmission/reception

  //Slave select
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //SSM:Software slave management
  //SSI:Internal slave select
  SPI1->CR1 &= ~(SPI_CR1_SSM | SPI_CR1_SSI); //0: Software slave management disabled

  //Enable SPI
  //in SPI control register 1 (SPI_CR1) (not used in I2S mode)
  //SPE:SPI enable
  SPI1->CR1 |= SPI_CR1_SPE; //1: Peripheral enabled

  //Clear all flags
  //SPI status register (SPI_SR)
  //read to clear
  tempRd = SPI1->SR;
  (void) tempRd;
}

/*
 * @brief SPI Transmit
 */
bool spi_transmit(uint8_t *pData, uint8_t len, uint32_t timeout);

/*
 * @brief SPI Receive
 */
bool spi_recieve(uint8_t *pData, uint8_t len, uint32_t timeout);

/*
 * @brief RFID Chip Select (CS) pin configuration
 */
void spi_RFID_CS_config(void);

/*
 * @brief Chip Select (CS) Set/Reset
 */
void spi_RFID_CS_write(bool state);
