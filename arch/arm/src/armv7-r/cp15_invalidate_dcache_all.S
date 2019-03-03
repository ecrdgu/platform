
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cp15.h"

	.file	"cp15_invalidate_dcache_all.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	cp15_invalidate_dcache_all

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: cp15_invalidate_dcache_all
 *
 * Description:
 *   Invalidate the entire contents of D cache.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

	.globl	cp15_invalidate_dcache_all
	.type	cp15_invalidate_dcache_all, function

cp15_invalidate_dcache_all:

	mrc		CP15_CCSIDR(r0)			/* Read the Cache Size Identification Register */
	ldr		r3, =0xffff				/* Isolate the NumSets field (bits 13-27) */
	and		r0, r3, r0, lsr #13		/* r0=NumSets (number of sets - 1) */

	mov		r1, #0					/* r1 = way loop counter */
way_loop:

	mov		r3, #0					/* r3 = set loop counter */
set_loop:
	mov		r2, r1, lsl #30			/* r2 = way loop counter << 30 */
	orr		r2, r3, lsl #5			/* r2 = set/way cache operation format */
	mcr		CP15_DCISW(r2)			/* Data Cache Invalidate by Set/Way */
	add		r3, r3, #1				/* Increment set counter */
	cmp		r0, r3					/* Last set? */
	bne		set_loop				/* Keep looping if not */

	add		r1, r1, #1				/* Increment the way counter */
	cmp		r1, #4					/* Last way? (four ways assumed) */
	bne		way_loop				/* Keep looping if not */

	dsb
	bx		lr
	.size cp15_invalidate_dcache_all, . - cp15_invalidate_dcache_all
	.end
