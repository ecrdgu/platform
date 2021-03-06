
#ifndef __ARCH_ARM_SRC_STM32_STM32_CAN_H
#define __ARCH_ARM_SRC_STM32_STM32_CAN_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include "chip.h"
#include "chip/stm32_can.h"

#include <ecr/can/can.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Configuration ********************************************************************/
/* Up to 2 CAN interfaces are supported */

#if STM32_NCAN < 2
#  undef CONFIG_STM32_CAN2
#endif

#if STM32_NCAN < 1
#  undef CONFIG_STM32_CAN1
#endif

#if defined(CONFIG_CAN) && (defined(CONFIG_STM32_CAN1) || defined(CONFIG_STM32_CAN2))

/* CAN BAUD */

#if defined(CONFIG_STM32_CAN1) && !defined(CONFIG_CAN1_BAUD)
#  error "CONFIG_CAN1_BAUD is not defined"
#endif

#if defined(CONFIG_STM32_CAN2) && !defined(CONFIG_CAN2_BAUD)
#  error "CONFIG_CAN2_BAUD is not defined"
#endif

/* User-defined TSEG1 and TSEG2 settings may be used.
 *
 * CONFIG_CAN_TSEG1 = the number of CAN time quanta in segment 1
 * CONFIG_CAN_TSEG2 = the number of CAN time quanta in segment 2
 * CAN_BIT_QUANTA   = The number of CAN time quanta in on bit time
 */

#ifndef CONFIG_CAN_TSEG1
#  define CONFIG_CAN_TSEG1 6
#endif

#if CONFIG_CAN_TSEG1 < 1 || CONFIG_CAN_TSEG1 > CAN_BTR_TSEG1_MAX
#  errror "CONFIG_CAN_TSEG1 is out of range"
#endif

#ifndef CONFIG_CAN_TSEG2
#  define CONFIG_CAN_TSEG2 7
#endif

#if CONFIG_CAN_TSEG2 < 1 || CONFIG_CAN_TSEG2 > CAN_BTR_TSEG2_MAX
#  errror "CONFIG_CAN_TSEG2 is out of range"
#endif

/************************************************************************************
 * Public Types
 ************************************************************************************/

#ifndef __ASSEMBLY__

/************************************************************************************
 * Public Data
 ************************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/****************************************************************************
 * Name: stm32_caninitialize
 *
 * Description:
 *   Initialize the selected CAN port
 *
 * Input Parameters:
 *   Port number (for hardware that has mutiple CAN interfaces)
 *
 * Returned Value:
 *   Valid CAN device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

struct can_dev_s;
FAR struct can_dev_s *stm32_caninitialize(int port);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_CAN && (CONFIG_STM32_CAN1 || CONFIG_STM32_CAN2) */
#endif /* __ARCH_ARM_SRC_STM32_STM32_CAN_H */
