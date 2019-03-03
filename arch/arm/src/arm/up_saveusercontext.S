
/**************************************************************************
 * Included Files
 **************************************************************************/

#include <ecr/irq.h>
#include "up_internal.h"

/**************************************************************************
 * Pre-processor Definitions
 **************************************************************************/

/**************************************************************************
 * Private Types
 **************************************************************************/

/**************************************************************************
 * Private Function Prototypes
 **************************************************************************/

/**************************************************************************
 * Public Data
 **************************************************************************/

/**************************************************************************
 * Private Data
 **************************************************************************/

/**************************************************************************
 * Private Functions
 **************************************************************************/

/**************************************************************************
 * Public Functions
 **************************************************************************/

/**************************************************************************
 * Name: up_saveusercontext
 **************************************************************************/

	.text
	.globl	up_saveusercontext
	.type	up_saveusercontext, function
up_saveusercontext:
	/* On entry, a1 (r0) holds address of struct xcptcontext.
	 * Offset to the user region.
	 */

	/* Make sure that the return value will be non-zero (the
	 * value of the other volatile registers don't matter --
	 * r1-r3, ip).  This function is called throught the
	 * noraml C calling conventions and the values of these
	 * registers cannot be assumed at the point of setjmp
	 * return.
	 */

        mov	ip, #1
	str	ip, [r0, #(4*REG_R0)]

	/* Save the volatile registers (plus r12 which really
	 * doesn't need to be saved)
	 */

	add	r1, r0, #(4*REG_R4)
	stmia	r1, {r4-r14}

	/* Save the current cpsr */

	mrs	r2, cpsr		/* R3 = CPSR value */
	add	r1, r0, #(4*REG_CPSR)
	str	r2, [r1]

	/* Finally save the return address as the PC so that we
	 * return to the exit from this function.
	 */

        add	r1, r0, #(4*REG_PC)
	str	lr, [r1]

	/* Return 0 */

	mov	r0, #0		/* Return value == 0 */
	mov	pc, lr		/* Return */
	.size	up_saveusercontext, . - up_saveusercontext

