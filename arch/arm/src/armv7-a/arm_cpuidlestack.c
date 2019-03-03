
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>

#include <ecr/arch.h>
#include <ecr/sched.h>

#include "smp.h"
#include "up_internal.h"

#ifdef CONFIG_SMP

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Stack alignment macros */

#define STACK_ISALIGNED(a) ((uintptr_t)(a) & ~SMP_STACK_MASK)

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if CONFIG_SMP_NCPUS > 1
static FAR const uint32_t *g_cpu_stackalloc[CONFIG_SMP_NCPUS] =
{
    0
  , g_cpu1_idlestack
#if CONFIG_SMP_NCPUS > 2
  , g_cpu2_idlestack
#if CONFIG_SMP_NCPUS > 3
  , g_cpu3_idlestack
#endif /* CONFIG_SMP_NCPUS > 3 */
#endif /* CONFIG_SMP_NCPUS > 2 */
};
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_cpu_idlestack
 *
 * Description:
 *   Allocate a stack for the CPU[n] IDLE task (n > 0) if appropriate and
 *   setup up stack-related information in the IDLE task's TCB.  This
 *   function is always called before up_cpu_start().  This function is
 *   only called for the CPU's initial IDLE task; up_create_task is used for
 *   all normal tasks, pthreads, and kernel threads for all CPUs.
 *
 *   The initial IDLE task is a special case because the CPUs can be started
 *   in different wans in different environments:
 *
 *   1. The CPU may already have been started and waiting in a low power
 *      state for up_cpu_start().  In this case, the IDLE thread's stack
 *      has already been allocated and is already in use.  Here
 *      up_cpu_idlestack() only has to provide information about the
 *      already allocated stack.
 *
 *   2. The CPU may be disabled but started when up_cpu_start() is called.
 *      In this case, a new stack will need to be created for the IDLE
 *      thread and this function is then equivalent to:
 *
 *      return up_create_stack(tcb, stack_size, TCB_FLAG_TTYPE_KERNEL);
 *
 *   The following TCB fields must be initialized by this function:
 *
 *   - adj_stack_size: Stack size after adjustment for hardware, processor,
 *     etc.  This value is retained only for debug purposes.
 *   - stack_alloc_ptr: Pointer to allocated stack
 *   - adj_stack_ptr: Adjusted stack_alloc_ptr for HW.  The initial value of
 *     the stack pointer.
 *
 * Input Parameters:
 *   - cpu:         CPU index that indicates which CPU the IDLE task is
 *                  being created for.
 *   - tcb:         The TCB of new CPU IDLE task
 *   - stack_size:  The requested stack size for the IDLE task.  At least
 *                  this much must be allocated.  This should be
 *                  CONFIG_SMP_STACK_SIZE.
 *
 ****************************************************************************/

int up_cpu_idlestack(int cpu, FAR struct tcb_s *tcb, size_t stack_size)
{
#if CONFIG_SMP_NCPUS > 1
  uintptr_t stack_alloc;
  uintptr_t top_of_stack;

  DEBUGASSERT(cpu > 0 && cpu < CONFIG_SMP_NCPUS && tcb != NULL &&
              stack_size <= SMP_STACK_SIZE);

  /* Get the top of the stack */


  stack_alloc          = (uintptr_t)g_cpu_stackalloc[cpu];
  DEBUGASSERT(stack_alloc != 0 && STACK_ISALIGNED(stack_alloc));
  top_of_stack         = stack_alloc + SMP_STACK_TOP;

  tcb->adj_stack_size  = SMP_STACK_SIZE;
  tcb->stack_alloc_ptr = (FAR uint32_t *)stack_alloc;
  tcb->adj_stack_ptr   = (FAR uint32_t *)top_of_stack;
#endif

  return OK;
}

#endif /* CONFIG_SMP */