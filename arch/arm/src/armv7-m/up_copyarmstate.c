
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include <arch/irq.h>

#include "up_internal.h"

#if defined(CONFIG_ARCH_FPU) && \
    (!defined(CONFIG_ARMV7M_CMNVECTOR) || defined(CONFIG_ARMV7M_LAZYFPU))

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

  /* In the Cortex-M3 model, the state is copied from the stack to the TCB,
   * but only a reference is passed to get the state from the TCB.  So the
   * following check avoids copying the TCB save area onto itself:
   */

  if (src != dest)
    {
      /* Save the floating point registers: This will initialize the floating
       * registers at indices SW_INT_REGS through (SW_INT_REGS+SW_FPU_REGS-1)
       */

      up_savefpu(dest);

      /* Save the block of ARM registers that were saved by the interrupt
       * handling logic.  Indices: 0 through (SW_INT_REGS-1).
       */

      for (i = 0; i < SW_INT_REGS; i++)
        {
          *dest++ = *src++;
        }

      /* Skip over the floating point registers and save the block of ARM
       * registers that were saved by the hardware when the interrupt was
       * taken.  Indices: (SW_INT_REGS+SW_FPU_REGS) through
       * (XCPTCONTEXT_REGS-1)
       */

      src  += SW_FPU_REGS;
      dest += SW_FPU_REGS;

      for (i = 0; i < HW_XCPT_REGS; i++)
        {
          *dest++ = *src++;
        }
    }
}

#endif /* CONFIG_ARCH_FPU && (!CONFIG_ARMV7M_CMNVECTOR || CONFIG_ARMV7M_LAZYFPU) */
