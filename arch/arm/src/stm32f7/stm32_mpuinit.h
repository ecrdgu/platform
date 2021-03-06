
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_MPUINIT_H
#define __ARCH_ARM_SRC_STM32F7_STM32_MPUINIT_H

/****************************************************************************
 * Name: stm32_mpuinitialize
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>

/****************************************************************************
 * Name: stm32_mpuinitialize
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_mpuinitialize
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_mpuinitialize
 * Inline Functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Name: stm32_mpuinitialize
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_mpuinitialize
 *
 * Description:
 *   Configure the MPU to permit user-space access to only unrestricted
 *   STM32F7 resources.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_PROTECTED
void stm32_mpuinitialize(void);
#else
#  define stm32_mpuinitialize()
#endif

/****************************************************************************
 * Name: stm32_mpu_uheap
 *
 * Description:
 *  Map the user heap region.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_PROTECTED
void stm32_mpu_uheap(uintptr_t start, size_t size);
#else
#  define stm32_mpu_uheap(start,size)
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32F7_STM32_MPUINIT_H */
