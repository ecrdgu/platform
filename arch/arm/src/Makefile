-include $(TOPDIR)/Make.defs
-include chip$(DELIM)Make.defs

ifeq ($(CONFIG_ARCH_CORTEXA5),y)        # Cortex-A5 is ARMv7-A
ARCH_SUBDIR = armv7-a
else ifeq ($(CONFIG_ARCH_CORTEXA8),y)   # Cortex-A8 is ARMv7-A
ARCH_SUBDIR = armv7-a
else ifeq ($(CONFIG_ARCH_CORTEXA9),y)   # Cortex-A9 is ARMv7-A
ARCH_SUBDIR = armv7-a
else ifeq ($(CONFIG_ARCH_CORTEXR4),y)   # Cortex-R4 is ARMv7-R
ARCH_SUBDIR = armv7-r
else ifeq ($(CONFIG_ARCH_CORTEXR4F),y)  # Cortex-R4F is ARMv7-R
ARCH_SUBDIR = armv7-r
else ifeq ($(CONFIG_ARCH_CORTEXR5),y)   # Cortex-R5 is ARMv7-R
ARCH_SUBDIR = armv7-r
else ifeq ($(CONFIG_ARCH_CORTEXR5F),y)  # Cortex-R5F is ARMv7-R
ARCH_SUBDIR = armv7-r
else ifeq ($(CONFIG_ARCH_CORTEXR7),y)   # Cortex-R7 is ARMv7-R
ARCH_SUBDIR = armv7-r
else ifeq ($(CONFIG_ARCH_CORTEXR7F),y)  # Cortex-R7F is ARMv7-R
ARCH_SUBDIR = armv7-r
else ifeq ($(CONFIG_ARCH_CORTEXM3),y)   # Cortex-M3 is ARMv7-M
ARCH_SUBDIR = armv7-m
else ifeq ($(CONFIG_ARCH_CORTEXM4),y)   # Cortex-M4 is ARMv7E-M
ARCH_SUBDIR = armv7-m
else ifeq ($(CONFIG_ARCH_CORTEXM7),y)   # Cortex-M4 is ARMv7E-M
ARCH_SUBDIR = armv7-m
else ifeq ($(CONFIG_ARCH_CORTEXM0),y)   # Cortex-M0 is ARMv6-M
ARCH_SUBDIR = armv6-m
else                                    # ARM9, ARM7TDMI
ARCH_SUBDIR = arm
endif

CPPFLAGS += $(EXTRADEFINES)
CFLAGS += $(EXTRADEFINES)
CXXFLAGS += $(EXTRADEFINES)

ARCH_SRCDIR = $(TOPDIR)/arch/$(CONFIG_ARCH)/src
OUTBIN = build/$(OUTPUT)$(EXEEXT)
CFLAGS += -I$(ARCH_SRCDIR)/chip
CFLAGS += -I$(ARCH_SRCDIR)/common
CFLAGS += -I$(ARCH_SRCDIR)/$(ARCH_SUBDIR)
CFLAGS += -I$(TOPDIR)/sched

# The "head" object

HEAD_OBJ = $(HEAD_ASRC:.S=$(OBJEXT))
STARTUP_OBJS ?= $(HEAD_OBJ)

# Flat build or kernel-mode objects

ASRCS = $(CHIP_ASRCS) $(CMN_ASRCS)
AOBJS = $(ASRCS:.S=$(OBJEXT))

CSRCS = $(CHIP_CSRCS) $(CMN_CSRCS)
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

# User-mode objects

UASRCS = $(CHIP_UASRCS) $(CMN_UASRCS)
UAOBJS = $(UASRCS:.S=$(OBJEXT))

UCSRCS = $(CHIP_UCSRCS) $(CMN_UCSRCS)
UCOBJS = $(UCSRCS:.c=$(OBJEXT))

USRCS = $(UASRCS) $(UCSRCS)
UOBJS = $(UAOBJS) $(UCOBJS)

KBIN = libkarch$(LIBEXT)
UBIN = libuarch$(LIBEXT)
BIN  = libarch$(LIBEXT)

LDFLAGS += $(ARCHSCRIPT)

# Override in Make.defs if linker is not 'ld'

LDSTARTGROUP ?= --start-group
LDENDGROUP ?= --end-group

EXTRA_LIBS ?=
EXTRA_LIBPATHS ?=
LINKLIBS ?=

BOARDMAKE = $(if $(wildcard ./board/Makefile),y,)
LIBPATHS += -L"$(TOPDIR)/build/libraries"
ifeq ($(BOARDMAKE),y)
  LIBPATHS += -L"$(TOPDIR)/arch/$(CONFIG_ARCH)/src/board"
endif

LDLIBS = $(patsubst %.a,%,$(patsubst lib%,-l%,$(LINKLIBS)))
ifeq ($(BOARDMAKE),y)
  LDLIBS += -lboard
endif

LIBGCC = "${shell "$(CC)" $(ARCHCPUFLAGS) -print-libgcc-file-name}"
GCC_LIBDIR := ${shell dirname $(LIBGCC)}

VPATH += chip
VPATH += common
VPATH += $(ARCH_SUBDIR)

ifeq ($(CONFIG_ARCH_TOOLCHAIN_IAR),y)
  VPATH += chip$(DELIM)iar
  VPATH += $(ARCH_SUBDIR)$(DELIM)iar
else # ifeq ($(CONFIG_ARCH_TOOLCHAIN_GNU),y)
  VPATH += chip$(DELIM)gnu
  VPATH += $(ARCH_SUBDIR)$(DELIM)gnu
endif


all: $(HEAD_OBJ) $(BIN)

.PHONY: board$(DELIM)libboard$(LIBEXT)

$(AOBJS) $(UAOBJS) $(HEAD_OBJ): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(UCOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN) $(KBIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

$(UBIN): $(UOBJS)
	$(call ARCHIVE, $@, $(UOBJS))

board$(DELIM)libboard$(LIBEXT):
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" libboard$(LIBEXT) EXTRADEFINES=$(EXTRADEFINES)

$(OUTBIN): $(HEAD_OBJ) board$(DELIM)libboard$(LIBEXT)
	$(Q) echo "LD: $(notdir $(@))"
	$(Q) $(LD) --entry=__start $(LDFLAGS) $(LIBPATHS) $(EXTRA_LIBPATHS) \
		-o $(TOPDIR)/$(@) $(HEAD_OBJ) $(EXTRA_OBJS) \
		$(LDSTARTGROUP) $(LDLIBS) $(EXTRA_LIBS) $(LIBGCC) $(LDENDGROUP)
	$(Q) $(NM) $(TOPDIR)/$(@) | \
	grep -v '\(compiled\)\|\(\$(OBJEXT)$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
	sort > $(TOPDIR)$(DELIM)build$(DELIM)System.map

# This is part of the top-level export target
# Note that there may not be a head object if layout is handled
# by the linker configuration.

export_startup: board$(DELIM)libboard$(LIBEXT) $(STARTUP_OBJS)
ifneq ($(STARTUP_OBJS),)
	$(Q) if [ -d "$(EXPORT_DIR)$(DELIM)startup" ]; then \
		cp -f $(STARTUP_OBJS) "$(EXPORT_DIR)$(DELIM)startup$(DELIM)."; \
	 else \
		echo "$(EXPORT_DIR)$(DELIM)startup does not exist"; \
	exit 1; \
	fi
endif

# Dependencies

.depend: Makefile chip$(DELIM)Make.defs $(SRCS)
ifeq ($(BOARDMAKE),y)
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" depend
endif
	$(Q) $(MKDEP) $(patsubst %,--dep-path %,$(subst :, ,$(VPATH))) \
	 "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
ifeq ($(BOARDMAKE),y)
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" clean
endif
	$(call DELFILE, $(KBIN))
	$(call DELFILE, $(UBIN))
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
ifeq ($(BOARDMAKE),y)
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" distclean
endif
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
