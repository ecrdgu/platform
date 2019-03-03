
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>

#include <ecr/arch.h>

#include "cp15.h"
#include "sctlr.h"

#ifdef CONFIG_SMP

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_cpu_index
 *
 * Description:
 *   Return an index in the range of 0 through (CONFIG_SMP_NCPUS-1) that
 *   corresponds to the currently executing CPU.
 *
 *   If TLS is enabled, then the RTOS can get this information from the TLS
 *   info structure.  Otherwise, the MCU-specific logic must provide some
 *   mechanism to provide the CPU index.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   An integer index in the range of 0 through (CONFIG_SMP_NCPUS-1) that
 *   corresponds to the currently executing CPU.
 *
 ****************************************************************************/

int up_cpu_index(void)
{
   /* Read the Multiprocessor Affinity Register (MPIDR) */

  uint32_t mpidr = cp15_rdmpidr();

  /* And return the CPU ID field */

  return (mpidr & MPIDR_CPUID_MASK) >> MPIDR_CPUID_SHIFT;
}

#endif /* CONFIG_SMP */
