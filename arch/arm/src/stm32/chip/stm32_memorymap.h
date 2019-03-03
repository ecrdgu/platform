
#ifndef __ARCH_ARM_SRC_STM32_CHIP_STM32_MEMORYMAP_H
#define __ARCH_ARM_SRC_STM32_CHIP_STM32_MEMORYMAP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

#if defined(CONFIG_STM32_STM32L15XX)
#  include "chip/stm32l15xxx_memorymap.h"
#elif defined(CONFIG_STM32_STM32F10XX)
#  include "chip/stm32f10xxx_memorymap.h"
#elif defined(CONFIG_STM32_STM32F20XX)
#  include "chip/stm32f20xxx_memorymap.h"
#elif defined(CONFIG_STM32_STM32F30XX)
#  include "chip/stm32f30xxx_memorymap.h"
#elif defined(CONFIG_STM32_STM32F33XX)
#  include "chip/stm32f33xxx_memorymap.h"
#elif defined(CONFIG_STM32_STM32F37XX)
#  include "chip/stm32f37xxx_memorymap.h"
#elif defined(CONFIG_STM32_STM32F4XXX)
#  include "chip/stm32f40xxx_memorymap.h"
#else
#  error "Unsupported STM32 memory map"
#endif

#endif /* __ARCH_ARM_SRC_STM32_CHIP_STM32_MEMORYMAP_H */

