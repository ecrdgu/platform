
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "cache.h"

#ifdef CONFIG_ARMV7M_DCACHE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

void arch_invalidate_dcache(uintptr_t start, uintptr_t end)
{
  uint32_t ccsidr;
  uint32_t smask;
  uint32_t sshift;
  uint32_t ways;
  uint32_t wshift;
  uint32_t ssize;
  uint32_t set;
  uint32_t sw;

  /* Get the characteristics of the D-Cache */

  ccsidr = getreg32(NVIC_CCSIDR);
  smask  = CCSIDR_SETS(ccsidr);          /* (Number of sets) - 1 */
  sshift = CCSIDR_LSSHIFT(ccsidr) + 4;   /* log2(cache-line-size-in-bytes) */
  ways   = CCSIDR_WAYS(ccsidr);          /* (Number of ways) - 1 */

  /* Calculate the bit offset for the way field in the DCISW register by
   * counting the number of leading zeroes.  For example:
   *
   *   Number of  Value of ways  Field
   *   Ways       'ways'         Offset
   *     2         1             31
   *     4         3             30
   *     8         7             29
   *   ...
   */

  wshift = arm_clz(ways) & 0x1f;

  /* Invalidate the D-Cache over the range of addresses */

  ssize  = (1 << sshift);
  start &= ~(ssize - 1);
  ARM_DSB();

  do
    {
      int32_t tmpways = ways;

      /* Isolate the cache line associated with this address.  For example
       * if the cache line size is 32 bytes and the cache size is 16KB, then
       *
       *   sshift = 5      : Offset to the beginning of the set field
       *   smask  = 0x007f : Mask of the set field
       */

      set = ((uint32_t)start >> sshift) & smask;

      /* Clean and invalidate each way for this cacheline */

      do
        {
          sw = ((tmpways << wshift) | (set << sshift));
          putreg32(sw, NVIC_DCISW);
        }
      while (tmpways--);

      /* Increment the address by the size of one cache line. */

      start += ssize;
    }
  while (start < end);

  ARM_DSB();
  ARM_ISB();
}

#endif  /* CONFIG_ARMV7M_DCACHE */
