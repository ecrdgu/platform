
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
 * Name: modifyreg8
 *
 * Description:
 *   Atomically modify the specified bits in a memory mapped register
 *
 ****************************************************************************/

void modifyreg8(unsigned int addr, uint8_t clearbits, uint8_t setbits)
{
  irqstate_t flags;
  uint8_t    regval;

  flags   = spin_lock_irqsave();
  regval  = getreg8(addr);
  regval &= ~clearbits;
  regval |= setbits;
  putreg8(regval, addr);
  spin_unlock_irqrestore(flags);
}
