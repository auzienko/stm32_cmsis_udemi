/*
 * adc.h
 *
 *  Created on: Aug 19, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"

//ADC Single channel select
typedef enum ADC_SingleSelect_TAG
{
  ADC_SingleSelect_Potentiometer = 1,
  ADC_SingleSelect_Joystick_x,
  ADC_SingleSelect_Joystick_y,
}ADC_SingleSelect_e;

/*
 * @brief ADC GPIO pins: PA1 - potentiometer, PA2 - X, PA3 - Y joystick
 */
void  adc_GPIO_config(void);

/*
 * @brief ADC Single Channel configuration
 */
void  adc_singleChannel_config(ADC_SingleSelect_e channel);

/*
 * @brief ADC Start - SWSTART
 */
void  adc_start(void);

/*
 * @brief ADC Stop
 */
void  adc_stop(void);

/*
 * @brief ADC Poll for end of conversion
 */
bool  adc_pollForEndOfConversion(uint32_t msTimeout);

/*
 * @brief ADC Read value
 */
uint16_t  adc_readValue(void);

/*
 * @brief ADC Enable/Disable Interrupt - EOC (end of conversion)
 */
void  adc_enableEOC_IT(void);
void  adc_disableEOC_IT(void);

/*
 * @brief ADC Multi-channel configuration
 */
void  adc_multiChannel_config(void);

/*
 * @ADC DMA configuration
 */
void  adc_multiChannel_DMA_config(uint16_t *pAdcBuffer);

/*
 * @brief ADC Analog watchdog configuration
 */
void adc_AWD_config(uint8_t channel, uint16_t lowThreshold, uint16_t highThreshold);

/*
 * @brief ADC Injected configuration
 */
void adc_INJ_config(uint8_t channel);

/*
 * @brief ADC Injected Start
 */
void adc_INJ_start(void);

/*
 * @brief ADC poll for end of conversion
 */
bool adc_INJ_pollForConversion(uint32_t timeout);

#endif /* INC_ADC_H_ */
