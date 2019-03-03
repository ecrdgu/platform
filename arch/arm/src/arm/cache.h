
#ifndef __ARCH_ARM_SRC_ARM_CACHE_H
#define __ARCH_ARM_SRC_ARM_CACHE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Defintiions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void         cp15_flush_idcache(uint32_t start, uint32_t end);
#if 0 /* Not used */
void         cp15_invalidate_idcache(void);
void         cp15_invalidate_icache(void);
#endif
void         cp15_invalidate_dcache(uint32_t start, uint32_t end);
#if 0 /* Not used */
void         cp15_invalidate_dcache_all(void);
void         cp15_prefetch_icacheline(unsigned int value);
void         cp15_testcleaninvalidate_dcache(void);
void         cp15_drain_writebuffer(void);

unsigned int cp15_read_dcachelockdown(void);
void         cp15_write_dcachelockdown(unsigned int value);
unsigned int cp15_read_icachelockdown(void);
void         cp15_write_icachelockdown(unsigned int value);
#endif

#endif /* __ARCH_ARM_SRC_ARM_CACHE_H */

