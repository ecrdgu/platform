
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
 * Private Functions
 ****************************************************************************/

static int do_stop(void)
{
  uint32_t regval;

  /* Set SLEEPDEEP bit of Cortex System Control Register */

  regval  = getreg32(NVIC_SYSCON);
  regval |= NVIC_SYSCON_SLEEPDEEP;
  putreg32(regval, NVIC_SYSCON);

  /* Sleep until the wakeup interrupt or event occurs */

#ifdef CONFIG_PM_WFE
  /* Mode: SLEEP + Entry with WFE */

  asm("wfe");
#else
  /* Mode: SLEEP + Entry with WFI */

  asm("wfi");
#endif

  /* Clear SLEEPDEEP bit of Cortex System Control Register */

  regval  = getreg32(NVIC_SYSCON);
  regval &= ~NVIC_SYSCON_SLEEPDEEP;
  putreg32(regval, NVIC_SYSCON);

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_pmstop
 *
 * Description:
 *   Enter STOP mode.
 *
 * Input Parameters:
 *   lpds - true: To further reduce power consumption in Stop mode, put the
 *          internal voltage regulator in low-power mode using the LPDS bit
 *          of the Power control register (PWR_CR).
 *
 * Returned Value:
 *   Zero means that the STOP was successfully entered and the system has
 *   been re-awakened.  The internal volatage regulator is back to its
 *   original state.  Otherwise, STOP mode did not occur and a negated
 *   errno value is returned to indicate the cause of the failure.
 *
 ****************************************************************************/

int stm32l4_pmstop(bool lpds)
{
  uint32_t regval;

  /* Clear Low-Power Mode Selection (LPMS) bits in power control register 1. */

  regval  = getreg32(STM32L4_PWR_CR1);
  regval &= ~PWR_CR1_LPMS_MASK;

  /* Select Stop 1 mode with low-power regulator if so requested */

  if (lpds)
    {
      regval |= PWR_CR1_LPMS_STOP1LPR;
    }

  putreg32(regval, STM32L4_PWR_CR1);

  return do_stop();
}

/****************************************************************************
 * Name: stm32l4_pmstop2
 *
 * Description:
 *   Enter STOP2 mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero means that the STOP2 was successfully entered and the system has
 *   been re-awakened.  Otherwise, STOP2 mode did not occur and a negated
 *   errno value is returned to indicate the cause of the failure.
 *
 ****************************************************************************/

int stm32l4_pmstop2(void)
{
  uint32_t regval;

  /* Select Stop 2 mode in power control register 1. */

  regval  = getreg32(STM32L4_PWR_CR1);
  regval &= ~PWR_CR1_LPMS_MASK;
  regval |= PWR_CR1_LPMS_STOP2;
  putreg32(regval, STM32L4_PWR_CR1);

  return do_stop();
}
