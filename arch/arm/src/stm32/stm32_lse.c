
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "up_arch.h"

#include "stm32_pwr.h"
#include "stm32_rcc.h"
#include "stm32_waste.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_rcc_enablelse
 *
 * Description:
 *   Enable the External Low-Speed (LSE) oscillator.
 *
 * Todo:
 *   Check for LSE good timeout and return with -1,
 *
 ****************************************************************************/

void stm32_rcc_enablelse(void)
{
  /* The LSE is in the RTC domain and write access is denied to this domain
   * after reset, you have to enable write access using DBP bit in the PWR CR
   * register before to configuring the LSE.
   */

  stm32_pwr_enablebkp(true);

#if defined(CONFIG_STM32_STM32L15XX)
  /* Enable the External Low-Speed (LSE) oscillator by setting the LSEON bit
   * the RCC CSR register.
   */

  modifyreg32(STM32_RCC_CSR, 0, RCC_CSR_LSEON);

  /* Wait for the LSE clock to be ready */

  while ((getreg32(STM32_RCC_CSR) & RCC_CSR_LSERDY) == 0)
    {
      up_waste();
    }

#else
  /* Enable the External Low-Speed (LSE) oscillator by setting the LSEON bit
   * the RCC BDCR register.
   */

  modifyreg16(STM32_RCC_BDCR, 0, RCC_BDCR_LSEON);

  /* Wait for the LSE clock to be ready */

  while ((getreg16(STM32_RCC_BDCR) & RCC_BDCR_LSERDY) == 0)
    {
      up_waste();
    }

#endif

  /* Disable backup domain access if it was disabled on entry */

  stm32_pwr_enablebkp(false);
}
