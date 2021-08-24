/*
 * rcc.h
 *
 *  Created on: Aug 14, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include "main.h"

/**
 * @brief HSE configuration
 */
void rcc_HSE_config(void);

/*
 * @brief SysTick configuration
 */
void rcc_SysTick_config(uint32_t arr);

/*
 * @brief Increment ms Ticks
 */
void rcc_msIncTicks(void);

/*
 * @brief Get ms Ticks
 */
uint32_t rcc_msGetTicks(void);

/*
 * @brief ms Delay
 */
void rcc_msDelay(uint32_t ms);

#endif /* INC_RCC_H_ */
