
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* This file is only a thin shell that includes the correct DMA implementation
 * for the selected STM32 family.  The correct file cannot be selected by
 * the make system because it needs the intelligence that only exists in
 * chip.h that can associate an STM32 part number with an STM32 family.
 *
 * The STM32 F4 DMA differs from the F1 DMA primarily in that it adds the
 * concept of "streams" that are used to associate DMA sources with DMA
 * channels.
 */

#if defined(CONFIG_STM32_STM32L15XX) || defined(CONFIG_STM32_STM32F10XX) || \
    defined(CONFIG_STM32_STM32F30XX) || defined(CONFIG_STM32_STM32F37XX)
#  include "stm32f10xxx_dma.c"
#elif defined(CONFIG_STM32_STM32F33XX)
#  include "stm32f33xxx_dma.c"
#elif defined(CONFIG_STM32_STM32F20XX)
#  include "stm32f20xxx_dma.c"
#elif defined(CONFIG_STM32_STM32F4XXX)
#  include "stm32f40xxx_dma.c"
#endif
