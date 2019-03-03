
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"

/* This file is only a thin shell that includes the correct DMA implementation
 * for the selected STM32 family.  The correct file cannot be selected by
 * the make system because it needs the intelligence that only exists in
 * chip.h that can associate an STM32 part number with an STM32 family.
 *
 * TODO: do we need separate implementation for STM32L4X3?
 */

#if defined(CONFIG_STM32L4_STM32L4X3) || defined(CONFIG_STM32L4_STM32L4X5) || \
    defined(CONFIG_STM32L4_STM32L4X6)
#include "stm32l4x6xx_dma.c"
#else
#  error "Unsupported STM32L4 chip"
#endif

