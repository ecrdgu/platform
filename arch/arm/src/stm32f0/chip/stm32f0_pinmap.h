
#ifndef __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_PINMAP_H
#define __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_PINMAP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

#if defined(CONFIG_STM32F0_STM32F05X)
#  include "chip/stm32f05x_pinmap.h"
#elif defined(CONFIG_STM32F0_STM32F07X)
#  include "chip/stm32f07x_pinmap.h"
#elif defined(CONFIG_STM32F0_STM32F09X)
#  include "chip/stm32f09x_pinmap.h"
#else
#  error "Unsupported STM32F0 pin map"
#endif

#endif /* __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_PINMAP_H */

