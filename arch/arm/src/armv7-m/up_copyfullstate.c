
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <arch/irq.h>

#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_copyfullstate
 *
 * Description:
 *    Copy the entire register save area (including the floating point
 *    registers if applicable).  This is a little faster than most memcpy's
 *    since it does 32-bit transfers.
 *
 ****************************************************************************/

void up_copyfullstate(uint32_t *dest, uint32_t *src)
{
  int i;

  /* In the Cortex-M3 model, the state is copied from the stack to the TCB,
   * but only a reference is passed to get the state from the TCB.  So the
   * following check avoids copying the TCB save area onto itself:
   */

  if (src != dest)
    {
      for (i = 0; i < XCPTCONTEXT_REGS; i++)
        {
          *dest++ = *src++;
        }
    }
}
