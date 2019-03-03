
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>

#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <ecr/irq.h>
#include <ecr/timers/timer.h>

#include <arch/board/board.h>

#include "stm32l4_tim.h"

#if defined(CONFIG_TIMER) && \
    (defined(CONFIG_STM32L4_TIM1)  || defined(CONFIG_STM32L4_TIM2)  || \
     defined(CONFIG_STM32L4_TIM3)  || defined(CONFIG_STM32L4_TIM4)  || \
     defined(CONFIG_STM32L4_TIM5)  || defined(CONFIG_STM32L4_TIM6)  || \
     defined(CONFIG_STM32L4_TIM7)  || defined(CONFIG_STM32L4_TIM8)  || \
     defined(CONFIG_STM32L4_TIM15) || defined(CONFIG_STM32L4_TIM16) || \
     defined(CONFIG_STM32L4_TIM17))

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define STM32L4_TIM1_RES   16
#define STM32L4_TIM2_RES   32
#define STM32L4_TIM3_RES   16
#define STM32L4_TIM4_RES   16
#define STM32L4_TIM5_RES   32
#define STM32L4_TIM6_RES   16
#define STM32L4_TIM7_RES   16
#define STM32L4_TIM8_RES   16
#define STM32L4_TIM15_RES  16
#define STM32L4_TIM16_RES  16
#define STM32L4_TIM17_RES  16

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * timer_lowerhalf_s structure.
 */

struct stm32l4_lowerhalf_s
{
  FAR const struct timer_ops_s *ops;        /* Lower half operations */
  FAR struct stm32l4_tim_dev_s *tim;        /* stm32 timer driver */
  tccb_t                        callback;   /* Current upper half interrupt callback */
  FAR void                     *arg;        /* Argument passed to upper half callback */
  bool                          started;    /* True: Timer has been started */
  const uint8_t                 resolution; /* Number of bits in the timer (16 or 32 bits) */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Interrupt handling *******************************************************/

static int stm32l4_timer_handler(int irq, void *context, void *arg);

/* "Lower half" driver methods **********************************************/

static int stm32l4_start(FAR struct timer_lowerhalf_s *lower);
static int stm32l4_stop(FAR struct timer_lowerhalf_s *lower);
static int stm32l4_settimeout(FAR struct timer_lowerhalf_s *lower,
                              uint32_t timeout);
static void stm32l4_setcallback(FAR struct timer_lowerhalf_s *lower,
                                tccb_t callback, FAR void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* "Lower half" driver methods */

static const struct timer_ops_s g_timer_ops =
{
  .start       = stm32l4_start,
  .stop        = stm32l4_stop,
  .getstatus   = NULL,
  .settimeout  = stm32l4_settimeout,
  .setcallback = stm32l4_setcallback,
  .ioctl       = NULL,
};

#ifdef CONFIG_STM32L4_TIM1
static struct stm32l4_lowerhalf_s g_tim1_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM1_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM2
static struct stm32l4_lowerhalf_s g_tim2_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM2_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM3
static struct stm32l4_lowerhalf_s g_tim3_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM3_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM4
static struct stm32l4_lowerhalf_s g_tim4_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM4_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM5
static struct stm32l4_lowerhalf_s g_tim5_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM5_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM6
static struct stm32l4_lowerhalf_s g_tim6_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM6_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM7
static struct stm32l4_lowerhalf_s g_tim7_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM7_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM8
static struct stm32l4_lowerhalf_s g_tim8_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM8_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM15
static struct stm32l4_lowerhalf_s g_tim15_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM15_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM16
static struct stm32l4_lowerhalf_s g_tim16_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM16_RES,
};
#endif

#ifdef CONFIG_STM32L4_TIM17
static struct stm32l4_lowerhalf_s g_tim17_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = STM32L4_TIM17_RES,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_timer_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static int stm32l4_timer_handler(int irq, void *context, void *arg)
{
  FAR struct stm32l4_lowerhalf_s *lower = (FAR struct stm32l4_lowerhalf_s *) arg;
  uint32_t next_interval_us = 0;

  STM32L4_TIM_ACKINT(lower->tim, 0);

  if (lower->callback(&next_interval_us, lower->arg))
    {
      if (next_interval_us > 0)
        {
          STM32L4_TIM_SETPERIOD(lower->tim, next_interval_us);
        }
    }
  else
    {
      stm32l4_stop((struct timer_lowerhalf_s *)lower);
    }

  return OK;
}

/****************************************************************************
 * Name: stm32l4_start
 *
 * Description:
 *   Start the timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int stm32l4_start(FAR struct timer_lowerhalf_s *lower)
{
  FAR struct stm32l4_lowerhalf_s *priv = (FAR struct stm32l4_lowerhalf_s *)lower;

  if (!priv->started)
    {
      STM32L4_TIM_SETMODE(priv->tim, STM32L4_TIM_MODE_UP);

      if (priv->callback != NULL)
        {
          STM32L4_TIM_SETISR(priv->tim, stm32l4_timer_handler, priv, 0);
          STM32L4_TIM_ENABLEINT(priv->tim, 0);
        }

      priv->started = true;
      return OK;
    }

  /* Return EBUSY to indicate that the timer was already running */

  return -EBUSY;
}

/****************************************************************************
 * Name: stm32l4_stop
 *
 * Description:
 *   Stop the timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int stm32l4_stop(FAR struct timer_lowerhalf_s *lower)
{
  FAR struct stm32l4_lowerhalf_s *priv = (FAR struct stm32l4_lowerhalf_s *)lower;

  if (priv->started)
    {
      STM32L4_TIM_SETMODE(priv->tim, STM32L4_TIM_MODE_DISABLED);
      STM32L4_TIM_DISABLEINT(priv->tim, 0);
      STM32L4_TIM_SETISR(priv->tim, NULL, NULL, 0);
      priv->started = false;
      return OK;
    }

  /* Return ENODEV to indicate that the timer was not running */

  return -ENODEV;
}

/****************************************************************************
 * Name: stm32l4_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the "lower-half"
 *             driver state structure.
 *   timeout - The new timeout value in microseconds.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int stm32l4_settimeout(FAR struct timer_lowerhalf_s *lower,
                              uint32_t timeout)
{
  FAR struct stm32l4_lowerhalf_s *priv = (FAR struct stm32l4_lowerhalf_s *)lower;
  uint64_t maxtimeout;

  if (priv->started)
    {
      return -EPERM;
    }

  maxtimeout = (1 << priv->resolution) - 1;
  if (timeout > maxtimeout)
    {
      uint64_t freq = (maxtimeout * 1000000) / timeout;
      STM32L4_TIM_SETCLOCK(priv->tim, freq);
      STM32L4_TIM_SETPERIOD(priv->tim, maxtimeout);
    }
  else
    {
      STM32L4_TIM_SETCLOCK(priv->tim, 1000000);
      STM32L4_TIM_SETPERIOD(priv->tim, timeout);
    }

  return OK;
}

/****************************************************************************
 * Name: stm32l4_sethandler
 *
 * Description:
 *   Call this user provided timeout handler.
 *
 * Input Parameters:
 *   lower    - A pointer the publicly visible representation of the "lower-half"
 *              driver state structure.
 *   callback - The new timer expiration function pointer.  If this
 *              function pointer is NULL, then the reset-on-expiration
 *              behavior is restored,
 *   arg      - Argument that will be provided in the callback
 *
 * Returned Value:
 *   The previous timer expiration function pointer or NULL is there was
 *   no previous function pointer.
 *
 ****************************************************************************/

static void stm32l4_setcallback(FAR struct timer_lowerhalf_s *lower,
                                tccb_t callback, FAR void *arg)
{
  FAR struct stm32l4_lowerhalf_s *priv = (FAR struct stm32l4_lowerhalf_s *)lower;
  irqstate_t flags = enter_critical_section();

  /* Save the new callback */

  priv->callback = callback;
  priv->arg     = arg;

  if (callback != NULL && priv->started)
    {
      STM32L4_TIM_SETISR(priv->tim, stm32l4_timer_handler, priv, 0);
      STM32L4_TIM_ENABLEINT(priv->tim, 0);
    }
  else
    {
      STM32L4_TIM_DISABLEINT(priv->tim, 0);
      STM32L4_TIM_SETISR(priv->tim, NULL, NULL, 0);
    }

  leave_critical_section(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_timer_initialize
 *
 * Description:
 *   Bind the configuration timer to a timer lower half instance and
 *   register the timer drivers at 'devpath'
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.  This should be of the
 *     form /dev/timer0
 *   timer - the timer's number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

int stm32l4_timer_initialize(FAR const char *devpath, int timer)
{
  FAR struct stm32l4_lowerhalf_s *lower;

  switch (timer)
    {
#ifdef CONFIG_STM32L4_TIM1
      case 1:
        lower = &g_tim1_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM2
      case 2:
        lower = &g_tim2_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM3
      case 3:
        lower = &g_tim3_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM4
      case 4:
        lower = &g_tim4_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM5
      case 5:
        lower = &g_tim5_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM6
      case 6:
        lower = &g_tim6_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM7
      case 7:
        lower = &g_tim7_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM8
      case 8:
        lower = &g_tim8_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM15
      case 15:
        lower = &g_tim15_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM16
      case 16:
        lower = &g_tim16_lowerhalf;
        break;
#endif
#ifdef CONFIG_STM32L4_TIM17
      case 17:
        lower = &g_tim17_lowerhalf;
        break;
#endif
      default:
        return -ENODEV;
    }

  /* Initialize the elements of lower half state structure */

  lower->started  = false;
  lower->callback = NULL;
  lower->tim      = stm32l4_tim_init(timer);

  if (lower->tim == NULL)
    {
      return -EINVAL;
    }

  /* Register the timer driver as /dev/timerX.  The returned value from
   * timer_register is a handle that could be used with timer_unregister().
   * REVISIT: The returned handle is discard here.
   */

  FAR void *drvr = timer_register(devpath,
                                  (FAR struct timer_lowerhalf_s *)lower);
  if (drvr == NULL)
    {
      /* The actual cause of the failure may have been a failure to allocate
       * perhaps a failure to register the timer driver (such as if the
       * 'depath' were not unique).  We know here but we return EEXIST to
       * indicate the failure (implying the non-unique devpath).
       */

      return -EEXIST;
    }

  return OK;
}

#endif /* CONFIG_TIMER */