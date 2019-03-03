
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <arch/irq.h>

#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_undefinedinsn
 ****************************************************************************/

uint32_t *arm_undefinedinsn(uint32_t *regs)
{
  _alert("Undefined instruction at 0x%x\n", regs[REG_PC]);
  CURRENT_REGS = regs;
  PANIC();
  return regs; /* To keep the compiler happy */
}
