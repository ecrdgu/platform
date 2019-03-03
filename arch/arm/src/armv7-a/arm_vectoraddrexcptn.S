
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/irq.h>
#include "up_arch.h"

	.file	"arm_vectoraddrexcptn.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	arm_vectoraddrexcption

/****************************************************************************
 * Assembly Macros
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 *  Name: arm_vectoraddrexcption
 *
 * Description:
 *   Shouldn't happen.  This exception handler is in a separate file from
 *   other vector handlers because some processors (e.g., Cortex-A5) do not
 *   support the Address Exception vector.
 *
 ****************************************************************************/

	.globl	arm_vectoraddrexcptn
	.type	arm_vectoraddrexcptn, %function
arm_vectoraddrexcptn:
	b		arm_vectoraddrexcptn
	.size	arm_vectoraddrexcptn, . - arm_vectoraddrexcptn
	.end
