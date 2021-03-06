
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cp15.h"

	.file	"cp15_clean_dcache.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	cp15_clean_dcache

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: cp15_clean_dcache
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
 ****************************************************************************/

	.globl	cp15_clean_dcache
	.type	cp15_clean_dcache, function

cp15_clean_dcache:

	mrc		CP15_CTR(r3)				/* Read the Cache Type Register */
	lsr		r3, r3, #16				/* Isolate the DMinLine field */
	and		r3, r3, #0xf
	mov		r2, #4
	mov		r2, r2, lsl r3			/* Get the cache line size in bytes */

	sub		r3, r2, #1				/* R3=Cache line size mask */
	bic		r0, r0, r3				/* R0=aligned start address */

	/* Loop, cleaning each cache line by writing its contents to memory */

1:
	mcr		CP15_DCCMVAC(r0)		/* Clean data cache line to PoC by VA */
	add		r0, r0, r2				/* R12=Next cache line */
	cmp		r0, r1					/* Loop until all cache lines have been cleaned */
	blo		1b

	dsb
	bx		lr
	.size cp15_clean_dcache, . - cp15_clean_dcache
	.end
