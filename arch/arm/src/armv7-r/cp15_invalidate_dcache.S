
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cp15.h"

	.file	"cp15_invalidate_dcache.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	cp15_invalidate_dcache

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: cp15_invalidate_dcache
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
 ****************************************************************************/

	.globl	cp15_invalidate_dcache
	.type	cp15_invalidate_dcache, function

cp15_invalidate_dcache:

	mrc		CP15_CTR(r3)				/* Read the Cache Type Register */
	lsr		r3, r3, #16				/* Isolate the DMinLine field */
	and		r3, r3, #0xf
	mov		r2, #4
	mov		r2, r2, lsl r3			/* Get the cache line size in bytes */

	sub		r3, r2, #1				/* R3=Cache line size mask */
	tst		r0, r3
	bic		r0, r0, r3				/* R0=aligned start address */

	mcrne	CP15_DCCIMVAC(r0)		/* Clean and invalidate data cache line by VA to PoC */

	tst		r1, r3
	bic		r1, r1, r3				/* R0=aligned end address */
	mcrne	CP15_DCCIMVAC(r1)		/* Clean and invalidate data cache line by VA to PoC */

	/* Loop, invalidating each D cache line */
1:
	mcr		CP15_DCIMVAC(r0)		/* Invalidate data cache line by VA to PoC */
	add		r0, r0, r2				/* R12=Next cache line */
	cmp		r0, r1					/* Loop until all cache lines have been invalidate */
	blo		1b

	dsb
	bx		lr
	.size cp15_invalidate_dcache, . - cp15_invalidate_dcache
	.end
