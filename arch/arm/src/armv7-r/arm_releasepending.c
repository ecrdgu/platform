
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sched.h>
#include <debug.h>
#include <ecr/arch.h>
#include <ecr/sched.h>

#include "sched/sched.h"
#include "group/group.h"
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
          /* Yes, then we have to do things differently.
           * Just copy the CURRENT_REGS into the OLD rtcb.
           */

           up_savestate(rtcb->xcp.regs);

          /* Restore the exception context of the rtcb at the (new) head
           * of the ready-to-run task list.
           */

          rtcb = this_task();

          /* Update scheduler parameters */

          sched_resume_scheduler(rtcb);

          /* Then switch contexts.  Any necessary address environment
           * changes will be made when the interrupt returns.
           */

          up_restorestate(rtcb->xcp.regs);
        }

      /* Copy the exception context into the TCB of the task that
       * was currently active. if up_saveusercontext returns a non-zero
       * value, then this is really the previously running task
       * restarting!
       */

      else if (!up_saveusercontext(rtcb->xcp.regs))
        {
          /* Restore the exception context of the rtcb at the (new) head
           * of the ready-to-run task list.
           */

          rtcb = this_task();

          /* Update scheduler parameters */

          sched_resume_scheduler(rtcb);

          /* Then switch contexts */

          up_fullcontextrestore(rtcb->xcp.regs);
        }
    }
}
