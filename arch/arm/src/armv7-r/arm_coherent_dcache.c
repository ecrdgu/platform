
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>

#include "cp15_cacheops.h"

#include <ecr/arch.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_coherent_dcache
 *
 * Description:
 *   Ensure that the I and D caches are coherent within specified region
 *   by cleaning the D cache (i.e., flushing the D cache contents to memory
 *   and invalidating the I cache. This is typically used when code has been
 *   written to a memory region, and will be executed.
 *
 * Input Parameters:
 *   addr - virtual start address of region
 *   len  - Size of the address region in bytes
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void up_coherent_dcache(uintptr_t addr, size_t len)
{
  if (len > 0)
    {
      /* Perform the operation on the L1 cache */

      cp15_coherent_dcache(addr, addr + len - 1);

#ifdef CONFIG_ARCH_L2CACHE
      /* If we have an L2 cache, then there more things that need to done */

#  warning This is insufficient
#endif
    }
}
