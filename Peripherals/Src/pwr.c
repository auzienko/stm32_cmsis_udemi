/*
 * pwr.c
 *
 *  Created on: Sep 9, 2021
 *      Author: aleksandruzienko
 */

#include "pwr.h"

/*
 * @brief Enter Sleep Mode
 */
void pwr_enter_sleep_mode(void)
{
  //Disable SysTick Interrupt (Avoid waking up the MCU)
  //in SysTick control and status register (STK_CTRL) (in Cortex M3 programming manual)
  SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);

  //Set SleepOnExit
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPONEXIT
  SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; //1: Enter sleep, or deep sleep, on return from an interrupt service routine.

  //Clear Sleep DEEP
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPDEEP
  SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk); //0: Sleep

  //Put the device into Sleep __WFI();
  __WFI();

  //On wake up --> Resume SysTick Interrupt
  //in SysTick control and status register (STK_CTRL) (in Cortex M3 programming manual)
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/*
 * @brief Enter Stop Mode
 */
void pwr_enter_stop_mode(void)
{
  //1. Disable SysTick Interrupt
  //2. Clear SleepOnExit
  //3. Set DEEP SLEEP
  //4. Clear PDDS bit in PWR
  //5. Put the device into low-power --> Stop __WFI();
  //6. Resume SysTick Interrupt

  //Disable SysTick Interrupt (Avoid waking up the MCU)
  //in SysTick control and status register (STK_CTRL) (in Cortex M3 programming manual)
  SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);

  //Clear SleepOnExit
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPONEXIT
  SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_Msk);

  //Set Sleep DEEP
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPDEEP
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; //1: Deep Sleep

  //Clear PDDS bit in PWR
  //in Power control register (PWR_CR)
  //PDDS:Power down deep sleep.
  PWR->CR &= ~(PWR_CR_PDDS); //0: Enter Stop mode when the CPU enters Deepsleep.

  //Put the device into low-power --> Stop __WFI();
  __WFI();

  //On wake up --> Resume SysTick Interrupt
  //in SysTick control and status register (STK_CTRL) (in Cortex M3 programming manual)
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/*
 * @brief Enter Standby Mode
 */
void pwr_enter_standby_mode(void)
{
  //1. Enable Wakeup functionality
  //2. Disable SysTick Interrupt
  //3. Set DEEP SLEEP bit
  //4. Set PDDS bit in PWR
  //5. Clear SleepOnExit
  //6. Clear Standby Flag
  //7. Clear Wakeup Flag
  //8. Standby mode

  //Enable Wakeup functionality
  //in Power control/status register (PWR_CSR)
  //EWUP:Enable WKUP pin
  PWR->CSR |= PWR_CSR_EWUP; //1: WKUP pin is used for wakeup from Standby mode

  //Disable SysTick Interrupt (Avoid waking up the MCU)
  //in SysTick control and status register (STK_CTRL) (in Cortex M3 programming manual)
  SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);

  //Set DEEP SLEEP bit
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPDEEP
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; //1: Deep Sleep

  //Set PDDS bit in PWR
  //in Power control register (PWR_CR)
  //PDDS:Power down deep sleep.
  PWR->CR |= PWR_CR_PDDS; //1: Enter Standby mode when the CPU enters Deepsleep.

  //Clear SleepOnExit
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPONEXIT
  SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_Msk);

  //Clear Standby Flag
  //in Power control register (PWR_CR)
  //CSBF:Clear stand by flag.
  PWR->CR |= PWR_CR_CSBF; //1: Clear the SBF Standby Flag (write).

  //Clear Wakeup Flag
  //in Power control register (PWR_CR)
  //CWUF:Clear wakeup flag.
  PWR->CR |= PWR_CR_CWUF; //1: Clear the WUF Wakeup Flag after 2 System clock cycles. (write)

  //Standby mode
  __WFI();
}
