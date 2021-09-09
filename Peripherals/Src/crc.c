/*
 * crc.c
 *
 *  Created on: Sep 9, 2021
 *      Author: aleksandruzienko
 */

#include "crc.h"

/*
 * @brief CRC Enable
 */
void crc_enable(void)
{
  //1. Enable CRC Clock
  //2. Initiate a CRC Reset

  //Enable CRC Clock
  //in AHB peripheral clock enable register (RCC_AHBENR)
  //CRCEN:CRC clock enable
  RCC->AHBENR |= RCC_AHBENR_CRCEN; //1: CRC clock enabled

  //Initiate a CRC Reset
  crc_reset();
}

/*
 * @brief CRC Disable
 */
void crc_disable(void)
{
  //1. Disable CRC Clock

  //Disable CRC Clock
  //in AHB peripheral clock enable register (RCC_AHBENR)
  //CRCEN:CRC clock enable
  RCC->AHBENR &= ~(RCC_AHBENR_CRCEN); //0: CRC clock disabled
}

/*
 * @brief CRC Reset
 */
void crc_reset(void)
{
  //in Control register (CRC_CR)
  CRC->CR = 1;
}

/*
 * @brief CRC Accumulate
 */
void crc_accumulate(uint32_t value32)
{
  //in Data register (CRC_DR)
  CRC->DR = value32;
}

/*
 * @brief CRC Read
 */
uint32_t crc_read(void)
{
  //in Data register (CRC_DR)
  return (CRC->DR); //Calculated CRC from all our accumulated data points
}
