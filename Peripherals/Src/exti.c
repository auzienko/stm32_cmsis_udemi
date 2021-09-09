/*
 * exti.c
 *
 *  Created on: Aug 18, 2021
 *      Author: aleksandruzienko
 */

#include "exti.h"

/*
 * @brief Push button EXTI configuration
 */
void exti_PB_config(void)
{
  //Enable AFIO clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //AFIOEN (Alternate function IO clock enable)
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // 1: Alternate Function IO clock enabled

  //PA0 maps to EXTI0
  //in External interrupt configuration register 1 (AFIO_EXTICR1)
  AFIO->EXTICR[0] &= ~(0xFUL << 0); //0000: PA[x] pin

  //Unblock EXTI0
  //in Interrupt mask register (EXTI_IMR)
  EXTI->IMR |= (0x1UL << 0);

  //Enable Rising edge only
  //in Rising trigger selection register (EXTI_RTSR) AND
  //in Falling trigger selection register (EXTI_FTSR)
  EXTI->RTSR |= (0x1UL << 0);
  EXTI->FTSR &= ~(0x1UL << 0);

  NVIC_SetPriority(EXTI0_IRQn, 5);
  NVIC_EnableIRQ(EXTI0_IRQn);
}
