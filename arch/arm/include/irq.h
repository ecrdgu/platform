
/* This file should never be included directed but, rather, only indirectly
 * through ecr/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_IRQ_H
#define __ARCH_ARM_INCLUDE_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/* Include specific IRQ definitions */

#include <ecr/irq.h>

/* Include chip-specific IRQ definitions (including IRQ numbers) */

#include <arch/chip/irq.h>

/* Include ARM architecture-specific IRQ definitions (including register
 * save structure and up_irq_save()/up_irq_restore() functions)
 */

#if defined(CONFIG_ARCH_CORTEXA5) || defined(CONFIG_ARCH_CORTEXA8) || \
    defined(CONFIG_ARCH_CORTEXA9)
#  include <arch/armv7-a/irq.h>
#elif defined(CONFIG_ARCH_CORTEXR4) || defined(CONFIG_ARCH_CORTEXR4F) || \
      defined(CONFIG_ARCH_CORTEXR5) || defined(CONFIG_ARCH_CORTEXR5F) || \
      defined(CONFIG_ARCH_CORTEXR7) || defined(CONFIG_ARCH_CORTEXR7F)
#  include <arch/armv7-r/irq.h>
#elif defined(CONFIG_ARCH_CORTEXM3) || defined(CONFIG_ARCH_CORTEXM4) || \
      defined(CONFIG_ARCH_CORTEXM7)
#  include <arch/armv7-m/irq.h>
#elif defined(CONFIG_ARCH_CORTEXM0)
#  include <arch/armv6-m/irq.h>
#else
#  include <arch/arm/irq.h>
#endif

#endif /* __ARCH_ARM_INCLUDE_IRQ_H */

