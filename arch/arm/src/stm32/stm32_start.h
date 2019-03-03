
#ifndef __ARCH_ARM_SRC_STM32_STM32_START_H
#define __ARCH_ARM_SRC_STM32_STM32_START_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_boardinitialize
 *
 * Description:
 *   All STM32 architectures must provide the following entry point.  This entry
 *   point is called early in the initialization -- after clocking and memory have
 *   been configured but before caches have been enabled and before any devices have
 *   been initialized.
 *
 ************************************************************************************/

void stm32_boardinitialize(void);

#endif /* __ARCH_ARM_SRC_STM32_STM32_START_H */
