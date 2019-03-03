
#ifndef __ARCH_ARM_SRC_STM32_CHIP_STM32F33XXX_MEMORYMAP_H
#define __ARCH_ARM_SRC_STM32_CHIP_STM32F33XXX_MEMORYMAP_H

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* STM32F33XXX Address Blocks *******************************************************/

#define STM32_CODE_BASE      0x00000000     /* 0x00000000-0x1fffffff: 512Mb code block */
#define STM32_SRAM_BASE      0x20000000     /* 0x20000000-0x3fffffff: 512Mb sram block */
#define STM32_PERIPH_BASE    0x40000000     /* 0x40000000-0x5fffffff: 512Mb peripheral block */
                                            /* 0x60000000-0xdfffffff: Reserved */
#define STM32_CORTEX_BASE    0xe0000000     /* 0xe0000000-0xffffffff: 512Mb Cortex-M4 block */

#define STM32_REGION_MASK    0xf0000000
#define STM32_IS_SRAM(a)     ((((uint32_t)(a)) & STM32_REGION_MASK) == STM32_SRAM_BASE)

/* Code Base Addresses **************************************************************/

#define STM32_BOOT_BASE      0x00000000     /* 0x00000000-0x0000ffff: Aliased boot memory */
                                            /* 0x00010000-0x07ffffff: Reserved */
#define STM32_FLASH_BASE     0x08000000     /* 0x08000000-0x0800ffff: FLASH memory */
                                            /* 0x08100000-0x1ffeffff: Reserved */
#define STM32_CCMRAM_BASE    0x10000000     /* 0x10000000-0x10000fff: 4Kb CCM data RAM */
                                            /* 0x10001000-0x1ffeffff: Reserved */
#define STM32_SYSMEM_BASE    0x1fffd800     /* 0x1fff0000-0x1fff7a0f: System memory */
                                            /* 0x1fff7a10-0x1fff7fff: Reserved */
#define STM32_OPTION_BASE    0x1ffff800     /* 0x1fffc000-0x1fffc007: Option bytes */
                                            /* 0x1fffc008-0x1fffffff: Reserved */


/* System Memory Addresses **********************************************************/

#define STM32_SYSMEM_UID     0x1ffff7ac     /* The 96-bit unique device identifier */
#define STM32_SYSMEM_FSIZE   0x1ffff7cc     /* This bitfield indicates the size of
                                             * the device Flash memory expressed in
                                             * Kbytes.  Example: 0x040 corresponds
                                             * to 64 Kbytes
                                             */

/* Peripheral Base Addresses ********************************************************/

#define STM32_APB1_BASE      0x40000000     /* 0x40000000-0x40009fff: APB1 */
                                            /* 0x4000a000-0x4000ffff: Reserved */
#define STM32_APB2_BASE      0x40010000     /* 0x40010000-0x40006bff: APB2 */
                                            /* 0x40016c00-0x4001ffff: Reserved */
#define STM32_AHB1_BASE      0x40020000     /* 0x40020000-0x400243ff: APB1 */
                                            /* 0x40024400-0x4007ffff: Reserved */
#define STM32_AHB2_BASE      0x48000000     /* 0x48000000-0x480017ff: AHB2 */
                                            /* 0x48001800-0x4fffffff: Reserved */
#define STM32_AHB3_BASE      0x50000000     /* 0x50000000-0x500003ff: AHB3 */

/* APB1 Base Addresses **************************************************************/

#define STM32_TIM2_BASE      0x40000000     /* 0x40000000-0x400003ff TIM2 */
#define STM32_TIM3_BASE      0x40000400     /* 0x40000400-0x400007ff TIM3 */
#define STM32_TIM6_BASE      0x40001000     /* 0x40001000-0x400013ff TIM6 */
#define STM32_TIM7_BASE      0x40001400     /* 0x40001400-0x400017ff TIM7 */
#define STM32_RTC_BASE       0x40002800     /* 0x40002800-0x40002bff RTC and BKP */
#define STM32_WWDG_BASE      0x40002c00     /* 0x40002c00-0x40002fff WWDG */
#define STM32_IWDG_BASE      0x40003000     /* 0x40003000-0x400033ff IWDG */
#define STM32_USART2_BASE    0x40004400     /* 0x40004400-0x400047ff USART2 */
#define STM32_USART3_BASE    0x40004800     /* 0x40004800-0x40004bff USART3 */
#define STM32_I2C1_BASE      0x40005400     /* 0x40005400-0x400057ff I2C1 */
#define STM32_CAN1_BASE      0x40006400     /* 0x40006400-0x400067ff bxCAN */
#define STM32_PWR_BASE       0x40007000     /* 0x40007000-0x400073ff PWR */
#define STM32_DAC1_BASE      0x40007400     /* 0x40007400-0x400077ff DAC1 */
#define STM32_DAC2_BASE      0x40009800     /* 0x40009800-0x40009bff DAC2 */

/* APB2 Base Addresses **************************************************************/

#define STM32_SYSCFG_BASE    0x40010000     /* 0x40010000-0x400103FF SYSCFG, and */
#define STM32_COMP_BASE      0x40010000     /* COMP, and */
#define STM32_OPAMP_BASE     0x40010000     /* OPAMP */
#define STM32_EXTI_BASE      0x40010400     /* 0x40010400-0x400107FF EXTI */
#define STM32_TIM1_BASE      0x40012c00     /* 0x40012c00-0x40012fff TIM1 */
#define STM32_SPI1_BASE      0x40013000     /* 0x40013000-0x400133ff SPI1 */
#define STM32_USART1_BASE    0x40013800     /* 0x40013800-0x40013bff USART1 */
#define STM32_TIM15_BASE     0x40014000     /* 0x40014000-0x400143ff TIM15 */
#define STM32_TIM16_BASE     0x40014400     /* 0x40014400-0x400147ff TIM16 */
#define STM32_TIM17_BASE     0x40014800     /* 0x40014800-0x40014bff TIM17 */
#define STM32_HRTIM1_BASE    0x40017400     /* 0x40017400-0x400177ff HRTIM1 */

/* AHB1 Base Addresses **************************************************************/

#define STM32_DMA1_BASE      0x40020000     /* 0x40020000-0x400203ff: DMA1  */
#define STM32_RCC_BASE       0x40021000     /* 0x40021000-0x400213ff: Reset and Clock control RCC */
#define STM32_FLASHIF_BASE   0x40022000     /* 0x40022000-0x400223ff: Flash memory interface */
#define STM32_CRC_BASE       0x40023000     /* 0x40023000-0x400233ff: CRC */
#define STM32_TSC_BASE       0x40024000     /* 0x40024000-0x400243ff: TSC */

/* AHB2 Base Addresses **************************************************************/

#define STM32_GPIOA_BASE     0x48000000     /* 0x48000000-0x480003ff: GPIO Port A */
#define STM32_GPIOB_BASE     0x48000400     /* 0x48000400-0x480007ff: GPIO Port B */
#define STM32_GPIOC_BASE     0x48000800     /* 0x48000800-0x48000bff: GPIO Port C */
#define STM32_GPIOD_BASE     0X48000C00     /* 0x48000c00-0x48000fff: GPIO Port D */
#define STM32_GPIOE_BASE     0X48001000     /* 0x48001000-0x480013ff: GPIO Port E */
#define STM32_GPIOF_BASE     0x48001400     /* 0x48001400-0x480017ff: GPIO Port F */

/* AHB3 Base Addresses **************************************************************/

#define STM32_ADC12_BASE      0x50000000     /* 0x50000000-0x500003ff: ADC12 */

/* Cortex-M4 Base Addresses *********************************************************/
/* Other registers -- see armv7-m/nvic.h for standard Cortex-M4 registers in this
 * address range
 */

#define STM32_SCS_BASE      0xe000e000
#define STM32_DEBUGMCU_BASE 0xe0042000

#endif /* __ARCH_ARM_SRC_STM32_CHIP_STM32F33XXX_MEMORYMAP_H */
