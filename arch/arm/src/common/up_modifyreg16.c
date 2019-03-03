
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
 * Name: modifyreg16
 *
 * Description:
 *   Atomically modify the specified bits in a memory mapped register
 *
 ****************************************************************************/

void modifyreg16(unsigned int addr, uint16_t clearbits, uint16_t setbits)
{
  irqstate_t flags;
  uint16_t   regval;

  flags   = spin_lock_irqsave();
  regval  = getreg16(addr);
  regval &= ~clearbits;
  regval |= setbits;
  putreg16(regval, addr);
  spin_unlock_irqrestore(flags);
}
