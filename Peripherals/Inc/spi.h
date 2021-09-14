/*
 * spi.h
 *
 *  Created on: Sep 4, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "main.h"

/*
 * @brief SPI GPIO pins setup
 * CS - PA4, SCK - PA5, MOSI - PA7, MISO - PA6
 */
void spi_GPIO_config(void);

/*
 * @brief SPI Peripheral configuration
 */
void spi_config(void);

/*
 * @brief SPI Transmit
 */
bool spi_transmit(uint8_t *pData, uint8_t len, uint32_t timeout);

/*
 * @brief SPI Receive
 */
bool spi_receive(uint8_t *pData, uint8_t len, uint32_t timeout);

/*
 * @brief SPI Transmit/Receive
 */
bool spi_transmitReceive(uint8_t *pDataTx, uint8_t *pDataRx, uint8_t len, uint32_t timeout);

/*
 * @brief RFID Chip Select (CS) pin configuration
 */
void spi_RFID_CS_config(void);

/*
 * @brief RFID Chip Select (CS) Set/Reset
 */
void spi_RFID_CS_write(bool state);

/*
 * @brief SD Chip Select (CS) pin configuration
 */
void spi_SD_CS_config(void);

/*
 * @brief SD Chip Select (CS) Set/Reset
 */
void spi_SD_CS_write(bool state);

#endif /* INC_SPI_H_ */
