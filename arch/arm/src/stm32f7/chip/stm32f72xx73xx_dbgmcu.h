
#ifndef __ARCH_ARM_SRC_STM32F7_CHIP_STM32F72XX73XXDBGMCU_H
#define __ARCH_ARM_SRC_STM32F7_CHIP_STM32F72XX73XXDBGMCU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Addresses *******************************************************/

#define STM32_DBGMCU_IDCODE       0xe0042000  /* MCU identifier */
#define STM32_DBGMCU_CR           0xe0042004  /* MCU debug */
#define STM32_DBGMCU_APB1_FZ      0xe0042008  /* Debug MCU APB1 freeze register */
#define STM32_DBGMCU_APB2_FZ      0xe004200c  /* Debug MCU APB2 freeze register */

/* Register Bitfield Definitions ********************************************/

/* MCU identifier */

#define DBGMCU_IDCODE_DEVID_SHIFT (0)       /* Bits 11-0: Device Identifier */
#define DBGMCU_IDCODE_DEVID_MASK  (0x0fff << DBGMCU_IDCODE_DEVID_SHIFT)
#define DBGMCU_IDCODE_REVID_SHIFT (16)      /* Bits 31-16:  Revision Identifier */
#define DBGMCU_IDCODE_REVID_MASK  (0xffff << DBGMCU_IDCODE_REVID_SHIFT)

/* MCU debug */

#define DBGMCU_CR_SLEEP           (1 << 0)  /* Bit 0: Debug Sleep Mode */
#define DBGMCU_CR_STOP            (1 << 1)  /* Bit 1: Debug Stop Mode */
#define DBGMCU_CR_STANDBY         (1 << 2)  /* Bit 2: Debug Standby mode */
#define DBGMCU_CR_TRACEIOEN       (1 << 5)  /* Bit 5: Trace enabled */

#define DBGMCU_CR_TRACEMODE_SHIFT (6)        /* Bits 7-6: Trace mode pin assignement */
#define DBGMCU_CR_TRACEMODE_MASK  (3 << DBGMCU_CR_TRACEMODE_SHIFT)
#define DBGMCU_CR_ASYNCH          (0 << DBGMCU_CR_TRACEMODE_SHIFT) /* Asynchronous Mode */

/* Debug MCU APB1 freeze register */

#define DBGMCU_APB1_TIM2STOP      (1 << 0)   /* Bit 0: TIM2 stopped when core is halted */
#define DBGMCU_APB1_TIM3STOP      (1 << 1)   /* Bit 1: TIM3 stopped when core is halted */
#define DBGMCU_APB1_TIM4STOP      (1 << 2)   /* Bit 2: TIM4 stopped when core is halted */
#define DBGMCU_APB1_TIM5STOP      (1 << 3)   /* Bit 3: TIM5 stopped when core is halted */
#define DBGMCU_APB1_TIM6STOP      (1 << 4)   /* Bit 4: TIM6 stopped when core is halted */
#define DBGMCU_APB1_TIM7STOP      (1 << 5)   /* Bit 5: TIM7 stopped when core is halted */
#define DBGMCU_APB1_TIM12STOP     (1 << 6)   /* Bit 6: TIM12 stopped when core is halted */
#define DBGMCU_APB1_TIM13STOP     (1 << 7)   /* Bit 7: TIM13 stopped when core is halted */
#define DBGMCU_APB1_TIM14STOP     (1 << 8)   /* Bit 8: TIM14 stopped when core is halted */
#define DBGMCU_APB1_LPTIM1STOP    (1 << 9)   /* Bit 9: LPTIM1 stopped when core is halted */
#define DBGMCU_APB1_RTCSTOP       (1 << 10)  /* Bit 10: RTC stopped when Core is halted */
#define DBGMCU_APB1_WWDGSTOP      (1 << 11)  /* Bit 11: Window Watchdog stopped when core is halted */
#define DBGMCU_APB1_IWDGSTOP      (1 << 12)  /* Bit 12: Independent Watchdog stopped when core is halted */
#define DBGMCU_APB1_I2C1STOP      (1 << 21)  /* Bit 21: I2C1 SMBUS timeout mode stopped when Core is halted */
#define DBGMCU_APB1_I2C2STOP      (1 << 22)  /* Bit 22: I2C2 SMBUS timeout mode stopped when Core is halted */
#define DBGMCU_APB1_I2C3STOP      (1 << 23)  /* Bit 23: I2C3 SMBUS timeout mode stopped when Core is halted */
#define DBGMCU_APB1_CAN1STOP      (1 << 25)  /* Bit 25: CAN1 stopped when core is halted */

/* Debug MCU APB2 freeze register */

#define DBGMCU_APB2_TIM1STOP      (1 << 0)   /* Bit 0:  TIM1 stopped when core is halted */
#define DBGMCU_APB2_TIM8STOP      (1 << 1)   /* Bit 1:  TIM8 stopped when core is halted */
#define DBGMCU_APB2_TIM9STOP      (1 << 16)  /* Bit 16: TIM9 stopped when core is halted */
#define DBGMCU_APB2_TIM10STOP     (1 << 17)  /* Bit 17: TIM10 stopped when core is halted */
#define DBGMCU_APB2_TIM11STOP     (1 << 18)  /* Bit 18: TIM11 stopped when core is halted */

#endif /* __ARCH_ARM_SRC_STM32F7_CHIP_STM32F72XX74XXDBGMCU_H */