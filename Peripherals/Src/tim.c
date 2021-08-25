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

/*
 * @brief Periodic TIM2 Interrupt
 */
void tim_TIM2_periodic_config(uint32_t msPeriod)
{
  //Enable TIM2 timer
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //TIM2EN:TIM2 timer clock enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  //Count up
  //in TIMx control register 1 (TIMx_CR1)
  //DIR:Direction
  TIM2->CR1 &= ~(TIM_CR1_DIR); //0: Counter used as upcounter

  //Periodic
  //in TIMx control register 1 (TIMx_CR1)
  //OPM:One-pulse mode
  TIM2->CR1 &= ~(TIM_CR1_OPM); //0: Counter is not stopped at update event

  //Mode --> RESET
  //in TIMx control register 2 (TIMx_CR2)
  //MMS[2:0]: Master mode selection
  TIM2->CR2 &= ~(TIM_CR2_MMS); //000: Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO).

  //Delay is ms
  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM2->PSC = (msPeriod * 10) - 1;

  //Period
  //in TIMx auto-reload register (TIMx_ARR)
  TIM2->ARR = 7200 - 1; // 10KHz

  //Clear Update Interrupt flag
  //in TIMxstatusregister(TIMx_SR)
  //UIF:Update interrupt flag
  TIM2->SR &= ~(TIM_SR_UIF);

  //Enable Update interrupt
  //in TIMx DMA/Interrupt enable register (TIMx_DIER)
  //UIE:Update interrupt enable
  TIM2->DIER |= TIM_DIER_UIE; //1: Update interrupt enabled.

  //Enable NVIC Interrupt
  NVIC_EnableIRQ(TIM2_IRQn);

  //Start Periodic TIM2
  //in TIMx control register 1 (TIMx_CR1)
  //CEN:Counter enable
  TIM2->CR1 |= TIM_CR1_CEN; //1: Counter enabled
}

/*
 * @brief Configure TIM1_CH1N --> PB13
 */
void tim_TIM1_CH1N_GPIO_config(void)
{
  //Reset remaping TIM1 to default
  //in AF remap and debug I/O configuration register (AFIO_MAPR)
  //TIM1_REMAP[1:0]:TIM1 remapping
  AFIO->MAPR &= ~(AFIO_MAPR_TIM1_REMAP); //00: No remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15)

  //Enable Port B clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //IOPBEN:IO port B clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //1: IO port B clock enabled

  //Config Port B13 to Output mode
  //in Port configuration register high (GPIOx_CRH) (x=A..G)
  //MODEy[1:0]:Portx mode bits(y=8..15)
  GPIOB->CRH &= ~(GPIO_CRH_MODE13); //clear
  GPIOB->CRH |= GPIO_CRH_MODE13_1; //10: Output mode, max speed 2 MHz.

  //Config Port B13 to Alternate function output Push-pull
  //in Port configuration register high (GPIOx_CRH) (x=A..G)
  //CNFy[1:0]:Portx configuration bits(y=8..15)
  GPIOB->CRH &= ~(GPIO_CRH_CNF13); //clear
  GPIOB->CRH |= GPIO_CRH_CNF13_1; //10: Alternate function output Push-pull

}

/*
 * @brief TIM1 Output Compare (OC) configuration
 */
void tim_TIM1_OC_config(uint32_t msPeriod)
{
  //Enable TIM1
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //TIM1EN:TIM1 timer clock enable
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //1: TIM1 timer clock enabled

  //Count up
  //in TIMx control register 1 (TIMx_CR1)
  //DIR:Direction
  TIM1->CR1 &= ~(TIM_CR1_DIR); //0: Counter used as upcounter

  //Periodic
  //in TIMx control register 1 (TIMx_CR1)
  //OPM:One-pulse mode
  TIM1->CR1 &= ~(TIM_CR1_OPM); //0: Counter is not stopped at update event

  //Mode --> RESET
  //in TIMx control register 2 (TIMx_CR2)
  //MMS[2:0]: Master mode selection
  TIM1->CR2 &= ~(TIM_CR2_MMS); //000: Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO).

  //Delay is ms
  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM1->PSC = (msPeriod * 10) - 1;

  //Period
  //in TIMx auto-reload register (TIMx_ARR)
  TIM1->ARR = 7200 - 1; // 10KHz

  //Clear Update Interrupt flag
  //in TIMxstatusregister(TIMx_SR)
  //UIF:Update interrupt flag
  TIM1->SR &= ~(TIM_SR_UIF);

  /* Output Compare channel - TIM1_CH1N */

  //Disable Output Compare (OC)
  //in TIM1 and TIM8 capture/compare enable register (TIMx_CCER)
  TIM1->CCER = 0; //disable all

  //Set CH1N as output
  //in TIM1 and TIM8 capture/compare mode register 1 (TIMx_CCMR1)
  //CC1S:Capture/Compare 1 selection
  TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S); //00: CC1 channel is configured as output

  //Toggle output
  //in TIM1 and TIM8 capture/compare mode register 1 (TIMx_CCMR1)
  //OC1M:Output compare 1 mode
  TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); //clear
  TIM1->CCMR1 |= (TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1); //011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.

  //polarity
  ///in TIM1 and TIM8 capture/compare enable register (TIMx_CCER)
  //CC1NP:Capture/Compare 1 complementary output polarity
  TIM1->CCER &= ~(TIM_CCER_CC1NP);

  //Counter value to toggle output
  //in TIM1 and TIM8 capture/compare register 1 (TIMx_CCR1)
  //CCR1[15:0]:Capture/Compare 1 value
  TIM1->CCR1 = 10;

  //Enable CH1N
  //in TIM1 and TIM8 capture/compare enable register (TIMx_CCER)
  //CC1NE:Capture/Compare 1 complementary output enable
  //& CC1NP:Capture/Compare1complementaryoutputpolarity
  TIM1->CCER |= (TIM_CCER_CC1NE | TIM_CCER_CC1NP); //1: On - OC1N signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits.

  //Slave controller reset
  //in TIM1 and TIM8 slave mode control register (TIMx_SMCR)
  TIM1->SMCR = 0;

  //Enable MOE
  //in TIM1 and TIM8 break and dead-time register (TIMx_BDTR)
  //MOE:Main output enable
  TIM1->BDTR |= TIM_BDTR_MOE;

  //Start Periodic TIM1
  //in TIMx control register 1 (TIMx_CR1)
  //CEN:Counter enable
  TIM1->CR1 |= TIM_CR1_CEN; //1: Counter enabled
}

