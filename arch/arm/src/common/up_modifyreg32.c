
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <debug.h>

#include <ecr/irq.h>
#include <ecr/arch.h>

#include "up_arch.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: modifyreg32
 *
 * Description:
 *   Atomically modify the specified bits in a memory mapped register
 *
 ****************************************************************************/

void modifyreg32(unsigned int addr, uint32_t clearbits, uint32_t setbits)
{
  irqstate_t flags;
  uint32_t   regval;

  flags   = spin_lock_irqsave();
  regval  = getreg32(addr);
  regval &= ~clearbits;
  regval |= setbits;
  putreg32(regval, addr);
  spin_unlock_irqrestore(flags);
}
