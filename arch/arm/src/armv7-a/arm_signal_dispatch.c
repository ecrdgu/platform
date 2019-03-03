
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/arch.h>

#include "svcall.h"
#include "pgalloc.h"
#include "up_internal.h"

#if ((defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)) || \
      defined(CONFIG_BUILD_KERNEL)) && !defined(CONFIG_DISABLE_SIGNALS)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_signal_dispatch
 *
 * Description:
 *   In this kernel mode build, this function will be called to execute a
 *   a signal handler in user-space.  When the signal is delivered, a
 *   kernel-mode stub will first run to perform some housekeeping functions.
 *   This kernel-mode stub will then be called transfer control to the user
 *   mode signal handler by calling this function.
 *
 *   Normally the a user-mode signalling handling stub will also execute
 *   before the ultimate signal handler is called.  See
 *   arch/arm/src/armv[6\7]/up_signal_handler.  This function is the
 *   user-space, signal handler trampoline function.  It is called from
 *   up_signal_dispatch() in user-mode.
 *
 * Input Parameters:
 *   sighand - The address user-space signal handling function
 *   signo, info, and ucontext - Standard arguments to be passed to the
 *     signal handling function.
 *
 * Returned Value:
 *   None.  This function does not return in the normal sense.  It returns
 *   via an architecture specific system call made by up_signal_handler().
 *   However, this will look like a normal return by the caller of
 *   up_signal_dispatch.
 *
 ****************************************************************************/

void up_signal_dispatch(_sa_sigaction_t sighand, int signo,
                        FAR siginfo_t *info, FAR void *ucontext)
{
  /* We are signalling a user group, but does the signal handler lie in the
   * user address space?  Or the kernel address space?  The OS does
   * intercept some signals for its own purpose (such as the death-of-child
   * signal.
   */

  if (arm_uservaddr((uintptr_t)sighand))
    {
      /* Yes.. Let sys_call4() do all of the work to get us into user space */

      (void)sys_call4(SYS_signal_handler, (uintptr_t)sighand, (uintptr_t)signo,
                      (uintptr_t)info, (uintptr_t)ucontext);
    }
  else
    {
      /* No.. we are already in kernel mode so just call the handler */

      sighand(signo, info, ucontext);
    }
}

#endif /* (CONFIG_BUILD_PROTECTED || CONFIG_BUILD_KERNEL) && !CONFIG_DISABLE_SIGNALS */
