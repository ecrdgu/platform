
#ifndef __ARCH_ARM_SRC_STM32_STM32_CCM_H
#define __ARCH_ARM_SRC_STM32_STM32_CCM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <ecr/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Assume that we can support the CCM heap */

#define HAVE_CCM_HEAP 1

/* Only the STM32 F2, F3, and F4 have CCM memory */

#if defined(CONFIG_STM32_STM32F30XX)
#  define CCM_START 0x10000000
#  define CCM_END   0x10002000
#elif defined(CONFIG_STM32_STM32F20XX) || defined(CONFIG_STM32_STM32F4XXX) || \
      defined(CONFIG_STM32_STM32F33XX)
#  define CCM_START 0x10000000
#  define CCM_END   0x10010000
#else
#  undef HAVE_CCM_HEAP
#endif

/* In order to use the CCM heap, it had to have been excluded from the main
 * heap.
 */

#ifndef CONFIG_STM32_CCMEXCLUDE
#  undef HAVE_CCM_HEAP
#endif

/* Can we support the CCM heap? */

#ifdef HAVE_CCM_HEAP

/* ccm_initialize must be called early in initialization in order to
 * initialize the CCM heap.
 */

#define ccm_initialize() \
  mm_initialize(&g_ccm_heap, (FAR void *)CCM_START, CCM_END-CCM_START)

/* The ccm_addregion interface could be used if, for example, you want to
 * add some other memory region to the CCM heap.  I don't really know why
 * you might want to do that, but the functionality is essentially free.
 */

#define ccm_addregion(b,s) mm_addregion(&g_ccm_heap, b, s);

/* Then, once g_ccm_heap has been setup by ccm_initialize(), these memory
 * allocators can be used just like the standard memory allocators.
 */

#define ccm_malloc(s)      mm_malloc(&g_ccm_heap, s)
#define ccm_zalloc(s)      mm_zalloc(&g_ccm_heap, s)
#define ccm_calloc(n,s)    mm_calloc(&g_ccm_heap, n,s)
#define ccm_free(p)        mm_free(&g_ccm_heap, p)
#define ccm_realloc(p,s)   mm_realloc(&g_ccm_heap, p, s)
#define ccm_memalign(a,s)  mm_memalign(&g_ccm_heap, a, s)

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

EXTERN struct mm_heap_s g_ccm_heap;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

/****************************************************************************
 * Name: ccm_procfs_register
 *
 * Description:
 *   Register the CCM procfs file system entry
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_CCM_PROCFS
int ccm_procfs_register(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* HAVE_CCM_HEAP */
#endif /* __ARCH_ARM_SRC_STM32_STM32_CCM_H */
