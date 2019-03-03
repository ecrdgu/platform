
/* This file should never be included directed but, rather, only indirectly
 * through include/syscall.h or include/sys/sycall.h
 */

#ifndef __ARCH_ARM_INCLUDE_SYSCALL_H
#define __ARCH_ARM_INCLUDE_SYSCALL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/* Include ARM architecture-specific syscall macros */

#if defined(CONFIG_ARCH_CORTEXA5) || defined(CONFIG_ARCH_CORTEXA8)
#  include <arch/armv7-a/syscall.h>
#elif defined(CONFIG_ARCH_CORTEXM3) || defined(CONFIG_ARCH_CORTEXM4) || \
      defined(CONFIG_ARCH_CORTEXM7)
#  include <arch/armv7-m/syscall.h>
#elif defined(CONFIG_ARCH_CORTEXM0)
#  include <arch/armv6-m/syscall.h>
#else
#  include <arch/arm/syscall.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline functions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_SYSCALL_H */

