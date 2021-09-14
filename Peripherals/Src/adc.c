/*
 * adc.c
 *
 *  Created on: Aug 19, 2021
 *      Author: aleksandruzienko
 */

#include "adc.h"

/*
 * @brief ADC GPIO pins: PA1 - potentiometer, PA2 - X, PA3 - Y joystick
 */
void adc_GPIO_config(void)
{
  //Enable Port A clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //IOPAEN:IO port A clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  //PA1 configuration
  //in Port configuration register low (GPIOx_CRL) (x=A..G)
  GPIOA->CRL &= ~(GPIO_CRL_MODE1); // 00: Analog mode
  GPIOA->CRL &= ~(GPIO_CRL_CNF1); // 00: Input mode (reset state)
  //PA2
  GPIOA->CRL &= ~(GPIO_CRL_MODE2); // 00: Analog mode
  GPIOA->CRL &= ~(GPIO_CRL_CNF2); // 00: Input mode (reset state)
  //PA3
  GPIOA->CRL &= ~(GPIO_CRL_MODE3); // 00: Analog mode
  GPIOA->CRL &= ~(GPIO_CRL_CNF3); // 00: Input mode (reset state)
}

/*
 * @brief ADC Single Channel configuration
 */
void adc_singleChannel_config(ADC_SingleSelect_e channel)
{
  //Enable ADC Clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //ADC1EN:ADC1 interface clock enable
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  //Right Align
  //in ADC control register 2 (ADC_CR2)
  //ALIGN: Data alignment
  ADC1->CR2 &= ~(ADC_CR2_ALIGN); // 0: Right Alignment

  //Single Conversion
  //in ADC control register 2 (ADC_CR2)
  //CONT:Continuous conversion
  ADC1->CR2 &= ~(ADC_CR2_CONT); // 0: Single conversion mode

  //Software Trigger
  //in ADC control register 2 (ADC_CR2)
  //EXTSEL[2:0]:External event select for regular group
  ADC1->CR2 |= ADC_CR2_EXTSEL; //SWSTART (Soft ware start)

  //Sample time to 28 Cycle = (12 + 28)/12MHz = 3.3us
  //in ADC sample time register 2 (ADC_SMPR2)
  //PA1
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP1); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP1_0 | ADC_SMPR2_SMP1_1); // 011: 28.5 cycles
  //PA2
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP2); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP2_0 | ADC_SMPR2_SMP2_1); // 011: 28.5 cycles
  //PA3
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP3); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP3_0 | ADC_SMPR2_SMP3_1); // 011: 28.5 cycles

  //Number of conversion = 1
  //in ADC regular sequence register 1 (ADC_SQR1)
  //L[3:0]: Regular channel sequence length
  ADC1->SQR1  &= ~(ADC_SQR1_L); // 0000: 1 conversion

  //Rank1 Channel1,2,3
  //in ADC regular sequence register 3 (ADC_SQR3)
  ADC1->SQR3 = channel;

  //Power up ADC
  //in ADC control register 2 (ADC_CR2)
  ADC1->CR2 |= ADC_CR2_ADON;

  //Wait for ADC clock to stabilise (couple micro seconds)
  for (uint16_t i = 0; i < 36; i++); //72 cycles = 1uSec, for loop 4 cycles each, 72*2/4 = 36
}

/*
 * @brief ADC Start - SWSTART
 */
void adc_start(void)
{
  //reset ADC status register (ADC_SR)
  ADC1->SR = 0;

  //start
  //in ADC control register 2 (ADC_CR2)
  //SWSTART: Start conversion of regular channels
  ADC1->CR2 |= ADC_CR2_SWSTART;

  //(page 218 rm) Power up ADC in ADC control register 2 (ADC_CR2)
  //ADC on-off control
  //The ADC can be powered-on by setting the ADON bit in the ADC_CR2 register.
  //When the ADON bit is set for the first time, it wakes up the ADC from Power Down mode.
  ADC1->CR2 |= ADC_CR2_ADON;
}

/*
 * @brief ADC Stop
 */
void adc_stop(void)
{
  ADC1->SR = 0;
  ADC1->CR2 &= ~(ADC_CR2_SWSTART);
}

/*
 * @brief ADC Poll for end of conversion
 */
bool adc_pollForEndOfConversion(uint32_t msTimeout)
{
  uint32_t startTicks;

  startTicks = rcc_msGetTicks();
  while(((ADC1->SR) & ADC_SR_EOC) == 0) //ADC status register (ADC_SR) EOC:End of conversion
  {
    if ((rcc_msGetTicks() - startTicks) > msTimeout)
      return (false);
  }
  return (true);
}

/*
 * @brief ADC Read value
 */
uint16_t adc_readValue(void)
{
  //ADC regular data register (ADC_DR)
  return ((ADC1->DR) & 0xFFF); //12-bits resolution Right Aligned
}

/*
 * @brief ADC Enable/Disable Interrupt - EOC (end of conversion)
 */
void adc_enableEOC_IT(void)
{
  //ADC control register 1 (ADC_CR1) EOCIE:Interrupt enable for EOC
  ADC1->CR1 |= ADC_CR1_EOCIE;
  //ADC NVIC Interrupt
  NVIC_EnableIRQ(ADC1_2_IRQn);
}

void adc_disableEOC_IT(void)
{
  ADC1->CR1 &= ~(ADC_CR1_EOCIE);
  NVIC_DisableIRQ(ADC1_2_IRQn);
}

/*
 * @brief ADC Multi-channel configuration
 */
void  adc_multiChannel_config(void)
{
  //Enable ADC1 Clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //ADC1EN:ADC1 interface clock enable
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  //Right Align
  //in ADC control register 2 (ADC_CR2)
  //ALIGN: Data alignment
  ADC1->CR2 &= ~(ADC_CR2_ALIGN); // 0: Right Alignment

  //Sample time to 28 Cycle = (12 + 28)/12MHz = 3.3us
  //in ADC sample time register 2 (ADC_SMPR2)
  //PA1
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP1); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP1_0 | ADC_SMPR2_SMP1_1); // 011: 28.5 cycles
  //PA2
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP2); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP2_0 | ADC_SMPR2_SMP2_1); // 011: 28.5 cycles
  //PA3
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP3); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP3_0 | ADC_SMPR2_SMP3_1); // 011: 28.5 cycles

  //Software Trigger
  //in ADC control register 2 (ADC_CR2)
  //EXTSEL[2:0]:External event select for regular group
  //ADC1->CR2 |= ADC_CR2_EXTSEL; //SWSTART (Soft ware start)

  //TIM3_TRGO (timer trigger output)
  //in ADC control register 2 (ADC_CR2)
  //EXTTRIG:External trigger conversion mode for regular channels
  ADC1->CR2 |= ADC_CR2_EXTTRIG; //1: Conversion on external event enabled
  //EXTSEL[2:0]:External event select for regular group
  ADC1->CR2 &= ~(ADC_CR2_EXTSEL);
  ADC1->CR2 |= ADC_CR2_EXTSEL_2; //100: Timer 3 TRGO event

  //Sequencer
  //in ADC regular sequence register 3 (ADC_SQR3)
  //Rank 1 - PA1, Channel 1
  ADC1->SQR3 |= ADC_SQR3_SQ1_0;
  //Rank 2 - PA2, Channel 2
  ADC1->SQR3 |= ADC_SQR3_SQ2_1;
  //Rank 3 - PA3, Channel 3
  ADC1->SQR3 |= (ADC_SQR3_SQ3_0 | ADC_SQR3_SQ3_1);

  //Number of conversion
  //in ADC regular sequence register 1 (ADC_SQR1)
  ADC1->SQR1 &= ~(ADC_SQR1_L);
  ADC1->SQR1 |= ADC_SQR1_L_1; //3 conversions

  //Enable Scan mode
  //in ADC control register 1 (ADC_CR1)
  //SCAN: Scan mode
  ADC1->CR1 |= ADC_CR1_SCAN;

  //Enable DMA - ADC register
  //in ADC control register 2 (ADC_CR2)
  //DMA: Direct memory access mode
  ADC1->CR2 |= ADC_CR2_DMA;

  //Single Conversion
  //in ADC control register 2 (ADC_CR2)
  //CONT:Continuous conversion
  ADC1->CR2 &= ~(ADC_CR2_CONT); // 0: Single conversion mode

  //Power up ADC
  //in ADC control register 2 (ADC_CR2)
  ADC1->CR2 |= ADC_CR2_ADON;

  //Wait for ADC clock to stabilise (couple micro seconds)
  for (uint16_t i = 0; i < 36; i++); //72 cycles = 1uSec, for loop 4 cycles each, 72*2/4 = 36
}

/*
 * @ADC DMA configuration
 */
void  adc_multiChannel_DMA_config(uint16_t *pAdcBuffer)
{
  //DMA1 - Channel 1 --> ADC1 (ref man page 282)
  //Enable DMA1 Clock
  //in AHB peripheral clock enable register (RCC_AHBENR)
  //DMA1EN: DMA1 clock enable
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;

  //Clear DMA1 - Channel 1 status flags
  //in DMA interrupt flag clear register (DMA_IFCR)
  DMA1->IFCR |= 0x0F; //clear all 4 flags

  //Peripheral address (src) --> ADC_DR
  //in DMA channel x peripheral address register (DMA_CPARx) (x = 1..7, where x = channel number)
  DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));

  //Memory address (dst) --> uint16_t adc_array[3]
  //in DMA channel x memory address register (DMA_CMARx) (x = 1..7, where x = channel number)
  DMA1_Channel1->CMAR = (uint32_t)pAdcBuffer;

  //Number of transfer
  //in DMA channel x number of data register (DMA_CNDTRx) (x = 1..7, where x = channel number)
  DMA1_Channel1->CNDTR = 3;

  //Circular / Normal
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //CIRC: Circular mode
  DMA1_Channel1->CCR |= DMA_CCR_CIRC; // 1: Circular mode enabled

  //Enable Memory increment
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //MINC: Memory increment mode
  DMA1_Channel1->CCR |= DMA_CCR_MINC; //1: Memory increment mode enabled

  //Disable Peripheral increment
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //PINC: Peripheral increment mode
  DMA1_Channel1->CCR &= ~(DMA_CCR_PINC); //0: Peripheral increment mode disabled

  //Size ADC 12-bits, Peripheral 16-bits
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //PSIZE[1:0]:Peripheral size
  DMA1_Channel1->CCR &= ~(DMA_CCR_PSIZE);
  DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0; // 01: 16-bits

  //Size Memory 16-bits
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //MSIZE[1:0]:Memory size
  DMA1_Channel1->CCR &= ~(DMA_CCR_MSIZE);
  DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0; // 01: 16-bits

  //Direction - Peripheral to Memory
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //DIR: Data transfer direction
  DMA1_Channel1->CCR &= ~(DMA_CCR_DIR); // 0: Read from peripheral

  //Enable DMA
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //EN:Channel enable
  DMA1_Channel1->CCR |= DMA_CCR_EN; //1: Channel enabled

  //Enable Transfer Complete Interrupt - DMA
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //TCIE:Transfer complete interrupt enable
  DMA1_Channel1->CCR |= DMA_CCR_TCIE; //1: TC interrupt enabled

  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/*
 * @brief ADC Analog watchdog configuration
 * @param lowThreshold/highThreshold 0 .. 4095
 */
void adc_AWD_config(uint8_t channel, uint16_t lowThreshold, uint16_t highThreshold)
{
  //Set ADC1 Threshold registers
  //in ADC watchdog low threshold register (ADC_LTR)
  ADC1->LTR = (lowThreshold & 0xFFF);
  //in ADC watchdog high threshold register (ADC_HTR)
  ADC1->HTR = (highThreshold & 0xFFF);

  //Select AWD (Analog Watch Dog) channel
  //in ADC control register 1 (ADC_CR1)
  //AWDCH[4:0]:Analog watch dog channel select bits
  ADC1->CR1 &= ~(ADC_CR1_AWDCH); //clear
  ADC1->CR1 |= (channel << 0);

  //Enable AWD Interrupt
  //in ADC control register 1 (ADC_CR1)
  //AWDIE:Analog watch dog interrupt enable
  ADC1->CR1 |= ADC_CR1_AWDIE; //1: Analog watchdog interrupt enabled

  //AWD on Single Channel
  //in ADC control register 1 (ADC_CR1)
  //AWDSGL:Enable the watch dog on a single channel in scan mode
  ADC1->CR1 |= ADC_CR1_AWDSGL; //1: Analog watchdog enabled on a single channel

  //Disable Injected mode AWD (prevent conflicts)
  //in ADC control register 1 (ADC_CR1)
  //JAWDEN:Analog watch dog enable on injected channels
  ADC1->CR1 &= ~(ADC_CR1_JAWDEN); //0: Analog watchdog disabled on injected channels

  //Enable Regular channel AWD
  //in ADC control register 1 (ADC_CR1)
  //AWDEN:Analog watch dog enable on regular channels
  ADC1->CR1 |= ADC_CR1_AWDEN; //1: Analog watchdog enabled on regular channels

  //Enable ADC1 Interrupt
  NVIC_EnableIRQ(ADC1_2_IRQn);
}

/*
 * @brief ADC Injected configuration
 */
void adc_INJ_config(uint8_t channel)
{
  //Disable Auto Inject
  //in ADC control register 1 (ADC_CR1)
  //JAUTO: Automatic Injected Group conversion
  ADC1->CR1 &= ~(ADC_CR1_JAUTO); //0: Automatic injected group conversion disabled

  //Software trigger
  //in ADC control register 2 (ADC_CR2)
  //JEXTSEL[2:0]:External event select for injected group
  ADC1->CR2 |= (ADC_CR2_JEXTSEL_0 | ADC_CR2_JEXTSEL_1 | ADC_CR2_JEXTSEL_2); //111: JSWSTART

  //Enable Injected EOC Interrupt
  //in ADC control register 1 (ADC_CR1)
  //JEOCIE:Interrupt enable for injected channels
  ADC1->CR1 |= ADC_CR1_JEOCIE; //1: JEOC interrupt enabled. An interrupt is generated when the JEOC bit is set.

  //Number of conversion
  //in ADC injected sequence register (ADC_JSQR)
  //JL[1:0]:Injected sequence length
  ADC1->JSQR &= ~(ADC_JSQR_JL); //00: 1 conversion

  //Set Injected Channel
  //in ADC injected sequence register (ADC_JSQR)
  // (ref man) When JL=0 (1 injected conversion in the sequencer), the ADC converts only JSQ4[4:0] channel
  // => JSQ4
  ADC1->JSQR &= ~(ADC_JSQR_JSQ4); //clear
  ADC1->JSQR |= (channel << ADC_JSQR_JSQ4_Pos);

  //Enable Discontinuous
  //in ADC control register 1 (ADC_CR1)
  //JDISCEN:Discontinuous mode on injected channels
  ADC1->CR1 |= ADC_CR1_JDISCEN; //1: Discontinuous mode on injected channels enabled

  //Sample time to 28 Cycle = (12 + 28)/12MHz = 3.3us
  //Already set in  adc_multiChannel_config
  //PA1
  //ADC1->SMPR2 &= ~(ADC_SMPR2_SMP1); //clear
  //ADC1->SMPR2 |= (ADC_SMPR2_SMP1_0 | ADC_SMPR2_SMP1_1); // 011: 28.5 cycles

  //Software trigger
  //in ADC control register 2 (ADC_CR2)
  //JEXTTRIG:External trigger conversion mode for injected channels
  ADC1->CR2 |= ADC_CR2_JEXTTRIG; //1: Conversion on external event enabled

  //Enable ADC
  //Already set in  adc_multiChannel_config
  //Power up ADC
  //in ADC control register 2 (ADC_CR2)
  //ADC1->CR2 |= ADC_CR2_ADON;

  //Wait for ADC clock to stabilise (couple micro seconds)
  //for (uint16_t i = 0; i < 36; i++); //72 cycles = 1uSec, for loop 4 cycles each, 72*2/4 = 36

  //ADC NVIC Interrupt
  NVIC_EnableIRQ(ADC1_2_IRQn);
}

/*
 * @brief ADC Injected Start
 */
void adc_INJ_start(void)
{
  //in ADC control register 2 (ADC_CR2)
  //JSWSTART:Start conversion of injected channels
  ADC1->CR2 |= ADC_CR2_JSWSTART; //1: Starts conversion of injected channels
}

/*
 * @brief ADC poll for end of conversion
 */
bool adc_INJ_pollForConversion(uint32_t timeout)
{
  uint32_t startTicks;

  startTicks = rcc_msGetTicks();
  while(((ADC1->SR) & ADC_SR_JEOC) == 0) //ADC status register (ADC_SR) JEOC:Injected channel end of conversion
  {
    if ((rcc_msGetTicks() - startTicks) > timeout)
      return (false);
  }
  return (true);
}

/*
 * @brief ADC MIC (PA3 Config)
 */
void adc_MIC_config(void)
{
  //Enable ADC1 Clock
  //in APB2 peripheral clock enable register (RCC_APB2ENR)
  //ADC1EN:ADC1 interface clock enable
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  //Right Align
  //in ADC control register 2 (ADC_CR2)
  //ALIGN: Data alignment
  ADC1->CR2 &= ~(ADC_CR2_ALIGN); // 0: Right Alignment

  //Sample time to 28.5 Cycle = (12 + 28.5)/12.5MHz = 3us
  //in ADC sample time register 2 (ADC_SMPR2)
  //PA3
  ADC1->SMPR2 &= ~(ADC_SMPR2_SMP3); //clear
  ADC1->SMPR2 |= (ADC_SMPR2_SMP3_0 | ADC_SMPR2_SMP3_1); // 011: 28.5 cycles

  //TIM3_TRGO (timer trigger output)
  //in ADC control register 2 (ADC_CR2)
  //EXTTRIG:External trigger conversion mode for regular channels
  ADC1->CR2 |= ADC_CR2_EXTTRIG; //1: Conversion on external event enabled
  //EXTSEL[2:0]:External event select for regular group
  ADC1->CR2 &= ~(ADC_CR2_EXTSEL);
  ADC1->CR2 |= ADC_CR2_EXTSEL_2; //100: Timer 3 TRGO event

  //Sequencer
  //in ADC regular sequence register 3 (ADC_SQR3)
  //Rank 1 - PA3, Channel 1
  ADC1->SQR3 |= (3UL << 0); //Port A 3

  //Number of conversion
  //in ADC regular sequence register 1 (ADC_SQR1)
  ADC1->SQR1 &= ~(ADC_SQR1_L);//0000: 1 conversion

  //Enable Scan mode
  //in ADC control register 1 (ADC_CR1)
  //SCAN: Scan mode
  ADC1->CR1 |= ADC_CR1_SCAN;

  //Enable DMA - ADC register
  //in ADC control register 2 (ADC_CR2)
  //DMA: Direct memory access mode
  ADC1->CR2 |= ADC_CR2_DMA;

  //Single Conversion
  //in ADC control register 2 (ADC_CR2)
  //CONT:Continuous conversion
  ADC1->CR2 &= ~(ADC_CR2_CONT); // 0: Single conversion mode

  //Power up ADC
  //in ADC control register 2 (ADC_CR2)
  ADC1->CR2 |= ADC_CR2_ADON;

  //Wait for ADC clock to stabilise (couple micro seconds)
  for (uint16_t i = 0; i < 36; i++); //72 cycles = 1uSec, for loop 4 cycles each, 72*2/4 = 36
}

/*
 * @brief ADC MIC DMA
 */
void adc_MIC_DMA_config(uint16_t *pAdcBuffer, uint16_t size)
{
  //DMA1 - Channel 1 --> ADC1 (ref man page 282)
  //Enable DMA1 Clock
  //in AHB peripheral clock enable register (RCC_AHBENR)
  //DMA1EN: DMA1 clock enable
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;

  //Clear DMA1 - Channel 1 status flags
  //in DMA interrupt flag clear register (DMA_IFCR)
  DMA1->IFCR |= 0x0F; //clear all 4 flags

  //Peripheral address (src) --> ADC_DR
  //in DMA channel x peripheral address register (DMA_CPARx) (x = 1..7, where x = channel number)
  DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));

  //Memory address (dst)
  //in DMA channel x memory address register (DMA_CMARx) (x = 1..7, where x = channel number)
  DMA1_Channel1->CMAR = (uint32_t)pAdcBuffer;

  //Number of transfer
  //in DMA channel x number of data register (DMA_CNDTRx) (x = 1..7, where x = channel number)
  DMA1_Channel1->CNDTR = size;

  //Circular / Normal
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //CIRC: Circular mode
  DMA1_Channel1->CCR |= DMA_CCR_CIRC; // 1: Circular mode enabled

  //Enable Memory increment
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //MINC: Memory increment mode
  DMA1_Channel1->CCR |= DMA_CCR_MINC; //1: Memory increment mode enabled

  //Disable Peripheral increment
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //PINC: Peripheral increment mode
  DMA1_Channel1->CCR &= ~(DMA_CCR_PINC); //0: Peripheral increment mode disabled

  //Size ADC 12-bits, Peripheral 16-bits
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //PSIZE[1:0]:Peripheral size
  DMA1_Channel1->CCR &= ~(DMA_CCR_PSIZE);
  DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0; // 01: 16-bits

  //Size Memory 16-bits
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //MSIZE[1:0]:Memory size
  DMA1_Channel1->CCR &= ~(DMA_CCR_MSIZE);
  DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0; // 01: 16-bits

  //Direction - Peripheral to Memory
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //DIR: Data transfer direction
  DMA1_Channel1->CCR &= ~(DMA_CCR_DIR); // 0: Read from peripheral

  //Enable DMA
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //EN:Channel enable
  DMA1_Channel1->CCR |= DMA_CCR_EN; //1: Channel enabled

  //Enable Transfer Complete Interrupt - DMA
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //TCIE:Transfer complete interrupt enable
  DMA1_Channel1->CCR |= DMA_CCR_TCIE; //1: TC interrupt enabled

  //Enable Half Transfer Interrupt
  //in DMA channel x configuration register (DMA_CCRx) (x = 1..7, where x = channel number)
  //HTIE:Half transfer interrupt enable
  DMA1_Channel1->CCR |= DMA_CCR_HTIE; //1: HT interrupt enabled

  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
