
TOPDIR ?= $(APPDIR)/import
-include $(TOPDIR)/Make.defs

APPDIR = ${shell pwd}

-include $(APPDIR)/Make.defs

# Application Directories

# BUILDIRS is the list of top-level directories containing Make.defs files
# CLEANDIRS is the list of all top-level directories containing Makefiles.
#   It is used only for cleaning.

BUILDIRS   := $(dir $(filter-out import/Make.defs,$(wildcard */Make.defs)))
CLEANDIRS  := $(dir $(wildcard */Makefile))

# CONFIGURED_APPS is the application directories that should be built in
#   the current configuration.

CONFIGURED_APPS =

define Add_Application
  include $(1)Make.defs
endef

$(foreach BDIR, $(BUILDIRS), $(eval $(call Add_Application,$(BDIR))))

# Library path

LIBPATH ?= $(TOPDIR)$(DELIM)build$(DELIM)libraries

# The install path

EXE_DIR = $(APPDIR)$(DELIM)exe
BIN_DIR = $(EXE_DIR)$(DELIM)system$(DELIM)bin

# The final build target

BIN = libapps$(LIBEXT)

# Symbol table for loadable apps.

SYMTABSRC = $(EXE_DIR)$(DELIM)symtab_apps.c
SYMTABOBJ = $(SYMTABSRC:.c=$(OBJEXT))

# Build targets

all: $(BIN)
.PHONY: import install dirlinks context context_serialize clean_context context_rest .depdirs preconfig depend clean distclean
.PRECIOUS: libapps$(LIBEXT)

define MAKE_template
	$(Q) cd $(1) && $(MAKE) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" BIN_DIR="$(BIN_DIR)"

endef

define SDIR_template
$(1)_$(2):
	$(Q) cd $(1) && $(MAKE) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" BIN_DIR="$(BIN_DIR)"

endef

$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),all)))
$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),install)))
$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),context)))
$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),depend)))
$(foreach SDIR, $(CLEANDIRS), $(eval $(call SDIR_template,$(SDIR),clean)))
$(foreach SDIR, $(CLEANDIRS), $(eval $(call SDIR_template,$(SDIR),distclean)))

# In the KERNEL build, we must build and install all of the modules.  No
# symbol table is needed

ifeq ($(CONFIG_BUILD_KERNEL),y)

.install: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_install)

install: $(BIN_DIR) .install

$(BIN_DIR):
	$(Q) mkdir -p $(BIN_DIR)

.import: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_all)
	$(Q) $(MAKE) install TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"

import: $(BIN_DIR)
	$(Q) $(MAKE) .import TOPDIR="$(APPDIR)$(DELIM)import"

else

# In FLAT and protected modes, the modules have already been created.  A
# symbol table is required.

ifeq ($(CONFIG_BUILD_LOADABLE),)

$(BIN): $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_all)

else

$(SYMTABSRC): $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_all)
	$(Q) $(MAKE) install TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"
	$(Q) $(APPDIR)$(DELIM)tools$(DELIM)mksymtab.sh $(EXE_DIR)$(DELIM)system $(SYMTABSRC)

$(SYMTABOBJ): %$(OBJEXT): %.c
	$(call COMPILE, -fno-lto $<, $@)

$(BIN): $(SYMTABOBJ)
	$(call ARCHIVE, $(BIN), $^)
endif # !CONFIG_BUILD_KERNEL && CONFIG_BUILD_LOADABLE

.install: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_install)

$(BIN_DIR):
	$(Q) mkdir -p $(BIN_DIR)

install: $(BIN_DIR) .install

.import: $(BIN) install

import:
	$(Q) $(MAKE) .import TOPDIR="$(APPDIR)$(DELIM)import"

endif # CONFIG_BUILD_KERNEL

dirlinks:

context_rest: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_context)

context_serialize:
	$(Q) $(MAKE) -C builtin context TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"
	$(Q) $(MAKE) context_rest

context: context_serialize

Kconfig:
	$(foreach SDIR, $(BUILDIRS), $(call MAKE_template,$(SDIR),preconfig))
	$(Q) $(MKKCONFIG)

preconfig: Kconfig

.depdirs: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_depend)

.depend: context Makefile .depdirs
	$(Q) touch $@

depend: .depend

clean_context:

clean: $(foreach SDIR, $(CLEANDIRS), $(SDIR)_clean)
	$(call DELFILE, $(SYMTABSRC))
	$(call DELFILE, $(SYMTABOBJ))
	$(call DELFILE, $(BIN))
	$(call DELFILE, Kconfig)
	$(call DELDIR, $(BIN_DIR))
	$(call DELDIR, $(EXE_DIR))
	$(call CLEAN)

distclean: $(foreach SDIR, $(CLEANDIRS), $(SDIR)_distclean)
	$(Q) ( if [ -e external ]; then \
		echo "********************************************************"; \
		echo "* The external directory/link must be removed manually *"; \
		echo "********************************************************"; \
	   fi; \
	)
	$(call DELFILE, .depend)
	$(call DELFILE, $(SYMTABSRC))
	$(call DELFILE, $(SYMTABOBJ))
	$(call DELFILE, $(BIN))
	$(call DELFILE, Kconfig)
	$(call DELDIR, $(BIN_DIR))
	$(call DELDIR, $(EXE_DIR))
	$(call CLEAN)
