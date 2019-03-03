TOPDIR := ${shell pwd | sed -e 's/ /\\ /g'}

-include $(TOPDIR)/.config
include $(TOPDIR)/tools/Config.mk
-include $(TOPDIR)/Make.defs

# Control build verbosity
#
#  V=1,2: Enable echo of commands
#  V=2:   Enable bug/verbose options in tools and scripts

ifeq ($(V),1)
export Q :=
else
ifeq ($(V),2)
export Q :=
else
export Q := @
endif
endif

# Default tools

ifeq ($(DIRLINK),)
DIRLINK = $(TOPDIR)/tools/link.sh
DIRUNLINK = $(TOPDIR)/tools/unlink.sh
endif

# Process architecture and board-specific directories

ARCH_DIR = arch/$(CONFIG_ARCH)
ARCH_SRC = $(ARCH_DIR)/src
ARCH_INC = $(ARCH_DIR)/include

# Add-on directories.  These may or may not be in place in the
# Project source tree (they must be specifically installed)

include tools/Directories.mk

# This is the name of the final target (relative to the top level directorty)

OUTPUTPATH = build
BIN      = $(OUTPUTPATH)/$(OUTPUT)$(EXEEXT)

all: $(BIN)
.PHONY: dirlinks context clean_context check_context config oldconfig menuconfig nconfig qconfig gconfig subdir_clean clean subdir_distclean distclean

# Targets used to build include/ecr/version.h.  Creation of version.h is
# part of the overall NuttX configuration sequence. Notice that the
# tools/mkversion tool is built and used to create include/ecr/version.h

tools/mkversion$(HOSTEXEEXT):
	$(Q) $(MAKE) -C tools -f Makefile.host TOPDIR="$(TOPDIR)"  mkversion$(HOSTEXEEXT)

$(TOPDIR)/.version:
	$(Q) if [ ! -f .version ]; then \
		echo "No .version file found, creating one"; \
		tools/version.sh -v 0.0 -b 0 .version; \
		chmod 755 .version; \
	fi

include/ecr/version.h: $(TOPDIR)/.version tools/mkversion$(HOSTEXEEXT)
	$(Q) tools/mkversion $(TOPDIR) > include/ecr/version.h

# Targets used to build include/ecr/config.h.  Creation of config.h is
# part of the overall NuttX configuration sequence. Notice that the
# tools/mkconfig tool is built and used to create include/ecr/config.h

tools/mkconfig$(HOSTEXEEXT):
	$(Q) $(MAKE) -C tools -f Makefile.host TOPDIR="$(TOPDIR)"  mkconfig$(HOSTEXEEXT)

include/ecr/config.h: $(TOPDIR)/.config tools/mkconfig$(HOSTEXEEXT)
	$(Q) tools/mkconfig $(TOPDIR) > include/ecr/config.h

# Targets used to create dependencies

tools/mkdeps$(HOSTEXEEXT):
	$(Q) $(MAKE) -C tools -f Makefile.host TOPDIR="$(TOPDIR)" mkdeps$(HOSTEXEEXT)

# dirlinks, and helpers
#
# Directories links.  Most of establishing the NuttX configuration involves
# setting up symbolic links with 'generic' directory names to specific,
# configured directories.

# Link the arch/<arch-name>/include directory to include/arch

include/arch: .config
	@echo "LN: include/arch to $(ARCH_DIR)/include"
	$(Q) $(DIRLINK) $(TOPDIR)/$(ARCH_DIR)/include include/arch

# Link arch/<arch-name>/include/<chip-name> to arch/<arch-name>/include/chip

$(ARCH_SRC)/chip: .config
ifneq ($(CONFIG_ARCH_CHIP),)
	@echo "LN: $(ARCH_SRC)/chip to $(ARCH_SRC)/$(CONFIG_ARCH_CHIP)"
	$(Q) $(DIRLINK) $(TOPDIR)/$(ARCH_SRC)/$(CONFIG_ARCH_CHIP) $(ARCH_SRC)/chip
endif

# Link arch/<arch-name>/src/<chip-name> to arch/<arch-name>/src/chip

include/arch/chip: include/arch
ifneq ($(CONFIG_ARCH_CHIP),)
	@echo "LN: include/arch/chip to $(ARCH_INC)/$(CONFIG_ARCH_CHIP)"
	$(Q) $(DIRLINK) $(TOPDIR)/$(ARCH_INC)/$(CONFIG_ARCH_CHIP) include/arch/chip
endif

dirlinks: include/arch include/arch/chip $(ARCH_SRC)/chip
	$(Q) $(MAKE) -C apps dirlinks TOPDIR="$(TOPDIR)"

# context
#
# The context target is invoked on each target build to assure that NuttX is
# properly configured.  The basic configuration steps include creation of the
# the config.h and version.h header files in the include/nuttx directory and
# the establishment of symbolic links to configured directories.

context: check_context include/ecr/config.h include/ecr/version.h dirlinks
	$(Q) for dir in $(CONTEXTDIRS) ; do \
		$(MAKE) -C $$dir TOPDIR="$(TOPDIR)" context; \
	done

# clean_context
#
# This is part of the distclean target.  It removes all of the header files
# and symbolic links created by the context target.

clean_context:
	$(Q) $(MAKE) -C apps TOPDIR="$(TOPDIR)" clean_context
	$(call DELFILE, include/ecr/config.h)
	$(call DELFILE, include/ecr/version.h)
	$(Q) $(DIRUNLINK) include/arch/chip
	$(Q) $(DIRUNLINK) include/arch
	$(Q) $(DIRUNLINK) $(ARCH_SRC)/chip

# check_context
#
# This target checks if project has been configured. This project must install
# certain files in the top-level build directory. This target verifies that those
# configuration files have been installed and that project is ready to be built.

check_context:
	$(Q) if [ ! -e ${TOPDIR}/.config -o ! -e ${TOPDIR}/Make.defs ]; then \
		echo "" ; \
		echo "Project has not been configured:" ; \
		echo "  tools/configure.sh <target>" ; \
		echo "" ; \
		exit 1 ; \
	fi

# Archive targets.  The target build sequence will first create a series of
# libraries, one per configured source file directory.  The final NuttX
# execution will then be built from those libraries.  The following targets
# build those libraries.

include tools/LibTargets.mk

# $(BIN)
#
# Create the final Project executable in build process.

$(BIN): depend

download: $(BIN)

# Configuration targets
#
# These targets depend on the kconfig-frontends packages. To use these, you
# must first download and install the kconfig-frontends package from this
# location: http://ymorin.is-a-geek.org/projects/kconfig-frontends.

do_config: apps_preconfig
	$(Q) kconfig-conf Kconfig

config: do_config clean_context

do_oldconfig: apps_preconfig
	$(Q) kconfig-conf --oldconfig Kconfig

oldconfig: do_oldconfig clean_context

do_olddefconfig: apps_preconfig
	$(Q) kconfig-conf --olddefconfig Kconfig

olddefconfig: do_olddefconfig clean_context

do_menuconfig: apps_preconfig
	$(Q) kconfig-mconf Kconfig

menuconfig: do_menuconfig clean_context

do_nconfig: apps_preconfig
	$(Q) kconfig-nconf Kconfig

nconfig: do_nconfig clean_context

do_qconfig: apps_preconfig
	$(Q) kconfig-qconf Kconfig

qconfig: do_qconfig clean_context

do_gconfig: apps_preconfig
	$(Q) kconfig-gconf Kconfig

gconfig: do_gconfig clean_context

# General housekeeping targets:  dependencies, cleaning, etc.

depend: context tools/mkdeps$(HOSTEXEEXT)
	$(Q) for dir in $(USERDEPDIRS) ; do \
		$(MAKE) -C $$dir TOPDIR="$(TOPDIR)" depend ; \
	done
	$(Q) for dir in $(KERNDEPDIRS) ; do \
		$(MAKE) -C $$dir TOPDIR="$(TOPDIR)" EXTRADEFINES=$(KDEFINE) depend; \
	done

subdir_clean:
	$(Q) for dir in $(CLEANDIRS) ; do \
		if [ -e $$dir/Makefile ]; then \
			$(MAKE) -C $$dir TOPDIR="$(TOPDIR)" clean ; \
		fi \
	done
	$(Q) $(MAKE) -C tools -f Makefile.host TOPDIR="$(TOPDIR)" clean

clean: subdir_clean
	$(call DELDIR, $(OUTPUTPATH))
	$(call CLEAN)

subdir_distclean:
	$(Q) for dir in $(CLEANDIRS) ; do \
		if [ -e $$dir/Makefile ]; then \
			$(MAKE) -C $$dir TOPDIR="$(TOPDIR)" distclean ; \
		fi \
	done

distclean: clean subdir_distclean clean_context
	$(call DELFILE, Make.defs)
	$(call DELFILE, .config)
	$(call DELFILE, .config.old)

# Application housekeeping targets. For the most part, the application
# directory is treated like any other build directory in this script.
# However, as a convenience, the following targets are included to support
# housekeeping functions in the user application directory from the Project
# build directory.
#
# apps_preconfig: Prepare applications to be configured

apps_preconfig:
	$(Q) $(MAKE) -C apps TOPDIR="$(TOPDIR)" preconfig
