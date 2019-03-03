
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include <ecr/board.h>

#include "up_arch.h"
#include "nvic.h"

/****************************************************************************
 * Public functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_systemreset
 *
 * Description:
 *   Internal, Cortex-M0 reset logic.
 *
 ****************************************************************************/

void up_systemreset(void)
{
  uint32_t regval;

  /* Set up for the system reset, retaining the priority group from the
   * the AIRCR register.
   */

  regval  = getreg32(NVIC_AIRCR) & NVIC_AIRCR_PRIGROUP_MASK;
  regval |= ((0x5fa << NVIC_AIRCR_VECTKEY_SHIFT) | NVIC_AIRCR_SYSRESETREQ);
  putreg32(regval, NVIC_AIRCR);

  /* Ensure completion of memory accesses */

  __asm volatile ("dsb");

  /* Wait for the reset */

  for (; ; );
}


/****************************************************************************
 * Name: board_reset
 *
 * Description:
 *   Reset board.  This function may or may not be supported by a
 *   particular board architecture.
 *
 * Input Parameters:
 *   status - Status information provided with the reset event.  This
 *     meaning of this status information is board-specific.  If not used by
 *     a board, the value zero may be provided in calls to board_reset.
 *
 * Returned Value:
 *   If this function returns, then it was not possible to power-off the
 *   board due to some constraints.  The return value int this case is a
 *   board-specific reason for the failure to shutdown.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARDCTL_RESET
int board_reset(int status)
{
  up_systemreset();
  return 0;
}
#endif
