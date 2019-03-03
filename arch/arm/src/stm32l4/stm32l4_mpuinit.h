
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_MPUINIT_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_MPUINIT_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/****************************************************************************
 * Name: stm32l4_mpuinitialize
 *
 * Description:
 *   Configure the MPU to permit user-space access to only unrestricted MCU
 *   resources.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_PROTECTED
void stm32l4_mpuinitialize(void);
#else
#  define stm32l4_mpuinitialize()
#endif

/****************************************************************************
 * Name: stm32l4_mpu_uheap
 *
 * Description:
 *  Map the user heap region.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_PROTECTED
void stm32l4_mpu_uheap(uintptr_t start, size_t size);
#else
#  define stm32l4_mpu_uheap(start,size)
#endif

#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_MPUINIT_H */
