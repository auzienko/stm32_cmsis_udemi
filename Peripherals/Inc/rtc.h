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

/*
 * @brief Alarm configuration
 */
void rtc_alarm_config(void);

/*
 * @brief Alarm counter Set
 */
void rtc_set_unix_alarm(uint32_t unix);

/*
 * @brief Alarm counter Get
 */
uint32_t rtc_get_unix_alarm(void);

/*
 * @brief Set Alarm
 */
void rtc_set_alarm(RTC_TimeDate_t *pTimeDate);

/*
 * @brief Get Alarm
 */
void rtc_get_alarm(RTC_TimeDate_t *pTimeDate);

/*
 * @brief Enable Backup registers write access
 */
void rtc_backup_registers_enable(void);

/*
 * @brief Tamper Enable
 */
void rtc_tamper_enable(void);

/*
 * @brief Tamper Disable
 */
void rtc_tamper_disable(void);

#endif /* INC_RTC_H_ */
