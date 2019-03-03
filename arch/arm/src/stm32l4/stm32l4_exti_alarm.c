
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
#include "stm32l4_gpio.h"
#include "stm32l4_exti.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Interrupt handlers attached to the ALARM EXTI */

static xcpt_t g_alarm_callback;
static void  *g_callback_arg;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_exti_alarm_isr
 *
 * Description:
 *   EXTI ALARM interrupt service routine/dispatcher
 *
 ****************************************************************************/

static int stm32l4_exti_alarm_isr(int irq, void *context, FAR void *arg)
{
  int ret = OK;

  /* Dispatch the interrupt to the handler */

  if (g_alarm_callback != NULL)
    {
      ret = g_alarm_callback(irq, context, g_callback_arg);
    }

  /* Clear the pending EXTI interrupt */

  putreg32(EXTI1_RTC_ALARM, STM32L4_EXTI1_PR);

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_exti_alarm
 *
 * Description:
 *   Sets/clears EXTI alarm interrupt.
 *
 * Input Parameters:
 *  - rising/falling edge: enables interrupt on rising/falling edget
 *  - event:  generate event when set
 *  - func:   when non-NULL, generate interrupt
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure indicating the
 *   nature of the failure.
 *
 ****************************************************************************/

int stm32l4_exti_alarm(bool risingedge, bool fallingedge, bool event,
                       xcpt_t func, void *arg)
{
  g_alarm_callback = func;
  g_callback_arg   = arg;

  /* Install external interrupt handlers (if not already attached) */

  if (func)
    {
      irq_attach(STM32L4_IRQ_RTCALRM, stm32l4_exti_alarm_isr, NULL);
      up_enable_irq(STM32L4_IRQ_RTCALRM);
    }
  else
    {
      up_disable_irq(STM32L4_IRQ_RTCALRM);
    }

  /* Configure rising/falling edges */

  modifyreg32(STM32L4_EXTI1_RTSR,
              risingedge ? 0 : EXTI1_RTC_ALARM,
              risingedge ? EXTI1_RTC_ALARM : 0);
  modifyreg32(STM32L4_EXTI1_FTSR,
              fallingedge ? 0 : EXTI1_RTC_ALARM,
              fallingedge ? EXTI1_RTC_ALARM : 0);

  /* Enable Events and Interrupts */

  modifyreg32(STM32L4_EXTI1_EMR,
              event ? 0 : EXTI1_RTC_ALARM,
              event ? EXTI1_RTC_ALARM : 0);
  modifyreg32(STM32L4_EXTI1_IMR,
              func ? 0 : EXTI1_RTC_ALARM,
              func ? EXTI1_RTC_ALARM : 0);

  return OK;
}
