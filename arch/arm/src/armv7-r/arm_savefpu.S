
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <arch/irq.h>

#ifdef CONFIG_ARCH_FPU

	.file		"arm_savefpu.S"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Public Symbols
 ************************************************************************************/

	.globl		up_savefpu

/************************************************************************************
 * Public Functions
 ************************************************************************************/

	.text

/************************************************************************************
 * Name: up_savefpu
 *
 * Description:
 *   Given the pointer to a register save area (in R0), save the state of the
 *   floating point registers.
 *
 * C Function Prototype:
 *   void up_savefpu(uint32_t *regs);
 *
 * Input Parameters:
 *   regs - A pointer to the register save area in which to save the floating point
 *     registers
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

	.globl	up_savefpu
	.type	up_savefpu, function

up_savefpu:

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
	bx		lr

	.size	up_savefpu, .-up_savefpu
#endif /* CONFIG_ARCH_FPU */
	.end
