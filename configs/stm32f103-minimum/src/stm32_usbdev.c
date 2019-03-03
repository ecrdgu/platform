
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <ecr/usb/usbdev.h>
#include <ecr/usb/usbdev_trace.h>

#include "up_arch.h"
#include "stm32.h"
#include "stm32f103_minimum.h"

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called to setup USB-related GPIO pins for the STM32F103 Minimum board.
 *
 ************************************************************************************/

void stm32_usbinitialize(void)
{
  /* USB Soft Connect Pullup */

  stm32_configgpio(GPIO_USB_PULLUP);
}

/************************************************************************************
 * Name:  stm32_usbpullup
 *
 * Description:
 *   If USB is supported and the board supports a pullup via GPIO (for USB software
 *   connect and disconnect), then the board software must provide stm32_pullup.
 *   See include/ecr/usb/usbdev.h for additional description of this method.
 *   Alternatively, if no pull-up GPIO the following EXTERN can be redefined to be
 *   NULL.
 *
 ************************************************************************************/

int stm32_usbpullup(FAR struct usbdev_s *dev, bool enable)
{
  usbtrace(TRACE_DEVPULLUP, (uint16_t)enable);
  stm32_gpiowrite(GPIO_USB_PULLUP, !enable);
  return OK;
}

/************************************************************************************
 * Name:  stm32_usbsuspend
 *
 * Description:
 *   Board logic must provide the stm32_usbsuspend logic if the USBDEV driver is
 *   used.  This function is called whenever the USB enters or leaves suspend mode.
 *   This is an opportunity for the board logic to shutdown clocks, power, etc.
 *   while the USB is suspended.
 *
 ************************************************************************************/

void stm32_usbsuspend(FAR struct usbdev_s *dev, bool resume)
{
  uinfo("resume: %d\n", resume);
}
