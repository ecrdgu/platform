
include $(APPDIR)/Make.defs

# If this is an executable program (with MAINSRC), we must build it as a
# loadable module for the KERNEL build (always) or if the tristate module
# has the value "m"

ifneq ($(MAINSRC),)
  ifeq ($($(MODULE)),m)
    BUILD_MODULE = y
  endif
endif

ifeq ($(CONFIG_BUILD_KERNEL),y)
  BUILD_MODULE = y
endif

# Pass the definition to the C/C++ code via the CFLAGS/CXXFLAGS

ifeq ($(BUILD_MODULE),y)
  CFLAGS += ${shell $(DEFINE) "$(CC)" BUILD_MODULE}
  CXXFLAGS += ${shell $(DEFINE) "$(CC)" BUILD_MODULE}
endif

# File extensions

CXXEXT ?= .cxx

# Object files

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:$(CXXEXT)=$(OBJEXT))

ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
  MAINOBJ = $(MAINSRC:$(CXXEXT)=$(OBJEXT))
else
  MAINOBJ = $(MAINSRC:.c=$(OBJEXT))
endif

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS) $(CXXOBJS)

ifneq ($(BUILD_MODULE),y)
  OBJS += $(MAINOBJ)
endif

# Module install directory

BIN = $(APPDIR)$(DELIM)libapps$(LIBEXT)
INSTALL_DIR = $(BIN_DIR)

ROOTDEPPATH += --dep-path .

VPATH += :.

# Targets follow

all:: .built
.PHONY: clean preconfig depend distclean
.PRECIOUS: $(APPDIR)/libapps$(LIBEXT)

ifneq ($(CONFIG_BUILD_LOADABLE),y)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)

ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
$(MAINOBJ): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)
else
$(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
endif

else

define ELFASSEMBLE
	@echo "AS: $1"
	$(Q) $(CC) -c $(AELFFLAGS) $1 -o $2
endef

define ELFCOMPILE
	@echo "CC: $1"
	$(Q) $(CC) -c $(CELFFLAGS) $1 -o $2
endef

define ELFCOMPILEXX
	@echo "CXX: $1"
	$(Q) $(CXX) -c $(CXXELFFLAGS) $1 -o $2
endef

$(AOBJS): %$(OBJEXT): %.S
	$(if $(AELFFLAGS), \
			$(call ELFASSEMBLE, $<, $@), \
			$(call ASSEMBLE, $<, $@) \
	)

$(COBJS): %$(OBJEXT): %.c
	$(if $(CELFFLAGS), \
			$(call ELFCOMPILE, $<, $@), \
			$(call COMPILE, $<, $@) \
	)

$(CXXOBJS): %$(OBJEXT): %$(CXXEXT)
	$(if $(CXXELFFLAGS), \
			$(call ELFCOMPILEXX, $<, $@), \
			$(call COMPILEXX, $<, $@) \
	)

ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
$(MAINOBJ): %$(OBJEXT): %$(CXXEXT)
	$(if $(CXXELFFLAGS), \
			$(call ELFCOMPILEXX, $<, $@), \
			$(call COMPILEXX, $<, $@) \
	)
else
$(MAINOBJ): %$(OBJEXT): %.c
	$(if $(CELFFLAGS), \
			$(call ELFCOMPILE, $<, $@), \
			$(call COMPILE, $<, $@) \
	)
endif

endif

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch $@

ifeq ($(BUILD_MODULE), y)

  PROGPRFX = $(INSTALL_DIR)$(DELIM)

PROGLIST := $(addprefix $(PROGPRFX),$(PROGNAME))
PROGOBJ := $(MAINOBJ)

$(PROGLIST): $(MAINOBJ) $(OBJS)
ifneq ($(PROGOBJ),)
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) $(ARCHCRT0OBJ) $(firstword $(PROGOBJ)) $(LDLIBS) -o $(strip $(firstword $(PROGLIST)))_
	$(Q) $(NM) -u $(strip $(firstword $(PROGLIST)))_
	$(Q) install -m 0755 -D $(strip $(firstword $(PROGLIST)))_ $(firstword $(PROGLIST))
	$(call DELFILE, $(strip $(firstword $(PROGLIST)))_)
#	$(Q) $(STRIP) $(firstword $(PROGLIST))
	$(eval PROGLIST=$(filter-out $(firstword $(PROGLIST)),$(PROGLIST)))
	$(eval PROGOBJ=$(filter-out $(firstword $(PROGOBJ)),$(PROGOBJ)))
endif

install:: $(PROGLIST)

else
install::

endif # BUILD_MODULE

preconfig::

ifeq ($(CONFIG_NSH_BUILTIN_APPS),y)
ifneq ($(BUILD_MODULE),y)
REGLIST := $(addprefix $(BUILTIN_REGISTRY)$(DELIM),$(APPNAME)_main.bdat)
APPLIST := $(APPNAME)

ifneq ($(APPNAME),)
ifneq ($(PRIORITY),)
ifneq ($(STACKSIZE),)
$(REGLIST): $(DEPCONFIG) Makefile
	$(call REGISTER,$(firstword $(APPLIST)),$(firstword $(PRIORITY)),$(firstword $(STACKSIZE)),$(if $(BUILD_MODULE),,$(firstword $(APPLIST))_main))
	$(eval APPLIST=$(filter-out $(firstword $(APPLIST)),$(APPLIST)))
	$(if $(filter-out $(firstword $(PRIORITY)),$(PRIORITY)),$(eval PRIORITY=$(filter-out $(firstword $(PRIORITY)),$(PRIORITY))))
	$(if $(filter-out $(firstword $(STACKSIZE)),$(STACKSIZE)),$(eval STACKSIZE=$(filter-out $(firstword $(STACKSIZE)),$(STACKSIZE))))

context:: $(REGLIST)
else
context::
endif
else
context::
endif
else
context::
endif
else
context::
endif
else
context::
endif

.depend: Makefile $(SRCS)
ifeq ($(filter %$(CXXEXT),$(SRCS)),)
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(filter-out Makefile,$^) >Make.dep
else
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(filter-out Makefile,$^) >Make.dep
endif
	$(Q) touch $@

depend:: .depend

clean::
	$(call DELFILE, .built)
	$(call CLEAN)

distclean:: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
