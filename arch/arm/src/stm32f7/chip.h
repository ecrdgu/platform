
#ifndef __ARCH_ARM_SRC_STM32F7_CHIP_H
#define __ARCH_ARM_SRC_STM32F7_CHIP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/* Include the memory map and the chip definitions file.  Other chip hardware files
 * should then include this file for the proper setup.
 */

#include <arch/irq.h>
#include <arch/stm32f7/chip.h>
#include "chip/stm32_pinmap.h"
#include "chip/stm32_memorymap.h"

/* If the common ARMv7-M vector handling logic is used, then it expects the
 * following definition in this file that provides the number of supported external
 * interrupts which, for this architecture, is provided in the arch/stm32f7/chip.h
 * header file.
 */

#define ARMV7M_PERIPHERAL_INTERRUPTS NR_INTERRUPTS

/* Cache line sizes (in bytes)for the STM32F7 */

#define ARMV7M_DCACHE_LINESIZE 32  /* 32 bytes (8 words) */
#define ARMV7M_ICACHE_LINESIZE 32  /* 32 bytes (8 words) */

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public Data
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#endif /* __ARCH_ARM_SRC_STM32F7_CHIP_H */
