
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include "up_arch.h"
#include "stm32l4_rcc.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_rcc_enablelsi
 *
 * Description:
 *   Enable the Internal Low-Speed (LSI) RC Oscillator.
 *
 ****************************************************************************/

void stm32l4_rcc_enablelsi(void)
{
  /* Enable the Internal Low-Speed (LSI) RC Oscillator by setting the LSION
   * bit the RCC CSR register.
   */

  modifyreg32(STM32L4_RCC_CSR, 0, RCC_CSR_LSION);

  /* Wait for the internal LSI oscillator to be stable. */

  while ((getreg32(STM32L4_RCC_CSR) & RCC_CSR_LSIRDY) == 0);
}

/****************************************************************************
 * Name: stm32l4_rcc_disablelsi
 *
 * Description:
 *   Disable the Internal Low-Speed (LSI) RC Oscillator.
 *
 ****************************************************************************/

void stm32l4_rcc_disablelsi(void)
{
  /* Enable the Internal Low-Speed (LSI) RC Oscillator by setting the LSION
   * bit the RCC CSR register.
   */

  modifyreg32(STM32L4_RCC_CSR, RCC_CSR_LSION, 0);

  /* LSIRDY should go low after 3 LSI clock cycles */
}
