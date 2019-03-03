
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_SPI_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_SPI_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include "chip.h"
#include "chip/stm32l4_spi.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Public Data
 ************************************************************************************/

struct spi_dev_s;

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32l4_spibus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus
 *
 * Input Parameters:
 *   bus number (for hardware that has mutiple SPI interfaces)
 *
 * Returned Value:
 *   Valid SPI device structure reference on succcess; a NULL on failure
 *
 ************************************************************************************/

FAR struct spi_dev_s *stm32l4_spibus_initialize(int bus);

/************************************************************************************
 * Name:  stm32l4_spi1/2/...select and stm32l4_spi1/2/...status
 *
 * Description:
 *   The external functions, stm32l4_spi1/2/...select, stm32l4_spi1/2/...status, and
 *   stm32l4_spi1/2/...cmddata must be provided by board-specific logic.  These are
 *   implementations of the select, status, and cmddata methods of the SPI interface
 *   defined by struct spi_ops_s (see include/ecr/spi/spi.h). All other methods
 *   (including stm32l4_spibus_initialize()) are provided by common STM32 logic.  To use this
 *   common SPI logic on your board:
 *
 *   1. Provide logic in stm32l4_board_initialize() to configure SPI chip select
 *      pins.
 *   2. Provide stm32l4_spi1/2/...select() and stm32l4_spi1/2/...status() functions in your
 *      board-specific logic.  These functions will perform chip selection and
 *      status operations using GPIOs in the way your board is configured.
 *   3. If CONFIG_SPI_CMDDATA is defined in your NuttX configuration file, then
 *      provide stm32l4_spi1/2/...cmddata() functions in your board-specific logic.
 *      These functions will perform cmd/data selection operations using GPIOs in the
 *      way your board is configured.
 *   4. Add a calls to stm32l4_spibus_initialize() in your low level application
 *      initialization logic
 *   5. The handle returned by stm32l4_spibus_initialize() may then be used to bind the
 *      SPI driver to higher level logic (e.g., calling
 *      mmcsd_spislotinitialize(), for example, will bind the SPI driver to
 *      the SPI MMC/SD driver).
 *
 ************************************************************************************/

#ifdef CONFIG_STM32L4_SPI1
void stm32l4_spi1select(FAR struct spi_dev_s *dev, uint32_t devid, bool selected);
uint8_t stm32l4_spi1status(FAR struct spi_dev_s *dev, uint32_t devid);
int stm32l4_spi1cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

#ifdef CONFIG_STM32L4_SPI2
void stm32l4_spi2select(FAR struct spi_dev_s *dev, uint32_t devid, bool selected);
uint8_t stm32l4_spi2status(FAR struct spi_dev_s *dev, uint32_t devid);
int stm32l4_spi2cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

#ifdef CONFIG_STM32L4_SPI3
void stm32l4_spi3select(FAR struct spi_dev_s *dev, uint32_t devid, bool selected);
uint8_t stm32l4_spi3status(FAR struct spi_dev_s *dev, uint32_t devid);
int stm32l4_spi3cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

/************************************************************************************
 * Name: stm32l4_spi1/2/...register
 *
 * Description:
 *   If the board supports a card detect callback to inform the SPI-based MMC/SD
 *   driver when an SD card is inserted or removed, then CONFIG_SPI_CALLBACK should
 *   be defined and the following function(s) must be implemented.  These functions
 *   implements the registercallback method of the SPI interface (see
 *   include/ecr/spi/spi.h for details)
 *
 * Input Parameters:
 *   dev -      Device-specific state data
 *   callback - The function to call on the media change
 *   arg -      A caller provided value to return with the callback
 *
 * Returned Value:
 *   0 on success; negated errno on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_SPI_CALLBACK
#ifdef CONFIG_STM32L4_SPI1
int stm32l4_spi1register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif

#ifdef CONFIG_STM32L4_SPI2
int stm32l4_spi2register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif

#ifdef CONFIG_STM32L4_SPI3
int stm32l4_spi3register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_SPI_H */
