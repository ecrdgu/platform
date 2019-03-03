
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <arch/irq.h>

#include "nvic.h"
#include "svcall.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Public Symbols
 ************************************************************************************/

	.cpu	cortex-m0
	.file	"up_saveusercontext.S"

/************************************************************************************
 * Macros
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: up_saveusercontext
 *
 * Description:
 *   Save the current thread context.  Full prototype is:
 *
 *   int  up_saveusercontext(uint32_t *saveregs);
 *
 * Returned Value:
 *   0: Normal return
 *   1: Context switch return
 *
 ************************************************************************************/

	.text
	.align	2
	.code	16
	.thumb_func
	.globl	up_saveusercontext
	.type	up_saveusercontext, function
up_saveusercontext:

	/* Perform the System call with R0=0 and R1=regs */

	mov		r1, r0					/* R1: regs */
	mov		r0, #SYS_save_context	/* R0: save context (also return value) */
	svc		0						/* Force synchronous SVCall (or Hard Fault) */

	/* There are two return conditions.  On the first return, R0 (the
	 * return value will be zero.  On the second return we need to
	 * force R0 to be 1.
	 */

	mov		r3, #(4*REG_R0)			/* R3=Offset to R0 storage */
	add		r2, r1, r3				/* R2=Address of R0 storage */
	mov		r3, #1					/* R3=Return value of one */
	str		r3, [r2, #0]			/* Save return value */
	bx		lr						/* "normal" return with r0=0 or
									 * context switch with r0=1 */
	.size	up_saveusercontext, .-up_saveusercontext
	.end

