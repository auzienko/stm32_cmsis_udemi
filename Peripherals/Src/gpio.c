/*
 * gpio.c
 *
 *  Created on: Aug 17, 2021
 *      Author: aleksandruzienko
 */

#include "gpio.h"

/*
 * @brief LEDs GPIO Configuration, PB13 (green), PB14 (red)
 */
void  gpio_LED_config(void)
{
  //Enable Port B clock in APB2 peripheral clock enable register (RCC_APB2ENR) IOPBEN (IO port B clock enable)
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  //Mode to output in Port configuration register high (GPIOx_CRH) (x=A..G)
  GPIOB->CRH &= ~(GPIO_CRH_MODE13); //clear
  GPIOB->CRH |= GPIO_CRH_MODE13_1; //10: Output mode, max speed 2 MHz.
  GPIOB->CRH &= ~(GPIO_CRH_MODE14); //clear
  GPIOB->CRH |= GPIO_CRH_MODE14_1; //10: Output mode, max speed 2 MHz.
  //Push-pull in Port configuration register high (GPIOx_CRH) (x=A..G) CNFy(Port x configuration bits(y=8..15))
  GPIOB->CRH &= ~(GPIO_CRH_CNF13);
  GPIOB->CRH &= ~(GPIO_CRH_CNF14);
}

/*
 * @brief Write/Toggle function for LEDs
 */
void gpio_LED_writeGreen(bool state)
{
  //Bit-banding
  PB13_BB_ADDR = state;
}

void gpio_LED_writeRed(bool state)
{
  //Bit-banding
  PB14_BB_ADDR = state;
}

void gpio_LED_toggleGreen(void)
{
  GPIOB->ODR ^= GPIO_ODR_ODR13; //1 xor 1 = 0, 1 xor 0 = 1
}

void gpio_LED_toggleRed(void)
{
  GPIOB->ODR ^= GPIO_ODR_ODR14;
}

/*
 * @brief Button (input) configuration, PA0
 */
void gpio_PB_config(void) // PB - push button
{
  //Enable Port A clock in APB2 peripheral clock enable register (RCC_APB2ENR) IOPAEN (IO port A clock enable)
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //Input mode in Port configuration register low (GPIOx_CRL) (x=A..G)
  GPIOA->CRL &= ~(GPIO_CRL_MODE0); //00: Input mode (reset state)
  //Floating input
  GPIOA->CRL &= ~(GPIO_CRL_CNF0); //clear
  GPIOA->CRL |= GPIO_CRL_CNF0_0;// 01: Floating input (reset state)
}

/*
 * @brief Read push-button
 */
bool gpio_PB_read(void)
{
  //Bit-banding
  return ((bool)(PA0_BB_ADDR));
}

/*
 * brief Slide switches (input) configuration, PA8, PA15
 */
void gpio_SW_config (void)
{
  //Enable Port A clock in APB2 peripheral clock enable register (RCC_APB2ENR) IOPAEN (IO port A clock enable)
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //Input mode in Port configuration register low (GPIOx_CRL) (x=A..G)
  GPIOA->CRH &= ~(GPIO_CRH_MODE8); //00: Input mode (reset state)
  GPIOA->CRH &= ~(GPIO_CRH_MODE15); //00: Input mode (reset state)
  //Floating input
  GPIOA->CRH &= ~(GPIO_CRH_CNF8); //clear
  GPIOA->CRH |= GPIO_CRH_CNF8_0;// 01: Floating input (reset state)
  GPIOA->CRH &= ~(GPIO_CRH_CNF15); //clear
  GPIOA->CRH |= GPIO_CRH_CNF15_0;// 01: Floating input (reset state)
  //Remap PA15 to disable JTDI
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //AFIOEN Alternate function IO clock enable
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; //PWREN Power interface clock enable
  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG); //000: Full SWJ (JTAG-DP + SW-DP): Reset State
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1; //010: JTAG-DP Disabled and SW-DP Enabled
}

/*
 * @brief Read switches SW1, SW2
 */
bool gpio_SW1_read(void)
{
  //Port input data register (GPIOx_IDR) (x=A..G) IDRy (Port input data(y=0..15))
  return ((bool)(GPIOA->IDR & GPIO_IDR_IDR15));
}
bool gpio_SW2_read(void)
{
  //Port input data register (GPIOx_IDR) (x=A..G) IDRy (Port input data(y=0..15))
  return ((bool)(GPIOA->IDR & GPIO_IDR_IDR8));
}

/*
 * @brief TM1637 LED Display GPIO pins configuration
 */
void gpio_tm1637_config(void)
{
  //CLK: PB10
  //DIO: PB11 (data IO)
  //Enable Port B Clock in APB2 peripheral clock enable register (RCC_APB2ENR) IOPBEN (IO port B clock enable)
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  //Output Mode, Low speed
  GPIOB->CRH &= ~(GPIO_CRH_MODE10); //clear
  GPIOB->CRH |= GPIO_CRH_MODE10_1; //10: Output mode, max speed 2 MHz.
  GPIOB->CRH &= ~(GPIO_CRH_MODE11); //clear
  GPIOB->CRH |= GPIO_CRH_MODE11_1; //10: Output mode, max speed 2 MHz.
  //Push-pull output type
  GPIOB->CRH &= ~(GPIO_CRH_CNF10); //clear and set 00: General purpose output push-pull
  GPIOB->CRH &= ~(GPIO_CRH_CNF11); //clear and set 00: General purpose output push-pull
}
