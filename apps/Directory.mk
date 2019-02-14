-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# Sub-directories

SUBDIRS = $(dir $(wildcard */Makefile))

all: nothing

.PHONY: nothing context depend clean distclean

define SDIR_template
$(1)_$(2):
	$(Q) cd $(1) && $(MAKE) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"
endef

$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),preconfig)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),context)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),depend)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),clean)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),distclean)))

nothing:

install:

preconfig: $(foreach SDIR, $(SUBDIRS), $(SDIR)_preconfig)
ifneq ($(MENUDESC),)
	$(Q) $(MKKCONFIG) -m $(MENUDESC)
endif

context: $(foreach SDIR, $(SUBDIRS), $(SDIR)_context)

depend: $(foreach SDIR, $(SUBDIRS), $(SDIR)_depend)

clean: $(foreach SDIR, $(SUBDIRS), $(SDIR)_clean)

distclean: $(foreach SDIR, $(SUBDIRS), $(SDIR)_distclean)
ifneq ($(MENUDESC),)
	$(call DELFILE, Kconfig)
endif

-include Make.dep
