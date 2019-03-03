
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <ecr/arch.h>

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "up_arch.h"
#include "stm32l4_pwr.h"
#include "stm32l4_rcc.h"

/************************************************************************************
 * Private Functions
 ************************************************************************************/

static inline uint16_t stm32l4_pwr_getreg(uint8_t offset)
{
  return (uint16_t)getreg32(STM32L4_PWR_BASE + (uint32_t)offset);
}

static inline void stm32l4_pwr_putreg(uint8_t offset, uint16_t value)
{
  putreg32((uint32_t)value, STM32L4_PWR_BASE + (uint32_t)offset);
}

static inline void stm32l4_pwr_modifyreg(uint8_t offset, uint16_t clearbits, uint16_t setbits)
{
  modifyreg32(STM32L4_PWR_BASE + (uint32_t)offset, (uint32_t)clearbits, (uint32_t)setbits);
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: enableclk
 *
 * Description:
 *   Enable/disable the clock to the power control peripheral.  Enabling must be done
 *   after the APB1 clock is validly configured, and prior to using any functionality
 *   controlled by the PWR block (i.e. much of anything else provided by this module).
 *
 * Input Parameters:
 *   enable - True: enable the clock to the Power control (PWR) block.
 *
 * Returned Value:
 *   True:  the PWR block was previously enabled.
 *
 ************************************************************************************/

bool stm32l4_pwr_enableclk(bool enable)
{
  uint32_t regval;
  bool wasenabled;

  regval = getreg32(STM32L4_RCC_APB1ENR1);
  wasenabled = ((regval & RCC_APB1ENR1_PWREN) != 0);

  /* Power interface clock enable. */

  if (wasenabled && !enable)
    {
      /* Disable power interface clock */

      regval &= ~RCC_APB1ENR1_PWREN;
      putreg32(regval, STM32L4_RCC_APB1ENR1);
    }
  else if (!wasenabled && enable)
    {
      /* Enable power interface clock */

      regval |= RCC_APB1ENR1_PWREN;
      putreg32(regval, STM32L4_RCC_APB1ENR1);
    }

  return wasenabled;
}

/************************************************************************************
 * Name: stm32l4_pwr_enablebkp
 *
 * Description:
 *   Enables access to the backup domain (RTC registers, RTC backup data registers
 *   and backup SRAM).
 *
 * Input Parameters:
 *   writable - True: enable ability to write to backup domain registers
 *
 * Returned Value:
 *   True: The backup domain was previously writable.
 *
 ************************************************************************************/

bool stm32l4_pwr_enablebkp(bool writable)
{
  uint16_t regval;
  bool waswritable;

  /* Get the current state of the STM32L4 PWR control register 1 */

  regval      = stm32l4_pwr_getreg(STM32L4_PWR_CR1_OFFSET);
  waswritable = ((regval & PWR_CR1_DBP) != 0);

  /* Enable or disable the ability to write */

  if (waswritable && !writable)
    {
      /* Disable backup domain access */

      regval &= ~PWR_CR1_DBP;
      stm32l4_pwr_putreg(STM32L4_PWR_CR1_OFFSET, regval);
    }
  else if (!waswritable && writable)
    {
      /* Enable backup domain access */

      regval |= PWR_CR1_DBP;
      stm32l4_pwr_putreg(STM32L4_PWR_CR1_OFFSET, regval);

      /* Enable does not happen right away */

      up_udelay(4);
    }

  return waswritable;
}

/************************************************************************************
 * Name: stm32l4_pwr_enableusv
 *
 * Description:
 *   Enables or disables the USB Supply Valid monitoring.  Setting this bit is
 *   mandatory to use the USB OTG FS peripheral.
 *
 * Input Parameters:
 *   set - True: Vddusb is valid; False: Vddusb is not present. Logical and electrical
 *         isolation is applied to ignore this supply.
 *
 * Returned Value:
 *   True: The bit was previously set.
 *
 ************************************************************************************/

bool stm32l4_pwr_enableusv(bool set)
{
  uint16_t regval;
  bool wasset;

  /* Get the current state of the STM32L4 PWR control register 2 */

  regval      = stm32l4_pwr_getreg(STM32L4_PWR_CR2_OFFSET);
  wasset = ((regval & PWR_CR2_USV) != 0);

  /* Enable or disable the ability to write */

  if (wasset && !set)
    {
      /* Disable the Vddusb monitoring */

      regval &= ~PWR_CR2_USV;
      stm32l4_pwr_putreg(STM32L4_PWR_CR2_OFFSET, regval);
    }
  else if (!wasset && set)
    {
      /* Enable the Vddusb monitoring */

      regval |= PWR_CR2_USV;
      stm32l4_pwr_putreg(STM32L4_PWR_CR2_OFFSET, regval);
    }

  return wasset;
}
