
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <time.h>
#include <debug.h>
#include <ecr/arch.h>
#include <arch/board/board.h>

#include "nvic.h"
#include "clock/clock.h"
#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Select MCU-specific settings
 *
 * The RCC feeds the external clock of the Cortex System Timer (SysTick) with
 * the AHB clock (HCLK) divided by 8. The SysTick can work either with this
 * clock or with the Cortex clock (HCLK), configurable in the SysTick control
 * and status register.
 *
 * The SysTick calibration value is fixed to 18750, which gives a reference
 * time base of 1 ms with the SysTick clock set to 18.75 MHz (HCLK/8, with
 * HCLK set to 150 MHz).
 */

#define STM32_SYSTICK_CLOCK  (STM32_BOARD_HCLK / 8)

/* The desired timer interrupt frequency is provided by the definition
 * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
 * system clock ticks per second.  That value is a user configurable setting
 * that defaults to 100 (100 ticks per second = 10 MS interval).
 *
 * For example, suppose HCLK = 216 MHz and CLK_TCK = 100, then:
 *
 *   STM32_SYSTICK_CLOCK = 216 MHz / 8 = 27 MHz
 *   SYSTICK_RELOAD      = (27,000,000 / 100) - 1 = 269,999
 */

#define SYSTICK_RELOAD ((STM32_SYSTICK_CLOCK / CLK_TCK) - 1)

/* The size of the reload field is 24 bits.  Verify that the reload value
 * will fit in the reload register.
 */

#if SYSTICK_RELOAD > 0x00ffffff
#  error SYSTICK_RELOAD exceeds the range of the RELOAD register
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  stm32_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/

static int stm32_timerisr(int irq, uint32_t *regs, void *arg)
{
  /* Process timer interrupt */

  sched_process_timer();
  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  arm_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize the timer
 *   interrupt.
 *
 ****************************************************************************/

void arm_timer_initialize(void)
{
  uint32_t regval;

  /* Configure SysTick to interrupt at the requested rate */

  putreg32(SYSTICK_RELOAD, NVIC_SYSTICK_RELOAD);
  putreg32(0, NVIC_SYSTICK_CURRENT);

  /* Attach the timer interrupt vector */

  (void)irq_attach(STM32_IRQ_SYSTICK, (xcpt_t)stm32_timerisr, NULL);

  /* Enable SysTick interrupts:
   *
   *   NVIC_SYSTICK_CTRL_CLKSOURCE=0 : Use the implementation defined clock
   *                                   source which, for the STM32F7, will be
   *                                   HCLK/8
   *   NVIC_SYSTICK_CTRL_TICKINT=1   : Generate interrupts
   *   NVIC_SYSTICK_CTRL_ENABLE      : Enable the counter
   */

  regval = (NVIC_SYSTICK_CTRL_TICKINT | NVIC_SYSTICK_CTRL_ENABLE);
  putreg32(regval, NVIC_SYSTICK_CTRL);

  /* And enable the timer interrupt */

  up_enable_irq(STM32_IRQ_SYSTICK);
}
