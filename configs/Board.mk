-include $(TOPDIR)/Make.defs

ifneq ($(ZDSVERSION),)
AOBJS = $(ASRCS:.S=$(OBJEXT))
else
AOBJS = $(ASRCS:$(ASMEXT)=$(OBJEXT))
endif
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:.cxx=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

SCHEDSRCDIR = $(TOPDIR)$(DELIM)sched
ARCHSRCDIR = $(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src
ifneq ($(CONFIG_ARCH_FAMILY),)
  ARCH_FAMILY = $(patsubst "%",%,$(CONFIG_ARCH_FAMILY))
endif

ifneq ($(ZDSVERSION),)
USRINCLUDES = -usrinc:".;$(SCHEDSRCDIR);$(ARCHSRCDIR);$(ARCHSRCDIR)\common"
else
CFLAGS += -I$(SCHEDSRCDIR)
CFLAGS += -I$(ARCHSRCDIR)$(DELIM)chip
CFLAGS += -I$(ARCHSRCDIR)$(DELIM)common
ifneq ($(ARCH_FAMILY),)
  CFLAGS += -I$(ARCHSRCDIR)$(DELIM)$(ARCH_FAMILY)
endif
endif

ifneq ($(ZDSVERSION),)
INCLUDES = $(ARCHSTDINCLUDES) $(USRINCLUDES)
CFLAGS = $(ARCHWARNINGS) $(ARCHOPTIMIZATION) $(ARCHCPUFLAGS) $(INCLUDES) $(ARCHDEFINES) $(EXTRADEFINES)
endif

all: libboard$(LIBEXT)

ifneq ($(ZDSVERSION),)
$(ASRCS) $(HEAD_ASRC): %$(ASMEXT): %.S
	$(Q) $(CPP) $(CPPFLAGS) $< -o $@.tmp
	$(Q) cat $@.tmp | sed -e "s/^#/;/g" > $@
	$(Q) rm $@.tmp
endif

$(AOBJS): %$(OBJEXT): %$(ASMEXT)
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(LINKOBJS): %$(OBJEXT): %.cxx
	$(call COMPILEXX, $<, $@)

libboard$(LIBEXT): $(OBJS) $(CXXOBJS)
	$(Q) $(AR) $@
ifneq ($(OBJS),)
	$(call ARCHIVE, $@, $(OBJS) $(CXXOBJS))
endif

.depend: Makefile $(SRCS) $(CXXSRCS)
ifneq ($(ZDSVERSION),)
	$(Q) $(MKDEP) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
else
	$(Q) $(MKDEP) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
endif
ifneq ($(CXXSRCS),)
	$(Q) $(MKDEP) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >>Make.dep
endif
	$(Q) touch $@

depend: .depend

ifneq ($(BOARD_CONTEXT),y)
context:
endif

clean:
	$(call DELFILE, libboard$(LIBEXT))
	$(call CLEAN)
	$(EXTRA_CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
	$(EXTRA_DISTCLEAN)

-include Make.dep
