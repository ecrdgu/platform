
#ifndef __ARCH_ARM_SRC_ARMV7_A_ADDRENV_H
#define __ARCH_ARM_SRC_ARMV7_A_ADDRENV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>

#ifdef CONFIG_ARCH_ADDRENV

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Aligned size of the kernel stack */

#ifdef CONFIG_ARCH_KERNEL_STACK
#  define ARCH_KERNEL_STACKSIZE ((CONFIG_ARCH_KERNEL_STACKSIZE + 7) & ~7)
#endif

/* Using a 4KiB page size, each 1MiB section maps to a PTE containing
 * 256*2KiB entries
 */

#define ENTRIES_PER_L2TABLE 256

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

#endif /* __ASSEMBLY__ */

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: arm_addrenv_create_region
 *
 * Description:
 *   Create one memory region.
 *
 * Returned Value:
 *   On success, the number of pages allocated is returned.  Otherwise, a
 *   negated errno value is returned.
 *
 ****************************************************************************/

int arm_addrenv_create_region(FAR uintptr_t **list, unsigned int listlen,
                              uintptr_t vaddr, size_t regionsize,
                              uint32_t mmuflags);

/****************************************************************************
 * Name: arm_addrenv_destroy_region
 *
 * Description:
 *   Destroy one memory region.
 *
 ****************************************************************************/

void arm_addrenv_destroy_region(FAR uintptr_t **list, unsigned int listlen,
                                uintptr_t vaddr, bool keep);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif  /* CONFIG_ARCH_ADDRENV */
#endif  /* __ARCH_ARM_SRC_ARMV7_A_ADDRENV_H */
