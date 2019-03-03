
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

	.syntax	unified
	.thumb
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

	add		r2, r1, #(4*REG_R0)
	mov		r3, #1
	str		r3, [r2, #0]
	bx		lr						/* "normal" return with r0=0 or
									 * context switch with r0=1 */
	.size	up_saveusercontext, .-up_saveusercontext
	.end
