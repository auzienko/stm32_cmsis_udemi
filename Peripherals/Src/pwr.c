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

  //Clear SleepOnExit
  //in Cortex M3 programming manual
  //in System control register (SCB_SCR)
  //SLEEPONEXIT
  SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_Msk); //0: Do not sleep when returning to Thread mode.

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
