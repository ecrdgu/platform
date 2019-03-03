
#ifndef __ARCH_ARM_SRC_STM32F0_STM32F0_HSI48_H
#define __ARCH_ARM_SRC_STM32F0_STM32F0_HSI48_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#ifdef CONFIG_STM32F0_HAVE_HSI48

/************************************************************************************
 * Public Types
 ************************************************************************************/

enum syncsrc_e
{
  SYNCSRC_GPIO = 0, /* GPIO selected as SYNC signal source */
  SYNCSRC_LSE,      /* LSE selected as SYNC signal source */
  SYNCSRC_USB,      /* USB SOF selected as SYNC signal source */
};

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/****************************************************************************
 * Name: stm32f0_enable_hsi48
 *
 * Description:
 *   On STM32F04x, STM32F07x and STM32F09x devices only, the HSI48 clock
 *   signal is generated from an internal 48 MHz RC oscillator and can be
 *   used directly as a system clock or divided and be used as PLL input.
 *
 *   The internal 48MHz RC oscillator is mainly dedicated to provide a high
 *   precision clock to the USB peripheral by means of a special Clock
 *   Recovery System (CRS) circuitry, which could use the USB SOF signal or
 *   the LSE or an external signal to automatically adjust the oscillator
 *   frequency on-fly, in a very small steps. This oscillator can also be
 *   used as a system clock source when the system is in run mode; it will
 *   be disabled as soon as the system enters in Stop or Standby mode. When
 *   the CRS is not used, the HSI48 RC oscillator runs on its default
 *   frequency which is subject to manufacturing process variations.
 *
 * Input Parameters:
 *   Identifies the syncrhonization source for the HSI48.  When used as the
 *   USB source clock, this must be set to SYNCSRC_USB.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void stm32f0_enable_hsi48(enum syncsrc_e syncsrc);

/****************************************************************************
 * Name: stm32f0_disable_hsi48
 *
 * Description:
 *   Disable the HSI48 clock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void stm32f0_disable_hsi48(void);

#endif /* CONFIG_STM32F0_HAVE_HSI48 */
#endif /* __ARCH_ARM_SRC_STM32F0_STM32F0_HSI48_H */
