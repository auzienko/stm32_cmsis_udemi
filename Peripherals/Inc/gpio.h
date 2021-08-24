/*
 * gpio.h
 *
 *  Created on: Aug 17, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

//Bit-Banding
#define BB_BASE_ADDRS     0x42000000
#define GPIOB_ODR_OFFSET  0x00010c0c // 0x4001 0C00 (GPIO Port B) + Address offset: 0x0C (GPIOx_ODR) - 0x4000 0000
#define BB_PIN13          13
#define BB_PIN14          14
#define PB13_BB_ADDR      *(uint32_t *)(BB_BASE_ADDRS + GPIOB_ODR_OFFSET * 32 + BB_PIN13 * 4)
#define PB14_BB_ADDR      *(uint32_t *)(BB_BASE_ADDRS + GPIOB_ODR_OFFSET * 32 + BB_PIN14 * 4)

//Push button Bit-banding (now with CMSIS macros)
#define GPIOA_IDR_OFFSET  (GPIOA_BASE - PERIPH_BASE + 0x08)
#define PA0_BB_ADDR       *(uint32_t *)(PERIPH_BB_BASE + GPIOA_IDR_OFFSET * 32 + 0 * 4)


/*
 * @brief LEDs GPIO Configuration, PB13 (green), PB14 (red)
 */
void  gpio_LED_config(void);

/*
 * @brief Write/Toggle function for LEDs
 */
void gpio_LED_writeGreen(bool state);
void gpio_LED_writeRed(bool state);
void gpio_LED_toggleGreen(void);
void gpio_LED_toggleRed(void);

/*
 * @brief Button (input) configuration, PA0
 */
void gpio_PB_config(void); // PB - push button

/*
 * @brief Read push-button
 */
bool gpio_PB_read(void);

/*
 * brief Slide switches (input) configuration, PA8, PA15
 */
void gpio_SW_config (void);

/*
 * @brief Read switches SW1, SW2
 */
bool gpio_SW1_read(void);
bool gpio_SW2_read(void);

/*
 * @brief TM1637 LED Display GPIO pins configuration
 */
void gpio_tm1637_config(void);

#endif /* INC_GPIO_H_ */
