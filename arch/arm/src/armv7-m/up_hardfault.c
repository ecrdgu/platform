
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

#include <ecr/userspace.h>
#include <arch/irq.h>

#include "up_arch.h"
#include "nvic.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If CONFIG_ARMV7M_USEBASEPRI=n, then debug output from this file may
 * interfere with context switching!
 */

#ifdef CONFIG_DEBUG_HARDFAULT
# define hfalert(format, ...)  _alert(format, ##__VA_ARGS__)
#else
# define hfalert(x...)
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
#if defined(CONFIG_DEBUG_HARDFAULT) || !defined(CONFIG_ARMV7M_USEBASEPRI)
  uint32_t *regs = (uint32_t *)context;
#endif

  /* Get the value of the program counter where the fault occurred */

#ifndef CONFIG_ARMV7M_USEBASEPRI
  uint16_t *pc = (uint16_t *)regs[REG_PC] - 1;

  /* Check if the pc lies in known FLASH memory.
   * REVISIT:  What if the PC lies in "unknown" external memory?  Best
   * use the BASEPRI register if you have external memory.
   */

#ifdef CONFIG_BUILD_PROTECTED
  /* In the kernel build, SVCalls are expected in either the base, kernel
   * FLASH region or in the user FLASH region.
   */

  if (((uintptr_t)pc >= (uintptr_t)_START_TEXT &&
       (uintptr_t)pc <  (uintptr_t)_END_TEXT) ||
      ((uintptr_t)pc >= (uintptr_t)USERSPACE->us_textstart &&
       (uintptr_t)pc <  (uintptr_t)USERSPACE->us_textend))
#else
  /* SVCalls are expected only from the base, kernel FLASH region */

  if ((uintptr_t)pc >= (uintptr_t)_START_TEXT &&
      (uintptr_t)pc <  (uintptr_t)_END_TEXT)
#endif
    {
      /* Fetch the instruction that caused the Hard fault */

      uint16_t insn = *pc;
      hfalert("  PC: %p INSN: %04x\n", pc, insn);

      /* If this was the instruction 'svc 0', then forward processing
       * to the SVCall handler
       */

      if (insn == INSN_SVC0)
        {
          hfalert("Forward SVCall\n");
          return up_svcall(irq, context, arg);
        }
    }
#endif

  /* Dump some hard fault info */

  hfalert("Hard Fault:\n");
  hfalert("  IRQ: %d regs: %p\n", irq, regs);
  hfalert("  BASEPRI: %08x PRIMASK: %08x IPSR: %08x CONTROL: %08x\n",
          getbasepri(), getprimask(), getipsr(), getcontrol());
  hfalert("  CFAULTS: %08x HFAULTS: %08x DFAULTS: %08x BFAULTADDR: %08x AFAULTS: %08x\n",
          getreg32(NVIC_CFAULTS), getreg32(NVIC_HFAULTS),
          getreg32(NVIC_DFAULTS), getreg32(NVIC_BFAULT_ADDR),
          getreg32(NVIC_AFAULTS));
  hfalert("  R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
          regs[REG_R0],  regs[REG_R1],  regs[REG_R2],  regs[REG_R3],
          regs[REG_R4],  regs[REG_R5],  regs[REG_R6],  regs[REG_R7]);
  hfalert("  R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
          regs[REG_R8],  regs[REG_R9],  regs[REG_R10], regs[REG_R11],
          regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);

#ifdef CONFIG_ARMV7M_USEBASEPRI
#  ifdef REG_EXC_RETURN
  hfalert("  xPSR: %08x BASEPRI: %08x EXC_RETURN: %08x (saved)\n",
          CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_BASEPRI],
          CURRENT_REGS[REG_EXC_RETURN]);
#  else
  hfalert("  xPSR: %08x BASEPRI: %08x (saved)\n",
          CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_BASEPRI]);
#  endif
#else
#  ifdef REG_EXC_RETURN
  hfalert("  xPSR: %08x PRIMASK: %08x EXC_RETURN: %08x (saved)\n",
          CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_PRIMASK],
          CURRENT_REGS[REG_EXC_RETURN]);
#  else
  hfalert("  xPSR: %08x PRIMASK: %08x (saved)\n",
          CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_PRIMASK]);
#  endif
#endif

  (void)up_irq_save();
  _alert("PANIC!!! Hard fault: %08x\n", getreg32(NVIC_HFAULTS));
  PANIC();
  return OK;
}
