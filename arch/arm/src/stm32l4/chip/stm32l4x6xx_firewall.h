
#ifndef __ARCH_ARM_SRC_STM32L4_CHIP_STM32L4X6XX_FIREWALL_H
#define __ARCH_ARM_SRC_STM32L4_CHIP_STM32L4X6XX_FIREWALL_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <sys/types.h>

#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Register Offsets *****************************************************************/

#define STM32L4_FIREWALL_CSSA_OFFSET    0x0000
#define STM32L4_FIREWALL_CSL_OFFSET     0x0004
#define STM32L4_FIREWALL_NVDSSA_OFFSET  0x0008
#define STM32L4_FIREWALL_NVDSL_OFFSET   0x000C
#define STM32L4_FIREWALL_VDSSA_OFFSET   0x0010
#define STM32L4_FIREWALL_VDSL_OFFSET    0x0014
#define STM32L4_FIREWALL_CR_OFFSET      0x0020

/* Register Addresses ***************************************************************/

#define STM32L4_FIREWALL_CSSA           (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_CSSA_OFFSET)
#define STM32L4_FIREWALL_CSL            (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_CSL_OFFSET)
#define STM32L4_FIREWALL_NVDSSA         (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_NVDSSA_OFFSET)
#define STM32L4_FIREWALL_NVDSL          (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_NVDSL_OFFSET)
#define STM32L4_FIREWALL_VDSSA          (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_VDSSA_OFFSET)
#define STM32L4_FIREWALL_VDSL           (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_VDSL_OFFSET)
#define STM32L4_FIREWALL_CR             (STM32L4_FIREWALL_BASE+STM32L4_FIREWALL_CR_OFFSET)

/* Register Bitfield Definitions ****************************************************/

/* Code Segment Start Address */

#define FIREWALL_CSSADD_SHIFT           8
#define FIREWALL_CSSADD_MASK            (0xFFFF << FIREWALL_CSSADD_SHIFT)

/* Code Segment Length */

#define FIREWALL_CSSLENG_SHIFT          8
#define FIREWALL_CSSLENG_MASK           (0x3FFF << FIREWALL_CSSLENG_SHIFT)

/* Non-volatile Data Segment Start Address */

#define FIREWALL_NVDSADD_SHIFT          8
#define FIREWALL_NVDSADD_MASK           (0xFFFF << FIREWALL_NVDSADD_SHIFT)

/* Non-volatile Data Segment Length */

#define FIREWALL_NVDSLENG_SHIFT         8
#define FIREWALL_NVDSLENG_MASK          (0x3FFF << FIREWALL_NVDSLENG_SHIFT)

/* Volatile Data Segment Start Address */

#define FIREWALL_VDSADD_SHIFT           6
#if defined(CONFIG_STM32L4_STM32L496XX)
#define FIREWALL_VDSADD_MASK            (0x0FFF << FIREWALL_VDSADD_SHIFT)
#else
#define FIREWALL_VDSADD_MASK            (0x07FF << FIREWALL_VDSADD_SHIFT)
#endif

/* Volatile Data Segment Length */

#define FIREWALL_VDSLENG_SHIFT          6
#if defined(CONFIG_STM32L4_STM32L496XX)
#define FIREWALL_VDSLENG_MASK           (0x0FFF << FIREWALL_VDSLENG_SHIFT)
#else
#define FIREWALL_VDSLENG_MASK           (0x07FF << FIREWALL_VDSLENG_SHIFT)
#endif

/* Configuration Register */

#define FIREWALL_CR_FPA                 (1 << 0) /* Bit 0: Firewall prearm */
#define FIREWALL_CR_VDS                 (1 << 1) /* Bit 1: Volatile data shared */
#define FIREWALL_CR_VDE                 (1 << 2) /* Bit 2: Volatile data execution */

#endif /* __ARCH_ARM_SRC_STM32L4_CHIP_STM32L4X6XX_FIREWALL_H */
