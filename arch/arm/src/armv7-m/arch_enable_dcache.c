
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
 * Name: arch_enable_dcache
 *
 * Description:
 *   Enable the D-Cache
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void arch_enable_dcache(void)
{
  uint32_t ccsidr;
  uint32_t ccr;
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

  /* Invalidate the entire D-Cache */

  ARM_DSB();
  do
    {
      int32_t tmpways = ways;

      do
        {
          sw = ((tmpways << wshift) | (sets << sshift));
          putreg32(sw, NVIC_DCISW);
        }
      while (tmpways--);
    }
  while (sets--);

  ARM_DSB();

  /* Enable the D-Cache */

  ccr  = getreg32(NVIC_CFGCON);
  ccr |= NVIC_CFGCON_DC;
  putreg32(ccr, NVIC_CFGCON);

  ARM_DSB();
  ARM_ISB();
}

#endif  /* CONFIG_ARMV7M_DCACHE */
