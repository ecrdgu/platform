
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include "up_vfork.h"

	MODULE vfork
	SECTION .text:CODE:NOROOT(2)

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Public Symbols
 ************************************************************************************/

	PUBLIC	vfork
	EXTERN	up_vfork

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: vfork
 *
 * Description:
 *   The vfork() function has the same effect as fork(), except that the behavior is
 *   undefined if the process created by vfork() either modifies any data other than
 *   a variable of type pid_t used to store the return value from vfork(), or returns
 *   from the function in which vfork() was called, or calls any other function before
 *   successfully calling _exit() or one of the exec family of functions.
 *
 *   This thin layer implements vfork by simply calling up_vfork() with the vfork()
 *   context as an argument.  The overall sequence is:
 *
 *   1) User code calls vfork().  vfork() collects context information and
 *      transfers control up up_vfork().
 *   2) up_vfork()and calls task_vforksetup().
 *   3) task_vforksetup() allocates and configures the child task's TCB.  This
 *      consists of:
 *      - Allocation of the child task's TCB.
 *      - Initialization of file descriptors and streams
 *      - Configuration of environment variables
 *      - Setup the intput parameters for the task.
 *      - Initialization of the TCB (including call to up_initial_state()
 *   4) up_vfork() provides any additional operating context. up_vfork must:
 *      - Allocate and initialize the stack
 *      - Initialize special values in any CPU registers that were not
 *        already configured by up_initial_state()
 *   5) up_vfork() then calls task_vforkstart()
 *   6) task_vforkstart() then executes the child thread.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Upon successful completion, vfork() returns 0 to the child process and returns
 *   the process ID of the child process to the parent process. Otherwise, -1 is
 *   returned to the parent, no child process is created, and errno is set to
 *   indicate the error.
 *
 ************************************************************************************/

	THUMB

vfork:
	/* Create a stack frame */

	mov		r0, sp					/* Save the value of the stack on entry */
	sub		sp, sp, #VFORK_SIZEOF	/* Allocate the structure on the stack */

	/* CPU registers */
	/* Save the volatile registers */

	str		r4, [sp, #VFORK_R4_OFFSET]
	str		r5, [sp, #VFORK_R5_OFFSET]
	str		r6, [sp, #VFORK_R6_OFFSET]
	str		r7, [sp, #VFORK_R7_OFFSET]
	str		r8, [sp, #VFORK_R8_OFFSET]
	str		r9, [sp, #VFORK_R9_OFFSET]
	str		r10, [sp, #VFORK_R10_OFFSET]

	/* Save the frame pointer, stack pointer, and return address */

	str		r11, [sp, #VFORK_FP_OFFSET] /* fp not defined. use r11 */
	str		r0, [sp, #VFORK_SP_OFFSET]
	str		lr, [sp, #VFORK_LR_OFFSET]

	/* Floating point registers (not yet) */

	/* Then, call up_vfork(), passing it a pointer to the stack structure */

	mov		r0, sp
	bl		up_vfork

	/* Release the stack data and return the value returned by up_vfork */

	ldr		lr, [sp, #VFORK_LR_OFFSET]
	add		sp, sp, #VFORK_SIZEOF
	bx		lr

	END
