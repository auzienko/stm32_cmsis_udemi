/*
 * rtc.c
 *
 *  Created on: Aug 28, 2021
 *      Author: aleksandruzienko
 */

#include "rtc.h"

const char  *daysOfWeekString[7] =
{
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SUT",
    "SUN"
};

/*
 * @brief RTC LSE configuration
 */
void  rtc_LSE_config(void)
{
  uint32_t      RTC_bdcr;
  __IO uint32_t tempRead;

  //Enable PWR clock
  //in APB1 peripheral clock enable register (RCC_APB1ENR)
  //PWREN:Power interface clock enable
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; //1: Power interface clock enable

  //Enable write access to backup domain
  //in Power control register (PWR_CR)
  //DBP:Disable backup domain write protection
  PWR->CR |= PWR_CR_DBP; //1: Access to RTC and Backup registers enabled
  while (!(PWR->CR & PWR_CR_DBP)); //wait for ready

  //Check if LSE is already the clock source --> return
  //in Backup domain control register (RCC_BDCR)
  //RTCSEL[1:0]:RTC clock source selection
  RTC_bdcr = RCC->BDCR;
  if ((RTC_bdcr & RCC_BDCR_RTCSEL_0) && (RTC_bdcr & ~RCC_BDCR_RTCSEL_1)) //01: LSE oscillator clock used as RTC clock
  {
    printf("LSE is already selected\n\r");
    return ;
  }

  //Reset backup domain (allow to change clock source)
  //in Backup domain control register (RCC_BDCR)
  tempRead = (RTC_bdcr & ~RCC_BDCR_RTCSEL);
  RCC->BDCR = RCC_BDCR_BDRST;
  RCC->BDCR = 0;

  //Restore value
  //in Backup domain control register (RCC_BDCR)
  RCC->BDCR = tempRead;

  //Enable LSE and wait for ready
  //in Backup domain control register (RCC_BDCR)
  //LSEON:External Low Speed oscillator enable
  RCC->BDCR |= RCC_BDCR_LSEON; //1: External 32 kHz oscillator ON
  while (!(RCC->BDCR & RCC_BDCR_LSERDY)); //wait for ready

  //Select LSE as source
  //in Backup domain control register (RCC_BDCR)
  //RTCSEL[1:0]:RTC clock source selection
  RCC->BDCR |= RCC_BDCR_RTCSEL_0; //01: LSE oscillator clock used as RTC clock

  //Enable RTC
  //in Backup domain control register (RCC_BDCR)
  //RTCEN:RTC clock enable
  RCC->BDCR |= RCC_BDCR_RTCEN; //1: RTC clock enabled
}

/*
 * @brief RTC peripheral configuration
 */
void  rtc_config(void)
{
  //Check for ongoing operation
  //in RTC control register low (RTC_CRL)
  //RTOFF:RTC operation OFF
  while (!(RTC->CRL & RTC_CRL_RTOFF));

  //Enter configuration
  //in RTC control register low (RTC_CRL)
  //CNF:Configuration flag
  RTC->CRL |= RTC_CRL_CNF; //1: Enter configuration mode

  //Set prescaler
  //in RTC prescaler load register (RTC_PRLH / RTC_PRLL)
  //PRL[19:16]:RTC prescaler reload value high fTR_CLK = fRTCCLK/(PRL+1)
  //PRL[15:0]:RTC prescaler reload value low
  RTC->PRLH = 0;
  RTC->PRLL = 0x7FFFF;

  //Exit configuration
  //in RTC control register low (RTC_CRL)
  //CNF:Configuration flag
  RTC->CRL &= ~(RTC_CRL_CNF); //0: Exit configuration mode (start update of RTC registers).
  while (!(RTC->CRL & RTC_CRL_RTOFF));
}

/*
 * @brief RTC set Unix counter
 */
void  rtc_set_unix_time(uint32_t unix)
{
  //Check for ongoing operation
  //in RTC control register low (RTC_CRL)
  //RTOFF:RTC operation OFF
  while (!(RTC->CRL & RTC_CRL_RTOFF));

  //Enter configuration
  //in RTC control register low (RTC_CRL)
  //CNF:Configuration flag
  RTC->CRL |= RTC_CRL_CNF; //1: Enter configuration mode

  //Set counter
  //RTC counter register (RTC_CNTH / RTC_CNTL)
  //RTC_CNT[15:0]:RTC counter low
  RTC->CNTL = unix & 0xFFFF;
  RTC->CNTH = (unix >> 16) & 0xFFFF;

  //Exit configuration
  //in RTC control register low (RTC_CRL)
  //CNF:Configuration flag
  RTC->CRL &= ~(RTC_CRL_CNF); //0: Exit configuration mode (start update of RTC registers).
  while (!(RTC->CRL & RTC_CRL_RTOFF));
}

/*
 * @brief RTC get Unix counter
 */
uint32_t  rtc_get_unix_time(void)
{
  return ((uint32_t)(RTC->CNTL + (RTC->CNTH << 16)));
}

/*
 * @brief Set Time and Date
 * @note No need to set week day
 */
void rtc_set_TimeDate(RTC_TimeDate_t *pTimeDate)
{
  struct  tm  myTime;
  struct  tm  *timePtr = &myTime;
  time_t      timeSecs;

  //Convert Time Date typedef into time.h library structure
  timePtr->tm_hour = pTimeDate->hour;
  timePtr->tm_min = pTimeDate->min;
  timePtr->tm_sec = pTimeDate->sec;
  timePtr->tm_mday = pTimeDate->mday;
  timePtr->tm_mon = pTimeDate->month-1;
  timePtr->tm_year = pTimeDate->year-1900;
  timeSecs = mktime(timePtr);

  //Set STM32 RTC Counter value
  rtc_set_unix_time((uint32_t)timeSecs);
}

/*
 * @brief Get Time and Date
 * @note Return a valid weekday
 */
void rtc_get_TimeDate(RTC_TimeDate_t *pTimeDate)
{
  struct  tm  myTime;
  struct  tm *timePtr = &myTime;
  time_t      timeSecs;

  //Get STM32 RTC Unix counter
  timeSecs = (time_t)rtc_get_unix_time();

  //time.h library to convert back to normal TimeDate
  timePtr = gmtime(&timeSecs);

  //Return result to user
  pTimeDate->hour = timePtr->tm_hour;
  pTimeDate->min = timePtr->tm_min;
  pTimeDate->sec = timePtr->tm_sec;

  pTimeDate->mday = timePtr->tm_mday;
  pTimeDate->wday = timePtr->tm_wday;
  pTimeDate->month = timePtr->tm_mon + 1; //0 - 11 --> 1 - 12
  pTimeDate->year = timePtr->tm_year + 1900;
}

