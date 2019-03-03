
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include "chip.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* This file is only a thin shell that includes the correct RTC implementation
 * for the selected STM32 family.  The correct file cannot be selected by
 * the make system because it needs the intelligence that only exists in
 * chip.h that can associate an STM32 part number with an STM32 family.
 */

/* The STM32 F1 has a simple battery-backed counter for its RTC and has a
 * separate block for the BKP registers.
 */

#if defined(CONFIG_STM32_STM32F10XX)
#  include "stm32_rtcounter.c"

/* The other families use a more traditional Realtime Clock/Calendar (RTCC) with
 * broken-out data/time in BCD format.  The backup registers are integrated into
 * the RTCC in these families.
 */

#elif defined(CONFIG_STM32_STM32F20XX) || \
      defined(CONFIG_STM32_STM32F30XX)
#  include "stm32_rtcc.c"
#elif defined(CONFIG_STM32_STM32L15XX)
#  include "stm32l15xxx_rtcc.c"
#elif defined(CONFIG_STM32_STM32F4XXX)
#  include "stm32f40xxx_rtcc.c"
#endif
