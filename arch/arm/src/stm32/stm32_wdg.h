
#ifndef __ARCH_ARM_SRC_STM32_STM32_WDG_H
#define __ARCH_ARM_SRC_STM32_STM32_WDG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"
#include "chip/stm32_wdg.h"

#ifdef CONFIG_WATCHDOG

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_iwdginitialize
 *
 * Description:
 *   Initialize the IWDG watchdog time.  The watchdog timer is initialized
 *   and registers as 'devpath.  The initial state of the watchdog time is
 *   disabled.
 *
 * Input Parameters:
 *   devpath - The full path to the watchdog.  This should be of the form
 *     /dev/watchdog0
 *   lsifreq - The calibrated LSI clock frequency
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_IWDG
void stm32_iwdginitialize(FAR const char *devpath, uint32_t lsifreq);
#endif

/****************************************************************************
 * Name: stm32_wwdginitialize
 *
 * Description:
 *   Initialize the WWDG watchdog time.  The watchdog timer is initializeed and
 *   registers as 'devpath.  The initial state of the watchdog time is
 *   disabled.
 *
 * Input Parameters:
 *   devpath - The full path to the watchdog.  This should be of the form
 *     /dev/watchdog0
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_WWDG
void stm32_wwdginitialize(FAR const char *devpath);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_WATCHDOG */
#endif /* __ARCH_ARM_SRC_STM32_STM32_WDG_H */
