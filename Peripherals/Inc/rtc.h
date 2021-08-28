/*
 * rtc.h
 *
 *  Created on: Aug 28, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"

/*
 * @brief RTC LSE configuration
 */
void  rtc_LSE_config(void);

/*
 * @brief RTC peripheral configuration
 */
void  rtc_config(void);

/*
 * @brief RTC set Unix counter
 */
void  rtc_set_unix_time(uint32_t unix);

/*
 * @brief RTC get Unix counter
 */
uint32_t  rtc_get_unix_time(void);

#endif /* INC_RTC_H_ */
