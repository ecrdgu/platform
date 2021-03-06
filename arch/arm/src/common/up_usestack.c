
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <sched.h>
#include <assert.h>
#include <debug.h>

#include <ecr/kmalloc.h>
#include <ecr/arch.h>
#include <ecr/tls.h>

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
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_use_stack
 *
 * Description:
 *   Setup up stack-related information in the TCB using pre-allocated stack
 *   memory.  This function is called only from task_init() when a task or
 *   kernel thread is started (never for pthreads).
 *
 *   The following TCB fields must be initialized:
 *
 *   - adj_stack_size: Stack size after adjustment for hardware,
 *     processor, etc.  This value is retained only for debug
 *     purposes.
 *   - stack_alloc_ptr: Pointer to allocated stack
 *   - adj_stack_ptr: Adjusted stack_alloc_ptr for HW.  The
 *     initial value of the stack pointer.
 *
 * Input Parameters:
 *   - tcb: The TCB of new task
 *   - stack_size:  The allocated stack size.
 *
 *   NOTE:  Unlike up_stack_create() and up_stack_release, this function
 *   does not require the task type (ttype) parameter.  The TCB flags will
 *   always be set to provide the task type to up_use_stack() if it needs
 *   that information.
 *
 ****************************************************************************/

int up_use_stack(struct tcb_s *tcb, void *stack, size_t stack_size)
{
  size_t top_of_stack;
  size_t size_of_stack;

#ifdef CONFIG_TLS
  /* Make certain that the user provided stack is properly aligned */

  DEBUGASSERT(((uintptr_t)stack & TLS_STACK_MASK) == 0);
#endif

  /* Is there already a stack allocated? */

  if (tcb->stack_alloc_ptr)
    {
      /* Yes... Release the old stack allocation */

      up_release_stack(tcb, tcb->flags & TCB_FLAG_TTYPE_MASK);
    }

  /* Save the new stack allocation */

  tcb->stack_alloc_ptr = stack;

  /* The ARM uses a push-down stack:  the stack grows toward lower addresses
   * in memory.  The stack pointer register, points to the lowest, valid
   * work address (the "top" of the stack).  Items on the stack are
   * referenced as positive word offsets from sp.
   */

  top_of_stack = (uint32_t)tcb->stack_alloc_ptr + stack_size - 4;

  /* The ARM stack must be aligned to 8-byte alignment for EABI.
   * If necessary top_of_stack must be rounded down to the next
   * boundary
   */

  top_of_stack = STACK_ALIGN_DOWN(top_of_stack);

  /* The size of the stack in bytes is then the difference between
   * the top and the bottom of the stack (+4 because if the top
   * is the same as the bottom, then the size is one 32-bit element).
   * The size need not be aligned.
   */

  size_of_stack = top_of_stack - (uint32_t)tcb->stack_alloc_ptr + 4;

  /* Save the adjusted stack values in the struct tcb_s */

  tcb->adj_stack_ptr  = (uint32_t *)top_of_stack;
  tcb->adj_stack_size = size_of_stack;

#ifdef CONFIG_TLS
  /* Initialize the TLS data structure */

  memset(tcb->stack_alloc_ptr, 0, sizeof(struct tls_info_s));
#endif

#ifdef CONFIG_STACK_COLORATION
  /* If stack debug is enabled, then fill the stack with a recognizable
   * value that we can use later to test for high water marks.
   */

#ifdef CONFIG_TLS
  up_stack_color(
      (FAR void *)((uintptr_t)tcb->stack_alloc_ptr + sizeof(struct tls_info_s)),
      tcb->adj_stack_size - sizeof(struct tls_info_s));
#else
  up_stack_color(tcb->stack_alloc_ptr, tcb->adj_stack_size);
#endif
#endif

  return OK;
}
