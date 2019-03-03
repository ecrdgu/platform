
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
	.file	"up_fullcontextrestore.S"

/************************************************************************************
 * Macros
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: up_fullcontextrestore
 *
 * Description:
 *   Restore the current thread context.  Full prototype is:
 *
 *   void up_fullcontextrestore(uint32_t *restoreregs) noreturn_function;
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

	.thumb_func
	.globl	up_fullcontextrestore
	.type	up_fullcontextrestore, function
up_fullcontextrestore:

	/* Perform the System call with R0=1 and R1=regs */

	mov		r1, r0						/* R1: regs */
	mov		r0, #SYS_restore_context	/* R0: restore context */
	svc		0							/* Force synchronous SVCall (or Hard Fault) */

	/* This call should not return */

	bx		lr							/* Unnecessary ... will not return */
	.size	up_fullcontextrestore, .-up_fullcontextrestore
	.end

