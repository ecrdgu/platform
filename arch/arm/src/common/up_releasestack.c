
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sched.h>
#include <debug.h>

#include <ecr/arch.h>
#include <ecr/kmalloc.h>

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration */

#undef HAVE_KERNEL_HEAP
#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && \
     defined(CONFIG_MM_KERNEL_HEAP)
#  define HAVE_KERNEL_HEAP 1
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_release_stack
 *
 * Description:
 *   A task has been stopped. Free all stack related resources retained in
 *   the defunct TCB.
 *
 * Input Parameters:
 *   - dtcb:  The TCB containing information about the stack to be released
 *   - ttype:  The thread type.  This may be one of following (defined in
 *     include/ecr/sched.h):
 *
 *       TCB_FLAG_TTYPE_TASK     Normal user task
 *       TCB_FLAG_TTYPE_PTHREAD  User pthread
 *       TCB_FLAG_TTYPE_KERNEL   Kernel thread
 *
 *     This thread type is normally available in the flags field of the TCB,
 *     however, there are certain error recovery contexts where the TCB may
 *     not be fully initialized when up_release_stack is called.
 *
 *     If either CONFIG_BUILD_PROTECTED or CONFIG_BUILD_KERNEL are defined,
 *     then this thread type may affect how the stack is freed.  For example,
 *     kernel thread stacks may have been allocated from protected kernel
 *     memory.  Stacks for user tasks and threads must have come from memory
 *     that is accessible to user code.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void up_release_stack(FAR struct tcb_s *dtcb, uint8_t ttype)
{
  /* Is there a stack allocated? */

  if (dtcb->stack_alloc_ptr)
    {
#ifdef HAVE_KERNEL_HEAP
      /* Use the kernel allocator if this is a kernel thread */

      if (ttype == TCB_FLAG_TTYPE_KERNEL)
        {
          sched_kfree(dtcb->stack_alloc_ptr);
        }
      else
#endif
        {
          /* Use the user-space allocator if this is a task or pthread */

          sched_ufree(dtcb->stack_alloc_ptr);
        }

      /* Mark the stack freed */

      dtcb->stack_alloc_ptr = NULL;
    }

  /* The size of the allocated stack is now zero */

  dtcb->adj_stack_size = 0;
}
