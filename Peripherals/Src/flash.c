/*
 * flash.c
 *
 *  Created on: Sep 13, 2021
 *      Author: aleksandruzienko
 */

#include "flash.h"

/*
 * @brief Unlock function
 */
void flash_unlock(void)
{
  //Is flash already locked?
  //in Flash programming manual (PM0068)
  //in Flash control register (FLASH_CR)
  //LOCK:Lock
  if (FLASH->CR & FLASH_CR_LOCK)
  {
    //Key values
    //The key values are as follows:
    //● RDPRT key = 0x00A5
    //● KEY1 = 0x45670123
    //● KEY2 = 0xCDEF89AB
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
  }

}

/*
 * @brief Lock function
 */
void flash_lock(void)
{
  //Is flash already locked?
  //in Flash programming manual (PM0068)
  //in Flash control register (FLASH_CR)
  //LOCK:Lock
  FLASH->CR |= FLASH_CR_LOCK;
}

/*
 * @brief Erase function
 */
void flash_erase(uint32_t pageAddrs)
{
  //● Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH_CR register
  //● Set the PER bit in the FLASH_CR register
  //● Program the FLASH_AR register to select a page to erase
  //● Set the STRT bit in the FLASH_CR register
  //● Wait for the BSY bit to be reset
  //● Read the erased page and verify

  //Wait BSY Flag
  //in Flash status register (FLASH_SR)
  //BSY:Busy
  while(FLASH->SR & FLASH_SR_BSY);

  //Enable PER bit
  //in Flash control register (FLASH_CR)
  //PER:Page erase
  FLASH->CR |= FLASH_CR_PER; //Page Erase chosen

  //Set Page Address
  //in Flash address register (FLASH_AR)
  //FAR:Flash Address
  FLASH->AR = pageAddrs;

  //Start Erase
  //in Flash control register (FLASH_CR)
  //STRT:Start
  FLASH->CR |= FLASH_CR_STRT;

  //Wait BSY Flag
  //in Flash status register (FLASH_SR)
  //BSY:Busy
  while(FLASH->SR & FLASH_SR_BSY);

  //Disable PER bit
  //in Flash control register (FLASH_CR)
  //PER:Page erase
  FLASH->CR &= ~(FLASH_CR_PER);


}

/*
 * @brief Write - 16 bits
 */
void flash_write16(uint32_t addrs, uint16_t *pBuf, uint32_t len)
{
  //Set PG bit
  //in Flash control register (FLASH_CR)
  //PG:Programming
  FLASH->CR |= FLASH_CR_PG; //Flash programming chosen.

  //For loop
  for (uint32_t i = 0; i < len; i++)
  {
    //Write to the desired address u16
    *(__IO uint16_t *)addrs = pBuf[i];
    addrs += 2; //16bits are 2 bytes
  }

  //Wait for BSY
  //in Flash status register (FLASH_SR)
  //BSY:Busy
  while(FLASH->SR & FLASH_SR_BSY);

  //Clear PG bit
  //in Flash control register (FLASH_CR)
  //PG:Programming
  FLASH->CR &= ~(FLASH_CR_PG);
}

/*
 * @brief Read - 16 bits
 */
void flash_read16(uint32_t addrs, uint16_t *pBuf, uint32_t len)
{
  //For loop
  for (uint32_t i = 0; i < len; i++)
  {
    pBuf[i] = *(__IO uint16_t *)addrs;
    addrs += 2; //16bits are 2 bytes
  }
}
