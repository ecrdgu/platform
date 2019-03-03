
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
#include "stm32_exti_pwr.h"

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
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_exti_pvd_isr
 *
 * Description:
 *   EXTI PVD interrupt service routine/dispatcher
 *
 ****************************************************************************/

static int stm32_exti_pvd_isr(int irq, void *context, void *arg)
{
  int ret = OK;

  /* Clear the pending EXTI interrupt */

  putreg32(EXTI_PVD_LINE, STM32_EXTI_PR);

  /* And dispatch the interrupt to the handler */

  if (g_pvd_callback)
    {
      ret = g_pvd_callback(irq, context, g_callback_arg);
    }

  return ret;
}

/****************************************************************************
 * Public Functions
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
 *
 * Returned Value:
 *   Zero (OK) returned on success; a negated errno value is returned on
 *   failure.
 *
 ****************************************************************************/

int stm32_exti_pvd(bool risingedge, bool fallingedge, bool event,
                   xcpt_t func, void *arg)
{
  /* Get the previous GPIO IRQ handler; Save the new IRQ handler. */

  g_pvd_callback = func;
  g_callback_arg = arg;

  /* Install external interrupt handlers (if not already attached) */

  if (func)
    {
      irq_attach(STM32_IRQ_PVD, stm32_exti_pvd_isr, NULL);
      up_enable_irq(STM32_IRQ_PVD);
    }
  else
    {
      up_disable_irq(STM32_IRQ_PVD);
    }

  /* Configure rising/falling edges */

  modifyreg32(STM32_EXTI_RTSR,
              risingedge ? 0 : EXTI_PVD_LINE,
              risingedge ? EXTI_PVD_LINE : 0);
  modifyreg32(STM32_EXTI_FTSR,
              fallingedge ? 0 : EXTI_PVD_LINE,
              fallingedge ? EXTI_PVD_LINE : 0);

  /* Enable Events and Interrupts */

  modifyreg32(STM32_EXTI_EMR,
              event ? 0 : EXTI_PVD_LINE,
              event ? EXTI_PVD_LINE : 0);
  modifyreg32(STM32_EXTI_IMR,
              func ? 0 : EXTI_PVD_LINE,
              func ? EXTI_PVD_LINE : 0);

  return OK;
}
