
#ifndef __ARCH_ARM_SRC_STM32F0_STM32F0_USBDEV_H
#define __ARCH_ARM_SRC_STM32F0_STM32F0_USBDEV_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <ecr/usb/usbdev.h>
#include <stdint.h>

#include "chip.h"
#include "chip/stm32f0_usbdev.h"

/************************************************************************************
 * Public Functions
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
 * Name:  stm32f0_usbpullup
 *
 * Description:
 *   If USB is supported and the board supports a pullup via GPIO (for USB software
 *   connect and disconnect), then the board software must provide stm32f0_pullup.
 *   See include/ecr/usb/usbdev.h for additional description of this method.
 *
 ************************************************************************************/

int stm32f0_usbpullup(FAR struct usbdev_s *dev,  bool enable);

/************************************************************************************
 * Name:  stm32f0_usbsuspend
 *
 * Description:
 *   Board logic must provide the stm32f0_usbsuspend logic if the USBDEV driver is
 *   used.  This function is called whenever the USB enters or leaves suspend mode.
 *   This is an opportunity for the board logic to shutdown clocks, power, etc.
 *   while the USB is suspended.
 *
 ************************************************************************************/

void stm32f0_usbsuspend(FAR struct usbdev_s *dev, bool resume);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32F0_STM32F0_USBDEV_H */
