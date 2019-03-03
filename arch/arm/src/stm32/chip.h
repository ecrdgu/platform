
#ifndef __ARCH_ARM_SRC_STM32_CHIP_H
#define __ARCH_ARM_SRC_STM32_CHIP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/* Include the chip capabilities file */

#include <arch/stm32/chip.h>

/* Include the chip pin configuration file */

/* STM32L EnergyLite Line ***********************************************************/

#if defined(CONFIG_STM32_ENERGYLITE)

/* STM32L15xx family */

#  if defined(CONFIG_STM32_STM32L15XX)
#    include "chip/stm32l15xxx_pinmap.h"
#  else
#    error "Unsupported EnergyLite chip"
#  endif

/* STM32 F1 Family ******************************************************************/

#elif defined(CONFIG_STM32_STM32F10XX)

/* STM32F100 Value Line */

#  if defined(CONFIG_STM32_VALUELINE)
#    include "chip/stm32f100_pinmap.h"

/* STM32 F102 USB Access Medium Density Family */
#  elif defined(CONFIG_ARCH_CHIP_STM32F102CB)
#    include "chip/stm32f102_pinmap.h"

/* STM32 F103 Low / Medium Density Family */
#  elif defined(CONFIG_ARCH_CHIP_STM32F103C4) || \
        defined(CONFIG_ARCH_CHIP_STM32F103C8) || \
        defined(CONFIG_ARCH_CHIP_STM32F103CB)
#    include "chip/stm32f103c_pinmap.h"

/* STM32 F103 High Density Family */
/* STM32F103RC, STM32F103RD, and STM32F103RE are all provided in 64 pin packages and differ
 * only in the available FLASH and SRAM.
 */

#  elif defined(CONFIG_ARCH_CHIP_STM32F103RB) || \
        defined(CONFIG_ARCH_CHIP_STM32F103RC) || \
        defined(CONFIG_ARCH_CHIP_STM32F103RD) || \
        defined(CONFIG_ARCH_CHIP_STM32F103RE) || \
        defined(CONFIG_ARCH_CHIP_STM32F103RG)
#    include "chip/stm32f103r_pinmap.h"

/* STM32F103VC, STM32F103VD, and STM32F103VE are all provided in 100 pin packages and differ
 * only in the available FLASH and SRAM.
 */

#  elif defined(CONFIG_ARCH_CHIP_STM32F103VC) || defined(CONFIG_ARCH_CHIP_STM32F103VE)
#    include "chip/stm32f103v_pinmap.h"

/* STM32F103ZC, STM32F103ZD, and STM32F103ZE are all provided in 144 pin packages and differ
 * only in the available FLASH and SRAM.
 */
#  elif defined(CONFIG_ARCH_CHIP_STM32F103ZE)
#    include "chip/stm32f103z_pinmap.h"

/* STM32 F105/F107 Connectivity Line */

#  elif defined(CONFIG_ARCH_CHIP_STM32F105VB)
#    include "chip/stm32f105v_pinmap.h"

#  elif defined(CONFIG_ARCH_CHIP_STM32F105RB)
#    include "chip/stm32f105r_pinmap.h"

#  elif defined(CONFIG_ARCH_CHIP_STM32F107VC)
#    include "chip/stm32f107v_pinmap.h"
#  else
#    error "Unsupported STM32F10XXX chip"
#  endif

/* STM32 F2 Family ******************************************************************/

#elif defined(CONFIG_STM32_STM32F20XX)
#  include "chip/stm32f20xxx_pinmap.h"

/* STM32 F3 Family ******************************************************************/

#elif defined(CONFIG_STM32_STM32F30XX)
#  include "chip/stm32f30xxx_pinmap.h"
#elif defined(CONFIG_STM32_STM32F33XX)
#  include "chip/stm32f33xxx_pinmap.h"
#elif defined(CONFIG_STM32_STM32F37XX)
#  include "chip/stm32f37xxx_pinmap.h"

/* STM32 F4 Family ******************************************************************/

#elif defined(CONFIG_STM32_STM32F4XXX)
#  include "chip/stm32f40xxx_pinmap.h"
#else
#  error "No pinmap file for this STM32 chip"
#endif

/* If the common ARMv7-M vector handling logic is used, then include the
 * required vector definitions as well.
 */

#ifdef CONFIG_ARMV7M_CMNVECTOR
#  if defined(CONFIG_STM32_STM32L15XX)
#    include "chip/stm32l15xxx_vectors.h"
#  elif defined(CONFIG_STM32_STM32F10XX)
#    include "chip/stm32f10xxx_vectors.h"
#  elif defined(CONFIG_STM32_STM32F20XX)
#    include "chip/stm32f20xxx_vectors.h"
#  elif defined(CONFIG_STM32_STM32F30XX)
#    include "chip/stm32f30xxx_vectors.h"
#  elif defined(CONFIG_STM32_STM32F33XX)
#    include "chip/stm32f33xxx_vectors.h"
#  elif defined(CONFIG_STM32_STM32F37XX)
#    include "chip/stm32f37xxx_vectors.h"
#  elif defined(CONFIG_STM32_STM32F4XXX)
#    include "chip/stm32f40xxx_vectors.h"
#  else
#    error "No vector file for this STM32 family"
#  endif
#endif

/* Include the chip memory map. */

#include "chip/stm32_memorymap.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#endif /* __ARCH_ARM_SRC_STM32_CHIP_H */

