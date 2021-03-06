
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include "cp15.h"

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl	arm_fpuconfig

#ifdef CONFIG_ARCH_FPU
	.cpu	cortex-r4f
#else
	.cpu	cortex-r4
#endif
	.syntax	unified
	.file	"arm_fpuconfig.S"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: sam_fpuconfig
 *
 * Description:
 *   Configure the FPU.  Enables access to CP10 and CP11
 *
 ****************************************************************************/

	.globl	arm_fpuconfig
	.type	arm_fpuconfig, %function

arm_fpuconfig:

	/* Enable access to CP10 and CP11 in CP15.CACR */

	mrc		CP15_CPACR(r0)
	orr		r0, r0, #0xf00000
	mcr		CP15_CPACR(r0)

	/* Enable access to CP10 and CP11 in CP15.NSACR */
	/* REVISIT: Do we need to do this? */

	/* Set FPEXC.EN (B30) */

	fmrx	r0, fpexc
	orr		r0, r0, #0x40000000
	fmxr	fpexc, r0
	bx		lr
	.size	arm_fpuconfig, . - arm_fpuconfig
	.end
