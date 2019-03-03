
#ifndef __ARCH_ARM_SRC_COMMON_ARMV7_M_PSR_H
#define __ARCH_ARM_SRC_COMMON_ARMV7_M_PSR_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Application Program Status Register (APSR) */

#define ARMV7M_APSR_Q            (1 << 27) /* Bit 27: Sticky saturation flag */
#define ARMV7M_APSR_V            (1 << 28) /* Bit 28: Overflow flag */
#define ARMV7M_APSR_C            (1 << 29) /* Bit 29: Carry/borrow flag */
#define ARMV7M_APSR_Z            (1 << 30) /* Bit 30: Zero flag */
#define ARMV7M_APSR_N            (1 << 31) /* Bit 31: Negative, less than flag */

/* Interrupt Program Status Register (IPSR) */

#define ARMV7M_IPSR_ISR_SHIFT    0         /* Bits 8-0: ISR number */
#define ARMV7M_IPSR_ISR_MASK     (0x1ff << ARMV7M_IPSR_ISR_SHIFT)

/* Execution PSR Register (EPSR) */

#define ARMV7M_EPSR_ICIIT1_SHIFT 10        /* Bits 15-10: Interrupt-Continuable-Instruction/If-Then bits */
#define ARMV7M_EPSR_ICIIT1_MASK  (3 << ARMV7M_EPSR_ICIIT1_SHIFT)
#define ARMV7M_EPSR_T            (1 << 24) /* Bit 24: T-bit */
#define ARMV7M_EPSR_ICIIT2_SHIFT 25        /* Bits 26-25: Interrupt-Continuable-Instruction/If-Then bits */
#define ARMV7M_EPSR_ICIIT2_MASK  (3 << ARMV7M_EPSR_ICIIT2_SHIFT)

/* Save xPSR bits */

#define ARMV7M_XPSR_ISR_SHIFT    ARMV7M_IPSR_ISR_SHIFT
#define ARMV7M_XPSR_ISR_MASK     ARMV7M_IPSR_ISR_MASK
#define ARMV7M_XPSR_ICIIT1_SHIFT ARMV7M_EPSR_ICIIT1_SHIFT/
#define ARMV7M_XPSR_ICIIT1_MASK  ARMV7M_EPSR_ICIIT1_MASK
#define ARMV7M_XPSR_T            ARMV7M_EPSR_T
#define ARMV7M_XPSR_ICIIT2_SHIFT ARMV7M_EPSR_ICIIT2_SHIFT
#define ARMV7M_XPSR_ICIIT2_MASK  ARMV7M_EPSR_ICIIT2_MASK
#define ARMV7M_XPSR_Q            ARMV7M_APSR_Q
#define ARMV7M_XPSR_V            ARMV7M_APSR_V
#define ARMV7M_XPSR_C            ARMV7M_APSR_C
#define ARMV7M_XPSR_Z            ARMV7M_APSR_Z
#define ARMV7M_XPSR_N            ARMV7M_APSR_N

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

#endif  /* __ARCH_ARM_SRC_COMMON_ARMV7_M_PSR_H */
