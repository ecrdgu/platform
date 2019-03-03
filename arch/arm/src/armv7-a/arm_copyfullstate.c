
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

  /* In the current ARM model, the state is always copied to and from the
   * stack and TCB.
   */

  for (i = 0; i < XCPTCONTEXT_REGS; i++)
    {
      *dest++ = *src++;
    }
}
