
#ifndef __ARCH_ARM_SRC_STM32F0_STM32F0_START_H
#define __ARCH_ARM_SRC_STM32F0_STM32F0_START_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Public Data
 ************************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32f0_boardinitialize
 *
 * Description:
 *   All STM32 architectures must provide the following entry point.  This entry
 *   point is called early in the intitialization -- after all memory has been
 *   configured and mapped but before any devices have been initialized.
 *
 ************************************************************************************/

void stm32f0_boardinitialize(void);

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_STM32F0_STM32F0_START_H */
