
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>

#include <ecr/i2c/i2c_master.h>
#include <ecr/sensors/lm75.h>

#include "stm32.h"
#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_I2C) && defined(CONFIG_LM75_I2C) && defined(CONFIG_STM32_I2C1)

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_lm75initialize
 *
 * Description:
 *   Initialize and register the LM-75 Temperature Sensor driver.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/temp0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_lm75initialize(FAR const char *devpath)
{
  FAR struct i2c_master_s *i2c;
  int ret;

  /* Configure PB.5 as Input pull-up.  This pin can be used as a temperature
   * sensor interrupt (not fully implemented).
   */

  stm32_configgpio(GPIO_LM75_OSINT);

  /* Get an instance of the I2C1 interface */

  i2c =  stm32_i2cbus_initialize(1);
  if (!i2c)
    {
      return -ENODEV;
    }

  /* Then register the temperature sensor */

  ret = lm75_register(devpath, i2c, 0x48);
  if (ret < 0)
    {
      (void)stm32_i2cbus_uninitialize(i2c);
    }

  return ret;
}

/************************************************************************************
 * Name: stm32_lm75attach
 *
 * Description:
 *   Attach the LM-75 interrupt handler
 *
 * Input Parameters:
 *   irqhandler - the LM-75 interrupt handler
 *   arg        - The argument that will accompany the interrupt
 *
 * Returned Value:
 *   Zero (OK) returned on success; a negated errno value is returned on failure.
 *
 ************************************************************************************/

int stm32_lm75attach(xcpt_t irqhandler, void *arg)
{
  (void)stm32_gpiosetevent(GPIO_LM75_OSINT, true, true, true, irqhandler, arg);
  return OK;
}

#endif /* CONFIG_I2C && CONFIG_LM75_I2C && CONFIG_STM32_I2C1 */

