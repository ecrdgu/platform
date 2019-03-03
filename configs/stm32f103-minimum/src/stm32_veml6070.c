
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/spi/spi.h>
#include <ecr/sensors/veml6070.h>

#include "stm32.h"
#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_I2C) && defined(CONFIG_SENSORS_VEML6070)

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define VEML6070_I2C_PORTNO 1   /* On I2C1 */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_veml6070initialize
 *
 * Description:
 *   Initialize and register the VEML6070 UV-A Light sensor.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/uvlight0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_veml6070initialize(FAR const char *devpath)
{
  FAR struct i2c_master_s *i2c;
  int ret;

  sninfo("Initializing VEML6070!\n");

  /* Initialize I2C */

  i2c = stm32_i2cbus_initialize(VEML6070_I2C_PORTNO);

  if (!i2c)
    {
      return -ENODEV;
    }

  /* Then register the light sensor */

  ret = veml6070_register(devpath, i2c, VEML6070_I2C_DATA_LSB_CMD_ADDR);
  if (ret < 0)
    {
      snerr("ERROR: Error registering BM180\n");
    }

  return ret;
}

#endif /* CONFIG_I2C && CONFIG_SENSORS_VEML6070 && CONFIG_STM32_I2C1 */
