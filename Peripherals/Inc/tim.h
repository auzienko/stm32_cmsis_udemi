/*
 * tim.h
 *
 *  Created on: Aug 21, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

#include "main.h"

/*
 * @brief TIM3 ADC TRGO (trigger output) timer configuration
 */
void tim_TIM3_config(void);

/*
 * @brief Delay timer configuration - TIM2
 */
void tim_TIM2_delay_config(void);

/*
 * @brief milli second delay - TIM2
 */
void tim_TIM2_msDelay(uint32_t ms);

/*
 * @brief Periodic TIM2 Interrupt
 */
void tim_TIM2_periodic_config(uint32_t msPeriod);

/*
 * @brief Configure TIM1_CH1N --> PB13
 */
void tim_TIM1_CH1N_GPIO_config(void);

/*
 * @brief TIM1 Output Compare (OC) configuration
 */
void tim_TIM1_OC_config(uint32_t msPeriod);

#endif /* INC_TIM_H_ */
