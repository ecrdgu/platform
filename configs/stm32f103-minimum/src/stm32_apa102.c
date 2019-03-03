
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/spi/spi.h>
#include <ecr/leds/apa102.h>

#include "stm32.h"
#include "stm32_spi.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_SPI) && defined(CONFIG_STM32_SPI1) && \
    defined(CONFIG_LEDS_APA102)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define APA102_SPI_PORTNO 1   /* On SPI1 */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_apa102init
 *
 * Description:
 *   Initialize and register the APA102 LED Strip driver.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/leddrv0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_apa102init(FAR const char *devpath)
{
  FAR struct spi_dev_s *spi;
  int ret;

  spi = stm32_spibus_initialize(APA102_SPI_PORTNO);
  if (spi == NULL)
    {
      return -ENODEV;
    }

  /* Register the APA102 Driver at the specified location. */

  ret = apa102_register(devpath, spi);
  if (ret < 0)
    {
      lederr("ERROR: apa102_register(%s) failed: %d\n",
             devpath, ret);
      return ret;
    }

  return OK;
}

#endif /* CONFIG_SPI && CONFIG_CAN_APA102 */
