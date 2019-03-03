
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/drivers/pwm.h>
#include <ecr/timers/oneshot.h>
#include <ecr/audio/tone.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "stm32_pwm.h"
#include "stm32f103_minimum.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Configuration ********************************************************************/

#define HAVE_TONE 1

/* TIMx used to generate PWM signal to Buzzer/Speaker */

#define TONE_PWM_TIMER 2

/* TIMx used to generate oneshot */

#define ONESHOT_TIMER 3

/* Oneshot timer resolution in microseconds */

#define OST_RES        10

#ifndef CONFIG_PWM
#  undef HAVE_TONE
#endif

#ifndef CONFIG_STM32_TIM2
#  undef HAVE_TONE
#endif

#ifndef CONFIG_STM32_TIM2_PWM
#  undef HAVE_TONE
#endif

#ifdef HAVE_TONE

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_tone_setup
 *
 * Description:
 *   Configure and initialize the tone generator.
 *
 ************************************************************************************/

int stm32_tone_setup(void)
{
  static bool                initialized = false;
  struct pwm_lowerhalf_s     *tone;
  struct pwm_info_s          info;
  struct oneshot_lowerhalf_s *oneshot = NULL;
  int                        ret;

  /* Have we already initialized? */

  if (!initialized)
    {
      /* Call stm32_pwminitialize() to get an instance of the PWM interface */

      tone = stm32_pwminitialize(TONE_PWM_TIMER);
      if (!tone)
        {
          auderr("Failed to get the STM32 PWM lower half to AUDIO TONE\n");
          return -ENODEV;
        }

      /* Initialize TONE PWM */

      tone->ops->setup(tone);
      tone->ops->start(tone, &info);

      /* Initialize ONESHOT Timer */

      oneshot = oneshot_initialize(ONESHOT_TIMER, OST_RES);
      if (!oneshot)
        {
          auderr("Failed to initialize ONESHOT Timer!\n");
          return -ENODEV;
        }

      /* Register the Audio Tone driver at "/dev/tone0" */

      ret = tone_register("/dev/tone0", tone, oneshot);
      if (ret < 0)
        {
          auderr("ERROR: tone_register failed: %d\n", ret);
          return ret;
        }

      /* Now we are initialized */

      initialized = true;
    }

  return OK;
}

#endif /* HAVE_TONE */
