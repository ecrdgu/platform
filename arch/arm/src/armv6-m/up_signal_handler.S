
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <arch/syscall.h>

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)

/****************************************************************************
 * File info
 ****************************************************************************/

	.cpu	cortex-m0
	.file	"up_signal_handler.S"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_signal_handler
 *
 * Description:
 *   This function is the user-space, signal handler trampoline function.  It
 *   is called from up_signal_dispatch() in user-mode.
 *
 * Input Parameters:
 *   R0 = sighand
 *     The address user-space signal handling function
 *   R1-R3 = signo, info, and ucontext
 *     Standard arguments to be passed to the signal handling function.
 *
 * Returned Value:
 *   None.  This function does not return in the normal sense.  It returns
 *   via the SYS_signal_handler_return (see svcall.h)
 *
 ****************************************************************************/

	.text
	.align	2
	.code	16
	.thumb_func
	.globl	up_signal_handler
	.type	up_signal_handler, function
up_signal_handler:

	/* Save some register */

	push	{r4, r5			/* Save R4 and R5 on the stack */
	mov		r5, lr			/* Save LR in R5 */

	/* Call the signal handler */

	mov		r4, r0			/* R4=sighand */
	mov		r0, r1			/* R0=signo */
	mov		r1, r2			/* R1=info */
	mov		r2, r3			/* R2=ucontext */
	blx		r4				/* Call the signal handler */

	/* Restore the registers */

	mov		lr, r5			/* Restore LR */
	pop		{r4, r5}		/* Restore R4 and R5 */

	/* Execute the SYS_signal_handler_return SVCall (will not return) */

	mov		r0, #SYS_signal_handler_return
	svc		0
	nop

	.size	up_signal_handler, .-up_signal_handler
	.end

#endif /* CONFIG_BUILD_PROTECTED && !__KERNEL__ */
