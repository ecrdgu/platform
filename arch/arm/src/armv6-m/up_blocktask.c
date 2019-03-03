
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>
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
 * Name: up_block_task
 *
 * Description:
 *   The currently executing task at the head of the ready to run list must
 *   be stopped.  Save its context and move it to the inactive list specified
 *   by task_state.
 *
 * Input Parameters:
 *   tcb: Refers to a task in the ready-to-run list (normally the task at
 *     the head of the list).  It most be stopped, its context saved and
 *     moved into one of the waiting task lists.  It it was the task at the
 *     head of the ready-to-run list, then a context to the new ready to run
 *     task must be performed.
 *   task_state: Specifies which waiting task list should be hold the
 *     blocked task TCB.
 *
 ****************************************************************************/

void up_block_task(struct tcb_s *tcb, tstate_t task_state)
{
  struct tcb_s *rtcb = this_task();
  bool switch_needed;

  /* Verify that the context switch can be performed */

  ASSERT((tcb->task_state >= FIRST_READY_TO_RUN_STATE) &&
         (tcb->task_state <= LAST_READY_TO_RUN_STATE));

  /* Remove the tcb task from the ready-to-run list.  If we
   * are blocking the task at the head of the task list (the
   * most likely case), then a context switch to the next
   * ready-to-run task is needed. In this case, it should
   * also be true that rtcb == tcb.
   */

  switch_needed = sched_removereadytorun(tcb);

  /* Add the task to the specified blocked task list */

  sched_addblocked(tcb, (tstate_t)task_state);

  /* If there are any pending tasks, then add them to the ready-to-run
   * task list now
   */

  if (g_pendingtasks.head)
    {
      switch_needed |= sched_mergepending();
    }

  /* Now, perform the context switch if one is needed */

  if (switch_needed)
    {
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

          /* Reset scheduler parameters */

          sched_resume_scheduler(rtcb);

          /* Then switch contexts */

          up_restorestate(rtcb->xcp.regs);
        }

      /* No, then we will need to perform the user context switch */

      else
        {
          struct tcb_s *nexttcb = this_task();

          /* Reset scheduler parameters */

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
