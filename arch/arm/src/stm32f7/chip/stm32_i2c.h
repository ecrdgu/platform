
#ifndef __ARCH_ARM_SRC_STM32F7_CHIP_STM32_I2C_H
#define __ARCH_ARM_SRC_STM32F7_CHIP_STM32_I2C_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

#if defined(CONFIG_STM32F7_STM32F72XX) || defined(CONFIG_STM32F7_STM32F73XX) || \
    defined(CONFIG_STM32F7_STM32F74XX) || defined(CONFIG_STM32F7_STM32F75XX) || \
    defined(CONFIG_STM32F7_STM32F76XX) || defined(CONFIG_STM32F7_STM32F77XX)
#  include "chip/stm32f74xx77xx_i2c.h"
#else
#  error "Unsupported STM32 F7 sub family"
#endif

#endif /* __ARCH_ARM_SRC_STM32F7_CHIP_STM32_I2C_H */
