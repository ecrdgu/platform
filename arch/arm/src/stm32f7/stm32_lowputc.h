
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_LOWPUTC_H
#define __ARCH_ARM_SRC_STM32F7_STM32_LOWPUTC_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Name: stm32_lowsetup
 *
 * Description:
 *   Called at the very beginning of _start.  Performs low level initialization
 *   of serial console.
 *
 ************************************************************************************/

void stm32_lowsetup(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32F7_STM32_LOWPUTC_H */

