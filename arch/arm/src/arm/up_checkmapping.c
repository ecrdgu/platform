
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <debug.h>

#include <ecr/sched.h>
#include <ecr/page.h>

#include "up_internal.h"

#ifdef CONFIG_PAGING

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_checkmapping()
 *
 * Description:
 *  The function up_checkmapping() returns an indication if the page fill
 *  still needs to performed or not. In certain conditions, the page fault
 *  may occur on several threads and be queued multiple times. This function
 *  will prevent the same page from be filled multiple times.
 *
 * Input Parameters:
 *   tcb - A reference to the task control block of the task that we believe
 *         needs to have a page fill.  Architecture-specific logic can
 *         retrieve page fault information from the architecture-specific
 *         context information in this TCB and can consult processor resources
 *         (page tables or TLBs or ???) to determine if the fill still needs
 *         to be performed or not.
 *
 * Returned Value:
 *   This function will return true if the mapping is in place and false
 *   if the mapping is still needed.  Errors encountered should be
 *   interpreted as fatal.
 *
 * Assumptions:
 *   - This function is called from the normal tasking context (but with
 *     interrupts disabled).  The implementation must take whatever actions
 *     are necessary to assure that the operation is safe within this
 *     context.
 *
 ****************************************************************************/

bool up_checkmapping(FAR struct tcb_s *tcb)
{
  uintptr_t vaddr;
  uint32_t *pte;

  /* Since interrupts are disabled, we don't need to anything special. */

  DEBUGASSERT(tcb);

  /* Get the virtual address that caused the fault */

  vaddr = tcb->xcp.far;
  DEBUGASSERT(vaddr >= PG_PAGED_VBASE && vaddr < PG_PAGED_VEND);

  /* Get the PTE associated with this virtual address */

  pte = up_va2pte(vaddr);

  /* Return true if this virtual address is mapped. */

  return (*pte != 0);
}

#endif /* CONFIG_PAGING */
