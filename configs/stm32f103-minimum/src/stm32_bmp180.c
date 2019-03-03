
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/spi/spi.h>
#include <ecr/sensors/bmp180.h>

#include "stm32.h"
#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_I2C) && defined(CONFIG_SENSORS_BMP180)

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define MPL115A_I2C_PORTNO 1   /* On I2C1 */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_bmp180initialize
 *
 * Description:
 *   Initialize and register the MPL115A Pressure Sensor driver.
 *
 * Input parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/press0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_bmp180initialize(FAR const char *devpath)
{
  FAR struct i2c_master_s *i2c;
  int ret;

  sninfo("Initializing BMP180!\n");

  /* Initialize I2C */

  i2c = stm32_i2cbus_initialize(BMP180_I2C_PORTNO);

  if (!i2c)
    {
      return -ENODEV;
    }

  /* Then register the barometer sensor */

  ret = bmp180_register(devpath, i2c);
  if (ret < 0)
    {
      snerr("ERROR: Error registering BM180\n");
    }

  return ret;
}

#endif /* CONFIG_I2C && CONFIG_SENSORS_MPL115A && CONFIG_STM32_I2C1 */
