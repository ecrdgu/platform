
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_USERSPACE_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_USERSPACE_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/****************************************************************************
 * Name: stm32l4_userspace
 *
 * Description:
 *   For the case of the separate user-/kernel-space build, perform whatever
 *   platform specific initialization of the user memory is required.
 *   Normally this just means initializing the user space .data and .bss
 *   segments.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_PROTECTED
void stm32l4_userspace(void);
#endif

#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_USERSPACE_H */
