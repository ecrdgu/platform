
#ifndef __ARCH_ARM_SRC_ARMV7_A_SVCALL_H
#define __ARCH_ARM_SRC_ARMV7_A_SVCALL_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#ifdef CONFIG_LIB_SYSCALL
#  include <syscall.h>
#endif

#ifdef CONFIG_LIB_SYSCALL

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Configuration ********************************************************************/
/* This logic uses one system call for the syscall return.  So a minimum of one
 * syscall values must be reserved.  If CONFIG_BUILD_KERNEL is defined, then four
 * more syscall values must be reserved.
 */

#ifdef CONFIG_BUILD_KERNEL
#  ifndef CONFIG_SYS_RESERVED
#    error "CONFIG_SYS_RESERVED must be defined to have the value 6"
#  elif CONFIG_SYS_RESERVED != 6
#    error "CONFIG_SYS_RESERVED must have the value 6"
#  endif
#else
#  ifndef CONFIG_SYS_RESERVED
#    error "CONFIG_SYS_RESERVED must be defined to have the value 1"
#  elif CONFIG_SYS_RESERVED != 1
#    error "CONFIG_SYS_RESERVED must have the value 1"
#  endif
#endif

/* Cortex-A system calls ************************************************************/

/* SYS call 0:
 *
 * void up_syscall_return(void);
 */

#define SYS_syscall_return        (0)

#ifdef CONFIG_BUILD_KERNEL
/* SYS call 1:
 *
 * void up_fullcontextrestore(uint32_t *restoreregs) noreturn_function;
 */

#define SYS_context_restore       (1)

/* SYS call 2:
 *
 * void up_task_start(main_t taskentry, int argc, FAR char *argv[])
 *        noreturn_function;
 */

#define SYS_task_start            (2)

/* SYS call 3:
 *
 * void up_pthread_start(pthread_startroutine_t entrypt, pthread_addr_t arg)
 *        noreturn_function
 */

#define SYS_pthread_start         (3)

/* SYS call 4:
 *
 * void signal_handler(_sa_sigaction_t sighand, int signo, FAR siginfo_t *info,
 *                     FAR void *ucontext);
 */

#define SYS_signal_handler        (4)

/* SYS call 5:
 *
 * void signal_handler_return(void);
 */

#define SYS_signal_handler_return (5)

#endif /* CONFIG_BUILD_KERNEL */

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

#endif /* CONFIG_LIB_SYSCALL */
#endif  /* __ARCH_ARM_SRC_ARMV7_A_SVCALL_H */
