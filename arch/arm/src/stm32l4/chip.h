
#ifndef __ARCH_ARM_SRC_STM32L4_CHIP_H
#define __ARCH_ARM_SRC_STM32L4_CHIP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/* Include the memory map and the chip definitions file.  Other chip hardware files
 * should then include this file for the proper setup.
 */

#include <arch/irq.h>
#include <arch/stm32l4/chip.h>
#include "chip/stm32l4_pinmap.h"
#include "chip/stm32l4_memorymap.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* If the common ARMv7-M vector handling logic is used, then it expects the
 * following definition in this file that provides the number of supported external
 * interrupts which, for this architecture, is provided in the arch/stm32l4/chip.h
 * header file.
 */

#define ARMV7M_PERIPHERAL_INTERRUPTS NR_INTERRUPTS

/* Cache line sizes (in bytes) for the STM32L4 */

#define ARMV7M_DCACHE_LINESIZE 0  /* no cache */
#define ARMV7M_ICACHE_LINESIZE 0  /* no cache */

#endif /* __ARCH_ARM_SRC_STM32L4_CHIP_H */
