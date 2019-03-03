
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>

#include "up_arch.h"
#include "nvic.h"
#include "stm32l4_pwr.h"
#include "stm32l4_pm.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_pmstandby
 *
 * Description:
 *   Enter STANDBY mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   On success, this function will not return (STANDBY mode can only be
 *   terminated with a reset event).  Otherwise, STANDBY mode did not occur
 *   and a negated errno value is returned to indicate the cause of the
 *   failure.
 *
 ****************************************************************************/

int stm32l4_pmstandby(void)
{
  uint32_t regval;

  /* Clear the Wake-Up Flags by setting the CWUFx bits in the power status
   * clear register
   */
  regval = PWR_SCR_CWUF1 | PWR_SCR_CWUF2 | PWR_SCR_CWUF3 |
           PWR_SCR_CWUF4 | PWR_SCR_CWUF5;
  putreg32(regval, STM32L4_PWR_SCR);

  /* Select Standby mode */

  regval  = getreg32(STM32L4_PWR_CR1);
  regval &= ~PWR_CR1_LPMS_MASK;
  regval |= PWR_CR1_LPMS_STANDBY;

  putreg32(regval, STM32L4_PWR_CR1);

  /* Set SLEEPDEEP bit of Cortex System Control Register */

  regval  = getreg32(NVIC_SYSCON);
  regval |= NVIC_SYSCON_SLEEPDEEP;
  putreg32(regval, NVIC_SYSCON);

  /* Sleep until the wakeup reset occurs */

  asm("wfi");
  return OK;  /* Won't get here */
}
