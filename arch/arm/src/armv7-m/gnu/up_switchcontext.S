
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
	.file	"up_switchcontext.S"

/************************************************************************************
 * Macros
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: up_switchcontext
 *
 * Description:
 *   Save the current thread context and restore the specified context.
 *   Full prototype is:
 *
 *   void up_switchcontext(uint32_t *saveregs, uint32_t *restoreregs);
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

	.thumb_func
	.globl	up_switchcontext
	.type	up_switchcontext, function
up_switchcontext:

	/* Perform the System call with R0=1, R1=saveregs, R2=restoreregs */

	mov		r2, r1					/* R2: restoreregs */
	mov		r1, r0					/* R1: saveregs */
	mov		r0, #SYS_switch_context	/* R0: context switch */
	svc		0						/* Force synchronous SVCall (or Hard Fault) */

	/* We will get here only after the rerturn from the context switch */

	bx		lr
	.size	up_switchcontext, .-up_switchcontext
	.end

