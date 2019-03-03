
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_SAI_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_SAI_H

/******************************************************************************
 * Included Files
 ******************************************************************************/

#include <ecr/config.h>

#include "chip.h"
#include "chip/stm32l4_sai.h"

#include <ecr/audio/i2s.h>

/******************************************************************************
 * Pre-processor definitions
 ******************************************************************************/

#define SAI1_BLOCK_A     0
#define SAI1_BLOCK_B     1
#define SAI2_BLOCK_A     2
#define SAI2_BLOCK_B     3

/******************************************************************************
 * Public Function Prototypes
 ******************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: stm32l4_sai_initialize
 *
 * Description:
 *   Initialize the selected SAI block
 *
 * Input Parameters:
 *   intf - I2S interface number (identifying the "logical" SAI interface)
 *
 * Returned Value:
 *   Valid I2S device structure reference on success; a NULL on failure
 *
 ****************************************************************************/

struct i2s_dev_s *stm32l4_sai_initialize(int intf);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_SAI_H */
