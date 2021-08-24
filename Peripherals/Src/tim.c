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
