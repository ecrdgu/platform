
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <debug.h>

#include <ecr/board.h>
#include <ecr/lcd/lcd.h>
#include <ecr/lcd/ssd1306.h>
#include <ecr/i2c/i2c_master.h>

#include "stm32.h"
#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_LCD_SSD1306
#  error "The OLED driver requires CONFIG_LCD_SSD1306 in the configuration"
#endif

#ifndef CONFIG_LCD_MAXPOWER
#  define CONFIG_LCD_MAXPOWER 1
#endif

#define OLED_I2C_PORT         1 /* OLED display connected to I2C1 */

/****************************************************************************
 * Private Data
 ****************************************************************************/

FAR struct i2c_master_s *g_i2c;
FAR struct lcd_dev_s    *g_lcddev;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_lcd_initialize
 ****************************************************************************/

int board_lcd_initialize(void)
{
  /* Initialize I2C */

  g_i2c = stm32_i2cbus_initialize(OLED_I2C_PORT);
  if (!g_i2c)
    {
      lcderr("ERROR: Failed to initialize I2C port %d\n", OLED_I2C_PORT);
      return -ENODEV;
    }

  return OK;
}

/****************************************************************************
 * Name: board_lcd_getdev
 ****************************************************************************/

FAR struct lcd_dev_s *board_lcd_getdev(int devno)
{
  /* Bind the I2C port to the OLED */

  g_lcddev = ssd1306_initialize(g_i2c, NULL, devno);
  if (!g_lcddev)
    {
      lcderr("ERROR: Failed to bind SPI port 1 to OLED %d: %d\n", devno);
    }
  else
    {
      lcdinfo("Bound I2C port %d to OLED %d\n", OLED_I2C_PORT, devno);

      /* And turn the OLED on */

      (void)g_lcddev->setpower(g_lcddev, CONFIG_LCD_MAXPOWER);
      return g_lcddev;
    }

  return NULL;
}

/****************************************************************************
 * Name: board_lcd_uninitialize
 ****************************************************************************/

void board_lcd_uninitialize(void)
{
  /* TO-FIX */
}
