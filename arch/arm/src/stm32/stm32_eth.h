
#ifndef __ARCH_ARM_SRC_STM32_STM32_ETH_H
#define __ARCH_ARM_SRC_STM32_STM32_ETH_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include "chip.h"

#if STM32_NETHERNET > 0

#include "chip/stm32_eth.h"

#ifndef __ASSEMBLY__

/************************************************************************************
 * Public Functions
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
 * Function: stm32_ethinitialize
 *
 * Description:
 *   Initialize the Ethernet driver for one interface.  If the STM32 chip supports
 *   multiple Ethernet controllers, then board specific logic must implement
 *   up_netinitialize() and call this function to initialize the desired interfaces.
 *
 * Input Parameters:
 *   intf - In the case where there are multiple EMACs, this value identifies which
 *   EMAC is to be initialized.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ************************************************************************************/

#if STM32_NETHERNET > 1
int stm32_ethinitialize(int intf);
#endif

/************************************************************************************
 * Function: stm32_phy_boardinitialize
 *
 * Description:
 *   Some boards require specialized initialization of the PHY before it can be used.
 *   This may include such things as configuring GPIOs, resetting the PHY, etc.  If
 *   CONFIG_STM32_PHYINIT is defined in the configuration then the board specific
 *   logic must provide stm32_phyinitialize();  The STM32 Ethernet driver will call
 *   this function one time before it first uses the PHY.
 *
 * Input Parameters:
 *   intf - Always zero for now.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ************************************************************************************/

#ifdef CONFIG_STM32_PHYINIT
int stm32_phy_boardinitialize(int intf);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* STM32_NETHERNET > 0 */
#endif /* __ARCH_ARM_SRC_STM32_STM32_ETH_H */

