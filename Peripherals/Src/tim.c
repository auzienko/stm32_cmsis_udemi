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

/*
 * @brief PWM Channel GPIO Configuration - TIM3 CH1 (PB4 - Red), CH2 (PB5 - Green), CH3 (PB0 - Blue)
 */
void  tim_TIM3_GPIO_config(void)
{
  //Enable Alternative Function clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //AFIOEN:Alternate function IO clock enable
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // 1: Alternate Function IO clock enabled

  //Enable PWR
  //The PWR peripheral is used to control the device power supply configuration.
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //PWREN:Power interface clock enable
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; // 1: Power interface clock enable

  //Partial remap TIM3 CH1 (PB4 - Red), CH2 (PB5 - Green)
  //in AF remap and debug I/O configuration register (AFIO_MAPR)
  //TIM3_REMAP[1:0]:TIM3 remapping
  AFIO->MAPR &= ~(AFIO_MAPR_TIM3_REMAP); //clear
  AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_1; //10: Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)

  //Enable Port B clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //IOPBEN:IO port B clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //1: IO port B clock enabled

  //PB4
  //Config Port B4 to Output mode
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits (y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_MODE4); //clear
  GPIOB->CRL |= GPIO_CRL_MODE4_1; //10: Output mode, max speed 2 MHz.

  //Config Port B4 to Alternate function output Push-pull
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //CNFy[1:0]:Portx configuration bits (y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_CNF4); //clear
  GPIOB->CRL |= GPIO_CRL_CNF4_1; //10: Alternate function output Push-pull

  //PB5
  //Config Port B5 to Output mode
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits (y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_MODE5); //clear
  GPIOB->CRL |= GPIO_CRL_MODE5_1; //10: Output mode, max speed 2 MHz.

  //Config Port B4 to Alternate function output Push-pull
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //CNFy[1:0]:Portx configuration bits (y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_CNF5); //clear
  GPIOB->CRL |= GPIO_CRL_CNF5_1; //10: Alternate function output Push-pull

  //PB0
  //Config Port B0 to Output mode
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits (y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_MODE0); //clear
  GPIOB->CRL |= GPIO_CRL_MODE0_1; //10: Output mode, max speed 2 MHz.

  //Config Port B4 to Alternate function output Push-pull
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //CNFy[1:0]:Portx configuration bits (y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_CNF0); //clear
  GPIOB->CRL |= GPIO_CRL_CNF0_1; //10: Alternate function output Push-pull
}

/*
 * @brief PWM Duty Cycle - RGB (0 - 255)
 */
void tim_PWM_setRGB(uint8_t R, uint8_t G, uint8_t B)
{
  TIM3->CCR1 = (uint16_t)((TIM3->ARR + 0.0f) * (R / 255.0f));
  TIM3->CCR2 = (uint16_t)((TIM3->ARR + 0.0f) * (G / 255.0f));
  TIM3->CCR3 = (uint16_t)((TIM3->ARR + 0.0f) * (B / 255.0f));
}

/*
 * @brief TIM3 PWM configuration
 */
void  tim_TIM3_PWM_config(void)
{
  //Enable TIM3
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //TIM3EN:TIM3 timer clock enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //1: TIM3 timer clock enabled

  //Count up
  //in TIMx control register 1 (TIMx_CR1)
  //DIR:Direction
  TIM3->CR1 &= ~(TIM_CR1_DIR); //0: Counter used as upcounter

  //Periodic
  //in TIMx control register 1 (TIMx_CR1)
  //OPM:One-pulse mode
  TIM3->CR1 &= ~(TIM_CR1_OPM); //0: Counter is not stopped at update event

  //Mode --> RESET
  //in TIMx control register 2 (TIMx_CR2)
  //MMS[2:0]: Master mode selection
  TIM3->CR2 &= ~(TIM_CR2_MMS); //000: Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO).

  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM3->PSC = 0;

  //Period
  //in TIMx auto-reload register (TIMx_ARR)
  TIM3->ARR = 7200 - 1; // 10KHz

  //Clear Update Interrupt flag
  //in TIMxstatusregister(TIMx_SR)
  //UIF:Update interrupt flag
  TIM3->SR &= ~(TIM_SR_UIF);

  //Disable Output Compare (OC)
  //in TIM1 and TIM8 capture/compare enable register (TIMx_CCER)
  TIM3->CCER = 0; //disable all

  /* TIM3 CH1 */

  //Set mode output
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //CC1S:Capture/Compare 1 selection
  TIM3->CCMR1 &= ~(TIM_CCMR1_CC1S); //00: CC1 channel is configured as output

  //PWM mode
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //OC1M: Output compare 1 mode
  TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M);
  TIM3->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2); //110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.

  //polarity
  //in TIMx capture/compare enable register (TIMx_CCER)
  //CC1P:Capture/Compare 1 output polarity
  TIM3->CCER &= ~(TIM_CCER_CC1P); //0: OC1 active high.

  //set pulse width - duty cycle
  //in TIMx capture/compare register 1 (TIMx_CCR1)
  //clear, because we will use another function
  TIM3->CCR1 = 0;

  /* TIM3 CH2 */

  //Set mode output
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //CC2S:Capture/Compare 1 selection
  TIM3->CCMR1 &= ~(TIM_CCMR1_CC2S); //00: CC2 channel is configured as output

  //PWM mode
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //OC2M: Output compare 1 mode
  TIM3->CCMR1 &= ~(TIM_CCMR1_OC2M);
  TIM3->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2); //110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.

  //polarity
  //in TIMx capture/compare enable register (TIMx_CCER)
  //CC2P:Capture/Compare 1 output polarity
  TIM3->CCER &= ~(TIM_CCER_CC2P); //0: OC1 active high.

  //set pulse width - duty cycle
  //in TIMx capture/compare register 2 (TIMx_CCR2)
  //clear, because we will use another function
  TIM3->CCR2 = 0;

  /* TIM3 CH3 */

  //Set mode output
  //in TIMx capture/compare mode register 2 (TIMx_CCMR2)
  //CC3S:Capture/Compare 1 selection
  TIM3->CCMR2 &= ~(TIM_CCMR2_CC3S); //00: CC3 channel is configured as output

  //PWM mode
  //in TIMx capture/compare mode register 2 (TIMx_CCMR2)
  //OC3M: Output compare 1 mode
  TIM3->CCMR2 &= ~(TIM_CCMR2_OC3M);
  TIM3->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2); //110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.

  //polarity
  //in TIMx capture/compare enable register (TIMx_CCER)
  //CC3P:Capture/Compare 1 output polarity
  TIM3->CCER &= ~(TIM_CCER_CC3P); //0: OC1 active high.

  //set pulse width - duty cycle
  //in TIMx capture/compare register 3 (TIMx_CCR3)
  //clear, because we will use another function
  TIM3->CCR3 = 0;

  //Enable Channels 1, 2, 3
  //in TIMx capture/compare enable register (TIMx_CCER)
  //CC1E:Capture/Compare 1 output enable, CC2E 2, CC3E 3
  TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E); //1: On - OCx (1,2,3) signal is output on the corresponding output pin.

  //Start TIM3
  //in TIMx control register 1 (TIMx_CR1)
  //CEN:Counter enable
  TIM3->CR1 |= TIM_CR1_CEN; //1: Counter enabled
}

/*
 * @brief Encoder GPIO pins; PB6, PB7 configuration;
 */
void  tim_TIM4_Enconder_GPIO_config(void)
{
  //Enable PB clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //IOPBEN:I/O port B clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //1:I/O port B clock enabled

  //PB6 & PB7 "Floating input" configuration
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  //MODEy[1:0]:Portx mode bits(y=0..7)
  //CNFy[1:0]:Portx configuration bits(y=0..7)
  GPIOB->CRL &= ~(GPIO_CRL_MODE6); //00: Input mode (reset state)
  GPIOB->CRL &= ~(GPIO_CRL_MODE7); //00: Input mode (reset state)
  GPIOB->CRL &= ~(GPIO_CRL_CNF6); //clear
  GPIOB->CRL |= GPIO_CRL_CNF6_0; //01: Floating input (reset state)
  GPIOB->CRL &= ~(GPIO_CRL_CNF7); //clear
  GPIOB->CRL |= GPIO_CRL_CNF7_0; //01: Floating input (reset state)
}

/*
 * @brief Encoder TIM Peripheral configuration
 */
void  tim_TIM4_Encoder_config()
{
  //Enable TIM4
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //TIM4EN:TIM4 timer clock enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //1: TIM4 timer clock enabled

  //Count up
  //in TIMx control register 1 (TIMx_CR1)
  //DIR:Direction
  TIM4->CR1 &= ~(TIM_CR1_DIR); //0: Counter used as upcounter

  //Periodic
  //in TIMx control register 1 (TIMx_CR1)
  //OPM:One-pulse mode
  TIM4->CR1 &= ~(TIM_CR1_OPM); //0: Counter is not stopped at update event

  //Mode --> RESET
  //in TIMx control register 2 (TIMx_CR2)
  //MMS[2:0]: Master mode selection
  TIM4->CR2 &= ~(TIM_CR2_MMS); //000: Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO).

  //Prescaler
  //in TIMx prescaler (TIMx_PSC)
  //PSC[15:0]:Prescaler value
  TIM4->PSC = 0;

  //Period
  //in TIMx auto-reload register (TIMx_ARR)
  TIM4->ARR = 0xFFFF;

  //Clear Update Interrupt flag
  //in TIMxstatusregister(TIMx_SR)
  //UIF:Update interrupt flag
  TIM4->SR &= ~(TIM_SR_UIF);

  /* Encoder mode settings */
  //Reset Slave mode registers
  //in TIMx slave mode control register (TIMx_SMCR)
  //SMS:Slave mode selection
  TIM4->SMCR &= ~(TIM_SMCR_SMS); //000: Slave mode disabled - if CEN = ‘1 then the prescaler is clocked directly by the internal clock.
  //ECE:External clock enable
  TIM4->SMCR &= ~(TIM_SMCR_ECE); //0: External clock mode 2 disabled

  //Set Encoder mode
  //in TIMx slave mode control register (TIMx_SMCR)
  //SMS:Slave mode selection
  TIM4->SMCR |= TIM_SMCR_SMS_0; //001: Encoder mode 1 - Counter counts up/down on TI2FP1 edge depending on TI1FP2 level.

  //Set CC (capture compare) as Input IC (input capture) CH1 & CH2
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //CC1S:Capture/Compare 1 selection
  TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S); //clear
  TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0); //01: CC1 channel is configured as input, IC1 is mapped on TI1.
  TIM4->CCMR1 |= (TIM_CCMR1_CC2S_0); //01: CC2 channel is configured as input, IC2 is mapped on TI2

  //Set CC prescaler to 0
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //IC1PSC:Input capture 1 prescaler
  TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC | TIM_CCMR1_IC2PSC); //00: no prescaler, capture is done each time an edge is detected on the capture input

  //Input filter
  //in TIMx capture/compare mode register 1 (TIMx_CCMR1)
  //IC1F:Inputcapture1filter
  TIM4->CCMR1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC2F); //clear
  TIM4->CCMR1 |= TIM_CCMR1_IC1F_1; //0010: fSAMPLING=fCK_INT, N=4
  TIM4->CCMR1 |= TIM_CCMR1_IC2F_1; //0010: fSAMPLING=fCK_INT, N=4

  //Set polarity to rising
  //in TIMx capture/compare enable register (TIMx_CCER)
  //CC1P:Capture/Compare 1 output polarity (+ CC2P:Capture/Compare 2 output polarity)
  TIM4->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P); //0: non-inverted: capture is done on a rising edge of IC1 /IC2.

  //Enable Encoder
  //in TIMx capture/compare enable register (TIMx_CCER)
  //CC1E:Capture/Compare 1 output enable & CC2E:Capture/Compare 2 output enable
  TIM4->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E); //1: Capture enabled.

  //Start TIM4
  //in TIMx control register 1 (TIMx_CR1)
  //CEN:Counter enable
  TIM4->CR1 |= TIM_CR1_CEN; //1: Counter enabled
}

/*
 * @brief TIM3 ADC Mic
 */
void tim_TIM3_MIC_config(void)
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

    //Prescaler
    //in TIMx prescaler (TIMx_PSC)
    //PSC[15:0]:Prescaler value
    TIM3->PSC = 0; // -->72MHz

    //Period
    //in TIMx auto-reload register (TIMx_ARR)
    //72Mhz/41.1kHz = 1632
    TIM3->ARR = 1632 - 1; // 72MHz --> 41.1kHz

    //Update registers on event
    //in TIMx event generation register (TIMx_EGR)
    //UG:Update generation
    TIM3->EGR |= TIM_EGR_UG; //1: Re-initialize the counter and generates an update of the registers.

    //Start timer
    //in TIMx control register 1 (TIMx_CR1)
    //CEN:Counter enable
    //TIM3->CR1 |= TIM_CR1_CEN; //1: Counter enabled
}
