
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "up_arch.h"

#include "stm32l4_pwr.h"
#include "stm32l4_rcc.h"
#include "stm32l4_waste.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_rcc_enablelse
 *
 * Description:
 *   Enable the External Low-Speed (LSE) oscillator.
 *
 * Todo:
 *   Check for LSE good timeout and return with -1,
 *
 ****************************************************************************/

void stm32l4_rcc_enablelse(void)
{
  bool writable;
  uint32_t regval;

  /* The LSE is in the RTC domain and write access is denied to this domain
   * after reset, you have to enable write access using DBP bit in the PWR CR
   * register before to configuring the LSE.
   */

  writable = stm32l4_pwr_enablebkp(true);

  /* Enable the External Low-Speed (LSE) oscillator by setting the LSEON bit
   * the RCC BDCR register.
   */

  regval  = getreg32(STM32L4_RCC_BDCR);
  regval |= RCC_BDCR_LSEON;
  putreg32(regval,STM32L4_RCC_BDCR);

  /* Wait for the LSE clock to be ready */

  while (((regval = getreg32(STM32L4_RCC_BDCR)) & RCC_BDCR_LSERDY) == 0)
    {
      up_waste();
    }

  /* Disable backup domain access if it was disabled on entry */

  (void)stm32l4_pwr_enablebkp(writable);
}
