
#ifndef __ARCH_ARM_SRC_ARMV7_M_ITM_SYSLOG_H
#define __ARCH_ARM_SRC_ARMV7_M_ITM_SYSLOG_H

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: itm_syslog_initialize
 *
 * Description:
 *   Performs ARM-specific initialize for the ITM SYSLOG functions.
 *   Additional, board specific logic may be required to:
 *
 *   - Enable/configured serial wire output pins
 *   - Enable debug clocking.
 *
 *   Those operations must be performed by MCU-specific logic before this
 *   function is called.
 *
 ****************************************************************************/

#ifdef CONFIG_ARMV7M_ITMSYSLOG
void itm_syslog_initialize(void);
#else
#  define itm_syslog_initialize()
#endif

#endif /* __ARCH_ARM_SRC_ARMV7_M_ITM_SYSLOG_H */
