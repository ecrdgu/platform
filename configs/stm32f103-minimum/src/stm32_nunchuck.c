
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>

#include <ecr/irq.h>
#include <ecr/arch.h>
#include <ecr/fs/fs.h>
#include <ecr/input/nunchuck.h>

#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NUNCHUCK_I2C_PORTNO 1   /* On I2C1 */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nunchuck_initialize
 *
 * Description:
 *   Initialize and register the Nunchuck joystick driver
 *
 ****************************************************************************/

int nunchuck_initialize(FAR char *devname)
{
  FAR struct i2c_master_s *i2c;
  int ret;

  iinfo("Initializing Wii Nunchuck!\n");

  /* Initialize I2C */

  i2c = stm32_i2cbus_initialize(NUNCHUCK_I2C_PORTNO);
  if (i2c == NULL)
    {
      return -ENODEV;
    }

  /* Register the joystick device as /dev/nunchuck0 */

  iinfo("Initialize joystick driver: %s\n", devname);

  ret = nunchuck_register(devname, i2c);
  if (ret < 0)
    {
      ierr("ERROR: nunchuck_register failed: %d\n", ret);
    }

  return ret;
}
