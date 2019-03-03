
#ifndef __ARCH_ARM_SRC_STM32_STM32_FLASH_H
#define __ARCH_ARM_SRC_STM32_STM32_FLASH_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <ecr/progmem.h>

#include "chip.h"
#include "chip/stm32_flash.h"

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_eeprom_size
 *
 * Description:
 *   Get EEPROM data memory size
 *
 * Returned Value:
 *  Length of EEPROM memory region
 *
 ************************************************************************************/

size_t stm32_eeprom_size(void);

/************************************************************************************
 * Name: stm32_eeprom_getaddress
 *
 * Description:
 *   Get EEPROM data memory address
 *
 * Returned Value:
 *  Address of EEPROM memory region
 *
 ************************************************************************************/

size_t stm32_eeprom_getaddress(void);

/************************************************************************************
 * Name: stm32_eeprom_write
 *
 * Description:
 *    Write buffer to EEPROM data memory address
 *
 * Returned Value:
 *  Number of written bytes or error code.
 *
 ************************************************************************************/

ssize_t stm32_eeprom_write(size_t addr, const void *buf, size_t buflen);

/************************************************************************************
 * Name: stm32_eeprom_erase
 *
 * Description:
 *    Erase memory on EEPROM data memory address
 *
 * Returned Value:
 *  Number of erased bytes or error code.
 *
 ************************************************************************************/

ssize_t stm32_eeprom_erase(size_t addr, size_t eraselen);

#endif /* __ARCH_ARM_SRC_STM32_STM32_FLASH_H */
