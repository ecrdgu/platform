
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include <ecr/arch.h>
#include <ecr/sched.h>
#include <ecr/sched_note.h>

#include "up_internal.h"
#include "cp15_cacheops.h"
#include "gic.h"
#include "sched/sched.h"

#ifdef CONFIG_SMP

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_registerdump
 ****************************************************************************/

#if 0 /* Was useful in solving some startup problems */
static inline void arm_registerdump(FAR struct tcb_s *tcb)
{
  int regndx;

  _info("CPU%d:\n", up_cpu_index());

  /* Dump the startup registers */

  for (regndx = REG_R0; regndx <= REG_R15; regndx += 8)
    {
      uint32_t *ptr = (uint32_t *)&tcb->xcp.regs[regndx];
      _info("R%d: %08x %08x %08x %08x %08x %08x %08x %08x\n",
            regndx, ptr[0], ptr[1], ptr[2], ptr[3],
            ptr[4], ptr[5], ptr[6], ptr[7]);
    }

  _info("CPSR: %08x\n", tcb->xcp.regs[REG_CPSR]);
}
#else
# define arm_registerdump(tcb)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_start_handler
 *
 * Description:
 *   This is the handler for SGI1.  This handler simply returns from the
 *   interrupt, restoring the state of the new task at the head of the ready
 *   to run list.
 *
 * Input Parameters:
 *   Standard interrupt handling
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int arm_start_handler(int irq, FAR void *context, FAR void *arg)
{
  FAR struct tcb_s *tcb = this_task();

  sinfo("CPU%d Started\n", this_cpu());

#ifdef CONFIG_SCHED_INSTRUMENTATION
  /* Notify that this CPU has started */

  sched_note_cpu_started(tcb);
#endif

  /* Reset scheduler parameters */

  sched_resume_scheduler(tcb);

  /* Dump registers so that we can see what is going to happen on return */

  arm_registerdump(tcb);

  /* Then switch contexts. This instantiates the exception context of the
   * tcb at the head of the assigned task list.  In this case, this should
   * be the CPUs NULL task.
   */

  up_restorestate(tcb->xcp.regs);
  return OK;
}

/****************************************************************************
 * Name: up_cpu_start
 *
 * Description:
 *   In an SMP configution, only one CPU is initially active (CPU 0). System
 *   initialization occurs on that single thread. At the completion of the
 *   initialization of the OS, just before beginning normal multitasking,
 *   the additional CPUs would be started by calling this function.
 *
 *   Each CPU is provided the entry point to is IDLE task when started.  A
 *   TCB for each CPU's IDLE task has been initialized and placed in the
 *   CPU's g_assignedtasks[cpu] list.  Not stack has been alloced or
 *   initialized.
 *
 *   The OS initialization logic calls this function repeatedly until each
 *   CPU has been started, 1 through (CONFIG_SMP_NCPUS-1).
 *
 * Input Parameters:
 *   cpu - The index of the CPU being started.  This will be a numeric
 *         value in the range of from one to (CONFIG_SMP_NCPUS-1).  (CPU
 *         0 is already active)
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int up_cpu_start(int cpu)
{
  sinfo("Starting CPU%d\n", cpu);

  DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

#ifdef CONFIG_SCHED_INSTRUMENTATION
  /* Notify of the start event */

  sched_note_cpu_start(this_task(), cpu);
#endif

  /* Make the content of CPU0 L1 cache has been written to coherent L2 */

  cp15_clean_dcache(CONFIG_RAM_START, CONFIG_RAM_END - 1);

  /* Execute SGI1 */

  return arm_cpu_sgi(GIC_IRQ_SGI1, (1 << cpu));
}

#endif /* CONFIG_SMP */
