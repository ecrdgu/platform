
/* This file should never be included directed but, rather,
 * only indirectly through ecr/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_STM32L4_IRQ_H
#define __ARCH_ARM_INCLUDE_STM32L4_IRQ_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* IRQ numbers.  The IRQ number corresponds vector number and hence map directly to
 * bits in the NVIC.  This does, however, waste several words of memory in the IRQ
 * to handle mapping tables.
 */

/* Processor Exceptions (vectors 0-15) */

#define STM32L4_IRQ_RESERVED       (0) /* Reserved vector (only used with CONFIG_DEBUG_FEATURES) */
                                     /* Vector  0: Reset stack pointer value */
                                     /* Vector  1: Reset (not handler as an IRQ) */
#define STM32L4_IRQ_NMI            (2) /* Vector  2: Non-Maskable Interrupt (NMI) */
#define STM32L4_IRQ_HARDFAULT      (3) /* Vector  3: Hard fault */
#define STM32L4_IRQ_MEMFAULT       (4) /* Vector  4: Memory management (MPU) */
#define STM32L4_IRQ_BUSFAULT       (5) /* Vector  5: Bus fault */
#define STM32L4_IRQ_USAGEFAULT     (6) /* Vector  6: Usage fault */
                                     /* Vectors 7-10: Reserved */
#define STM32L4_IRQ_SVCALL        (11) /* Vector 11: SVC call */
#define STM32L4_IRQ_DBGMONITOR    (12) /* Vector 12: Debug Monitor */
                                     /* Vector 13: Reserved */
#define STM32L4_IRQ_PENDSV        (14) /* Vector 14: Pendable system service request */
#define STM32L4_IRQ_SYSTICK       (15) /* Vector 15: System tick */

/* External interrupts (vectors >= 16).  These definitions are chip-specific */

#define STM32L4_IRQ_FIRST         (16) /* Vector number of the first external interrupt */

#if defined(CONFIG_STM32L4_STM32L4X3)
#  include <arch/stm32l4/stm32l4x3xx_irq.h>
#elif defined(CONFIG_STM32L4_STM32L4X5)
#  include <arch/stm32l4/stm32l4x5xx_irq.h>
#elif defined(CONFIG_STM32L4_STM32L4X6)
#  include <arch/stm32l4/stm32l4x6xx_irq.h>
#else
#  error "Unsupported STM32 L4 chip"
#endif

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public Data
 ************************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_STM32L4_IRQ_H */

