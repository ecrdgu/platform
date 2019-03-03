
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#include <ecr/arch.h>

#include "up_internal.h"
#include "up_arch.h"

#include "psr.h"
#include "exc_return.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_initial_state
 *
 * Description:
 *   A new thread is being started and a new TCB
 *   has been created. This function is called to initialize
 *   the processor specific portions of the new TCB.
 *
 *   This function must setup the intial architecture registers
 *   and/or  stack so that execution will begin at tcb->start
 *   on the next context switch.
 *
 ****************************************************************************/

void up_initial_state(struct tcb_s *tcb)
{
  struct xcptcontext *xcp = &tcb->xcp;

  /* Initialize the initial exception register context structure */

  memset(xcp, 0, sizeof(struct xcptcontext));

  /* Save the initial stack pointer */

  xcp->regs[REG_SP]      = (uint32_t)tcb->adj_stack_ptr;

#ifdef CONFIG_ARMV7M_STACKCHECK
  /* Set the stack limit value */

  xcp->regs[REG_R10]     = (uint32_t)tcb->stack_alloc_ptr + 64;
#endif

  /* Save the task entry point (stripping off the thumb bit) */

  xcp->regs[REG_PC]      = (uint32_t)tcb->start & ~1;

  /* Specify thumb mode */

  xcp->regs[REG_XPSR]    = ARMV7M_XPSR_T;

  /* If this task is running PIC, then set the PIC base register to the
   * address of the allocated D-Space region.
   */

#ifdef CONFIG_PIC
  if (tcb->dspace != NULL)
    {
      /* Set the PIC base register (probably R10) to the address of the
       * alloacated D-Space region.
       */

      xcp->regs[REG_PIC] = (uint32_t)tcb->dspace->region;
    }

#ifdef CONFIG_NXFLAT
  /* Make certain that bit 0 is set in the main entry address.  This
   * is only an issue when NXFLAT is enabled.  NXFLAT doesn't know
   * anything about thumb; the addresses that NXFLAT sets are based
   * on file header info and won't have bit 0 set.
   */

  tcb->entry.main = (main_t)((uint32_t)tcb->entry.main | 1);
#endif
#endif /* CONFIG_PIC */

#if (defined(CONFIG_ARMV7M_CMNVECTOR) && !defined(CONFIG_ARMV7M_LAZYFPU)) || \
    defined(CONFIG_BUILD_PROTECTED)
  /* All tasks start via a stub function in kernel space.  So all
   * tasks must start in privileged thread mode.  If CONFIG_BUILD_PROTECTED
   * is defined, then that stub function will switch to unprivileged
   * mode before transferring control to the user task.
   */

  xcp->regs[REG_EXC_RETURN] = EXC_RETURN_PRIVTHR;

#endif /* (CONFIG_ARMV7M_CMNVECTOR && !CONFIG_ARMV7M_LAZYFPU) || CONFIG_BUILD_PROTECTED */

#if defined(CONFIG_ARMV7M_CMNVECTOR) && !defined(CONFIG_ARMV7M_LAZYFPU) && \
    defined(CONFIG_ARCH_FPU)

  xcp->regs[REG_FPSCR] = 0;      /* REVISIT: Initial FPSCR should be configurable */
  xcp->regs[REG_FPReserved] = 0;

#endif /* CONFIG_ARMV7M_CMNVECTOR && !CONFIG_ARMV7M_LAZYFPU && CONFIG_ARCH_FPU */

  /* Enable or disable interrupts, based on user configuration */

#ifdef CONFIG_SUPPRESS_INTERRUPTS

#ifdef CONFIG_ARMV7M_USEBASEPRI
  xcp->regs[REG_BASEPRI] = NVIC_SYSH_DISABLE_PRIORITY;
#else
  xcp->regs[REG_PRIMASK] = 1;
#endif

#else /* CONFIG_SUPPRESS_INTERRUPTS */

#ifdef CONFIG_ARMV7M_USEBASEPRI
  xcp->regs[REG_BASEPRI] = NVIC_SYSH_PRIORITY_MIN;
#endif

#endif /* CONFIG_SUPPRESS_INTERRUPTS */
}
