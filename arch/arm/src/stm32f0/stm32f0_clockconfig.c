
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <debug.h>

#include <ecr/arch.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"
#include "stm32f0_rcc.h"
#include "stm32f0_clockconfig.h"
#include "chip/stm32f0_syscfg.h"
#include "chip/stm32f0_flash.h"
#include "chip/stm32f0_gpio.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32f0_clockconfig
 *
 * Description:
 *   Called to initialize the STM32F0xx.  This does whatever setup is needed
 *   to put the SoC in a usable state.  This includes the initialization of
 *   clocking using the settings in board.h.
 *
 ****************************************************************************/

void stm32f0_clockconfig(void)
{
  uint32_t regval;

  /*  Verify if PLL is already setup.  If so configure to use HSI mode */

  if ((getreg32(STM32F0_RCC_CFGR) & RCC_CFGR_SWS_MASK) == RCC_CFGR_SWS_PLL)
    {
      /* Select HSI mode */

      regval  = getreg32(STM32F0_RCC_CFGR);
      regval &= ~RCC_CFGR_SW_MASK;
      putreg32(regval, STM32F0_RCC_CFGR);

      while ((getreg32(STM32F0_RCC_CFGR) & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_HSI);
    }

  /* Disable the PLL */

  regval  = getreg32(STM32F0_RCC_CR);
  regval &= ~RCC_CR_PLLON;
  putreg32(regval, STM32F0_RCC_CR);
  while ((getreg32(STM32F0_RCC_CR) & RCC_CR_PLLRDY) != 0);

  /* Enable FLASH prefetch buffer and set flash latency */

  regval = getreg32(STM32_FLASH_ACR);
  regval &= ~FLASH_ACR_LATENCY_MASK;
  regval |= (FLASH_ACR_LATENCY_1 | FLASH_ACR_PRTFBE);
  putreg32(regval, STM32_FLASH_ACR);

  /* Set HCLK = SYSCLK */

  regval  = getreg32(STM32F0_RCC_CFGR);
  regval &= ~RCC_CFGR_HPRE_MASK;
  regval |= RCC_CFGR_HPRE_SYSCLK;
  putreg32(regval, STM32F0_RCC_CFGR);

  /* Set PCLK = HCLK */

  regval &= ~RCC_CFGR_PPRE1_MASK;
  regval |= RCC_CFGR_PPRE1_HCLK;
  putreg32(regval, STM32F0_RCC_CFGR);

  /* Configure the PLL to generate the system clock
   *
   * 1. Use source = HSI/2
   * 2. Use PREDIV = 1
   * 3. Use multiplier from board.h
   */

  regval &= ~(RCC_CFGR_PLLSRC_MASK | RCC_CFGR_PLLXTPRE_MASK | RCC_CFGR_PLLMUL_MASK);
  regval |= (RCC_CFGR_PLLSRC_HSId2 | RCC_CFGR_PLLXTPRE_DIV1 | STM32F0_CFGR_PLLMUL);
  putreg32(regval, STM32F0_RCC_CFGR);

  /* Enable the PLL */

  regval  = getreg32(STM32F0_RCC_CR);
  regval |= RCC_CR_PLLON;
  putreg32(regval, STM32F0_RCC_CR);
  while ((getreg32(STM32F0_RCC_CR) & RCC_CR_PLLRDY) == 0);

  /* Configure to use the PLL */

  regval  = getreg32(STM32F0_RCC_CFGR);
  regval |= RCC_CFGR_SW_PLL;
  putreg32(regval, STM32F0_RCC_CFGR);
  while ((getreg32(STM32F0_RCC_CFGR) & RCC_CFGR_SW_MASK) != RCC_CFGR_SW_PLL);

  /* Enable basic peripheral support */
  /* Enable all GPIO modules */

  regval  = getreg32(STM32F0_RCC_AHBENR);
  regval |= RCC_AHBENR_IOPAEN | RCC_AHBENR_IOPAEN | RCC_AHBENR_IOPAEN |\
            RCC_AHBENR_IOPAEN | RCC_AHBENR_IOPAEN | RCC_AHBENR_IOPAEN;
  putreg32(regval, STM32F0_RCC_AHBENR);
}
