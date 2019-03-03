
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_DBGMCU_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_DBGMCU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"

#if defined(CONFIG_STM32L4_STM32L4X3)
#  include "chip/stm32l4x3xx_dbgmcu.h"
#elif defined(CONFIG_STM32L4_STM32L4X5)
#  include "chip/stm32l4x5xx_dbgmcu.h"
#elif defined(CONFIG_STM32L4_STM32L4X6)
#  include "chip/stm32l4x6xx_dbgmcu.h"
#else
#  error "Unsupported STM32L4 chip"
#endif

#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_DBGMCU_H */
