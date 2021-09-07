/*
 * i2c.h
 *
 *  Created on: Sep 7, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "main.h"

/*
 * @brief I2C GPIO pins configuration
 * SCL PB8, SDA PB9, INT PB12
 */
void i2c_I2C1_GPIO_config(void);

/*
 * @brief I2C Peripheral configuration
 */
void i2c_I2C1_config(void);

/*
 * @brief I2C Check Address
 */
bool i2c_I2C1_isSlaveAddressExist(uint8_t addrs, uint32_t timeout);

/*
 * @brief I2C Transmit (Master)
 */
bool i2c_I2C1_masterTransmit(uint8_t addrs, uint8_t *pData, uint16_t len, uint32_t timeout);

/*
 * @brief I2C Receive (Master)
 */
bool i2c_I2C1_masterReceive(uint8_t addrs, uint8_t *pData, uint16_t len, uint32_t timeout);

#endif /* INC_I2C_H_ */
