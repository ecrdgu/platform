
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_BBSRAM_H
#define __ARCH_ARM_SRC_STM32F7_STM32_BBSRAM_H

/****************************************************************************
 * The purpose of this driver is to add battery backup file to the file
 * system. There can be CONFIG_STM32F7_BBRSRAM_COUNT files defined.
 * These files are of fixed size up to the maximum of the backing 4K SRAM.
 *
 * If CONFIG_SAVE_CRASHDUMP is defined The driver also supports a feature
 * to save the context of a PANIC in one of these files.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_STM32F7_STM32F74XX) || defined(CONFIG_STM32F7_STM32F75XX) || \
    defined(CONFIG_STM32F7_STM32F76XX) || defined(CONFIG_STM32F7_STM32F77XX)
#  define STM32F7_BBSRAM_SIZE 4096
#else
#  error "No backup SRAM on this STM32 Device"
#endif

#if !defined(CONFIG_STM32F7_BBSRAM_FILES)
#  define CONFIG_STM32F7_BBSRAM_FILES 4
#endif

/* REVISIT: What guarantees that STM32F7_BBSRAM_GETDESC_IOCTL has a unique
 * value among all over _DIOC() values?
 */

#define STM32F7_BBSRAM_GETDESC_IOCTL _DIOC(0x0010) /* Returns a bbsramd_s */

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

enum bbsramdf_e
{
  BBSRAM_CRC_VALID = 1,        /* The crc is valid */
  BBSRAM_DIRTY     = 2,        /* The file was closed */
};

struct bbsramd_s
{
  uint8_t flags;               /* The crc is valid and the file was closed */
  uint8_t fileno;              /* The minor number */
  uint16_t len;                /* Total Bytes in this file*/
  struct timespec lastwrite;   /* Last write time */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#  define EXTERN extern "C"
extern "C"
{
#else
#  define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/****************************************************************************
 * Function: stm32_bbsraminitialize
 *
 * Description:
 *   Initialize the Battery Backed up SRAM driver.
 *
 * Input Parameters:
 *   devpath - the path to instantiate the files.
 *   sizes   - Pointer to a any array of file sizes to create
 *             the last entry should be 0
 *             A size of -1 will use all the remaining spaces
 *
 * If the length of sizes is greater then CONFIG_STM32F7_BBSRAM_FILES
 * CONFIG_STM32F7_BBSRAM_FILES will be returned.
 *
 * Returned Value:
 *   Number of files created on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int stm32_bbsraminitialize(char *devpath, int *sizes);

/****************************************************************************
* Function: stm32_bbsram_savepanic
*
* Description:
*   Saves the panic context in a previously allocated BBSRAM file
*
* Parameters:
*   fileno  - the value returned by the ioctl STM32F7_BBSRAM_GETDESC_IOCTL
*   context - Pointer to a any array of bytes to save
*   length  - The length of the data pointed to byt context
*
* Returned Value:
*   Length saved or negated errno.
*
* Assumptions:
*
****************************************************************************/

#if defined(CONFIG_STM32F7_SAVE_CRASHDUMP)
int stm32_bbsram_savepanic(int fileno, uint8_t *context, int length);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32F7_STM32_BBSRAM_H */
