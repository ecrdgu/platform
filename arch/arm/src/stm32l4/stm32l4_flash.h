
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_FLASH_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_FLASH_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include "chip/stm32l4_flash.h"

/************************************************************************************
 * Public Functions
 ************************************************************************************/

void stm32l4_flash_lock(void);
void stm32l4_flash_unlock(void);

/****************************************************************************
 * Name: stm32l4_flash_user_optbytes
 *
 * Description:
 *   Modify the contents of the user option bytes (USR OPT) on the flash.
 *   This does not set OBL_LAUNCH so new options take effect only after
 *   next power reset.
 *
 * Input Parameters:
 *   clrbits - Bits in the option bytes to be cleared
 *   setbits - Bits in the option bytes to be set
 *
 * Returned Value:
 *   Option bytes after operation is completed
 *
 ****************************************************************************/

uint32_t stm32l4_flash_user_optbytes(uint32_t clrbits, uint32_t setbits);

#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_FLASH_H */
