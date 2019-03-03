
#ifndef __ARCH_ARM_INCLUDE_SPINLOCK_H
#define __ARCH_ARM_INCLUDE_SPINLOCK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef __ASSEMBLY__
#  include <stdint.h>
#endif /* __ASSEMBLY__ */

/* Include ARM architecture-specific IRQ definitions (including register
 * save structure and up_irq_save()/up_irq_restore() functions)
 */

#if defined(CONFIG_ARCH_CORTEXA5) || defined(CONFIG_ARCH_CORTEXA8) || \
    defined(CONFIG_ARCH_CORTEXA9)
#  include <arch/armv7-a/spinlock.h>
#elif defined(CONFIG_ARCH_CORTEXR4) || defined(CONFIG_ARCH_CORTEXR4F) || \
      defined(CONFIG_ARCH_CORTEXR5) || defined(CONFIG_ARCH_CORTEXR5F) || \
      defined(CONFIG_ARCH_CORTEXR7) || defined(CONFIG_ARCH_CORTEXR7F)
#  include <arch/armv7-r/spinlock.h>
#elif defined(CONFIG_ARCH_CORTEXM3) || defined(CONFIG_ARCH_CORTEXM4) || \
      defined(CONFIG_ARCH_CORTEXM7)
#  include <arch/armv7-m/spinlock.h>
#elif defined(CONFIG_ARCH_CORTEXM0)
#  include <arch/armv6-m/spinlock.h>
#else
#  include <arch/arm/spinlock.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Spinlock states */

#define SP_UNLOCKED 0  /* The Un-locked state */
#define SP_LOCKED   1  /* The Locked state */

/* Memory barriers for use with spinlock logic
 *
 * Data Memory Barrier (DMB) acts as a memory barrier. It ensures that all
 * explicit memory accesses that appear in program order before the DMB
 * instruction are observed before any explicit memory accesses that appear
 * in program order after the DMB instruction. It does not affect the
 * ordering of any other instructions executing on the processor
 *
 *   dmb st - Data memory barrier.  Wait for stores to complete.
 *
 * Data Synchronization Barrier (DSB) acts as a special kind of memory
 * barrier. No instruction in program order after this instruction executes
 * until this instruction completes. This instruction completes when: (1) All
 * explicit memory accesses before this instruction complete, and (2) all
 * Cache, Branch predictor and TLB maintenance operations before this
 * instruction complete.
 *
 *   dsb sy - Data syncrhonization barrier.  Assures that the CPU waits until
 *            all memory accesses are complete
 */

#define SP_DSB(n) __asm__ __volatile__ ("dsb sy" : : : "memory")
#define SP_DMB(n) __asm__ __volatile__ ("dmb st" : : : "memory")

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* The Type of a spinlock.
 *
 * ARMv6 architecture introuced the concept of exclusive accesses to memory
 * locations in the form of the Load-Exclusive (LDREX) and Store-Exclusive
 * (STREX) instructions in ARM and Thumb instruction sets.  ARMv6K extended
 * this to included byte, halfword, and doubleword variants of LDREX and
 * STREX.  ARMv7-M supports byte and halfwor, but not the doudleword varient
 * (ARMv6-M does not support exlusive access)
 *
 * ARM architectures prior to ARMv6 supported SWP and SWPB instructions that
 * atomically swap a 32-bit word for byte value between a register and a
 * memory location.  From the ARMv6 architecture, ARM deprecates the use
 * of SWP and SWPB.
 */

typedef uint8_t spinlock_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_testset
 *
 * Description:
 *   Perform an atomic test and set operation on the provided spinlock.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   lock - The address of spinlock object.
 *
 * Returned Value:
 *   The spinlock is always locked upon return.  The value of previous value
 *   of the spinlock variable is returned, either SP_LOCKED if the spinlock
 *   as previously locked (meaning that the test-and-set operation failed to
 *   obtain the lock) or SP_UNLOCKED if the spinlock was previously unlocked
 *   (meaning that we successfully obtained the lock)
 *
 ****************************************************************************/

/* See prototype in ecr/include/ecr/spinlock.h */

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_INCLUDE_SPINLOCK_H */
