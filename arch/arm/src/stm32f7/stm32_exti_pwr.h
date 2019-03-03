
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_EXTI_PWR_H
#define __ARCH_ARM_SRC_STM32F7_STM32_EXTI_PWR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include <ecr/irq.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_exti_pvd
 *
 * Description:
 *   Sets/clears EXTI PVD interrupt.
 *
 * Input Parameters:
 *  - rising/falling edge: enables interrupt on rising/falling edge
 *  - event:  generate event when set
 *  - func:   when non-NULL, generate interrupt
 *  - arg:    Argument passed to the interrupt callback
 *
 * Returned Value:
 *   Zero (OK) returned on success; a negated errno value is returned on
 *   failure.
 *
 ****************************************************************************/

int stm32_exti_pvd(bool risingedge, bool fallingedge, bool event,
                   xcpt_t func, void *arg);

#endif /* __ARCH_ARM_SRC_STM32F7_STM32_EXTI_PWR_H */
