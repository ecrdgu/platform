
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <debug.h>

#include <ecr/irq.h>

#include "sched/sched.h"
#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_prefetchabort
 *
 * Description:
 *   This is the prefetch abort exception handler. The ARM prefetch abort
 *   exception occurs when a memory fault is detected during an an
 *   instruction fetch.
 *
 ****************************************************************************/

uint32_t *arm_prefetchabort(uint32_t *regs, uint32_t ifar, uint32_t ifsr)
{
  /* Save the saved processor context in CURRENT_REGS where it can be accessed
   * for register dumps and possibly context switching.
   */

  CURRENT_REGS = regs;

  /* Crash -- possibly showing diagnostic debug information. */

  _alert("Prefetch abort. PC: %08x IFAR: %08x IFSR: %08x\n",
        regs[REG_PC], ifar, ifsr);
  PANIC();
  return regs; /* To keep the compiler happy */
}
