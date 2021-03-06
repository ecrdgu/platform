
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_ALARM_H
#define __ARCH_ARM_SRC_STM32F7_STM32_ALARM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <time.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
typedef CODE void (*alm_callback_t)(FAR void *arg, unsigned int alarmid);

/* These features map to STM32 RTC from stm32F7xx */

enum alm_id_e
{
  RTC_ALARMA = 0,               /* RTC ALARM A */
  RTC_ALARMB,                   /* RTC ALARM B */
  RTC_ALARM_LAST
};

/* Structure used to pass parameters to set an alarm */

struct alm_setalarm_s
{
  int as_id;                    /* enum alm_id_e */
  struct tm as_time;            /* Alarm expiration time */
  alm_callback_t as_cb;         /* Callback (if non-NULL) */
  FAR void *as_arg;             /* Argument for callback */
};

/* Structure used to pass parameters to query an alarm */

struct alm_rdalarm_s
{
  int ar_id;                    /* enum alm_id_e */
  FAR struct rtc_time *ar_time; /* Argument for storing ALARM RTC time */
};
#endif

#ifdef CONFIG_RTC_PERIODIC
typedef CODE int (*wakeupcb_t)(void);
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
/****************************************************************************
 * Name: stm32_rtc_setalarm
 *
 * Description:
 *   Set an alarm to an absolute time using associated hardware.
 *
 * Input Parameters:
 *  alminfo - Information about the alarm configuration.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/

int stm32_rtc_setalarm(FAR struct alm_setalarm_s *alminfo);

/****************************************************************************
 * Name: stm32_rtc_rdalarm
 *
 * Description:
 *   Query an alarm configured in hardware.
 *
 * Input Parameters:
 *  alminfo - Information about the alarm configuration.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/

int stm32_rtc_rdalarm(FAR struct alm_rdalarm_s *alminfo);

/****************************************************************************
 * Name: stm32_rtc_cancelalarm
 *
 * Description:
 *   Cancel an alarm.
 *
 * Input Parameters:
 *  alarmid - Identifies the alarm to be cancelled
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/

int stm32_rtc_cancelalarm(enum alm_id_e alarmid);
#endif /* CONFIG_RTC_ALARM */

#ifdef CONFIG_RTC_PERIODIC
/****************************************************************************
 * Name: stm32_rtc_setperiodic
 *
 * Description:
 *   Set a periodic RTC wakeup
 *
 * Input Parameters:
 *  period   - Time to sleep between wakeups
 *  callback - Function to call when the period expires.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/

int stm32_rtc_setperiodic(FAR const struct timespec *period,
                          wakeupcb_t callback);

/****************************************************************************
 * Name: stm32_rtc_cancelperiodic
 *
 * Description:
 *   Cancel a periodic wakeup
 *
 * Input Parameters:
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/

int stm32_rtc_cancelperiodic(void);
#endif /* CONFIG_RTC_PERIODIC */

#endif /* __ARCH_ARM_SRC_STM32F7_STM32_ALARM_H */
