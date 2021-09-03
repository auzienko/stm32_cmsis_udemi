/*
 * uart.h
 *
 *  Created on: Aug 16, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

/*
 * @brief UART1 GPIO Configuration
 */
void  uart_UART1_GPIO_config(void);

/*
 * @brief UART1 Peripheral Configuration
 */
void  uart_UART1_config(void);

/*
 * @brief UART1 Transmit function
 */
bool  uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout);

/*
 * @brief UART1 Transmit Receive
 */
bool  uart_UART1_receive(uint8_t *data, uint8_t len, uint32_t timeout);

/*
 * @brief UART1 DMA Configuration
 */
void uart_UART1_DMA_config(void);

/*
 * @brief UART1 DMA Transmit
 */
void uart_UART1_DMA_transmit(uint8_t *data, uint8_t len);

/*
 * @brief UART1 DMA Receive
 */
void uart_UART1_DMA_receive(uint8_t *data, uint8_t len);

#endif /* INC_UART_H_ */
