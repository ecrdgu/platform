
#ifndef __ARCH_ARM_SRC_STM32F0_CHIP_H
#define __ARCH_ARM_SRC_STM32F0_CHIP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "nvic.h"

/* Include the chip capabilities file */

#include <arch/stm32f0/chip.h>

#define ARMV6M_PERIPHERAL_INTERRUPTS 32

/* Include the memory map file.  Other chip hardware files should then include
 * this file for the proper setup.
 */

#include "chip/stm32f0_memorymap.h"

#endif /* __ARCH_ARM_SRC_STM32F0_CHIP_H */
