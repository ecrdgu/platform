
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <debug.h>

#include <arch/irq.h>

#include "up_arch.h"
#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_syscall
 *
 * Description:
 *   SWI interrupts will vector here with insn=the SWI instruction and
 *   xcp=the interrupt context
 *
 *   The handler may get the SWI number be de-referencing
 *   the return address saved in the xcp and decoding
 *   the SWI instruction
 *
 ****************************************************************************/

void up_syscall(uint32_t *regs)
{
  _alert("Syscall from 0x%x\n", regs[REG_PC]);
  CURRENT_REGS = regs;
  PANIC();
}
