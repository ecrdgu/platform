
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_1WIRE_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_1WIRE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "stm32l4_uart.h"

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_1wireinitialize
 *
 * Description:
 *   Initialize the selected 1-Wire port. And return a unique instance of struct
 *   struct onewire_dev_s.  This function may be called to obtain multiple
 *   instances of the interface, each of which may be set up with a
 *   different frequency and slave address.
 *
 * Input Parameters:
 *   Port number (for hardware that has multiple 1-Wire interfaces)
 *
 * Returned Value:
 *   Valid 1-Wire device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

FAR struct onewire_dev_s *stm32l4_1wireinitialize(int port);

/****************************************************************************
 * Name: stm32l4_1wireuninitialize
 *
 * Description:
 *   De-initialize the selected 1-Wire port, and power down the device.
 *
 * Input Parameters:
 *   Device structure as returned by the stm32l4_1wireinitialize()
 *
 * Returned Value:
 *   OK on success, ERROR when internal reference count mismatch or dev
 *   points to invalid hardware device.
 *
 ****************************************************************************/

int stm32l4_1wireuninitialize(FAR struct onewire_dev_s *dev);

#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_1WIRE_H */
