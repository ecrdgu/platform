
#ifndef STM32L4_EXTI_PWR_H_
#define STM32L4_EXTI_PWR_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include <ecr/irq.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_exti_pvd
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

int stm32l4_exti_pvd(bool risingedge, bool fallingedge, bool event,
                     xcpt_t func, void *arg);

#endif /* STM32L4_EXTI_PWR_H_ */
