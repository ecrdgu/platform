
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "cache.h"

#if defined(CONFIG_ARMV7M_DCACHE) && !defined(CONFIG_ARMV7M_DCACHE_WRITETHROUGH)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arch_flush_dcache_all
 *
 * Description:
 *   Flush the entire data cache by cleaning and invalidating the D cache.
 *
 * Input Parameters:
 *   None
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

void arch_flush_dcache_all(void)
{
  uint32_t ccsidr;
  uint32_t sshift;
  uint32_t wshift;
  uint32_t sw;
  uint32_t sets;
  uint32_t ways;

  /* Get the characteristics of the D-Cache */

  ccsidr = getreg32(NVIC_CCSIDR);
  sets   = CCSIDR_SETS(ccsidr);          /* (Number of sets) - 1 */
  sshift = CCSIDR_LSSHIFT(ccsidr) + 4;   /* log2(cache-line-size-in-bytes) */
  ways   = CCSIDR_WAYS(ccsidr);          /* (Number of ways) - 1 */

  /* Calculate the bit offset for the way field in the DCCISW register by
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

  ARM_DSB();

  /* Clean and invalidate the entire D-Cache */

  do
    {
      int32_t tmpways = ways;

      do
        {
          sw = ((tmpways << wshift) | (sets << sshift));
          putreg32(sw, NVIC_DCCISW);
        }
      while (tmpways--);
    }
  while (sets--);

  ARM_DSB();
  ARM_ISB();
}

#endif  /* CONFIG_ARMV7M_DCACHE && !CONFIG_ARMV7M_DCACHE_WRITETHROUGH */
