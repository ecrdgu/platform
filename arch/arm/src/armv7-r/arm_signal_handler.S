
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <arch/syscall.h>

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)

/****************************************************************************
 * File info
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
	.cpu	cortex-r4f
#else
	.cpu	cortex-r4
#endif
	.syntax	unified
	.file	"arm_signal_handler.S"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: up_signal_handler
 *
 * Description:
 *   This function is the user-space, signal handler trampoline function.  It
 *   is called from up_signal_dispatch() in user-mode.
 *
 *     R0-R3, R11 - volatile registers need not be preserved.
 *     R4-R10 - static registers must be preserved
 *     R12-R14 - LR and SP must be preserved
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

	.globl	up_signal_handler
	.type	up_signal_handler, function
up_signal_handler:

	/* Save some register */

	push	{lr}			/* Save LR on the stack */

	/* Call the signal handler */

	mov		ip, r0			/* IP=sighand */
	mov		r0, r1			/* R0=signo */
	mov		r1, r2			/* R1=info */
	mov		r2, r3			/* R2=ucontext */
	blx		ip				/* Call the signal handler */

	/* Restore the registers */

	pop		{r2}			/* Recover LR in R2 */
	mov		lr, r2			/* Restore LR */

	/* Execute the SYS_signal_handler_return SVCall (will not return) */

	mov		r0, #SYS_signal_handler_return
	svc		0
	nop

	.size	up_signal_handler, .-up_signal_handler
	.end

#endif /* CONFIG_BUILD_PROTECTED && !__KERNEL__ */
