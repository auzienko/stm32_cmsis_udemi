/*
 * tim.c
 *
 *  Created on: Aug 21, 2021
 *      Author: aleksandruzienko
 */

#include "tim.h"

/*
 * @brief TIM3 ADC TRGO (trigger output) timer configuration
 */
void tim_TIM3_config(void)
{
  //Enable TIM3 timer
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //TIM3EN:TIM3 timer clock enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  //Count up
  //in TIMx control register 1 (TIMx_CR1)
  //DIR:Direction
  TIM3->CR1 &= ~(TIM_CR1_DIR); //0: Counter used as upcounter

  //Periodic
  //in TIMx control register 1 (TIMx_CR1)
  //OPM:One-pulse mode
  TIM3->CR1 &= ~(TIM_CR1_OPM); //0: Counter is not stopped at update event

  //Mode --> TRGO
  //in TIMx control register 2 (TIMx_CR2)
  //MMS[2:0]: Master mode selection
  TIM3->CR2 &= ~(TIM_CR2_MMS);
  TIM3->CR2 |= TIM_CR2_MMS_1; //010: Update - The update event is selected as trigger output (TRGO). For instance a master timer can then be used as a prescaler for a slave timer.

  //10MHz
  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM3->PSC = 7200 - 1; // -->10KHz

  //Period
  //in TIMx auto-reload register (TIMx_ARR)
  //TIM3->ARR = 1000 - 1; // 10KHz --> 10Hz
  TIM3->ARR = 5000 - 1; // 10KHz --> 2Hz

  //Update registers on event
  //in TIMx event generation register (TIMx_EGR)
  //UG:Update generation
  TIM3->EGR |= TIM_EGR_UG; //1: Re-initialize the counter and generates an update of the registers.

  //Start timer
  //in TIMx control register 1 (TIMx_CR1)
  //CEN:Counter enable
  TIM3->CR1 |= TIM_CR1_CEN; //1: Counter enabled
}

/*
 * @brief Delay timer configuration - TIM2
 */
void tim_TIM2_delay_config(void)
{
  //Enable TIM2 timer
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //TIM2EN:TIM2 timer clock enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  //Count up
  //in TIMx control register 1 (TIMx_CR1)
  //DIR:Direction
  TIM2->CR1 &= ~(TIM_CR1_DIR); //0: Counter used as upcounter

  //One-pulse
  //in TIMx control register 1 (TIMx_CR1)
  //OPM:One-pulse mode
  TIM2->CR1 |= TIM_CR1_OPM; //1: Counter stops counting at the next update event (clearing the bit CEN)

  //Mode --> RESET
  //in TIMx control register 2 (TIMx_CR2)
  //MMS[2:0]: Master mode selection
  TIM2->CR2 &= ~(TIM_CR2_MMS); //000: Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO).

  //72MHz
  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM2->PSC = 0; // -->72MHz

  //Period
  //in TIMx auto-reload register (TIMx_ARR)
  TIM2->ARR = 7200 - 1; // 10KHz
}

/*
 * @brief milli second delay - TIM2
 */
void tim_TIM2_msDelay(uint32_t ms)
{
  //Delay is ms
  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM2->PSC = (ms * 10) - 1; // 10KHz --> ms delay value One pulse

  //Start timer
  //in TIMx control register 1 (TIMx_CR1)
  //CEN:Counter enable
  TIM2->CR1 |= TIM_CR1_CEN; //1: Counter enabled

  //Wait for Delay to elapse
  //in TIMxstatusregister(TIMx_SR)
  //UIF:Update interrupt flag
  while (!(TIM2->SR & TIM_SR_UIF));

  //Clear TIM Flag
  //in TIMxstatusregister(TIMx_SR)
  //UIF:Update interrupt flag
  TIM2->SR &= ~(TIM_SR_UIF);
}
