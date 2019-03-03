
#ifndef __ARCH_ARM_INCLUDE_STDARG_H
#define __ARCH_ARM_INCLUDE_STDARG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* This should work with any modern gcc (newer than 3.4 or so) */

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef __builtin_va_list va_list;

#endif /* __ARCH_ARM_INCLUDE_STDARG_H */
