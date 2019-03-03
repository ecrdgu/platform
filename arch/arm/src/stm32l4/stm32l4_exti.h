
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_EXTI_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_EXTI_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <ecr/irq.h>

#include "chip.h"
#include "chip/stm32l4_exti.h"

/************************************************************************************
 * Public Data
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
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: stm32l4_gpiosetevent
 *
 * Description:
 *   Sets/clears GPIO based event and interrupt triggers.
 *
 * Input Parameters:
 *  pinset      - GPIO pin configuration
 *  risingedge  - Enables interrupt on rising edges
 *  fallingedge - Enables interrupt on falling edges
 *  event       - Generate event when set
 *  func        - When non-NULL, generate interrupt
 *  arg         - Argument passed to the interrupt callback
 *
 * Returned Value:
 *  Zero (OK) is returned on success, otherwise a negated errno value is returned
 *  to indicate the nature of the failure.
 *
 ************************************************************************************/

int stm32l4_gpiosetevent(uint32_t pinset, bool risingedge, bool fallingedge,
                         bool event, xcpt_t func, void *arg);

/****************************************************************************
 * Name: stm32l4_exti_alarm
 *
 * Description:
 *   Sets/clears EXTI alarm interrupt.
 *
 * Input Parameters:
 *  - rising/falling edge: enables interrupt on rising/falling edges
 *  - event:  generate event when set
 *  - func:   when non-NULL, generate interrupt
 *  - arg:    Argument passed to the interrupt callback
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure indicating the
 *   nature of the failure.
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
int stm32l4_exti_alarm(bool risingedge, bool fallingedge, bool event,
                       xcpt_t func, void *arg);
#endif

/****************************************************************************
 * Name: stm32l4_exti_wakeup
 *
 * Description:
 *   Sets/clears EXTI wakeup interrupt.
 *
 * Input Parameters:
 *  - rising/falling edge: enables interrupt on rising/falling edges
 *  - event:  generate event when set
 *  - func:   when non-NULL, generate interrupt
 *  - arg:    Argument passed to the interrupt callback
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure indicating the
 *   nature of the failure.
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_PERIODIC
int stm32l4_exti_wakeup(bool risingedge, bool fallingedge, bool event,
                        xcpt_t func, void *arg);
#endif

/****************************************************************************
 * Name: stm32l4_exti_comp
 *
 * Description:
 *   Sets/clears comparator based events and interrupt triggers.
 *
 * Input Parameters:
 *  - cmp: comparator
 *  - rising/falling edge: enables interrupt on rising/falling edges
 *  - event:  generate event when set
 *  - func:   when non-NULL, generate interrupt
 *  - arg:    Argument passed to the interrupt callback
 *
 * Returned Value:
 *   Zero (OK) returned on success; a negated errno value is returned on
 *   failure.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32L4_COMP
int stm32l4_exti_comp(int cmp, bool risingedge, bool fallingedge,
                      bool event, xcpt_t func, void *arg);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_EXTI_H */
