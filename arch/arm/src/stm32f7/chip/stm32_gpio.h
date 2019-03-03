
#ifndef __ARCH_ARM_SRC_STM32F7_CHIP_STM32_GPIO_H
#define __ARCH_ARM_SRC_STM32F7_CHIP_STM32_GPIO_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

#if defined(CONFIG_STM32F7_STM32F72XX) || defined(CONFIG_STM32F7_STM32F73XX)
#  include "chip/stm32f72xx73xx_gpio.h"
#elif defined(CONFIG_STM32F7_STM32F74XX) || defined(CONFIG_STM32F7_STM32F75XX)
#  include "chip/stm32f74xx75xx_gpio.h"
#elif defined(CONFIG_STM32F7_STM32F76XX) || defined(CONFIG_STM32F7_STM32F77XX)
#  include "chip/stm32f76xx77xx_gpio.h"
#else
#  error "Unsupported STM32 F7 part"
#endif

#endif /* __ARCH_ARM_SRC_STM32F7_CHIP_STM32_GPIO_H */
