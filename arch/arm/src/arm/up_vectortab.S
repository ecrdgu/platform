
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Assembly Macros
 ****************************************************************************/

/****************************************************************************
 * Name: _vector_start
 *
 * Description:
 *   Vector initialization block
 ****************************************************************************/

	.globl	_vector_start

/* These will be relocated to VECTOR_BASE. */

_vector_start:
	ldr	pc, .Lresethandler		/* 0x00: Reset */
	ldr	pc, .Lundefinedhandler		/* 0x04: Undefined instruction */
	ldr	pc, .Lswihandler		/* 0x08: Software interrupt */
	ldr	pc, .Lprefetchaborthandler	/* 0x0c: Prefetch abort */
	ldr	pc, .Ldataaborthandler		/* 0x10: Data abort */
	ldr	pc, .Laddrexcptnhandler		/* 0x14: Address exception */
	ldr	pc, .Lirqhandler		/* 0x18: IRQ */
	ldr	pc, .Lfiqhandler		/* 0x1c: FIQ */

	.globl   __start
	.globl	up_vectorundefinsn
	.globl	up_vectorswi
	.globl	up_vectorprefetch
	.globl	up_vectordata
	.globl	up_vectoraddrexcptn
	.globl	up_vectorirq
	.globl	up_vectorfiq

.Lresethandler:
	.long   __start
.Lundefinedhandler:
	.long	up_vectorundefinsn
.Lswihandler:
	.long	up_vectorswi
.Lprefetchaborthandler:
	.long	up_vectorprefetch
.Ldataaborthandler:
	.long	up_vectordata
.Laddrexcptnhandler:
	.long	up_vectoraddrexcptn
.Lirqhandler:
	.long	up_vectorirq
.Lfiqhandler:
	.long	up_vectorfiq

	.globl	_vector_end
_vector_end:
	.end
