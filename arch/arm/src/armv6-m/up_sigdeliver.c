
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <sched.h>
#include <debug.h>

#include <ecr/irq.h>
#include <ecr/arch.h>
#include <ecr/board.h>
#include <arch/board/board.h>

#include "sched/sched.h"
#include "up_internal.h"
#include "up_arch.h"

#ifndef CONFIG_DISABLE_SIGNALS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_sigdeliver
 *
 * Description:
 *   This is the a signal handling trampoline.  When a signal action was
 *   posted.  The task context was mucked with and forced to branch to this
 *   location with interrupts disabled.
 *
 ****************************************************************************/

void up_sigdeliver(void)
{
  /* NOTE the "magic" guard space added to regs.  This is a little kludge
   * because up_fullcontextrestore (called below) will do a stack-to-stack
   * copy an may overwrite the regs[] array contents.  Sorry.
   */

  struct tcb_s  *rtcb = this_task();
  uint32_t regs[XCPTCONTEXT_REGS + 4];
  sig_deliver_t sigdeliver;

  /* Save the errno.  This must be preserved throughout the signal handling
   * so that the user code final gets the correct errno value (probably
   * EINTR).
   */

  int saved_errno = rtcb->pterrno;

  board_autoled_on(LED_SIGNAL);

  sinfo("rtcb=%p sigdeliver=%p sigpendactionq.head=%p\n",
        rtcb, rtcb->xcp.sigdeliver, rtcb->sigpendactionq.head);
  ASSERT(rtcb->xcp.sigdeliver != NULL);

  /* Save the real return state on the stack. */

  up_copyfullstate(regs, rtcb->xcp.regs);
  regs[REG_PC]         = rtcb->xcp.saved_pc;
  regs[REG_PRIMASK]    = rtcb->xcp.saved_primask;
  regs[REG_XPSR]       = rtcb->xcp.saved_xpsr;
#ifdef CONFIG_BUILD_PROTECTED
  regs[REG_LR]         = rtcb->xcp.saved_lr;
#endif

  /* Get a local copy of the sigdeliver function pointer. We do this so that
   * we can nullify the sigdeliver function pointer in the TCB and accept
   * more signal deliveries while processing the current pending signals.
   */

  sigdeliver           = rtcb->xcp.sigdeliver;
  rtcb->xcp.sigdeliver = NULL;

  /* Then restore the task interrupt state */

  up_irq_restore((uint8_t)regs[REG_PRIMASK]);

  /* Deliver the signal */

  sigdeliver(rtcb);

  /* Output any debug messages BEFORE restoring errno (because they may
   * alter errno), then disable interrupts again and restore the original
   * errno that is needed by the user logic (it is probably EINTR).
   */

  sinfo("Resuming\n");
  (void)up_irq_save();
  rtcb->pterrno = saved_errno;

  /* Then restore the correct state for this thread of
   * execution.
   */

  board_autoled_off(LED_SIGNAL);
  up_fullcontextrestore(regs);
}

#endif /* !CONFIG_DISABLE_SIGNALS */
