
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <assert.h>
#include <debug.h>

#include <arch/irq.h>

#include "up_arch.h"
#include "nvic.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#undef DEBUG_MEMFAULTS         /* Define to debug memory management faults */

#ifdef DEBUG_MEMFAULTS
# define mferr(format, ...)  _alert(format, ##__VA_ARGS__)
# define mfinfo(format, ...) _alert(format, ##__VA_ARGS__)
#else
# define mferr(x...)
# define mfinfo(x...)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_memfault
 *
 * Description:
 *   This is Memory Management Fault exception handler.  Normally we get here
 *   when the Cortex M3 MPU is enabled and an MPU fault is detected.  However,
 *   I understand that there are other error conditions that can also generate
 *   memory management faults.
 *
 ****************************************************************************/

int up_memfault(int irq, FAR void *context, FAR void *arg)
{
  /* Dump some memory management fault info */

  (void)up_irq_save();
  _alert("PANIC!!! Memory Management Fault:\n");
  mfinfo("  IRQ: %d context: %p\n", irq, regs);
  _alert("  CFAULTS: %08x MMFAR: %08x\n",
        getreg32(NVIC_CFAULTS), getreg32(NVIC_MEMMANAGE_ADDR));
  mfinfo("  BASEPRI: %08x PRIMASK: %08x IPSR: %08x CONTROL: %08x\n",
         getbasepri(), getprimask(), getipsr(), getcontrol());
  mfinfo("  R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
         regs[REG_R0],  regs[REG_R1],  regs[REG_R2],  regs[REG_R3],
         regs[REG_R4],  regs[REG_R5],  regs[REG_R6],  regs[REG_R7]);
  mfinfo("  R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
         regs[REG_R8],  regs[REG_R9],  regs[REG_R10], regs[REG_R11],
         regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);

#ifdef CONFIG_ARMV7M_USEBASEPRI
#  ifdef REG_EXC_RETURN
  mfinfo("  xPSR: %08x BASEPRI: %08x EXC_RETURN: %08x (saved)\n",
         CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_BASEPRI],
         CURRENT_REGS[REG_EXC_RETURN]);
#  else
  mfinfo("  xPSR: %08x BASEPRI: %08x (saved)\n",
         CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_BASEPRI]);
#  endif
#else
#  ifdef REG_EXC_RETURN
  mfinfo("  xPSR: %08x PRIMASK: %08x EXC_RETURN: %08x (saved)\n",
         CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_PRIMASK],
         CURRENT_REGS[REG_EXC_RETURN]);
#  else
  mfinfo("  xPSR: %08x PRIMASK: %08x (saved)\n",
         CURRENT_REGS[REG_XPSR],  CURRENT_REGS[REG_PRIMASK]);
#  endif
#endif

  PANIC();
  return OK; /* Won't get here */
}
