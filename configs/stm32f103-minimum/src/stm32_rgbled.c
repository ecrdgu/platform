
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <errno.h>
#include <debug.h>

#include <ecr/drivers/pwm.h>
#include <ecr/leds/rgbled.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "stm32_pwm.h"
#include "stm32f103_minimum.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Configuration ********************************************************************/

#define HAVE_RGBLED 1

#ifndef CONFIG_PWM
#  undef HAVE_RGBLED
#endif

#ifndef CONFIG_STM32_TIM1
#  undef HAVE_RGBLED
#endif

#ifndef CONFIG_STM32_TIM2
#  undef HAVE_RGBLED
#endif

#ifndef CONFIG_STM32_TIM4
#  undef HAVE_RGBLED
#endif

#ifndef CONFIG_STM32_TIM1_PWM
#  undef HAVE_RGBLED
#endif

#ifndef CONFIG_STM32_TIM2_PWM
#  undef HAVE_RGBLED
#endif

#ifndef CONFIG_STM32_TIM4_PWM
#  undef HAVE_RGBLED
#endif

#if CONFIG_STM32_TIM1_CHANNEL != RGBLED_RPWMCHANNEL
#  undef HAVE_PWM
#endif

#if CONFIG_STM32_TIM2_CHANNEL != RGBLED_GPWMCHANNEL
#  undef HAVE_PWM
#endif

#if CONFIG_STM32_TIM4_CHANNEL != RGBLED_BPWMCHANNEL
#  undef HAVE_PWM
#endif

#ifdef HAVE_RGBLED

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_rgbled_setup
 *
 * Description:
 *   Initial for support of a connected RGB LED using PWM.
 *
 ************************************************************************************/

int stm32_rgbled_setup(void)
{
  static bool initialized = false;
  struct pwm_lowerhalf_s    *ledr;
  struct pwm_lowerhalf_s    *ledg;
  struct pwm_lowerhalf_s    *ledb;
  struct pwm_info_s info;
  int ret;

  /* Have we already initialized? */

  if (!initialized)
    {
      /* Call stm32_pwminitialize() to get an instance of the PWM interface */

      ledr = stm32_pwminitialize(RGBLED_RPWMTIMER);
      if (!ledr)
        {
          lederr("ERROR: Failed to get the STM32 PWM lower half to LEDR\n");
          return -ENODEV;
        }

      /* Define frequency and duty cycle */

      info.frequency = 100;
      info.duty = 0;

      /* Initialize LED R */

      ledr->ops->setup(ledr);
      ledr->ops->start(ledr, &info);

      /* Call stm32_pwminitialize() to get an instance of the PWM interface */

      ledg = stm32_pwminitialize(RGBLED_GPWMTIMER);
      if (!ledg)
        {
          lederr("ERROR: Failed to get the STM32 PWM lower half to LEDG\n");
          return -ENODEV;
        }

      /* Initialize LED G */

      ledg->ops->setup(ledg);
      ledg->ops->start(ledg, &info);

      /* Call stm32_pwminitialize() to get an instance of the PWM interface */

      ledb = stm32_pwminitialize(RGBLED_BPWMTIMER);
      if (!ledb)
        {
          lederr("ERROR: Failed to get the STM32 PWM lower half to LEDB\n");
          return -ENODEV;
        }

      /* Initialize LED B */

      ledb->ops->setup(ledb);
      ledb->ops->start(ledb, &info);

      /* Register the RGB LED diver at "/dev/rgbled0" */

      ret = rgbled_register("/dev/rgbled0", ledr, ledg, ledb);
      if (ret < 0)
        {
          lederr("ERROR: rgbled_register failed: %d\n", ret);
          return ret;
        }

      /* Now we are initialized */

      initialized = true;
    }

  return OK;
}

#else
#  error "HAVE_RGBLED is undefined"
#endif /* HAVE_RGBLED */
