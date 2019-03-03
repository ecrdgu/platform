
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <debug.h>

#include <ecr/sched.h>
#include <ecr/page.h>

#include "chip.h"
#include "pg_macros.h"
#include "up_internal.h"

#ifdef CONFIG_PAGING

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_va2pte
 *
 * Description:
 *  Convert a virtual address within the paged text region into a pointer to
 *  the corresponding page table entry.
 *
 * Input Parameters:
 *   vaddr - The virtual address within the paged text region.
 *
 * Returned Value:
 *   A pointer to  the corresponding page table entry.
 *
 * Assumptions:
 *   - This function is called from the normal tasking context (but with
 *     interrupts disabled).  The implementation must take whatever actions
 *     are necessary to assure that the operation is safe within this
 *     context.
 *
 ****************************************************************************/

uint32_t *arm_va2pte(uintptr_t vaddr)
{
  uint32_t L1;
  uint32_t *L2;
  unsigned int ndx;

  /* The virtual address is expected to lie in the paged text region */

  DEBUGASSERT(vaddr >= PG_PAGED_VBASE && vaddr < PG_PAGED_VEND);

  /* Get the L1 table entry associated with this virtual address */

  L1 = *(uint32_t *)PG_POOL_VA2L1VADDR(vaddr);

  /* Get the address of the L2 page table from the L1 entry */

  L2 = (uint32_t *)PG_POOL_L12VPTABLE(L1);

  /* Get the index into the L2 page table.  Each L1 entry maps
   * 256 x 4Kb or 1024 x 1Kb pages.
   */

  ndx = (vaddr & 0x000fffff) >> PAGESHIFT;

  /* Return true if this virtual address is mapped. */

  return &L2[ndx];
}

#endif /* CONFIG_PAGING */
