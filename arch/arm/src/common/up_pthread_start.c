
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <pthread.h>
#include <ecr/arch.h>

#include "svcall.h"
#include "up_internal.h"

#if ((defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)) || \
      defined(CONFIG_BUILD_KERNEL)) && !defined(CONFIG_DISABLE_PTHREAD)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_pthread_start
 *
 * Description:
 *   In this kernel mode build, this function will be called to execute a
 *   pthread in user-space.  When the pthread is first started, a kernel-mode
 *   stub will first run to perform some housekeeping functions.  This
 *   kernel-mode stub will then be called transfer control to the user-mode
 *   pthread.
 *
 *   Normally the a user-mode start-up stub will also execute before the
 *   pthread actually starts.  See libc/pthread/pthread_startup.c
 *
 * Input Parameters:
 *   entrypt - The user-space address of the pthread entry point
 *   arg     - Standard argument for the pthread entry point
 *
 * Returned Value:
 *   This function should not return.  It should call the user-mode start-up
 *   stub and that stub should call pthread_exit if/when the user pthread
 *   terminates.
 *
 ****************************************************************************/

void up_pthread_start(pthread_startroutine_t entrypt, pthread_addr_t arg)
{
  /* Let sys_call2() do all of the work */

  sys_call2(SYS_pthread_start, (uintptr_t)entrypt, (uintptr_t)arg);
}

#endif /* (CONFIG_BUILD_PROTECTED || CONFIG_BUILD_KERNEL) && !CONFIG_DISABLE_PTHREAD */
