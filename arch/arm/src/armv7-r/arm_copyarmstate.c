
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include <arch/irq.h>

#include "up_internal.h"

#ifdef CONFIG_ARCH_FPU

/****************************************************************************
 * Name: up_copyarmstate
 *
 * Description:
 *    Copy the ARM portion of the register save area (omitting the floating
 *    point registers) and save the floating pointer register directly.
 *
 ****************************************************************************/

void up_copyarmstate(uint32_t *dest, uint32_t *src)
{
  int i;

  /* In the Cortex-R model, the state is copied from the stack to the TCB,
   * but only a reference is passed to get the state from the TCB.  So the
   * following check avoids copying the TCB save area onto itself:
   */

  if (src != dest)
    {
      /* Save the floating point registers: This will initialize the floating
       * registers at indices ARM_CONTEXT_REGS through (XCPTCONTEXT_REGS-1)
       */

      up_savefpu(dest);

      /* Then copy all of the ARM registers (omitting the floating point
       * registers).  Indices: 0 through (ARM_CONTEXT_REGS-1).
       */

      for (i = 0; i < ARM_CONTEXT_REGS; i++)
        {
          *dest++ = *src++;
        }
    }
}

#endif /* CONFIG_ARCH_FPU */
