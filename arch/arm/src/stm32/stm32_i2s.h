
#ifndef __ARCH_ARM_SRC_STM32_I2S_H
#define __ARCH_ARM_SRC_STM32_I2S_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>
#include <ecr/audio/i2s.h>

#include "chip.h"
#include "chip/stm32_i2s.h"

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
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_i2sdev_initialize
 *
 * Description:
 *   Initialize the selected I2S port
 *
 * Input Parameters:
 *   Port number (for hardware that has mutiple I2S interfaces)
 *
 * Returned Value:
 *   Valid I2S device structure reference on succcess; a NULL on failure
 *
 ************************************************************************************/

FAR struct i2s_dev_s *stm32_i2sdev_initialize(int port);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32_I2S_H */
