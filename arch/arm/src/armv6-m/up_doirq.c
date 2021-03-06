
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <assert.h>

#include <ecr/irq.h>
#include <ecr/arch.h>
#include <ecr/board.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
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

uint32_t *up_doirq(int irq, uint32_t *regs)
{
  board_autoled_on(LED_INIRQ);
#ifdef CONFIG_SUPPRESS_INTERRUPTS
  PANIC();
#else
  uint32_t *savestate;

  /* Nested interrupts are not supported in this implementation.  If you want
   * to implement nested interrupts, you would have to (1) change the way that
   * CURRENT_REGS is handled and (2) the design associated with
   * CONFIG_ARCH_INTERRUPTSTACK.  The savestate variable will not work for
   * that purpose as implemented here because only the outermost nested
   * interrupt can result in a context switch (it can probably be deleted).
   */

  /* Current regs non-zero indicates that we are processing an interrupt;
   * CURRENT_REGS is also used to manage interrupt level context switches.
   */

  savestate    = (uint32_t *)CURRENT_REGS;
  CURRENT_REGS = regs;

  /* Acknowledge the interrupt */

  up_ack_irq(irq);

  /* Deliver the IRQ */

  irq_dispatch(irq, regs);

  /* If a context switch occurred while processing the interrupt then
   * CURRENT_REGS may have change value.  If we return any value different
   * from the input regs, then the lower level will know that a context
   * switch occurred during interrupt processing.
   */

  regs = (uint32_t *)CURRENT_REGS;

  /* Restore the previous value of CURRENT_REGS.  NULL would indicate that
   * we are no longer in an interrupt handler.  It will be non-NULL if we
   * are returning from a nested interrupt.
   */

  CURRENT_REGS = savestate;
#endif
  board_autoled_off(LED_INIRQ);
  return regs;
}
