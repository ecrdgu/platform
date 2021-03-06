
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

	.file	"arm_fetchadd.S"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: up_fetchadd32
 *
 * Description:
 *   Perform an atomic fetch add operation on the provided 32-bit value.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   addr  - The address of 32-bit value to be incremented.
 *   value - The 32-bit addend
 *
 * Returned Value:
 *   The incremented value (volatile!)
 *
 ****************************************************************************/

	.globl	up_fetchadd32
	.type	up_fetchadd32, %function

up_fetchadd32:

1:
	ldrex	r2, [r0]			/* Fetch the value to be incremented */
	add		r2, r2, r1			/* Add the addend */

	strex	r3, r2, [r0]		/* Attempt to save the result */
	teq		r3, #0				/* r2 will be 1 is strex failed */
	bne		1b					/* Failed to lock... try again */

	mov		r0, r2				/* Return the incremented value */
	bx		lr					/* Successful! */
	.size	up_fetchadd32, . - up_fetchadd32

/****************************************************************************
 * Name: up_fetchsub32
 *
 * Description:
 *   Perform an atomic fetch subtract operation on the provided 32-bit value.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   addr  - The address of 32-bit value to be decremented.
 *   value - The 32-bit subtrahend
 *
 * Returned Value:
 *   The decremented value (volatile!)
 *
 ****************************************************************************/

	.globl	up_fetchsub32
	.type	up_fetchsub32, %function

up_fetchsub32:

1:
	ldrex	r2, [r0]			/* Fetch the value to be decremented */
	sub		r2, r2, r1			/* Subtract the subtrahend */

	strex	r3, r2, [r0]		/* Attempt to save the result */
	teq		r3, #0				/* r2 will be 1 is strex failed */
	bne		1b					/* Failed to lock... try again */

	mov		r0, r2				/* Return the decremented value */
	bx		lr					/* Successful! */
	.size	up_fetchsub32, . - up_fetchsub32

/****************************************************************************
 * Name: up_fetchadd16
 *
 * Description:
 *   Perform an atomic fetch add operation on the provided 16-bit value.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   addr  - The address of 16-bit value to be incremented.
 *   value - The 16-bit addend
 *
 * Returned Value:
 *   The incremented value (volatile!)
 *
 ****************************************************************************/

	.globl	up_fetchadd16
	.type	up_fetchadd16, %function

up_fetchadd16:

1:
	ldrexh	r2, [r0]			/* Fetch the value to be incremented */
	add		r2, r2, r1			/* Add the addend */

	strexh	r3, r2, [r0]		/* Attempt to save the result */
	teq		r3, #0				/* r2 will be 1 is strexh failed */
	bne		1b					/* Failed to lock... try again */

	mov		r0, r2				/* Return the incremented value */
	bx		lr					/* Successful! */
	.size	up_fetchadd16, . - up_fetchadd16

/****************************************************************************
 * Name: up_fetchsub16
 *
 * Description:
 *   Perform an atomic fetch subtract operation on the provided 16-bit value.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   addr  - The address of 16-bit value to be decremented.
 *   value - The 16-bit subtrahend
 *
 * Returned Value:
 *   The decremented value (volatile!)
 *
 ****************************************************************************/

	.globl	up_fetchsub16
	.type	up_fetchsub16, %function

up_fetchsub16:

1:
	ldrexh	r2, [r0]			/* Fetch the value to be decremented */
	sub		r2, r2, r1			/* Subtract the subtrahend */

	/* Attempt to save the decremented value */

	strexh	r3, r2, [r0]		/* Attempt to save the result */
	teq		r3, #0				/* r2 will be 1 is strexh failed */
	bne		1b					/* Failed to lock... try again */

	mov		r0, r2				/* Return the decremented value */
	bx		lr					/* Successful! */
	.size	up_fetchsub16, . - up_fetchsub16

/****************************************************************************
 * Name: up_fetchadd8
 *
 * Description:
 *   Perform an atomic fetch add operation on the provided 8-bit value.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   addr  - The address of 8-bit value to be incremented.
 *   value - The 8-bit addend
 *
 * Returned Value:
 *   The incremented value (volatile!)
 *
 ****************************************************************************/

	.globl	up_fetchadd8
	.type	up_fetchadd8, %function

up_fetchadd8:

1:
	ldrexb	r2, [r0]			/* Fetch the value to be incremented */
	add		r2, r2, r1			/* Add the addend */

	strexb	r3, r2, [r0]		/* Attempt to save the result */
	teq		r3, #0				/* r2 will be 1 is strexb failed */
	bne		1b					/* Failed to lock... try again */

	mov		r0, r2				/* Return the incremented value */
	bx		lr					/* Successful! */
	.size	up_fetchadd8, . - up_fetchadd8

/****************************************************************************
 * Name: up_fetchsub8
 *
 * Description:
 *   Perform an atomic fetch subtract operation on the provided 8-bit value.
 *
 *   This function must be provided via the architecture-specific logic.
 *
 * Input Parameters:
 *   addr  - The address of 8-bit value to be decremented.
 *   value - The 8-bit subtrahend
 *
 * Returned Value:
 *   The decremented value (volatile!)
 *
 ****************************************************************************/

	.globl	up_fetchsub8
	.type	up_fetchsub8, %function

up_fetchsub8:

1:
	ldrexb	r2, [r0]			/* Fetch the value to be decremented */
	sub		r2, r2, r1			/* Subtract the subtrahend */

	strexb	r3, r2, [r0]		/* Attempt to save the result */
	teq		r3, #0				/* r2 will be 1 is strexb failed */
	bne		1b					/* Failed to lock... try again */

	mov		r0, r2				/* Return the decremented value */
	bx		lr					/* Successful! */
	.size	up_fetchsub8, . - up_fetchsub8
	.end
