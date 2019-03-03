
#ifndef __ARCH_ARM_SRC_STM32_HCIUART_H
#define __ARCH_ARM_SRC_STM32_HCIUART_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum hciuart_devno_e
{
  HCIUART1 = 0,   /* HCI UART on STM32 USART1 */
  HCIUART2 = 1,   /* HCI UART on STM32 USART2 */
  HCIUART3 = 2,   /* HCI UART on STM32 USART3 */
                  /* UARTs 4-5 do not support RTS/CTS flow control */
  HCIUART5 = 5,   /* HCI UART on STM32 USART6 */
  HCIUART6 = 6,   /* HCI UART on STM32 UART7 */
  HCIUART7 = 7    /* HCI UART on STM32 UART8 */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: hciuart_instantiate
 *
 * Description:
 *   Obtain an instance of the HCI UART interface for the specified HCI UART
 *   This assumes that hciuart_initialize was called previously.
 *
 * Input Parameters:
 *   uart - Identifies the HCI UART to be configured
 *
 * Returned Value:
 *   On success, a reference to the HCI UART lower driver for the associated
 *   U[S]ART
 *
 ****************************************************************************/

const struct btuart_lowerhalf_s *hciuart_instantiate(enum hciuart_devno_e uart);

/****************************************************************************
 * Name: hciuart_initialize
 *
 * Description:
 *   Performs the low-level, one-time USART initialization.  This must be
 *   called before hciuart_instantiate.
 *
 * Input Paramters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void hciuart_initialize(void);

/****************************************************************************
 * Name: stm32_serial_dma_poll
 *
 * Description:
 *   Checks receive DMA buffers for received bytes that have not accumulated
 *   to the point where the DMA half/full interrupt has triggered.
 *
 *   This function should be called from a timer or other periodic context.
 *
 * Input Paramters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_HCIUART_RXDMA
void stm32_serial_dma_poll(void);
#endif

#endif /* __ARCH_ARM_SRC_STM32_HCIUART_H */
