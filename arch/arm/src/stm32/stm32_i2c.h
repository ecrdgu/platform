
#ifndef __ARCH_ARM_SRC_STM32_STM32_I2C_H
#define __ARCH_ARM_SRC_STM32_STM32_I2C_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/i2c/i2c_master.h>

#include "chip.h"
#if defined(CONFIG_STM32_STM32F30XX) || defined(CONFIG_STM32_STM32F37XX) || \
    defined(CONFIG_STM32_STM32F33XX)
#  include "chip/stm32f30xxx_i2c.h"
#else
#  include "chip/stm32_i2c.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If a dynamic timeout is selected, then a non-negative, non-zero micro-
 * seconds per byte value must be provided as well.
 */

#ifdef CONFIG_STM32_I2C_DYNTIMEO
#  if CONFIG_STM32_I2C_DYNTIMEO_USECPERBYTE < 1
#    warning "Ignoring CONFIG_STM32_I2C_DYNTIMEO because of CONFIG_STM32_I2C_DYNTIMEO_USECPERBYTE"
#    undef CONFIG_STM32_I2C_DYNTIMEO
#  endif
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_i2cbus_initialize
 *
 * Description:
 *   Initialize the selected I2C port. And return a unique instance of struct
 *   struct i2c_master_s.  This function may be called to obtain multiple
 *   instances of the interface, each of which may be set up with a
 *   different frequency and slave address.
 *
 * Input Parameters:
 *   Port number (for hardware that has multiple I2C interfaces)
 *
 * Returned Value:
 *   Valid I2C device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

FAR struct i2c_master_s *stm32_i2cbus_initialize(int port);

/****************************************************************************
 * Name: stm32_i2cbus_uninitialize
 *
 * Description:
 *   De-initialize the selected I2C port, and power down the device.
 *
 * Input Parameters:
 *   Device structure as returned by the stm32_i2cbus_initialize()
 *
 * Returned Value:
 *   OK on success, ERROR when internal reference count mismatch or dev
 *   points to invalid hardware device.
 *
 ****************************************************************************/

int stm32_i2cbus_uninitialize(FAR struct i2c_master_s *dev);

#endif /* __ARCH_ARM_SRC_STM32_STM32_I2C_H */
