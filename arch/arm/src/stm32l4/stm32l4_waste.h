
#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_WASTE_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_WASTE_H

/* Waste CPU Time */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/** Waste CPU Time
 *
 *   up_waste() is the logic that will be executed when portions of kernel
 *   or user-app is polling some register or similar, waiting for desired
 *   status. This time is wasted away. This function offers a measure of
 *   badly written piece of software or some undesired behavior.
 *
 *   At the same time this function adds to some IDLE time which portion
 *   cannot be used for other purposes (yet).
 **/

void up_waste(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_WASTE_H */
