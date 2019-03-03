
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/mount.h>

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <debug.h>

#include <ecr/i2c/i2c_master.h>
#include <ecr/mtd/mtd.h>
#include <ecr/fs/nxffs.h>

#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

#ifdef HAVE_AT24

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_at24_automount
 *
 * Description:
 *   Initialize and configure the AT24 serial EEPROM
 *
 ****************************************************************************/

int stm32_at24_automount(int minor)
{
  FAR struct i2c_master_s *i2c;
  FAR struct mtd_dev_s *mtd;
  static bool initialized = false;
  int ret;

  /* Have we already initialized? */

  if (!initialized)
    {
      /* No.. Get the I2C bus driver */

      finfo("Initialize I2C%d\n", AT24_I2C_BUS);
      i2c = stm32_i2cbus_initialize(AT24_I2C_BUS);
      if (!i2c)
        {
          ferr("ERROR: Failed to initialize I2C%d\n", AT24_I2C_BUS);
          return -ENODEV;
        }

      /* Now bind the I2C interface to the AT24 I2C EEPROM driver */

      finfo("Bind the AT24 EEPROM driver to I2C%d\n", AT24_I2C_BUS);
      mtd = at24c_initialize(i2c);
      if (!mtd)
        {
          ferr("ERROR: Failed to bind TWI%d to the AT24 EEPROM driver\n",
               AT24_I2C_BUS);
          return -ENODEV;
        }

#if defined(CONFIG_STM32F103MINIMUM_AT24_FTL)
      /* And finally, use the FTL layer to wrap the MTD driver as a block driver */

      finfo("Initialize the FTL layer to create /dev/mtdblock%d\n", AT24_MINOR);
      ret = ftl_initialize(AT24_MINOR, mtd);
      if (ret < 0)
        {
          ferr("ERROR: Failed to initialize the FTL layer: %d\n", ret);
          return ret;
        }

#elif defined(CONFIG_STM32F103MINIMUM_AT24_NXFFS)
      /* Initialize to provide NXFFS on the MTD interface */

      finfo("Initialize the NXFFS file system\n");
      ret = nxffs_initialize(mtd);
      if (ret < 0)
        {
          ferr("ERROR: NXFFS initialization failed: %d\n", ret);
          return ret;
        }

      /* Mount the file system at /mnt/at24 */

      finfo("Mount the NXFFS file system at /dev/at24\n");
      ret = mount(NULL, "/mnt/at24", "nxffs", 0, NULL);
      if (ret < 0)
        {
          ferr("ERROR: Failed to mount the NXFFS volume: %d\n", errno);
          return ret;
        }
#endif
      /* Now we are initialized */

      initialized = true;
    }

  return OK;
}

#endif /* HAVE_AT24 */
