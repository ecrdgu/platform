
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

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
 * Name: up_pginitialize()
 *
 * Description:
 *  Initialize the MMU for on-demand paging support..
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.  This function will crash if any errors are detected during MMU
 *   initialization
 *
 * Assumptions:
 *   - Called early in the platform initialization sequence so that no special
 *     concurrency protection is required.
 *
 ****************************************************************************/

void up_pginitialize(void)
{
  /* None needed at present.  This file is just retained in case the need
   * arises in the future.  Nothing calls up_pginitialize() now.  If needed,
   * if should be called early in arm_boot.c to assure that all paging is
   * ready.
   */
}

#endif /* CONFIG_PAGING */
