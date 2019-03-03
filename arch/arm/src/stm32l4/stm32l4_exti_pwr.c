
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
#include "stm32l4_exti_pwr.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Interrupt handlers attached to the PVD EXTI */

static xcpt_t g_pvd_callback;
static void  *g_callback_arg;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_exti_pvd_isr
 *
 * Description:
 *   EXTI PVD interrupt service routine/dispatcher
 *
 ****************************************************************************/

static int stm32l4_exti_pvd_isr(int irq, void *context, FAR void *arg)
{
  int ret = OK;

  /* Clear the pending EXTI interrupt */

  putreg32(EXTI1_PVD_LINE, STM32L4_EXTI1_PR);

  /* And dispatch the interrupt to the handler */

  if (g_pvd_callback != NULL)
    {
      ret = g_pvd_callback(irq, context, g_callback_arg);
    }

  return ret;
}

/****************************************************************************
 * Public Functions
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
 *
 * Returned Value:
 *   Zero (OK) returned on success; a negated errno value is returned on
 *   failure.
 *
 ****************************************************************************/

int stm32l4_exti_pvd(bool risingedge, bool fallingedge, bool event,
                        xcpt_t func, void *arg)
{
  /* Get the previous GPIO IRQ handler; Save the new IRQ handler. */

  g_pvd_callback = func;
  g_callback_arg = arg;

  /* Install external interrupt handlers (if not already attached) */

  if (func)
    {
      irq_attach(STM32L4_IRQ_PVD, stm32l4_exti_pvd_isr, NULL);
      up_enable_irq(STM32L4_IRQ_PVD);
    }
  else
    {
      up_disable_irq(STM32L4_IRQ_PVD);
    }

  /* Configure rising/falling edges */

  modifyreg32(STM32L4_EXTI1_RTSR,
              risingedge ? 0 : EXTI1_PVD_LINE,
              risingedge ? EXTI1_PVD_LINE : 0);
  modifyreg32(STM32L4_EXTI1_FTSR,
              fallingedge ? 0 : EXTI1_PVD_LINE,
              fallingedge ? EXTI1_PVD_LINE : 0);

  /* Enable Events and Interrupts */

  modifyreg32(STM32L4_EXTI1_EMR,
              event ? 0 : EXTI1_PVD_LINE,
              event ? EXTI1_PVD_LINE : 0);
  modifyreg32(STM32L4_EXTI1_IMR,
              func ? 0 : EXTI1_PVD_LINE,
              func ? EXTI1_PVD_LINE : 0);

  return OK;
}
