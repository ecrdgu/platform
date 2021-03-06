
#ifndef __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_CRC_H
#define __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_CRC_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Register Offsets *****************************************************************/

#define STM32F0_CRC_DR_OFFSET     0x0000  /* Data register */
#define STM32F0_CRC_IDR_OFFSET    0x0004  /* Independent Data register */
#define STM32F0_CRC_CR_OFFSET     0x0008  /* Control register */
#define STM32F0_CRC_INIT_OFFSET   0x0010  /* Initial CRC value register */
#define STM32F0_CRC_POL_OFFSET    0x0014  /* CRC polynomial register */

/* Register Addresses ***************************************************************/

#define STM32F0_CRC_DR            (STM32F0_CRC_BASE+STM32F0_CRC_DR_OFFSET)
#define STM32F0_CRC_IDR           (STM32F0_CRC_BASE+STM32F0_CRC_IDR_OFFSET)
#define STM32F0_CRC_CR            (STM32F0_CRC_BASE+STM32F0_CRC_CR_OFFSET)
#define STM32F0_CRC_INIT          (STM32F0_CRC_BASE+STM32F0_CRC_INIT_OFFSET)
#define STM32F0_CRC_POL           (STM32F0_CRC_BASE+STM32F0_CRC_POL_OFFSET)

/* Register Bitfield Definitions ****************************************************/

/* CRC independent data register */

#define CRC_IDR_MASK              0xff      /* These bits as a temporary location for one byte, not affected by RESET bit of CR */

/* CRC control register */

#define CRC_CR_RESET              (1 << 0)  /* This bit reset the CRC calculation unit and load CRC_DR with value of CRC_INIT */
#define CRC_CR_POLYSIZE_SHIFT     3         /* Bits 3-4: Polynomial size (for STM32F07x and STM32F09x) */
#define CRC_CR_POLYSIZE_MASK      (3 << CRC_CR_POLYSIZE_SHIFT)
#  define CRC_CR_POLYSIZE_32      (0 << CRC_CR_POLYSIZE_SHIFT) /* 00: 32 bit polynomial */
#  define CRC_CR_POLYSIZE_16      (1 << CRC_CR_POLYSIZE_SHIFT) /* 01: 16 bit polynomial */
#  define CRC_CR_POLYSIZE_8       (2 << CRC_CR_POLYSIZE_SHIFT) /* 10: 8 bit polynomial */
#  define CRC_CR_POLYSIZE_7       (3 << CRC_CR_POLYSIZE_SHIFT) /* 10: 8 bit polynomial */
#define CRC_CR_REVIN_SHIFT        5         /* Bits 5-6: These bits ontrol the reversal of the bit order of the input data */
#define CRC_CR_REVIN_MASK         (3 << CRC_CR_REVIN_SHIFT)
#  define CRC_CR_REVIN_NONE       (0 << CRC_CR_REVIN_SHIFT) /* 00: bit order is not affected */
#  define CRC_CR_REVIN_BYTE       (1 << CRC_CR_REVIN_SHIFT) /* 01: reversal done by byte */
#  define CRC_CR_REVIN_HWORD      (2 << CRC_CR_REVIN_SHIFT) /* 10: reversal done by half-word */
#  define CRC_CR_REVIN_WORD       (3 << CRC_CR_REVIN_SHIFT) /* 11: reversal done by word */
#define CRC_CR_REVOUT             (1 << 7)  /* This bit controls the reversal of the bit order of the output data */

#endif /* __ARCH_ARM_SRC_STM32F0_CHIP_STM32F0_CRC_H */
