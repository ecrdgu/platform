
#ifndef __ARCH_ARM_STC_STM32L4_CHIP_STM32L4_RNG_H
#define __ARCH_ARM_STC_STM32L4_CHIP_STM32L4_RNG_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Register Offsets *****************************************************************/

#define STM32L4_RNG_CR_OFFSET     0x0000  /* RNG Control Register */
#define STM32L4_RNG_SR_OFFSET     0x0004  /* RNG Status Register */
#define STM32L4_RNG_DR_OFFSET     0x0008  /* RNG Data Register */

/* Register Addresses ***************************************************************/

#define STM32L4_RNG_CR            (STM32L4_RNG_BASE+STM32L4_RNG_CR_OFFSET)
#define STM32L4_RNG_SR            (STM32L4_RNG_BASE+STM32L4_RNG_SR_OFFSET)
#define STM32L4_RNG_DR            (STM32L4_RNG_BASE+STM32L4_RNG_DR_OFFSET)

/* Register Bitfield Definitions ****************************************************/

/* RNG Control Register */

#define RNG_CR_RNGEN              (1 << 2)  /* Bit 2: RNG enable */
#define RNG_CR_IE                 (1 << 3)  /* Bit 3: Interrupt enable */

/* RNG Status Register */

#define RNG_SR_DRDY               (1 << 0) /* Bit 0: Data ready */
#define RNG_SR_CECS               (1 << 1) /* Bit 1: Clock error current status */
#define RNG_SR_SECS               (1 << 2) /* Bit 2: Seed error current status */
#define RNG_SR_CEIS               (1 << 5) /* Bit 5: Clock error interrupt status */
#define RNG_SR_SEIS               (1 << 6) /* Bit 6: Seed error interrupt status */

#endif /* __ARCH_ARM_STC_STM32L4_CHIP_STM32L4_RNG_H */
