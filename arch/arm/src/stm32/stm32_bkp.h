
#ifndef __ARCH_ARM_SRC_STM32_STM32_BKP_H
#define __ARCH_ARM_SRC_STM32_STM32_BKP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

/* Only the STM32 F1 family has a dedicated address region for BKP memory.  For F2,
 * F3, and F4 parts, the bKP registers lie in the same address region as the RTCC
 * and the definitions in chip/stm32_rtcc.h should be used to access backup
 * registers.  NOTE:  These definitions are not interchangeable!
 */

#include "chip.h"
#ifdef CONFIG_STM32_STM32F10XX
#  include "chip/stm32_bkp.h"
#else
#  include "chip/stm32_rtcc.h"
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#endif /* __ARCH_ARM_SRC_STM32_STM32_BKP_H */
