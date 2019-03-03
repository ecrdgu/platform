
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sched.h>
#include <debug.h>
#include <ecr/arch.h>
#include <ecr/sched.h>

#include "sched/sched.h"
#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_release_pending
 *
 * Description:
 *   Release and ready-to-run tasks that have
 *   collected in the pending task list.  This can call a
 *   context switch if a new task is placed at the head of
 *   the ready to run list.
 *
 ****************************************************************************/

void up_release_pending(void)
{
  struct tcb_s *rtcb = this_task();

  sinfo("From TCB=%p\n", rtcb);

  /* Merge the g_pendingtasks list into the ready-to-run task list */

  /* sched_lock(); */
  if (sched_mergepending())
    {
      /* The currently active task has changed!  We will need to
       * switch contexts.
       */

      /* Update scheduler parameters */

      sched_suspend_scheduler(rtcb);

      /* Are we operating in interrupt context? */

      if (CURRENT_REGS)
        {
          /* Yes, then we have to do things differently. Just copy the
           * CURRENT_REGS into the OLD rtcb.
           */

           up_savestate(rtcb->xcp.regs);

          /* Restore the exception context of the rtcb at the (new) head
           * of the ready-to-run task list.
           */

          rtcb = this_task();

          /* Update scheduler parameters */

          sched_resume_scheduler(rtcb);

          /* Then switch contexts */

          up_restorestate(rtcb->xcp.regs);
        }

      /* No, then we will need to perform the user context switch */

      else
        {
          struct tcb_s *nexttcb = this_task();

          /* Update scheduler parameters */

          sched_resume_scheduler(nexttcb);

          /* Switch context to the context of the task at the head of the
           * ready to run list.
           */

          up_switchcontext(rtcb->xcp.regs, nexttcb->xcp.regs);

          /* up_switchcontext forces a context switch to the task at the
           * head of the ready-to-run list.  It does not 'return' in the
           * normal sense.  When it does return, it is because the blocked
           * task is again ready to run and has execution priority.
           */
        }
    }
}
