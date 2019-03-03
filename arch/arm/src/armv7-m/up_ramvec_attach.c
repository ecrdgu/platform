
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/irq.h>
#include <ecr/arch.h>

#include "ram_vectors.h"

#ifdef CONFIG_ARCH_RAMVECTORS

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Common exception entrypoint */

void exception_common(void);

/****************************************************************************
 * Name: up_ramvec_attach
 *
 * Description:
 *   Configure the ram vector table so that IRQ number 'irq' will be
 *   dispatched by hardware to 'vector'
 *
 ****************************************************************************/

int up_ramvec_attach(int irq, up_vector_t vector)
{
  int ret = -EINVAL;

  irqinfo("%s IRQ%d\n", vector ? "Attaching" : "Detaching", irq);

  if ((unsigned)irq < NR_VECTORS)
    {
      irqstate_t flags;

      /* If the new vector is NULL, then the vector is being detached. In
       * this case, disable the itnerrupt and direct any interrupts to the
       * common exception handler.
       */

      flags = enter_critical_section();
      if (vector == NULL)
        {
          /* Disable the interrupt if we can before detaching it.  We might
           * not be able to do this for all interrupts.
           */

          up_disable_irq(irq);

          /* Detaching the vector really means re-attaching it to the
           * common exception handler.
           */

           vector = exception_common;
        }

      /* Save the new vector in the vector table */

      g_ram_vectors[irq] = vector;
      leave_critical_section(flags);
      ret = OK;
    }

  return ret;
}

#endif /* !CONFIG_ARCH_RAMVECTORS */
