
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/irq.h>
#include <ecr/arch.h>

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <arch/irq.h>

#include "up_arch.h"
#include "chip.h"
#include "stm32_gpio.h"
#include "stm32_exti.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Interrupt handlers attached to the RTC WAKEUP EXTI */

static xcpt_t g_wakeup_callback;
static void  *g_callback_arg;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_exti_wakeup_isr
 *
 * Description:
 *   EXTI periodic WAKEUP interrupt service routine/dispatcher
 *
 ****************************************************************************/

static int stm32_exti_wakeup_isr(int irq, void *context, FAR void *arg)
{
  int ret = OK;

  /* Dispatch the interrupt to the handler */

  if (g_wakeup_callback != NULL)
    {
      ret = g_wakeup_callback(irq, context, g_callback_arg);
    }

  /* Clear the pending EXTI interrupt */

  putreg32(EXTI_RTC_WAKEUP, STM32_EXTI_PR);

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_exti_wakeup
 *
 * Description:
 *   Sets/clears EXTI wakeup interrupt.
 *
 * Input Parameters:
 *  - rising/falling edge: enables interrupt on rising/falling edges
 *  - event:  generate event when set
 *  - func:   when non-NULL, generate interrupt
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure indicating the
 *   nature of the failure.
 *
 ****************************************************************************/

int stm32_exti_wakeup(bool risingedge, bool fallingedge, bool event,
                      xcpt_t func, void *arg)
{
  g_wakeup_callback = func;
  g_callback_arg    = arg;

  /* Install external interrupt handlers (if not already attached) */

  if (func)
    {
      irq_attach(STM32_IRQ_RTC_WKUP, stm32_exti_wakeup_isr, NULL);
      up_enable_irq(STM32_IRQ_RTC_WKUP);
    }
  else
    {
      up_disable_irq(STM32_IRQ_RTC_WKUP);
    }

  /* Configure rising/falling edges */

  modifyreg32(STM32_EXTI_RTSR,
              risingedge ? 0 : EXTI_RTC_WAKEUP,
              risingedge ? EXTI_RTC_WAKEUP : 0);
  modifyreg32(STM32_EXTI_FTSR,
              fallingedge ? 0 : EXTI_RTC_WAKEUP,
              fallingedge ? EXTI_RTC_WAKEUP : 0);

  /* Enable Events and Interrupts */

  modifyreg32(STM32_EXTI_EMR,
              event ? 0 : EXTI_RTC_WAKEUP,
              event ? EXTI_RTC_WAKEUP : 0);
  modifyreg32(STM32_EXTI_IMR,
              func ? 0 : EXTI_RTC_WAKEUP,
              func ? EXTI_RTC_WAKEUP : 0);

  return OK;
}
