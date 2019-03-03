
#ifndef __ARCH_ARM_SRC_ARMV6_M_EXC_RETURN_H
#define __ARCH_ARM_SRC_ARMV6_M_EXC_RETURN_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* The processor saves an EXC_RETURN value to the LR on exception entry. The
 * exception mechanism relies on this value to detect when the processor has
 * completed an exception handler.
 *
 * Bits [31:4] of an EXC_RETURN value are always 1.  When the processor loads a
 * value matching this pattern to the PC it detects that the operation is a not
 * a normal branch operation and instead, that the exception is complete.
 * Therefore, it starts the exception return sequence.
 *
 * Bits[3:0] of the EXC_RETURN value indicate the required return stack and eventual
 * processor mode.  The remaining bits of the EXC_RETURN value should be set to 1.
 */

/* EXC_RETURN_BASE: Bits that are always set in an EXC_RETURN value. */

#define EXC_RETURN_BASE          0xfffffff1

/* EXC_RETURN_PROCESS_STACK: The exception saved (and will restore) the hardware
 * context using the process stack pointer (if not set, the context was saved
 * using the main stack pointer)
 */

#define EXC_RETURN_PROCESS_BITNO (2)
#define EXC_RETURN_PROCESS_STACK (1 << EXC_RETURN_PROCESS_BITNO)

/* EXC_RETURN_THREAD_MODE: The exception will return to thread mode (if not set,
 * return stays in handler mode).
 */

#define EXC_RETURN_THREAD_BITNO  (3)
#define EXC_RETURN_THREAD_MODE   (1 << EXC_RETURN_THREAD_BITNO)

/* EXC_RETURN_HANDLER: Return to handler mode. Exception return gets state from
 * the main stack. Execution uses MSP after return.
 */

#define EXC_RETURN_HANDLER       0xfffffff1

/* EXC_RETURN_PRIVTHR: Return to privileged thread mode. Exception return gets
 * state from the main stack. Execution uses MSP after return.
 */

#define EXC_RETURN_PRIVTHR       0xfffffff9

/* EXC_RETURN_UNPRIVTHR: Return to unprivileged thread mode. Exception return gets
 * state from the process stack. Execution uses PSP after return.
 */

#define EXC_RETURN_UNPRIVTHR     0xfffffffd

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

#endif  /* __ARCH_ARM_SRC_ARMV6_M_EXC_RETURN_H */

