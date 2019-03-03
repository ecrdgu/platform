
#ifndef __ARCH_ARM_SRC_COMMON_ARMV6_M_PSR_H
#define __ARCH_ARM_SRC_COMMON_ARMV6_M_PSR_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Application Program Status Register (APSR) */

#define ARMV6M_APSR_V            (1 << 28) /* Bit 28: Overflow flag */
#define ARMV6M_APSR_C            (1 << 29) /* Bit 29: Carry/borrow flag */
#define ARMV6M_APSR_Z            (1 << 30) /* Bit 30: Zero flag */
#define ARMV6M_APSR_N            (1 << 31) /* Bit 31: Negative, less than flag */

/* Interrupt Program Status Register (IPSR) */

#define ARMV6M_IPSR_ISR_SHIFT    0         /* Bits 5-0: ISR number */
#define ARMV6M_IPSR_ISR_MASK     (31 << ARMV6M_IPSR_ISR_SHIFT)

/* Execution PSR Register (EPSR) */

#define ARMV6M_EPSR_T            (1 << 24) /* Bit 24: T-bit */

/* Save xPSR bits */

#define ARMV6M_XPSR_ISR_SHIFT    ARMV6M_IPSR_ISR_SHIFT
#define ARMV6M_XPSR_ISR_MASK     ARMV6M_IPSR_ISR_MASK
#define ARMV6M_XPSR_T            ARMV6M_EPSR_T
#define ARMV6M_XPSR_V            ARMV6M_APSR_V
#define ARMV6M_XPSR_C            ARMV6M_APSR_C
#define ARMV6M_XPSR_Z            ARMV6M_APSR_Z
#define ARMV6M_XPSR_N            ARMV6M_APSR_N

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

#endif  /* __ARCH_ARM_SRC_COMMON_ARMV6_M_PSR_H */
