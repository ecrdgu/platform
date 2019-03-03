
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
 * Name: arm_dataabort
 *
 * Input Parameters:
 *   regs - The standard, ARM register save array.
 *   dfar - Fault address register.
 *   dfsr - Fault status register.
 *
 * Description:
 *   This is the data abort exception handler. The ARM data abort exception
 *   occurs when a memory fault is detected during a data transfer.
 *
 ****************************************************************************/

uint32_t *arm_dataabort(uint32_t *regs, uint32_t dfar, uint32_t dfsr)
{
  /* Save the saved processor context in CURRENT_REGS where it can be accessed
   * for register dumps and possibly context switching.
   */

  CURRENT_REGS = regs;

  /* Crash -- possibly showing diagnostic debug information. */

  _alert("Data abort. PC: %08x DFAR: %08x DFSR: %08x\n",
        regs[REG_PC], dfar, dfsr);
  PANIC();
  return regs; /* To keep the compiler happy */
}
