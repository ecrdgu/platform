
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/sensors/qencoder.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "stm32_qencoder.h"
#include "stm32f103_minimum.h"

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_qencoder_initialize
 *
 * Description:
 *   All STM32 architectures must provide the following interface to work with
 *   examples/qencoder.
 *
 ************************************************************************************/

int stm32_qencoder_initialize(FAR const char *devpath, int timer)
{
  int ret;

  /* Initialize a quadrature encoder interface. */

  sninfo("Initializing the quadrature encoder using TIM%d\n", timer);
  ret = stm32_qeinitialize(devpath, timer);
  if (ret < 0)
    {
      snerr("ERROR: stm32_qeinitialize failed: %d\n", ret);
    }

  return ret;
}
