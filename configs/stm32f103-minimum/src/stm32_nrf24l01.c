
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>
#include <errno.h>

#include <ecr/spi/spi.h>
#include <ecr/wireless/nrf24l01.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "chip.h"
#include "stm32.h"

#include "stm32f103_minimum.h"

#ifdef CONFIG_WL_NRF24L01

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

static int stm32tiny_wl_irq_attach(xcpt_t isr, FAR void *arg);
static void stm32tiny_wl_chip_enable(bool enable);

/************************************************************************************
 * Private Data
 ************************************************************************************/

static FAR struct nrf24l01_config_s nrf_cfg =
{
  .irqattach = stm32tiny_wl_irq_attach,
  .chipenable = stm32tiny_wl_chip_enable,
};

static xcpt_t g_isr;
static FAR void *g_arg;

/************************************************************************************
 * Private Functions
 ************************************************************************************/

static int stm32tiny_wl_irq_attach(xcpt_t isr, FAR void *arg)
{
  wlinfo("Attach IRQ\n");
  g_isr = isr;
  g_arg = arg;
  (void)stm32_gpiosetevent(GPIO_NRF24L01_IRQ, false, true, false, g_isr, g_arg);
  return OK;
}

static void stm32tiny_wl_chip_enable(bool enable)
{
  wlinfo("CE:%d\n", enable);
  stm32_gpiowrite(GPIO_NRF24L01_CE, enable);
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_wlinitialize
 *
 * Description:
 *   Initialize the NRF24L01 wireless module
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

void stm32_wlinitialize(void)
{
#ifndef CONFIG_STM32_SPI1
# error "STM32_SPI1 is required to support nRF24L01 module on this board"
#endif

  FAR struct spi_dev_s *spidev;
  int result;

  /* Setup CE & IRQ line IOs */

  stm32_configgpio(GPIO_NRF24L01_CE);
  stm32_configgpio(GPIO_NRF24L01_IRQ);

  /* Init SPI bus */

  spidev = stm32_spibus_initialize(1);
  if (!spidev)
    {
      wlerr("ERROR: Failed to initialize SPI bus\n");
      return;
    }

  result = nrf24l01_register(spidev, &nrf_cfg);
  if (result != OK)
    {
      wlerr("ERROR: Failed to register initialize SPI bus\n");
      return;
    }
}

#endif /* CONFIG_WL_NRF24L01 */
