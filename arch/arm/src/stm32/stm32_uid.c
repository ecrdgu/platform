
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <chip/stm32_memorymap.h>

#include "stm32_uid.h"

#ifdef STM32_SYSMEM_UID /* Not defined for the STM32L */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

void stm32_get_uniqueid(uint8_t uniqueid[12])
{
  int i;

  for (i = 0; i < 12; i++)
    {
      uniqueid[i] = *((uint8_t*)(STM32_SYSMEM_UID)+i);
    }
}

#endif /* STM32_SYSMEM_UID */

