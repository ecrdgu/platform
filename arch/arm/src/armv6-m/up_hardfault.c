
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

#include <arch/irq.h>

#include "up_arch.h"
#include "nvic.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_DEBUG_HARDFAULT
# define hfinfo(format, ...)  _alert(format, ##__VA_ARGS__)
#else
# define hfinfo(x...)
#endif

#define INSN_SVC0        0xdf00 /* insn: svc 0 */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_hardfault
 *
 * Description:
 *   This is Hard Fault exception handler.  It also catches SVC call
 *   exceptions that are performed in bad contexts.
 *
 ****************************************************************************/

int up_hardfault(int irq, FAR void *context, FAR void *arg)
{
  uint32_t *regs = (uint32_t *)context;

  /* Get the value of the program counter where the fault occurred */

  uint16_t *pc = (uint16_t *)regs[REG_PC] - 1;

  /* Check if the pc lies in known FLASH memory.
   * REVISIT:  What if the PC lies in "unknown" external memory?
   */

#ifdef CONFIG_BUILD_PROTECTED
  /* In the kernel build, SVCalls are expected in either the base, kernel
   * FLASH region or in the user FLASH region.
   */

  if (((uintptr_t)pc >= (uintptr_t)&_stext &&
       (uintptr_t)pc <  (uintptr_t)&_etext) ||
      ((uintptr_t)pc >= (uintptr_t)USERSPACE->us_textstart &&
       (uintptr_t)pc <  (uintptr_t)USERSPACE->us_textend))
#else
  /* SVCalls are expected only from the base, kernel FLASH region */

  if ((uintptr_t)pc >= (uintptr_t)&_stext &&
      (uintptr_t)pc <  (uintptr_t)&_etext)
#endif
    {
      /* Fetch the instruction that caused the Hard fault */

      uint16_t insn = *pc;
      hfinfo("  PC: %p INSN: %04x\n", pc, insn);

      /* If this was the instruction 'svc 0', then forward processing
       * to the SVCall handler
       */

      if (insn == INSN_SVC0)
        {
          hfinfo("Forward SVCall\n");
          return up_svcall(irq, context, NULL);
        }
    }

#if defined(CONFIG_DEBUG_HARDFAULT)
  /* Dump some hard fault info */

  _alert("\nHard Fault:\n");
  _alert("  IRQ: %d regs: %p\n", irq, regs);
  _alert("  PRIMASK: %08x IPSR: %08x\n",
         getprimask(), getipsr());
  _alert("  R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
         regs[REG_R0],  regs[REG_R1],  regs[REG_R2],  regs[REG_R3],
         regs[REG_R4],  regs[REG_R5],  regs[REG_R6],  regs[REG_R7]);
  _alert("  R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
         regs[REG_R8],  regs[REG_R9],  regs[REG_R10], regs[REG_R11],
         regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);
  _alert("  xPSR: %08x PRIMASK: %08x (saved)\n",
         CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_PRIMASK]);
#endif

  (void)up_irq_save();
  _alert("PANIC!!! Hard fault\n");
  PANIC();
  return OK; /* Won't get here */
}
