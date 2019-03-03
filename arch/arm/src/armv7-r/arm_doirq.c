
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <ecr/irq.h>
#include <ecr/arch.h>
#include <assert.h>

#include <ecr/board.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"

#include "group/group.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

uint32_t *arm_doirq(int irq, uint32_t *regs)
{
  board_autoled_on(LED_INIRQ);

#ifdef CONFIG_SUPPRESS_INTERRUPTS
  PANIC();
#else
  /* Nested interrupts are not supported */

  DEBUGASSERT(CURRENT_REGS == NULL);

  /* Current regs non-zero indicates that we are processing an interrupt;
   * CURRENT_REGS is also used to manage interrupt level context switches.
   */

  CURRENT_REGS = regs;

  /* Deliver the IRQ */

  irq_dispatch(irq, regs);

#ifdef CONFIG_ARCH_FPU
  /* Check for a context switch.  If a context switch occurred, then
   * CURRENT_REGS will have a different value than it did on entry.  If an
   * interrupt level context switch has occurred, then restore the floating
   * point state and the establish the correct address environment before
   * returning from the interrupt.
   */

  if (regs != CURRENT_REGS)
    {
      /* Restore floating point registers */

      up_restorefpu((uint32_t *)CURRENT_REGS);
    }
#endif

  /* Set CURRENT_REGS to NULL to indicate that we are no longer in an
   * interrupt handler.
   */

  regs         = (uint32_t *)CURRENT_REGS;
  CURRENT_REGS = NULL;

  board_autoled_off(LED_INIRQ);
#endif
  return regs;
}
