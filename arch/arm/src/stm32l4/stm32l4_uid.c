
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <chip/stm32l4_memorymap.h>

#include "stm32l4_uid.h"

#ifdef STM32L4_SYSMEM_UID

/************************************************************************************
 * Public Functions
 ************************************************************************************/

void stm32l4_get_uniqueid(uint8_t uniqueid[12])
{
  int i;

  for (i = 0; i < 12; i++)
    {
      uniqueid[i] = *((uint8_t*)(STM32L4_SYSMEM_UID)+i);
    }
}

#endif /* STM32L4_SYSMEM_UID */
