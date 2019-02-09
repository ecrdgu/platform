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

# This is the name of the final target (relative to the top level directorty)

OUTPUTPATH = build
BIN      = $(OUTPUTPATH)/$(OUTPUT)$(EXEEXT)

all: $(BIN)
.PHONY: context clean_context check_context config oldconfig menuconfig nconfig qconfig gconfig subdir_clean clean subdir_distclean distclean

# context
#
# The context target is invoked on each target build to assure that NuttX is
# properly configured.  The basic configuration steps include creation of the
# the config.h and version.h header files in the include/nuttx directory and
# the establishment of symbolic links to configured directories.

context: check_context
	$(Q) for dir in $(CONTEXTDIRS) ; do \
		$(MAKE) -C $$dir TOPDIR="$(TOPDIR)" context; \
	done

# clean_context
#
# This is part of the distclean target.  It removes all of the header files
# and symbolic links created by the context target.

clean_context:

# check_context
#
# This target checks if project has been configured. This project must install
# certain files in the top-level build directory. This target verifies that those
# configuration files have been installed and that project is ready to be built.

check_context:
	$(Q) if [ ! -e ${TOPDIR}/.config -o ! -e ${TOPDIR}/Make.defs ]; then \
		echo "" ; \
		echo "Project has not been configured:" ; \
		echo "  Rename defconfig to .config" ; \
		echo "   after copy defconfig and Make.defs from configs/<board-name>/<config>" ; \
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

do_config:
	$(Q) kconfig-conf Kconfig

config: do_config clean_context

do_oldconfig:
	$(Q) kconfig-conf --oldconfig Kconfig

oldconfig: do_oldconfig clean_context

do_olddefconfig:
	$(Q) kconfig-conf --olddefconfig Kconfig

olddefconfig: do_olddefconfig clean_context

do_menuconfig:
	$(Q) kconfig-mconf Kconfig

menuconfig: do_menuconfig clean_context

do_nconfig:
	$(Q) kconfig-nconf Kconfig

nconfig: do_nconfig clean_context

do_qconfig:
	$(Q) kconfig-qconf Kconfig

qconfig: do_qconfig clean_context

do_gconfig:
	$(Q) kconfig-gconf Kconfig

gconfig: do_gconfig clean_context

# General housekeeping targets:  dependencies, cleaning, etc.

depend: context
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
