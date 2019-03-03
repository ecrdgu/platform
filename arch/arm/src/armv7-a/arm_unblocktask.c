
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
 *     ready to run task, executed.
 *
 ****************************************************************************/

void up_unblock_task(struct tcb_s *tcb)
{
  struct tcb_s *rtcb;
#ifdef CONFIG_SMP
  int cpu;

  /* Get the TCB of the currently executing task on this CPU (avoid using
   * this_task() because the TCBs may be in an inappropriate state right
   * now).
   */

  cpu  = this_cpu();
  rtcb = current_task(cpu);
#else
  rtcb = this_task();
#endif

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

#ifdef CONFIG_SMP
          rtcb = current_task(cpu);
#else
          rtcb = this_task();
#endif

          /* Update scheduler parameters */

          sched_resume_scheduler(rtcb);

          /* Then switch contexts.  Any necessary address environment
           * changes will be made when the interrupt returns.
           */

          up_restorestate(rtcb->xcp.regs);
        }

      /* We are not in an interrupt handler.  Copy the user C context
       * into the TCB of the task that was previously active.  if
       * up_saveusercontext returns a non-zero value, then this is really the
       * previously running task restarting!
       */

      else if (!up_saveusercontext(rtcb->xcp.regs))
        {
          /* Restore the exception context of the new task that is ready to
           * run (probably tcb).  This is the new rtcb at the head of the
           * ready-to-run task list.
           */

#ifdef CONFIG_SMP
          rtcb = current_task(cpu);
#else
          rtcb = this_task();
#endif

#ifdef CONFIG_ARCH_ADDRENV
          /* Make sure that the address environment for the previously
           * running task is closed down gracefully (data caches dump,
           * MMU flushed) and set up the address environment for the new
           * thread at the head of the ready-to-run list.
           */

          (void)group_addrenv(rtcb);
#endif
          /* Update scheduler parameters */

          sched_resume_scheduler(rtcb);

          /* Then switch contexts */

          up_fullcontextrestore(rtcb->xcp.regs);
        }
    }
}
