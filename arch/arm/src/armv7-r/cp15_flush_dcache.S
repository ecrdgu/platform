
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cp15.h"

	.file	"cp15_flush_dcache.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	cp15_flush_dcache

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: cp15_flush_dcache
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
 ****************************************************************************/

	.globl	cp15_flush_dcache
	.type	cp15_flush_dcache, function

cp15_flush_dcache:

	mrc		CP15_CTR(r3)				/* Read the Cache Type Register */
	lsr		r3, r3, #16				/* Isolate the DMinLine field */
	and		r3, r3, #0xf
	mov		r2, #4
	mov		r2, r2, lsl r3			/* Get the cache line size in bytes */

	sub		r3, r2, #1				/* R3=Cache line size mask */
	bic		r0, r0, r3				/* R0=aligned start address */

	/* Loop, cleaning and invaliding each D cache line in the address range */

1:
	mcrne	CP15_DCCIMVAC(r0)		/* Clean and invalidate data cache line by VA to PoC */
	add		r0, r0, r2				/* R12=Next cache line */
	cmp		r0, r1					/* Loop until all cache lines have been cleaned */
	blo		1b

	dsb
	bx		lr
	.size cp15_flush_dcache, . - cp15_flush_dcache
	.end
