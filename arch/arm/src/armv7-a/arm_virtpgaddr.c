
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "pgalloc.h"

#if defined(CONFIG_MM_PGALLOC) && defined(CONFIG_ARCH_PGPOOL_MAPPING)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_virtpgaddr
 *
 * Description:
 *   Check if the physical address lies in the page pool and, if so
 *   get the mapping to the virtual address in the user data area.
 *
 ****************************************************************************/

uintptr_t arm_virtpgaddr(uintptr_t paddr)
{
  /* REVISIT: Not implemented correctly.  The reverse lookup from physical
   * to virtual.  This will return a kernel accessible virtual address, but
   * not an address usable by the user code.
   *
   * The correct solutions is complex and, perhaps, will never be needed.
   */

  if (paddr >= CONFIG_ARCH_PGPOOL_PBASE && paddr < CONFIG_ARCH_PGPOOL_PEND)
    {
      return paddr - CONFIG_ARCH_PGPOOL_PBASE + CONFIG_ARCH_PGPOOL_VBASE;
    }

  return 0;
}

#endif /* CONFIG_MM_PGALLOC && CONFIG_ARCH_PGPOOL_MAPPING */
