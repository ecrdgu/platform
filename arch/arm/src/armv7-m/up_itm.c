
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include "up_arch.h"
#include "itm.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: itm_sendchar
 *
 * Description:
 *   The function transmits a character via the ITM channel 0, and
 *   - Just returns when no debugger is connected that has booked the output.
 *   - Is blocking when a debugger is connected, but the previous character
 *     sent has not been transmitted.
 *
 * Input Parameters:
 *   ch - Character to transmit.
 *
 * Returned Value:
 *   Character to transmit.
 *
 ****************************************************************************/

uint32_t itm_sendchar(uint32_t ch)
{
  if ((getreg32(ITM_TCR) & ITM_TCR_ITMENA_Msk) && /* ITM enabled */
      (getreg32(ITM_TER) & (1UL << 0)))           /* ITM Port #0 enabled */
    {
      while (getreg32(ITM_PORT(0)) == 0);
      putreg8((uint8_t)ch, ITM_PORT(0));
    }

  return ch;
}

/****************************************************************************
 * Name: itm_receivechar
 *
 * Description:
 *
 * Input Parameters:
 *  The function inputs a character via the external variable g_itm_rxbuffer.
 *
 * Returned Value:
 *   Received character or -1 No character pending.
 *
 ****************************************************************************/

int32_t itm_receivechar(void)
{
  int32_t ch = -1;  /* Assume no character available */

  if (g_itm_rxbuffer != ITM_RXBUFFER_EMPTY)
    {
      ch = g_itm_rxbuffer;
      g_itm_rxbuffer = ITM_RXBUFFER_EMPTY; /* Ready for next character */
    }

  return ch;
}

/****************************************************************************
 * Name: itm_checkchar
 *
 * Description:
 *
 * Input Parameters:
 *  The function checks whether a character is pending for reading in the
 *  variable g_itm_rxbuffer.
 *
 * Returned Value:
 *   0  No character available.
 *   1  Character available.
 *
 ****************************************************************************/

int32_t itm_checkchar (void)
{
  return (g_itm_rxbuffer != ITM_RXBUFFER_EMPTY);
}
