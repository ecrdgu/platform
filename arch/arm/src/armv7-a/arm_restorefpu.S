
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <arch/irq.h>

#ifdef CONFIG_ARCH_FPU

	.file		"arm_restorefpu.S"

/************************************************************************************
 * Public Symbols
 ************************************************************************************/

	.globl		up_restorefpu

/************************************************************************************
 * Public Functions
 ************************************************************************************/

	.text

/************************************************************************************
 * Name: up_restorefpu
 *
 * Description:
 *   Given the pointer to a register save area (in R0), restore the state of the
 *   floating point registers.
 *
 * C Function Prototype:
 *   void up_restorefpu(const uint32_t *regs);
 *
 * Input Parameters:
 *   regs - A pointer to the register save area containing the floating point
 *     registers.
 *
 * Returned Value:
 *   This function does not return anything explicitly.  However, it is called from
 *   interrupt level assembly logic that assumes that r0 is preserved.
 *
 ************************************************************************************/

	.globl	up_restorefpu
	.type	up_restorefpu, function

up_restorefpu:

	add		r1, r0, #(4*REG_S0)		/* R1=Address of FP register storage */

	/* Load all floating point registers.  Registers are loaded in numeric order,
	 * s0, s1, ... in increasing address order.
	 */

	vldmia	r1!, {s0-s31}			/* Restore the full FP context */

	/* Load the floating point control and status register.   At the end of the
	 * vstmia, r1 will point to the FPCSR storage location.
	 */

	ldr		r2, [r1], #4			/* Fetch the floating point control and status register */
	vmsr	fpscr, r2				/* Restore the FPCSR */
	bx		lr

	.size	up_restorefpu, .-up_restorefpu
#endif /* CONFIG_ARCH_FPU */
	.end

