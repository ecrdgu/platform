
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>
#include <ecr/arch.h>
#include <ecr/irq.h>

#include "up_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_interrupt_context
 *
 * Description: Return true is we are currently executing in
 * the interrupt handler context.
 ****************************************************************************/

bool up_interrupt_context(void)
{
  return CURRENT_REGS != NULL;
}
