
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/spi/spi.h>
#include <ecr/sensors/max6675.h>

#include "stm32.h"
#include "stm32_spi.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_SPI) && defined(CONFIG_SENSORS_MAX6675) && defined(CONFIG_STM32_SPI1)

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define MAX6675_SPI_PORTNO 1   /* On SPI1 */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_max6675initialize
 *
 * Description:
 *   Initialize and register the MAX6675 Temperature Sensor driver.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/temp0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_max6675initialize(FAR const char *devpath)
{
  FAR struct spi_dev_s *spi;
  int ret;

  spi = stm32_spibus_initialize(MAX6675_SPI_PORTNO);

  if (!spi)
    {
      return -ENODEV;
    }

  /* Then register the barometer sensor */

  ret = max6675_register(devpath, spi);
  if (ret < 0)
    {
      snerr("ERROR: Error registering MAX6675\n");
    }

  return ret;
}

#endif /* CONFIG_SPI && CONFIG_SENSORS_MAX6675 */

