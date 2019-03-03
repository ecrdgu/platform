
#ifndef __ARCH_ARM_SRC_SAMV7_SAM_START_H
#define __ARCH_ARM_SRC_SAMV7_SAM_START_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <ecr/compiler.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#include "up_internal.h"
#include "chip.h"

/************************************************************************************
 * Public Data
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* g_idle_topstack: _sbss is the start of the BSS region as defined by the linker
 * script. _ebss lies at the end of the BSS region. The idle task stack starts at
 * the end of BSS and is of size CONFIG_IDLETHREAD_STACKSIZE.  The IDLE thread is
 * the thread that the system boots on and, eventually, becomes the IDLE, do
 * nothing task that runs only when there is nothing else to run.  The heap
 * continues from there until the end of memory.  g_idle_topstack is a read-only
 * variable the provides this computed address.
 */

EXTERN const uintptr_t g_idle_topstack;

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_boardinitialize
 *
 * Description:
 *   All SAMV7 architectures must provide the following entry point.  This entry
 *   point is called early in the initialization -- after clocking and memory have
 *   been configured but before caches have been enabled and before any devices have
 *   been initialized.
 *
 ************************************************************************************/

void stm32_boardinitialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_SAMV7_SAM_START_H */
