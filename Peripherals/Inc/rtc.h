/*
 * rtc.h
 *
 *  Created on: Aug 28, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"
#include <time.h>

extern  const  char  *daysOfWeekString[7];

//RTC Time and Date structure
typedef struct RTC_TimeDate_TAG
{
  uint8_t hour;
  uint8_t min;
  uint8_t sec;

  uint8_t wday;
  uint8_t mday;
  uint8_t month;
  uint16_t year;
}RTC_TimeDate_t;

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

/*
 * @brief Set Time and Date
 * @note No need to set week day
 */
void rtc_set_TimeDate(RTC_TimeDate_t *pTimeDate);

/*
 * @brief Get Time and Date
 * @note Return a valid weekday
 */
void rtc_get_TimeDate(RTC_TimeDate_t *pTimeDate);

#endif /* INC_RTC_H_ */
