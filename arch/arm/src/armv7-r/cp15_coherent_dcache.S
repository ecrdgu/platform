
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cp15.h"

	.file	"cp15_coherent_dcache.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	cp15_coherent_dcache

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: cp15_coherent_dcache
 *
 * Description:
 *   Ensure that the I and D caches are coherent within specified region
 *   by cleaning the D cache (i.e., flushing the D cache contents to memory
 *   and invalidating the I cache. This is typically used when code has been
 *   written to a memory region, and will be executed.
 *
 * Input Parameters:
 *   start - virtual start address of region
 *   end   - virtual end address of region + 1
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

	.globl	cp15_coherent_dcache
	.type	cp15_coherent_dcache, function

cp15_coherent_dcache:
	mrc		CP15_CTR(r3)			/* Read the Cache Type Register */
	lsr		r3, r3, #16				/* Isolate the DMinLine field */
	and		r3, r3, #0xf
	mov		r2, #4
	mov		r2, r2, lsl r3			/* Get the cache line size in bytes */

	sub		r3, r2, #1				/* R3=Cache line size mask */
	bic		r12, r0, r3				/* R12=aligned start address */

	/* Loop, flushing each D cache line to memory */
1:
	mcr		CP15_DCCMVAU(r12)		/* Clean data or unified cache line by VA to PoU */
	add		r12, r12, r2			/* R12=Next cache line */
	cmp		r12, r1					/* Loop until all cache lines have been cleaned */
	blo		1b

	dsb

	mrc		CP15_CTR(r3)			/* Read the Cache Type Register */
	and		r3, r3, #0xf			/* Isolate the IminLine field */
	mov		r2, #4
	mov		r2, r2, lsl r3			/* Get the cache line size in bytes */

	sub		r3, r2, #1				/* R3=Cache line size mask */
	bic		r12, r0, r3				/* R12=aligned start address */

	/* Loop, invalidating each I cache line to memory */
1:
	mcr		CP15_ICIMVAU(r12)		/* Invalidate instruction cache by VA to PoU */
	add		r12, r12, r2			/* R12=Next cache line */
	cmp		r12, r1					/* Loop until all cache lines have been invalidated */
	blo		1b

	mov		r0, #0
#ifdef CONFIG_SMP
	mcr		CP15_BPIALLIS(r0)		/* Invalidate entire branch predictor array Inner Shareable */
#endif
	mcr		CP15_BPIALL(r0)			/* Invalidate all branch predictors */

	dsb
	isb
	bx		lr
	.size cp15_coherent_dcache, . - cp15_coherent_dcache
	.end
