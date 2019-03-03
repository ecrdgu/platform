
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <errno.h>
#include <debug.h>

#include <ecr/board.h>
#include <ecr/drivers/pwm.h>

#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "stm32_pwm.h"
#include "stm32f103_minimum.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Configuration *******************************************************************/
/* PWM
 *
 * The stm32f103-minimum has no real on-board PWM devices, but the board can be
 * configured to output a pulse train using TIM4 CH2.  This pin is used by FSMC
 * is connect to CN5 just for this purpose:
 *
 * PB0 ADC12_IN8/TIM3_CH3
 *
 */

#define HAVE_PWM 1

#ifndef CONFIG_PWM
#  undef HAVE_PWM
#endif

#ifndef CONFIG_STM32_TIM3
#  undef HAVE_PWM
#endif

#ifndef CONFIG_STM32_TIM3_PWM
#  undef HAVE_PWM
#endif

#if !defined(CONFIG_STM32_TIM3_CHANNEL) || CONFIG_STM32_TIM3_CHANNEL != STM32F103MINIMUM_PWMCHANNEL
#  undef HAVE_PWM
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_pwm_setup
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ************************************************************************************/

int stm32_pwm_setup(void)
{
#ifdef HAVE_PWM
  static bool initialized = false;
  struct pwm_lowerhalf_s *pwm;
  int ret;

  /* Have we already initialized? */

  if (!initialized)
    {
      /* Call stm32_pwminitialize() to get an instance of the PWM interface */

      pwm = stm32_pwminitialize(STM32F103MINIMUM_PWMTIMER);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the STM32 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm0" */

      ret = pwm_register("/dev/pwm0", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }

      /* Now we are initialized */

      initialized = true;
    }

  return OK;
#else
  return -ENODEV;
#endif
}

