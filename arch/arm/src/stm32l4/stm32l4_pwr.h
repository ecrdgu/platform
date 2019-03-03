
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_PWR_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_PWR_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>

#include "chip.h"
#include "chip/stm32l4_pwr.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

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

bool stm32l4_pwr_enableclk(bool enable);

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

bool stm32l4_pwr_enablebkp(bool writable);

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

bool stm32l4_pwr_enableusv(bool set);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_PWR_H */
