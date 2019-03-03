
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <stdint.h>
#include "stm32l4_waste.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

uint32_t idle_wastecounter = 0;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void up_waste(void)
{
  idle_wastecounter++;
}
