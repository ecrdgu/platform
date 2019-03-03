
#ifndef __ARCH_ARM_SRC_ARMV7_A_MPCORE_H
#define __ARCH_ARM_SRC_ARMV7_A_MPCORE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "chip.h"    /* For CHIP_MPCORE_VBASE */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* MPCore registers are memory mapped and accessed through a processor
 * specific private address space via the SCU.  The Cortex-A9 MCU chip.h
 * header file must provide the definition CHIP_MPCORE_VBASE to access this
 * the registers in this memory region.
 */

/* Peripheral Base Offsets **************************************************/

#define MPCORE_SCU_OFFSET  0x0000 /* 0x0000-0x00fc SCU registers */
#define MPCORE_ICC_OFFSET  0x0100 /* 0x0000-0x00FC Interrupt conroller interface */
#define MPCORE_GTM_OFFSET  0x0200 /* 0x0200-0x02ff Global timer */
                                  /* 0x0300-0x05ff Reserved */
#define MPCORE_PTM_OFFSET  0x0600 /* 0x0600-0x06ff Private timers and watchdogs */
                                  /* 0x0700-0x07ff Reserved */
#define MPCORE_ICD_OFFSET  0x1000 /* 0x1000-0x1fff Interrupt Distributor */

/* Peripheral Base Addresses ************************************************/

#define MPCORE_SCU_VBASE   (CHIP_MPCORE_VBASE+MPCORE_SCU_OFFSET)
#define MPCORE_ICC_VBASE   (CHIP_MPCORE_VBASE+MPCORE_ICC_OFFSET)
#define MPCORE_GTM_VBASE   (CHIP_MPCORE_VBASE+MPCORE_GTM_OFFSET)
#define MPCORE_PTM_VBASE   (CHIP_MPCORE_VBASE+MPCORE_PTM_OFFSET)
#define MPCORE_ICD_VBASE   (CHIP_MPCORE_VBASE+MPCORE_ICD_OFFSET)

#endif /* __ARCH_ARM_SRC_ARMV7_A_MPCORE_H */
