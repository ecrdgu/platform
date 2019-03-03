
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>

#include "up_arch.h"
#include "nvic.h"
#include "stm32l4_pwr.h"
#include "stm32l4_pm.h"
#include "stm32l4_rcc.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_pmlpr
 *
 * Description:
 *   Enter Low-Power Run (LPR) mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero means that LPR was successfully entered. Otherwise, LPR mode was not
 *   entered and a negated errno value is returned to indicate the cause of the
 *   failure.
 *
 ****************************************************************************/

int stm32l4_pmlpr(void)
{
  uint32_t regval;

  /* Enable MSI clock */

  regval  = getreg32(STM32L4_RCC_CR);
  regval |= RCC_CR_MSION;

  /* Set MSI clock to 2 MHz */

  regval &= ~RCC_CR_MSIRANGE_MASK;
  regval |= RCC_CR_MSIRANGE_2M; /* 2 MHz */
  regval |= RCC_CR_MSIRGSEL;    /* Select new MSIRANGE */
  putreg32(regval, STM32L4_RCC_CR);

  /* Select MSI clock as system clock source */

  regval  = getreg32(STM32L4_RCC_CFGR);
  regval &= ~RCC_CFGR_SW_MASK;
  regval |= RCC_CFGR_SW_MSI;
  putreg32(regval, STM32L4_RCC_CFGR);

  /* Wait until the MSI source is used as the system clock source */

  while ((getreg32(STM32L4_RCC_CFGR) & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_MSI)
    {
    }

  /* Enable Low-Power Run */

  regval  = getreg32(STM32L4_PWR_CR1);
  regval |= PWR_CR1_LPR;
  putreg32(regval, STM32L4_PWR_CR1);

  return OK;
}
