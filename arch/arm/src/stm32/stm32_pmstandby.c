
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>

#include "up_arch.h"
#include "nvic.h"
#include "stm32_pwr.h"
#include "stm32_pm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_pmstandby
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

int stm32_pmstandby(void)
{
  uint32_t regval;

  /* Clear the Wake-Up Flag by setting the CWUF bit in the power control
   * register.
   */

  regval  = getreg32(STM32_PWR_CR);
  regval |= PWR_CR_CWUF;
  putreg32(regval, STM32_PWR_CR);

  /* Set the Power Down Deep Sleep (PDDS) bit in the power control register. */

  regval |= PWR_CR_PDDS;
  putreg32(regval, STM32_PWR_CR);

  /* Set SLEEPDEEP bit of Cortex System Control Register */

  regval  = getreg32(NVIC_SYSCON);
  regval |= NVIC_SYSCON_SLEEPDEEP;
  putreg32(regval, NVIC_SYSCON);

  /* Sleep until the wakeup reset occurs */

  asm("wfi");
  return OK;  /* Won't get here */
}
