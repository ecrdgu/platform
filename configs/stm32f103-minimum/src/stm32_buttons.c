
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <errno.h>

#include <ecr/arch.h>
#include <ecr/board.h>
#include <arch/board/board.h>

#include "stm32_gpio.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_ARCH_BUTTONS)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_BUTTONS) && !defined(CONFIG_ARCH_IRQBUTTONS)
#  error "The NuttX Buttons Driver depends on IRQ support to work!\n"
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Pin configuration for each STM32F3Discovery button.  This array is indexed
 * by the BUTTON_* definitions in board.h
 */

static const uint32_t g_buttons[NUM_BUTTONS] =
{
  GPIO_BTN_USER1, GPIO_BTN_USER2
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_button_initialize
 *
 * Description:
 *   board_button_initialize() must be called to initialize button resources.
 *   After that, board_buttons() may be called to collect the current state
 *   of all buttons or board_button_irq() may be called to register button
 *   interrupt handlers.
 *
 ****************************************************************************/

void board_button_initialize(void)
{
  int i;

  /* Configure the GPIO pins as inputs.  NOTE that EXTI interrupts are
   * configured for all pins.
   */

  for (i = 0; i < NUM_BUTTONS; i++)
    {
      stm32_configgpio(g_buttons[i]);
    }
}

/****************************************************************************
 * Name: board_buttons
 ****************************************************************************/

uint32_t board_buttons(void)
{
  uint32_t ret = 0;
  int i;

  /* Check that state of each key */

  for (i = 0; i < NUM_BUTTONS; i++)
    {
       /* A LOW value means that the key is pressed.
        */

       bool released = stm32_gpioread(g_buttons[i]);

       /* Accumulate the set of depressed (not released) keys */

       if (!released)
         {
            ret |= (1 << i);
         }
    }

  return ret;
}

/****************************************************************************
 * Button support.
 *
 * Description:
 *   board_button_initialize() must be called to initialize button resources.
 *   After that, board_buttons() may be called to collect the current state
 *   of all buttons or board_button_irq() may be called to register button
 *   interrupt handlers.
 *
 *   After board_button_initialize() has been called, board_buttons() may be
 *   called to collect the state of all buttons.  board_buttons() returns
 *   an 32-bit bit set with each bit associated with a button.  See the
 *   BUTTON_*_BIT definitions in board.h for the meaning of each bit.
 *
 *   board_button_irq() may be called to register an interrupt handler that
 *   will be called when a button is depressed or released.  The ID value is
 *   a button enumeration value that uniquely identifies a button resource.
 *   See the BUTTON_* definitions in board.h for the meaning of enumeration
 *   value.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_IRQBUTTONS
int board_button_irq(int id, xcpt_t irqhandler, FAR void *arg)
{
  int ret = -EINVAL;

  /* The following should be atomic */

  if (id >= MIN_IRQBUTTON && id <= MAX_IRQBUTTON)
    {
      ret = stm32_gpiosetevent(g_buttons[id], true, true, true, irqhandler,
                               arg);
    }

  return ret;
}
#endif

#endif /* CONFIG_ARCH_BUTTONS */
