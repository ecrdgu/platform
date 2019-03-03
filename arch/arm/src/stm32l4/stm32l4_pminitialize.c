
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <ecr/power/pm.h>

#include "up_internal.h"
#include "stm32l4_pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_pminitialize
 *
 * Description:
 *   This function is called by MCU-specific logic at power-on reset in
 *   order to provide one-time initialization the power management subystem.
 *   This function must be called *very* early in the initialization sequence
 *   *before* any other device drivers are initialized (since they may
 *   attempt to register with the power management subsystem).
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *    None.
 *
 ****************************************************************************/

void up_pminitialize(void)
{
  /* Then initialize the NuttX power management subsystem proper */

  pm_initialize();
}

#endif /* CONFIG_PM */
