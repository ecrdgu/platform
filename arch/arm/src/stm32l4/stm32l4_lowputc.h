
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_LOWPUTC_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_LOWPUTC_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include "chip.h"

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
 * Name: stm32l4_lowsetup
 *
 * Description:
 *   Called at the very beginning of _start.  Performs low level initialization
 *   of serial console.
 *
 ************************************************************************************/

void stm32l4_lowsetup(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_LOWPUTC_H */

