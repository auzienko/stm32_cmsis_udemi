/*
 * rcc.c
 *
 *  Created on: Aug 14, 2021
 *      Author: aleksandruzienko
 */

#include "rcc.h"

//ms Tick
static __IO uint32_t msTicks = 0;

/**
 * @brief HSE configuration
 */
void rcc_HSE_config(void)
{
  /*
   * Configuration parameters --> STM32F103 Clock Tree
   * HSE = 8MHz
   * PLL_MUL = 9
   * USB divider = 1.5
   * AHB prescaler = 1
   * Cortex System timer (prescaler) = 1
   * --> 72MHz System Clock
   *
   * APB1 prescaler = 2 --> 36, 72 MHz
   * APB2 prescaler = 1 --> 72MHz
   * ADC prescaler = 6 --> 12MHz
   */

  //PLL configuration
  //PLL_N = 9
  //Clock configuration register RCC_CFGR
  //RCC->CFGR &= ~(0xFUl << 18); //clear [21:18] bits. F = 1111
  RCC->CFGR &= ~(RCC_CFGR_PLLMULL);
  //RCC->CFGR |= (0x7Ul << 18);
  RCC->CFGR |= RCC_CFGR_PLLMULL9;

  //USB divider = 1.5
  RCC->CFGR &= ~(RCC_CFGR_USBPRE);

  /* HSE Oscillator */
  //Enable HSE Oscillator
  //Clock control register RCC_CR
  RCC->CR |= (RCC_CR_HSEON);
  //Wait for it to stabilizer
  while((RCC->CR & RCC_CR_HSERDY) == 0);
  //Select HSE as PLL source (RCC_CFGR bit PLLSRC)
  RCC->CFGR |= RCC_CFGR_PLLSRC;
  //Enable PLL (in RCC_CR)
  RCC->CR |= RCC_CR_PLLON;
  //Wait for PLL ready
  while((RCC->CR & RCC_CR_PLLRDY) == 0);

  //Flash prefetch and wait state (FLASH_ACR Flash access control register)
  //2WS = 72MHz
  FLASH->ACR &= ~(FLASH_ACR_LATENCY); //clear
  FLASH->ACR |= FLASH_ACR_LATENCY_1;
  //Enable Prefetch buffer
  FLASH->ACR |= FLASH_ACR_PRFTBE;
  //Select PLL and main system clock (SW bit in RCC_CFGR register)
  RCC->CFGR &= ~(RCC_CFGR_SW); //clear
  RCC->CFGR |= RCC_CFGR_SW_1;
  //wait for PLL to be the active clock source
  while((RCC->CFGR & RCC_CFGR_SWS_1) == 0);

  //Preripherals clock setup
  //AHB prescaler (HPRE in RCC_CFGR)
  RCC->CFGR &= ~(RCC_CFGR_HPRE); //clear
  //APB1 prescaler (PPRE1 in RCC_CFGR)
  RCC->CFGR &= ~(RCC_CFGR_PPRE1); //clear
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
  //APB2 prescaler (PPRE2 in RCC_CFGR)
  RCC->CFGR &= ~(RCC_CFGR_PPRE2); //clear
  //ADC prescaler (ADCPRE in RCC_CFGR)
  RCC->CFGR &= ~(RCC_CFGR_ADCPRE); //clear
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

}

/*
 * @brief SysTick configuration
 */
void rcc_SysTick_config(uint32_t arr)
{
  //Reset Control register (STK_CTRL SysTick control and status register)
  //in programming manual
  SysTick->CTRL = 0;
  //Set the Reload value (SysTick reload value register (STK_LOAD))
  SysTick->LOAD = arr - 1;
  //Priority SysTick Interrupt (in NVIC)
  NVIC_SetPriority(SysTick_IRQn, 0);
  //Reset SysTick Value to 0
  SysTick->VAL = 0;
  //Enable SysTick from Control register
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

}

/*
 * @brief Increment ms Ticks
 */
void rcc_msIncTicks(void)
{
  msTicks++;
}

/*
 * @brief Get ms Ticks
 */
uint32_t rcc_msGetTicks(void)
{
  return (msTicks);
}

/*
 * @brief ms Delay
 */
void rcc_msDelay(uint32_t ms)
{
  uint32_t startTicks;

  startTicks = rcc_msGetTicks();
  while ((rcc_msGetTicks() - startTicks) < ms);
}

/*
 * @brief SysTick Interrupt callback
 */
//void SysTick_Handler(void)
//{
//  NVIC_ClearPendingIRQ(SysTick_IRQn);
//  rcc_msIncTicks();
//}
