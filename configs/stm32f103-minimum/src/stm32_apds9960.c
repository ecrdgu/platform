
/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <errno.h>
#include <debug.h>

#include <ecr/spi/spi.h>
#include <ecr/sensors/apds9960.h>

#include "stm32.h"
#include "stm32_i2c.h"
#include "stm32f103_minimum.h"

#if defined(CONFIG_I2C) && defined(CONFIG_SENSORS_APDS9960)

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define APDS9960_I2C_PORTNO 1   /* On I2C1 */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

struct stm32_apds9960config_s
{
  /* Configuration structure as seen by the APDS-9960 driver */

  struct apds9960_config_s config;

  /* Additional private definitions only known to this driver */

  FAR void *arg;  /* Argument to pass to the interrupt handler */
  FAR xcpt_t isr; /* ISR Handler */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

static int  apds9960_irq_attach(FAR struct apds9960_config_s *state, xcpt_t isr,
                                FAR void *arg);

/************************************************************************************
 * Private Data
 ************************************************************************************/

/* A reference to a structure of this type must be passed to the APDS-9960
 * driver.  This structure provides information about the configuration
 * of the APDS-9960 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active. The
 * memory must be writable because, under certain circumstances, the driver
 * may modify frequency or X plate resistance values.
 */

static struct stm32_apds9960config_s g_apds9960config =
{
  .config =
  {
    .irq_attach  = apds9960_irq_attach,
  },
};

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/* Attach the APDS-9960 interrupt handler to the GPIO interrupt */

static int apds9960_irq_attach(FAR struct apds9960_config_s *state, xcpt_t isr,
                               FAR void *arg)
{
  irqstate_t flags;

  sninfo("apds9960_irq_attach\n");

  flags = enter_critical_section();

  /* Setup interrupt for Falling Edge */

  (void)stm32_gpiosetevent(GPIO_APDS9960_INT, false, true, true, isr, arg);

  leave_critical_section(flags);

  return OK;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_apds9960initialize
 *
 * Description:
 *   Initialize and register the APDS9960 gesture sensor.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/gest0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ************************************************************************************/

int stm32_apds9960initialize(FAR const char *devpath)
{
  FAR struct i2c_master_s *i2c;
  int ret;

  sninfo("Initializing APDS9960!\n");

  /* Configure the GPIO interrupt */

  stm32_configgpio(GPIO_APDS9960_INT);

  /* Initialize I2C */

  i2c = stm32_i2cbus_initialize(APDS9960_I2C_PORTNO);
  if (i2c == NULL)
    {
      return -ENODEV;
    }

  /* Save this i2c in the config */

  g_apds9960config.config.i2c_dev  = i2c;
  g_apds9960config.config.i2c_addr = APDS9960_I2C_ADDR;

  /* Then register the gesture sensor */

  ret = apds9960_register(devpath, &g_apds9960config.config);
  if (ret < 0)
    {
      snerr("ERROR: Failed registering APDS-9960!\n");
    }

  return ret;
}

#endif /* CONFIG_I2C && CONFIG_SENSORS_APDS9960 && CONFIG_STM32_I2C1 */
