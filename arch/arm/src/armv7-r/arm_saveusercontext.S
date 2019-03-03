
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/irq.h>
#include "up_internal.h"

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	up_saveusercontext

#ifdef CONFIG_ARCH_FPU
	.cpu	cortex-r4f
#else
	.cpu	cortex-r4
#endif
	.syntax	unified
	.file	"arm_saveusercontext.S"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: up_saveusercontext
 ****************************************************************************/

	.globl	up_saveusercontext
	.type	up_saveusercontext, function

up_saveusercontext:

	/* On entry, a1 (r0) holds address of struct xcptcontext */

	/* Make sure that the return value will be non-zero (the value of the
	 * other volatile registers don't matter -- r1-r3, ip).  This function
	 * is called through the normal C calling conventions and the values of
	 * these registers cannot be assumed at the point of setjmp return.
	 */

	mov		ip, #1
	str		ip, [r0, #(4*REG_R0)]

	/* Save the volatile registers (plus r12 which really
	 * doesn't need to be saved)
	 */

	add		r1, r0, #(4*REG_R4)
	stmia	r1, {r4-r14}

	/* Save the current cpsr */

	mrs		r2, cpsr		/* R3 = CPSR value */
	add		r1, r0, #(4*REG_CPSR)
	str		r2, [r1]

	/* Save the return address as the PC so that we return to the exit from
	 * this function.
	 */

	add		r1, r0, #(4*REG_PC)
	str		lr, [r1]

	/* Save the floating point registers.
	 * REVISIT:  Not all of the floating point registers need to be saved.
	 * Some are volatile and need not be preserved across functions calls.
	 * But right now, I can't find the definitive list of the volatile
	 * floating point registers.
	 */

#ifdef CONFIG_ARCH_FPU
	add		r1, r0, #(4*REG_S0)		/* R1=Address of FP register storage */

	/* Store all floating point registers.  Registers are stored in numeric order,
	 * s0, s1, ... in increasing address order.
	 */

	vstmia	r1!, {s0-s31}			/* Save the full FP context */

	/* Store the floating point control and status register.  At the end of the
	 * vstmia, r1 will point to the FPCSR storage location.
	 */

	vmrs	r2, fpscr				/* Fetch the FPCSR */
	str		r2, [r1], #4			/* Save the floating point control and status register */
#endif

	/* Return 0 now indicating that this return is not a context switch */

	mov		r0, #0			/* Return value == 0 */
	mov		pc, lr			/* Return */
	.size	up_saveusercontext, . - up_saveusercontext
	.end
