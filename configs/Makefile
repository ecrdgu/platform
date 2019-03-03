-include $(TOPDIR)/Make.defs

BOARD_KCONFIG = $(TOPDIR)$(DELIM)configs$(DELIM)dummy$(DELIM)dummy_kconfig

# The board configuration should be installed in the arch/ directory

BOARD_DIR = $(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src$(DELIM)board
BOARD_INSTALLED = $(if $(wildcard $(BOARD_DIR)$(DELIM)Makefile),y,)

# Basic

CONFIG_ASRCS =
CONFIG_CSRCS =
CONFIG_CXXSRCS =

ASRCS = $(CONFIG_ASRCS)
AOBJS = $(ASRCS:.S=$(OBJEXT))

CSRCS = $(CONFIG_CSRCS)
COBJS = $(CSRCS:.c=$(OBJEXT))

CXXSRCS = $(CONFIG_CXXSRCS)
CXXOBJS = $(CXXSRCS:.cxx=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libconfigs$(LIBEXT)

all: $(BIN)
.PHONY: depend context clean_context clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cxx
	$(call COMPILEXX, $<, $@)

$(BIN): $(OBJS) $(CXXOBJS)
	$(call ARCHIVE, $@, $(OBJS) $(CXXOBJS))

.depend: Makefile $(SRCS) $(CXXSRCS)
ifneq ($(SRCS),)
	$(Q) $(MKDEP) --dep-path . "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
endif
ifneq ($(CXXSRCS),)
	$(Q) $(MKDEP) --dep-path . "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >>Make.dep
endif
	$(Q) touch $@

depend: .depend

dirlinks:

context:
ifeq ($(BOARD_INSTALLED),y)
	$(Q) $(MAKE) -C $(BOARD_DIR) TOPDIR="$(TOPDIR)" context
endif

clean_context:

clean: clean_context
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
