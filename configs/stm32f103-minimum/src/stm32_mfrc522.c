
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/spi/spi.h>
#include <ecr/contactless/mfrc522.h>

#include "stm32.h"
#include "stm32_spi.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_SPI) && defined(CONFIG_STM32_SPI1) && defined(CONFIG_CL_MFRC522)

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define MFRC522_SPI_PORTNO 1   /* On SPI1 */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_mfrc522initialize
 *
 * Description:
 *   Initialize and register the MFRC522 RFID driver.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/rfid0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_mfrc522initialize(FAR const char *devpath)
{
  FAR struct spi_dev_s *spi;
  int ret;

  spi = stm32_spibus_initialize(MFRC522_SPI_PORTNO);

  if (!spi)
    {
      return -ENODEV;
    }

  /* Then register the MFRC522 */

  ret = mfrc522_register(devpath, spi);
  if (ret < 0)
    {
      snerr("ERROR: Error registering MFRC522\n");
    }

  return ret;
}

#endif /* CONFIG_SPI && CONFIG_MFRC522 */
