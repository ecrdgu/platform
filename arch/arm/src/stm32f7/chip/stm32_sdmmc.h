
#ifndef __ARCH_ARM_SRC_STM32F7_CHIP_STM32_SDMMC_H
#define __ARCH_ARM_SRC_STM32F7_CHIP_STM32_SDMMC_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

#if defined(CONFIG_STM32F7_STM32F72XX) || defined(CONFIG_STM32F7_STM32F73XX)
#  include "stm32f72xx73xx_sdmmc.h"
#elif defined(CONFIG_STM32F7_STM32F74XX) || defined(CONFIG_STM32F7_STM32F75XX) || \
    defined(CONFIG_STM32F7_STM32F76XX) || defined(CONFIG_STM32F7_STM32F77XX)
#  include "stm32f74xx77xx_sdmmc.h"
#else
#  error "Unsupported STM32 F7 part"
#endif

#endif /* __ARCH_ARM_SRC_STM32F7_CHIP_STM32_SDMMC_H */
