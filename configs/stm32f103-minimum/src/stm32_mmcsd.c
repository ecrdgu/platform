
/*****************************************************************************
 * Included Files
 ****************************************************************************/

#include <debug.h>
#include <ecr/config.h>
#include <ecr/mmcsd.h>
#include <ecr/spi/spi.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#include "stm32.h"
#include "stm32f103_minimum.h"
#include "stm32_spi.h"

/*****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_STM32_SPI1
#  error "SD driver requires CONFIG_STM32_SPI1 to be enabled"
#endif

#ifdef CONFIG_DISABLE_MOUNTPOINT
#  error "SD driver requires CONFIG_DISABLE_MOUNTPOINT to be disabled"
#endif

/*****************************************************************************
 * Private Definitions
 ****************************************************************************/

static const int SD_SPI_PORT = 1; /* SD is connected to SPI1 port */
static const int SD_SLOT_NO  = 0; /* There is only one SD slot */

/*****************************************************************************
 * Private Functions
 ****************************************************************************/

/* NOTE:  We are using a SDCard adapter/module without Card Detect pin!
 * Then we don't need to Card Detect callback here.
 */

/*****************************************************************************
 * Public Functions
 ****************************************************************************/

/*****************************************************************************
 * Name: stm32_spi1register
 *
 * Description:
 *   Registers media change callback
 ****************************************************************************/

int stm32_spi1register(struct spi_dev_s *dev, spi_mediachange_t callback,
                       void *arg)
{
  spiinfo("INFO: Registering spi1 device\n");
  return OK;
}

/*****************************************************************************
 * Name: stm32_mmcsd_initialize
 *
 * Description:
 *   Initialize SPI-based SD card and card detect thread.
 ****************************************************************************/

int stm32_mmcsd_initialize(int minor)
{
  struct spi_dev_s *spi;
  int rv;

  mcinfo("INFO: Initializing mmcsd card\n");

  spi = stm32_spibus_initialize(SD_SPI_PORT);
  if (spi == NULL)
    {
      mcerr("ERROR: Failed to initialize SPI port %d\n", SD_SPI_PORT);
      return -ENODEV;
    }

  rv = mmcsd_spislotinitialize(minor, SD_SLOT_NO, spi);
  if (rv < 0)
    {
      mcerr("ERROR: Failed to bind SPI port %d to SD slot %d\n",
            SD_SPI_PORT, SD_SLOT_NO);
      return rv;
    }

  spiinfo("INFO: mmcsd card has been initialized successfully\n");
  return OK;
}
