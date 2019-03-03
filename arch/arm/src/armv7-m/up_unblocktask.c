
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sched.h>
#include <debug.h>
#include <ecr/arch.h>
#include <ecr/sched.h>

#include "sched/sched.h"
#include "clock/clock.h"
#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_unblock_task
 *
 * Description:
 *   A task is currently in an inactive task list
 *   but has been prepped to execute.  Move the TCB to the
 *   ready-to-run list, restore its context, and start execution.
 *
 * Input Parameters:
 *   tcb: Refers to the tcb to be unblocked.  This tcb is
 *     in one of the waiting tasks lists.  It must be moved to
 *     the ready-to-run list and, if it is the highest priority
 *     ready to run taks, executed.
 *
 ****************************************************************************/

void up_unblock_task(struct tcb_s *tcb)
{
  struct tcb_s *rtcb = this_task();

  /* Verify that the context switch can be performed */

  ASSERT((tcb->task_state >= FIRST_BLOCKED_STATE) &&
         (tcb->task_state <= LAST_BLOCKED_STATE));

  /* Remove the task from the blocked task list */

  sched_removeblocked(tcb);

  /* Add the task in the correct location in the prioritized
   * ready-to-run task list
   */

  if (sched_addreadytorun(tcb))
    {
      /* The currently active task has changed! We need to do
       * a context switch to the new task.
       */

      /* Update scheduler parameters */

      sched_suspend_scheduler(rtcb);

      /* Are we in an interrupt handler? */

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
