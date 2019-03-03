
#ifndef __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_MEMORYMAP_H
#define __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_MEMORYMAP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

#if defined(CONFIG_STM32F0_STM32F05X) || defined(CONFIG_STM32F0_STM32F07X) \
 || defined(CONFIG_STM32F0_STM32F09X)
#  include "chip/stm32f05xf07xf09x_memorymap.h"
#else
#  error "Unsupported STM32 memory map"
#endif

#endif /* __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_MEMORYMAP_H */
