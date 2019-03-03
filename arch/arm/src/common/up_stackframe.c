
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <sched.h>
#include <debug.h>

#include <ecr/arch.h>
#include <arch/irq.h>

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Macros
 ****************************************************************************/

/* For use with EABI and floating point, the stack must be aligned to 8-byte
 * addresses.
 */

#define CONFIG_STACK_ALIGNMENT 8

/* Stack alignment macros */

#define STACK_ALIGN_MASK    (CONFIG_STACK_ALIGNMENT-1)
#define STACK_ALIGN_DOWN(a) ((a) & ~STACK_ALIGN_MASK)
#define STACK_ALIGN_UP(a)   (((a) + STACK_ALIGN_MASK) & ~STACK_ALIGN_MASK)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_stack_frame
 *
 * Description:
 *   Allocate a stack frame in the TCB's stack to hold thread-specific data.
 *   This function may be called anytime after up_create_stack() or
 *   up_use_stack() have been called but before the task has been started.
 *
 *   Thread data may be kept in the stack (instead of in the TCB) if it is
 *   accessed by the user code directly.  This includes such things as
 *   argv[].  The stack memory is guaranteed to be in the same protection
 *   domain as the thread.
 *
 *   The following TCB fields will be re-initialized:
 *
 *   - adj_stack_size: Stack size after removal of the stack frame from
 *     the stack
 *   - adj_stack_ptr: Adjusted initial stack pointer after the frame has
 *     been removed from the stack.  This will still be the initial value
 *     of the stack pointer when the task is started.
 *
 * Input Parameters:
 *   - tcb:  The TCB of new task
 *   - frame_size:  The size of the stack frame to allocate.
 *
 *  Returned Value:
 *   - A pointer to bottom of the allocated stack frame.  NULL will be
 *     returned on any failures.  The alignment of the returned value is
 *     the same as the alignment of the stack itself.
 *
 ****************************************************************************/

FAR void *up_stack_frame(FAR struct tcb_s *tcb, size_t frame_size)
{
  uintptr_t topaddr;

  /* Align the frame_size */

  frame_size = STACK_ALIGN_UP(frame_size);

  /* Is there already a stack allocated? Is it big enough? */

  if (!tcb->stack_alloc_ptr || tcb->adj_stack_size <= frame_size)
    {
      return NULL;
    }

  /* Save the adjusted stack values in the struct tcb_s */

  topaddr               = (uintptr_t)tcb->adj_stack_ptr - frame_size;
  tcb->adj_stack_ptr    = (FAR void *)topaddr;
  tcb->adj_stack_size  -= frame_size;

  /* Reset the initial stack pointer */

  tcb->xcp.regs[REG_SP] = (uint32_t)tcb->adj_stack_ptr;

  /* And return the pointer to the allocated region */

  return (FAR void *)(topaddr + sizeof(uint32_t));
}
