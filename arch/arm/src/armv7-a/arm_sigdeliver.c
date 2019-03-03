
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
  struct tcb_s  *rtcb = this_task();
  uint32_t regs[XCPTCONTEXT_REGS];
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
  regs[REG_CPSR]       = rtcb->xcp.saved_cpsr;

  /* Get a local copy of the sigdeliver function pointer. we do this so that
   * we can nullify the sigdeliver function pointer in the TCB and accept
   * more signal deliveries while processing the current pending signals.
   */

  sigdeliver           = rtcb->xcp.sigdeliver;
  rtcb->xcp.sigdeliver = NULL;

  /* Then restore the task interrupt state */

  leave_critical_section(regs[REG_CPSR]);

  /* Deliver the signal */

  sigdeliver(rtcb);

  /* Output any debug messages BEFORE restoring errno (because they may
   * alter errno), then disable interrupts again and restore the original
   * errno that is needed by the user logic (it is probably EINTR).
   *
   * REVISIT: In SMP mode up_irq_save() probably only disables interrupts
   * on the local CPU.  We do not want to call enter_critical_section()
   * here, however, because we don't want this state to stick after the
   * call to up_fullcontextrestore().
   *
   * I would prefer that all interrupts are disabled when
   * up_fullcontextrestore() is called, but that may not be necessary.
   */

  sinfo("Resuming\n");

  (void)up_irq_save();
  rtcb->pterrno = saved_errno;

  /* Then restore the correct state for this thread of execution. */

  board_autoled_off(LED_SIGNAL);
  up_fullcontextrestore(regs);
}

#endif /* !CONFIG_DISABLE_SIGNALS */
