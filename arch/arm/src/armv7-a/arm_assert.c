
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <debug.h>

#include <ecr/irq.h>
#include <ecr/arch.h>
#include <ecr/board.h>
#include <ecr/syslog/syslog.h>
#include <ecr/usb/usbdev_trace.h>

#include <arch/board/board.h>

#include "up_arch.h"
#include "sched/sched.h"
#include "irq/irq.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* USB trace dumping */

#ifndef CONFIG_USBDEV_TRACE
#  undef CONFIG_ARCH_USBDUMP
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_getsp
 ****************************************************************************/

/* I don't know if the builtin to get SP is enabled */

static inline uint32_t up_getsp(void)
{
  uint32_t sp;
  __asm__
  (
    "\tmov %0, sp\n\t"
    : "=r"(sp)
  );
  return sp;
}

/****************************************************************************
 * Name: up_stackdump
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static void up_stackdump(uint32_t sp, uint32_t stack_base)
{
  uint32_t stack ;

  for (stack = sp & ~0x1f; stack < stack_base; stack += 32)
    {
      uint32_t *ptr = (uint32_t *)stack;
      _alert("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
             stack, ptr[0], ptr[1], ptr[2], ptr[3],
             ptr[4], ptr[5], ptr[6], ptr[7]);
    }
}
#else
#  define up_stackdump(sp,stack_base)
#endif

/****************************************************************************
 * Name: up_taskdump
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
static void up_taskdump(FAR struct tcb_s *tcb, FAR void *arg)
{
  /* Dump interesting properties of this task */

#if CONFIG_TASK_NAME_SIZE > 0
  _alert("%s: PID=%d Stack Used=%lu of %lu\n",
        tcb->name, tcb->pid, (unsigned long)up_check_tcbstack(tcb),
        (unsigned long)tcb->adj_stack_size);
#else
  _alert("PID: %d Stack Used=%lu of %lu\n",
        tcb->pid, (unsigned long)up_check_tcbstack(tcb),
        (unsigned long)tcb->adj_stack_size);
#endif
}
#endif

/****************************************************************************
 * Name: up_showtasks
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
static inline void up_showtasks(void)
{
  /* Dump interesting properties of each task in the crash environment */

  sched_foreach(up_taskdump, NULL);
}
#else
#  define up_showtasks()
#endif

/****************************************************************************
 * Name: up_registerdump
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static inline void up_registerdump(void)
{
  /* Are user registers available from interrupt processing? */

  if (CURRENT_REGS)
    {
      int regs;

      /* Yes.. dump the interrupt registers */

      for (regs = REG_R0; regs <= REG_R15; regs += 8)
        {
          uint32_t *ptr = (uint32_t *)&CURRENT_REGS[regs];
          _alert("R%d: %08x %08x %08x %08x %08x %08x %08x %08x\n",
                 regs, ptr[0], ptr[1], ptr[2], ptr[3],
                 ptr[4], ptr[5], ptr[6], ptr[7]);
        }

      _alert("CPSR: %08x\n", CURRENT_REGS[REG_CPSR]);
    }
}
#else
# define up_registerdump()
#endif

/****************************************************************************
 * Name: assert_tracecallback
 ****************************************************************************/

#ifdef CONFIG_ARCH_USBDUMP
static int usbtrace_syslog(FAR const char *fmt, ...)
{
  va_list ap;
  int ret;

  /* Let nx_vsyslog do the real work */

  va_start(ap, fmt);
  ret = nx_vsyslog(LOG_EMERG, fmt, &ap);
  va_end(ap);
  return ret;
}

static int assert_tracecallback(FAR struct usbtrace_s *trace, FAR void *arg)
{
  usbtrace_trprintf(usbtrace_syslog, trace->event, trace->value);
  return 0;
}
#endif

/****************************************************************************
 * Name: up_dumpstate
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static void up_dumpstate(void)
{
  struct tcb_s *rtcb = this_task();
  uint32_t sp   = up_getsp();
  uint32_t ustackbase;
  uint32_t ustacksize;
#if CONFIG_ARCH_INTERRUPTSTACK > 3
  uint32_t istackbase;
  uint32_t istacksize;
#endif
#ifdef CONFIG_ARCH_KERNEL_STACK
  uint32_t kstackbase = 0;
#endif

  /* Get the limits on the user stack memory */

  if (rtcb->pid == 0)
    {
      ustackbase = g_idle_topstack - 4;
      ustacksize = CONFIG_IDLETHREAD_STACKSIZE;
    }
  else
    {
      ustackbase = (uint32_t)rtcb->adj_stack_ptr;
      ustacksize = (uint32_t)rtcb->adj_stack_size;
    }

  _alert("Current sp: %08x\n", sp);

#if CONFIG_ARCH_INTERRUPTSTACK > 3
  /* Get the limits on the interrupt stack memory */

  istackbase = (uint32_t)&g_intstackbase;
  istacksize = (CONFIG_ARCH_INTERRUPTSTACK & ~3);

  /* Show interrupt stack info */

  _alert("Interrupt stack:\n");
  _alert("  base: %08x\n", istackbase);
  _alert("  size: %08x\n", istacksize);
#ifdef CONFIG_STACK_COLORATION
  _alert("  used: %08x\n", up_check_intstack());
#endif
#endif

  /* Show user stack info */

  _alert("User stack:\n");
  _alert("  base: %08x\n", ustackbase);
  _alert("  size: %08x\n", ustacksize);
#ifdef CONFIG_STACK_COLORATION
  _alert("  used: %08x\n", up_check_tcbstack(rtcb));
#endif

#ifdef CONFIG_ARCH_KERNEL_STACK
  /* This this thread have a kernel stack allocated? */

  if (rtcb->xcp.kstack)
    {
      kstackbase = (uint32_t)rtcb->xcp.kstack + CONFIG_ARCH_KERNEL_STACKSIZE - 4;

      _alert("Kernel stack:\n");
      _alert("  base: %08x\n", kstackbase);
      _alert("  size: %08x\n", CONFIG_ARCH_KERNEL_STACKSIZE);
    }
#endif

#if CONFIG_ARCH_INTERRUPTSTACK > 3
  /* Does the current stack pointer lie within the interrupt stack? */

  if (sp > istackbase - istacksize && sp < istackbase)
    {
      /* Yes.. dump the interrupt stack */

      _alert("Interrupt Stack\n", sp);
      up_stackdump(sp, istackbase);

      /* Extract the user stack pointer which should lie
       * at the base of the interrupt stack.
       */

      sp = g_intstackbase;
      _alert("User sp: %08x\n", sp);
    }
#endif

  /* Dump the user stack if the stack pointer lies within the allocated user
   * stack memory.
   */

  if (sp > ustackbase - ustacksize && sp < ustackbase)
    {
      _alert("User Stack\n", sp);
      up_stackdump(sp, ustackbase);
    }

#ifdef CONFIG_ARCH_KERNEL_STACK
  /* Dump the user stack if the stack pointer lies within the allocated
   * kernel stack memory.
   */

  if (sp >= (uint32_t)rtcb->xcp.kstack && sp < kstackbase)
    {
      _alert("Kernel Stack\n", sp);
      up_stackdump(sp, kstackbase);
    }
#endif

#ifdef CONFIG_SMP
  /* Show the CPU number */

  _alert("CPU%d:\n", up_cpu_index());
#endif

  /* Then dump the CPU registers (if available) */

  up_registerdump();

  /* Dump the state of all tasks (if available) */

  up_showtasks();

#ifdef CONFIG_ARCH_USBDUMP
  /* Dump USB trace data */

  (void)usbtrace_enumerate(assert_tracecallback, NULL);
#endif
}
#else
# define up_dumpstate()
#endif

/****************************************************************************
 * Name: _up_assert
 ****************************************************************************/

static void _up_assert(int errorcode) noreturn_function;
static void _up_assert(int errorcode)
{
  /* Are we in an interrupt handler or the idle task? */

  if (CURRENT_REGS || this_task()->pid == 0)
    {
      /* Disable interrupts on this CPU */

      (void)up_irq_save();

      for (; ; )
        {
#ifdef CONFIG_SMP
          /* Try (again) to stop activity on other CPUs */

          (void)spin_trylock(&g_cpu_irqlock);
#endif

#ifdef CONFIG_ARCH_LEDS
          /* FLASH LEDs a 2Hz */

          board_autoled_on(LED_PANIC);
          up_mdelay(250);
          board_autoled_off(LED_PANIC);
          up_mdelay(250);
#endif
        }
    }
  else
    {
      exit(errorcode);
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_assert
 ****************************************************************************/

void up_assert(const uint8_t *filename, int lineno)
{
#if CONFIG_TASK_NAME_SIZE > 0 && defined(CONFIG_DEBUG_ALERT)
  struct tcb_s *rtcb = this_task();
#endif
  board_autoled_on(LED_ASSERTION);

#if CONFIG_TASK_NAME_SIZE > 0
  _alert("Assertion failed at file:%s line: %d task: %s\n",
        filename, lineno, rtcb->name);
#else
  _alert("Assertion failed at file:%s line: %d\n",
        filename, lineno);
#endif
  up_dumpstate();

#ifdef CONFIG_BOARD_CRASHDUMP
  board_crashdump(up_getsp(), this_task(), filename, lineno);
#endif

  _up_assert(EXIT_FAILURE);
}
