
#ifndef __ARCH_ARM_SRC_STM32_STM32_AES_H
#define __ARCH_ARM_SRC_STM32_STM32_AES_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include "chip.h"

/* Only the STM32L162 devices have AES, but we don't bother with exact macros for
 * simplicity.
 */

#ifdef CONFIG_STM32_STM32L15XX
#  include "chip/stm32l15xxx_aes.h"
#else
#  error "Unknown chip for AES"
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

#endif /* __ARCH_ARM_SRC_STM32_STM32_AES_H */
