
#ifndef __ARCH_ARM_INCLUDE_TLS_H
#define __ARCH_ARM_INCLUDE_TLS_H 1

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <assert.h>
#include <ecr/arch.h>
#include <ecr/tls.h>

#ifdef CONFIG_TLS

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_getsp
 ****************************************************************************/

/* I don't know if the builtin to get SP is enabled */

static inline uint32_t up_getsp(void)
{
  uint32_t sp;
  __asm__
  (
    "\tmov %0, sp\n\t"
    : "=r"(sp)
  );

  return sp;
}

/****************************************************************************
 * Name: up_tls_info
 *
 * Description:
 *   Return the TLS information structure for the currently executing thread.
 *   When TLS is enabled, up_createstack() will align allocated stacks to
 *   the TLS_STACK_ALIGN value.  An instance of the following structure will
 *   be implicitly positioned at the "lower" end of the stack.  Assuming a
 *   "push down" stack, this is at the "far" end of the stack (and can be
 *   clobbered if the stack overflows).
 *
 *   If an MCU has a "push up" then that TLS structure will lie at the top
 *   of the stack and stack allocation and initialization logic must take
 *   care to preserve this structure content.
 *
 *   The stack memory is fully accessible to user mode threads.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   A pointer to TLS info structure at the beginning of the STACK memory
 *   allocation.  This is essentially an application of the TLS_INFO(sp)
 *   macro and has a platform dependency only in the manner in which the
 *   stack pointer (sp) is obtained and interpreted.
 *
 ****************************************************************************/

static inline FAR struct tls_info_s *up_tls_info(void)
{
  DEBUGASSERT(!up_interrupt_context());
  return TLS_INFO((uintptr_t)up_getsp());
}

#endif /* CONFIG_TLS */
#endif /* __ARCH_ARM_INCLUDE_TLS_H */
