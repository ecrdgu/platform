
#ifndef __ARCH_ARM_SRC_ARMV7_A_SMP_H
#define __ARCH_ARM_SRC_ARMV7_A_SMP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#ifdef CONFIG_SMP

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* ARM requires at least a 4-byte stack alignment.  For use with EABI and
 * floating point, the stack must be aligned to 8-byte addresses.  We will
 * always use the EABI stack alignment
 */

#define SMP_STACK_ALIGNMENT  8
#define SMP_STACK_MASK       7
#define SMP_STACK_SIZE       ((CONFIG_SMP_IDLETHREAD_STACKSIZE + 7) & ~7)
#define SMP_STACK_WORDS      (SMP_STACK_SIZE >> 2)
#define SMP_STACK_TOP        (SMP_STACK_SIZE - 8)

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#if CONFIG_SMP_NCPUS > 1
extern uint32_t g_cpu1_idlestack[SMP_STACK_WORDS];
#if CONFIG_SMP_NCPUS > 2
extern uint32_t g_cpu2_idlestack[SMP_STACK_WORDS];
#if CONFIG_SMP_NCPUS > 3
extern uint32_t g_cpu3_idlestack[SMP_STACK_WORDS];
#if CONFIG_SMP_NCPUS > 4
#  error This logic needs to extended for CONFIG_SMP_NCPUS > 4
#endif /* CONFIG_SMP_NCPUS > 4 */
#endif /* CONFIG_SMP_NCPUS > 3 */
#endif /* CONFIG_SMP_NCPUS > 2 */
#endif /* CONFIG_SMP_NCPUS > 1 */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: __cpu[n]_start
 *
 * Description:
 *   Boot functions for each CPU (other than CPU0).  These functions set up
 *   the ARM operating mode, the initial stack, and configure co-processor
 *   registers.  At the end of the boot, arm_cpu_boot() is called.
 *
 *   These functions are provided by the common ARMv7-A logic.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Do not return.
 *
 ****************************************************************************/

#if CONFIG_SMP_NCPUS > 1
void __cpu1_start(void);
#endif

#if CONFIG_SMP_NCPUS > 2
void __cpu2_start(void);
#endif

#if CONFIG_SMP_NCPUS > 3
void __cpu3_start(void);
#endif

#if CONFIG_SMP_NCPUS > 4
#  error This logic needs to extended for CONFIG_SMP_NCPUS > 4
#endif

/****************************************************************************
 * Name: arm_cpu_boot
 *
 * Description:
 *   Continues the C-level initialization started by the assembly language
 *   __cpu[n]_start function.  At a minimum, this function needs to initialize
 *   interrupt handling and, perhaps, wait on WFI for arm_cpu_start() to
 *   issue an SGI.
 *
 *   This function must be provided by the each ARMv7-A MCU and implement
 *   MCU-specific initialization logic.
 *
 * Input Parameters:
 *   cpu - The CPU index.  This is the same value that would be obtained by
 *      calling up_cpu_index();
 *
 * Returned Value:
 *   Does not return.
 *
 ****************************************************************************/

void arm_cpu_boot(int cpu);

#endif  /* __ASSEMBLY__ */
#endif  /* CONFIG_SMP */
#endif  /* __ARCH_ARM_SRC_ARMV7_A_SMP_H */
