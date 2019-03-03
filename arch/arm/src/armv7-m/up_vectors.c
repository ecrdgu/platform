
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define IDLE_STACK      ((unsigned)&_ebss+CONFIG_IDLETHREAD_STACKSIZE-4)

#ifndef ARMV7M_PERIPHERAL_INTERRUPTS
#  error ARMV7M_PERIPHERAL_INTERRUPTS must be defined to the number of I/O interrupts to be supported
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Chip-specific entrypoint */

extern void __start(void);

/* Common exception entrypoint */

extern void exception_common(void);

/************************************************************************************
 * Public data
 ************************************************************************************/

/* Provided by the linker script to indicate the end of the BSS */

extern char _ebss;

/* The v7m vector table consists of an array of function pointers, with the first
 * slot (vector zero) used to hold the initial stack pointer.
 *
 * As all exceptions (interrupts) are routed via exception_common, we just need to
 * fill this array with pointers to it.
 *
 * Note that the [ ... ] designated initialiser is a GCC extension.
 */

unsigned _vectors[] __attribute__((section(".vectors"))) =
{
  /* Initial stack */

  IDLE_STACK,

  /* Reset exception handler */

  (unsigned)&__start,

  /* Vectors 2 - n point directly at the generic handler */

  [2 ... (15 + ARMV7M_PERIPHERAL_INTERRUPTS)] = (unsigned)&exception_common
};
