# Archive targets.  The target build sequence will first create a series of
# libraries, one per configured source file directory.  The final NuttX
# execution will then be built from those libraries.  The following targets
# build those libraries.
#

LIBTARGET = $(OUTPUTPATH)$(DELIM)libraries

libs$(DELIM)libc$(DELIM)libkc$(LIBEXT): context
	$(Q) $(MAKE) -C libs$(DELIM)libc TOPDIR="$(TOPDIR)" libkc$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libkc$(LIBEXT): libs$(DELIM)libc$(DELIM)libkc$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

mm$(DELIM)libkmm$(LIBEXT): context
	$(Q) $(MAKE) -C mm TOPDIR="$(TOPDIR)" libkmm$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libkmm$(LIBEXT): mm$(DELIM)libkmm$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

$(ARCH_SRC)$(DELIM)libkarch$(LIBEXT): context
	$(Q) $(MAKE) -C $(ARCH_SRC) TOPDIR="$(TOPDIR)" libkarch$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libkarch$(LIBEXT): $(ARCH_SRC)$(DELIM)libkarch$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

sched$(DELIM)libsched$(LIBEXT): context
	$(Q) $(MAKE) -C sched TOPDIR="$(TOPDIR)" libsched$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libsched$(LIBEXT): sched$(DELIM)libsched$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

net$(DELIM)libnet$(LIBEXT): context
	$(Q) $(MAKE) -C net TOPDIR="$(TOPDIR)" libnet$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libnet$(LIBEXT): net$(DELIM)libnet$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

configs$(DELIM)libconfigs$(LIBEXT): context
	$(Q) $(MAKE) -C configs TOPDIR="$(TOPDIR)" libconfigs$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libconfigs$(LIBEXT): configs$(DELIM)libconfigs$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

crypto$(DELIM)libcrypto$(LIBEXT): context
	$(Q) $(MAKE) -C crypto TOPDIR="$(TOPDIR)" libcrypto$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libcrypto$(LIBEXT): crypto$(DELIM)libcrypto$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

fs$(DELIM)libfs$(LIBEXT): context
	$(Q) $(MAKE) -C fs TOPDIR="$(TOPDIR)" libfs$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libfs$(LIBEXT): fs$(DELIM)libfs$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

drivers$(DELIM)libdrivers$(LIBEXT): context
	$(Q) $(MAKE) -C drivers TOPDIR="$(TOPDIR)" libdrivers$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libdrivers$(LIBEXT): drivers$(DELIM)libdrivers$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

binfmt$(DELIM)libbinfmt$(LIBEXT): context
	$(Q) $(MAKE) -C binfmt TOPDIR="$(TOPDIR)" libbinfmt$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libbinfmt$(LIBEXT): binfmt$(DELIM)libbinfmt$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

wireless$(DELIM)libwireless$(LIBEXT): context
	$(Q) $(MAKE) -C wireless TOPDIR="$(TOPDIR)" libwireless$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libwireless$(LIBEXT): wireless$(DELIM)libwireless$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

$(ARCH_SRC)$(DELIM)libarch$(LIBEXT): context
	$(Q) $(MAKE) -C $(ARCH_SRC) TOPDIR="$(TOPDIR)" libarch$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libarch$(LIBEXT): $(ARCH_SRC)$(DELIM)libarch$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

$(LIBTARGET)$(DELIM)libdsp$(LIBEXT): libs$(DELIM)libdsp$(DELIM)libdsp$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

# Special case

syscall$(DELIM)libstubs$(LIBEXT): context
	$(Q) $(MAKE) -C syscall TOPDIR="$(TOPDIR)" libstubs$(LIBEXT) # KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBTARGET)$(DELIM)libstubs$(LIBEXT): syscall$(DELIM)libstubs$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

# Possible user-mode builds

libs$(DELIM)libc$(DELIM)libuc$(LIBEXT): context
	$(Q) $(MAKE) -C libs$(DELIM)libc TOPDIR="$(TOPDIR)" libuc$(LIBEXT) KERNEL=n

$(LIBTARGET)$(DELIM)libuc$(LIBEXT): libs$(DELIM)libc$(DELIM)libuc$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

mm$(DELIM)libumm$(LIBEXT): context
	$(Q) $(MAKE) -C mm TOPDIR="$(TOPDIR)" libumm$(LIBEXT) KERNEL=n

$(LIBTARGET)$(DELIM)libumm$(LIBEXT): mm$(DELIM)libumm$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

$(ARCH_SRC)$(DELIM)libuarch$(LIBEXT): context
	$(Q) $(MAKE) -C $(ARCH_SRC) TOPDIR="$(TOPDIR)" libuarch$(LIBEXT) KERNEL=n

$(LIBTARGET)$(DELIM)libuarch$(LIBEXT): $(ARCH_SRC)$(DELIM)libuarch$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

libs$(DELIM)libxx$(DELIM)$(LIBXX)$(LIBEXT): context
	$(Q) $(MAKE) -C libs$(DELIM)libxx TOPDIR="$(TOPDIR)" $(LIBXX)$(LIBEXT) KERNEL=n

$(LIBTARGET)$(DELIM)$(LIBXX)$(LIBEXT): libs$(DELIM)libxx$(DELIM)$(LIBXX)$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

$(APPDIR)$(DELIM)libapps$(LIBEXT): context
	$(Q) $(MAKE) -C $(APPDIR) TOPDIR="$(TOPDIR)" libapps$(LIBEXT) KERNEL=n

$(LIBTARGET)$(DELIM)libapps$(LIBEXT): $(APPDIR)$(DELIM)libapps$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

syscall$(DELIM)libproxies$(LIBEXT): context
	$(Q) $(MAKE) -C syscall TOPDIR="$(TOPDIR)" libproxies$(LIBEXT) KERNEL=n

$(LIBTARGET)$(DELIM)libproxies$(LIBEXT): syscall$(DELIM)libproxies$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

# Possible non-kernel builds

libs$(DELIM)libc$(DELIM)libc$(LIBEXT): context
	$(Q) $(MAKE) -C libs$(DELIM)libc TOPDIR="$(TOPDIR)" libc$(LIBEXT)

$(LIBTARGET)$(DELIM)libc$(LIBEXT): libs$(DELIM)libc$(DELIM)libc$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)

mm$(DELIM)libmm$(LIBEXT): context
	$(Q) $(MAKE) -C mm TOPDIR="$(TOPDIR)" libmm$(LIBEXT)

$(LIBTARGET)$(DELIM)libmm$(LIBEXT): mm$(DELIM)libmm$(LIBEXT)
	$(Q) $(call INSTALL_LIB,$<,$@)
