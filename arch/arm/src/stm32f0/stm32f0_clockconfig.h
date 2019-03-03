
#ifndef __ARCH_ARM_SRC_STM32F0_STM32F0_CLOCKCONFIG_H
#define __ARCH_ARM_SRC_STM32F0_STM32F0_CLOCKCONFIG_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/************************************************************************************
 * Public Data
 ************************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32f0_clockconfig
 *
 * Description:
 *   Called to initialize the STM32F0XX.  This does whatever setup is needed to put
 *   the MCU in a usable state.  This includes the initialization of clocking using
 *   the settings in board.h.
 *
 ************************************************************************************/

void stm32f0_clockconfig(void);

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_STM32F0_STM32F0_CLOCKCONFIG_H */
