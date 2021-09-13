/*
 * flash.h
 *
 *  Created on: Sep 13, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"

/*
 * @brief Unlock function
 */
void flash_unlock(void);

/*
 * @brief Lock function
 */
void flash_lock(void);

/*
 * @brief Erase function
 */
void flash_erase(uint32_t pageAddrs);

/*
 * @brief Write - 16 bits
 */
void flash_write16(uint32_t addrs, uint16_t *pBuf, uint32_t len);

/*
 * @brief Read - 16 bits
 */
void flash_read16(uint32_t addrs, uint16_t *pBuf, uint32_t len);

#endif /* INC_FLASH_H_ */
