
#ifndef __ARCH_ARM_SRC_ARMV7_A_CACHE_H
#define __ARCH_ARM_SRC_ARMV7_A_CACHE_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <ecr/irq.h>

#include "cp15_cacheops.h"
#include "l2cc.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Intrinsics are used in these inline functions */

#define arm_isb(n) __asm__ __volatile__ ("isb " #n : : : "memory")
#define arm_dsb(n) __asm__ __volatile__ ("dsb " #n : : : "memory")
#define arm_dmb(n) __asm__ __volatile__ ("dmb " #n : : : "memory")

#define ARM_DSB()  arm_dsb(15)
#define ARM_ISB()  arm_isb(15)
#define ARM_DMB()  arm_dmb(15)

 /************************************************************************************
 * Inline Functions
 ************************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Name: arch_invalidate_dcache
 *
 * Description:
 *   Invalidate the data cache within the specified region; we will be
 *   performing a DMA operation in this region and we want to purge old data
 *   in the cache.
 *
 * Input Parameters:
 *   start - virtual start address of region
 *   end   - virtual end address of region + 1
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   This operation is not atomic.  This function assumes that the caller
 *   has exclusive access to the address range so that no harm is done if
 *   the operation is pre-empted.
 *
 ****************************************************************************/

static inline void arch_invalidate_dcache(uintptr_t start, uintptr_t end)
{
  cp15_invalidate_dcache(start, end);
  l2cc_invalidate(start, end);
}

/****************************************************************************
 * Name: arch_invalidate_dcache_all
 *
 * Description:
 *   Invalidate the entire contents of D cache.
 *
 *   NOTE: This function forces L1 and L2 cache operations to be atomic
 *   by disabling interrupts.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline void arch_invalidate_dcache_all(void)
{
#ifdef CONFIG_ARCH_L2CACHE
  irqstate_t flags = enter_critical_section();
  cp15_invalidate_dcache_all();
  l2cc_invalidate_all();
  leave_critical_section(flags);
#else
  cp15_invalidate_dcache_all();
#endif
}

/************************************************************************************
 * Name: arch_invalidate_icache
 *
 * Description:
 *   Invalidate all instruction caches to PoU, also flushes branch target cache
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

#define arch_invalidate_icache() cp15_invalidate_icache()

/****************************************************************************
 * Name: arch_clean_dcache
 *
 * Description:
 *   Clean the data cache within the specified region by flushing the
 *   contents of the data cache to memory.
 *
 * Input Parameters:
 *   start - virtual start address of region
 *   end   - virtual end address of region + 1
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   This operation is not atomic.  This function assumes that the caller
 *   has exclusive access to the address range so that no harm is done if
 *   the operation is pre-empted.
 *
 ****************************************************************************/

static inline void arch_clean_dcache(uintptr_t start, uintptr_t end)
{
  cp15_clean_dcache(start, end);
  l2cc_clean(start, end);
}

/****************************************************************************
 * Name: arch_flush_dcache
 *
 * Description:
 *   Flush the data cache within the specified region by cleaning and
 *   invalidating the D cache.
 *
 * Input Parameters:
 *   start - virtual start address of region
 *   end   - virtual end address of region + 1
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   This operation is not atomic.  This function assumes that the caller
 *   has exclusive access to the address range so that no harm is done if
 *   the operation is pre-empted.
 *
 ****************************************************************************/

static inline void arch_flush_dcache(uintptr_t start, uintptr_t end)
{
  cp15_flush_dcache(start, end);
  l2cc_flush(start, end);
}

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif  /* __ARCH_ARM_SRC_ARMV7_A_CACHE_H */
