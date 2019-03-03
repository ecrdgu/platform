
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include "cache.h"
#include "mmu.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mmu_l1_setentry
 *
 * Description:
 *   Set a one level 1 translation table entry.  Only a single L1 page table
 *   is supported.
 *
 * Input Parameters:
 *   paddr - The physical address to be mapped.  Must be aligned to a 1MB
 *     address boundary
 *   vaddr - The virtual address to be mapped.  Must be aligned to a 1MB
 *     address boundary
 *   mmuflags - The MMU flags to use in the mapping.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l1_setentry(uint32_t paddr, uint32_t vaddr, uint32_t mmuflags)
{
  uint32_t *l1table = (uint32_t *)PGTABLE_BASE_VADDR;
  uint32_t  index   = vaddr >> 20;

  /* Save the page table entry */

  l1table[index] = (paddr | mmuflags);

  /* Flush the data cache entry.  Make sure that the modified contents
   * of the page table are flushed into physical memory.
   */

  cp15_clean_dcache_bymva((uint32_t)&l1table[index]);

  /* Invalidate the TLB cache associated with virtual address range */

  mmu_invalidate_region(vaddr, 1024*1024);
}
#endif

/****************************************************************************
 * Name: mmu_l1_restore
 *
 * Description:
 *   Restore one L1 table entry previously returned by mmu_l1_getentry() (or
 *   any other encoded L1 page table value).
 *
 * Input Parameters:
 *   vaddr - A virtual address to be mapped
 *   l1entry - The value to write into the page table entry
 *
 ****************************************************************************/

#if !defined(CONFIG_ARCH_ROMPGTABLE) && defined(CONFIG_ARCH_ADDRENV)
void mmu_l1_restore(uintptr_t vaddr, uint32_t l1entry)
{
  uint32_t *l1table = (uint32_t *)PGTABLE_BASE_VADDR;
  uint32_t  index   = vaddr >> 20;

  /* Set the encoded page table entry */

  l1table[index] = l1entry;

  /* Flush the data cache entry.  Make sure that the modified contents
   * of the page table are flushed into physical memory.
   */

  cp15_clean_dcache_bymva((uint32_t)&l1table[index]);

  /* Invalidate the TLB cache associated with virtual address range */

  mmu_invalidate_region(vaddr & PMD_PTE_PADDR_MASK, 1024*1024);
}
#endif

/****************************************************************************
 * Name: mmu_l2_setentry
 *
 * Description:
 *   Set one small (4096B) entry in a level2 translation table.
 *
 * Input Parameters:
 *   l2vaddr - the virtual address of the beginning of the L2 translation
 *     table.
 *   paddr - The physical address to be mapped.  Must be aligned to a 4KB
 *     address boundary
 *   vaddr - The virtual address to be mapped.  Must be aligned to a 4KB
 *     address boundary
 *   mmuflags - The MMU flags to use in the mapping.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l2_setentry(uint32_t l2vaddr, uint32_t paddr, uint32_t vaddr,
                     uint32_t mmuflags)
{
  uint32_t *l2table  = (uint32_t *)l2vaddr;
  uint32_t  index;

  /* The table divides a 1Mb address space up into 256 entries, each
   * corresponding to 4Kb of address space.  The page table index is
   * related to the offset from the beginning of 1Mb region.
   */

  index = (vaddr & 0x000ff000) >> 12;

  /* Save the table entry */

  l2table[index] = (paddr | mmuflags);

  /* Flush the data cache entry.  Make sure that the modified contents
   * of the page table are flushed into physical memory.
   */

  cp15_clean_dcache_bymva((uint32_t)&l2table[index]);

  /* Invalidate the TLB cache associated with virtual address range */

  cp15_invalidate_tlb_bymva(vaddr);
}
#endif

/****************************************************************************
 * Name: mmu_l1_map_region
 *
 * Description:
 *   Set multiple level 1 translation table entries in order to map a
 *   region of memory.
 *
 * Input Parameters:
 *   mapping - Describes the mapping to be performed.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l1_map_region(const struct section_mapping_s *mapping)
{
  uint32_t paddr    = mapping->physbase;
  uint32_t vaddr    = mapping->virtbase;
  uint32_t mmuflags = mapping->mmuflags;
  int i;

  /* Loop, writing each mapping into the L1 page table */

  for (i = 0; i < mapping->nsections; i++)
    {
      mmu_l1_setentry(paddr, vaddr, mmuflags);
      paddr += SECTION_SIZE;
      vaddr += SECTION_SIZE;
    }
}
#endif

/****************************************************************************
 * Name: mmu_invalidate_region
 *
 * Description:
 *   Invalidate TLBs for a range of addresses (all 4KB aligned).
 *
 * Input Parameters:
 *   vaddr - The beginning of the region to invalidate.
 *   size  - The size of the region in bytes to be invalidated.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_invalidate_region(uint32_t vstart, size_t size)
{
  uint32_t vaddr = vstart & 0xfffff000;
  uint32_t vend  = vaddr + size;

  /* Loop, invalidating regions */

  while (vaddr < vend)
    {
      cp15_invalidate_tlb_bymva(vaddr);
      vaddr += 4096;
    }
}
#endif
