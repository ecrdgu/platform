
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_PWR_H
#define __ARCH_ARM_SRC_STM32F7_STM32_PWR_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>

#include "chip.h"
#include "chip/stm32_pwr.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef __ASSEMBLY__

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

/************************************************************************************
 * Name: stm32_pwr_initbkp
 *
 * Description:
 *   Insures the referenced count access to the backup domain (RTC registers,
 *   RTC backup data registers and backup SRAM is consistent with the HW state
 *   without relying on a variable.
 *
 *   NOTE: This function should only be called by SoC Start up code.
 *
 * Input Parameters:
 *   writable - set the initial state of the enable and the
 *              bkp_writable_counter
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

void stm32_pwr_initbkp(bool writable);

/************************************************************************************
 * Name: stm32_pwr_enablebkp
 *
 * Description:
 *   Enables access to the backup domain (RTC registers, RTC backup data registers
 *   and backup SRAM).
 *
 * Input Parameters:
 *   writable - True: enable ability to write to backup domain registers
 *
 * Returned Value:
 *   none
 *
 ************************************************************************************/

void stm32_pwr_enablebkp(bool writable);

/************************************************************************************
 * Name: stm32_pwr_enablebreg
 *
 * Description:
 *   Enables the Backup regulator, the Backup regulator (used to maintain backup
 *   SRAM content in Standby and VBAT modes) is enabled. If BRE is reset, the backup
 *   regulator is switched off. The backup SRAM can still be used but its content will
 *   be lost in the Standby and VBAT modes. Once set, the application must wait that
 *   the Backup Regulator Ready flag (BRR) is set to indicate that the data written
 *   into the RAM will be maintained in the Standby and VBAT modes.
 *
 * Input Parameters:
 *   regon - state to set it to
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

void stm32_pwr_enablebreg(bool regon);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32F7_STM32_PWR_H */
