
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <arch/board/board.h>

#include "chip.h"
#include "stm32.h"
#include "stm32f103_minimum.h"

#ifndef CONFIG_ARCH_LEDS

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This array maps an LED number to GPIO pin configuration */

static const uint32_t g_ledcfg[BOARD_NLEDS] =
{
  GPIO_LED1,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_userled_initialize
 ****************************************************************************/

void board_userled_initialize(void)
{
  int i;

  /* Configure LED1-8 GPIOs for output */

  for (i = 0; i < BOARD_NLEDS; i++)
    {
      stm32_configgpio(g_ledcfg[i]);
    }
}

/****************************************************************************
 * Name: board_userled
 ****************************************************************************/

void board_userled(int led, bool ledon)
{
  if ((unsigned)led < BOARD_NLEDS)
    {
      stm32_gpiowrite(g_ledcfg[led], ledon);
    }
}

/****************************************************************************
 * Name: board_userled_all
 ****************************************************************************/

void board_userled_all(uint8_t ledset)
{
  int i;

  /* Configure LED1-8 GPIOs for output */

  for (i = 0; i < BOARD_NLEDS; i++)
    {
      stm32_gpiowrite(g_ledcfg[i], (ledset & (1 << i)) != 0);
    }
}

#endif /* !CONFIG_ARCH_LEDS */
