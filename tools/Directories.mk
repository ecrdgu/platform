# All add-on directories.
#
# ECR_ADDONS is the list of directories built into the ecr kernel.
# USER_ADDONS is the list of directories that will be built into the user
#   application

ECR_ADDONS :=
USER_ADDONS :=

# Lists of build directories.
#
# FSDIRS depend on file descriptor support; NONFSDIRS do not (except for parts
#   of FSDIRS).  We will exclude FSDIRS from the build if file descriptor
#   support is disabled.  NOTE that drivers, in general, depends on file
#   descriptor support but is always built because there are other components
#   in the drivers directory that are needed even if file descriptors are not
#   supported.
# CONTEXTDIRS include directories that have special, one-time pre-build
#   requirements.  Normally this includes things like auto-generation of
#   configuration specific files or creation of configurable symbolic links
# USERDIRS - When Project is build is a monolithic kernel, this provides the
#   list of directories that must be built
# OTHERDIRS - These are directories that are not built but probably should
#   be cleaned to prevent garbage from collecting in them when changing
#   configurations.

ifeq ($(CONFIG_LIBCXX),y)
LIBXX=libcxx
else
LIBXX=libxx
endif

NONFSDIRS = sched drivers configs $(ARCH_SRC) $(ECR_ADDONS)
FSDIRS = fs binfmt
CONTEXTDIRS = configs $(APPDIR)
USERDIRS =
OTHERDIRS = lib

NONFSDIRS += libs$(DELIM)libc mm
OTHERDIRS += $(USER_ADDONS)
ifeq ($(CONFIG_HAVE_CXX),y)
NONFSDIRS += libs$(DELIM)$(LIBXX)
else
OTHERDIRS += libs$(DELIM)$(LIBXX)
endif


ifeq ($(CONFIG_LIB_SYSCALL),y)
NONFSDIRS += syscall
CONTEXTDIRS += syscall
USERDIRS += syscall
else
OTHERDIRS += syscall
endif

ifeq ($(CONFIG_LIB_ZONEINFO_ROMFS),y)
CONTEXTDIRS += libs$(DELIM)libc
endif

ifeq ($(CONFIG_WIRELESS),y)
NONFSDIRS += wireless
else
OTHERDIRS += wireless
endif

CLEANDIRS   = $(NONFSDIRS) $(FSDIRS) $(USERDIRS) $(OTHERDIRS)
KERNDEPDIRS = $(NONFSDIRS)
USERDEPDIRS = $(USERDIRS)

# Add file system directories to KERNDEPDIRS (they are already in CLEANDIRS)

ifeq ($(CONFIG_NFILE_DESCRIPTORS),0)
ifeq ($(CONFIG_NET),y)
ifneq ($(CONFIG_NSOCKET_DESCRIPTORS),0)
KERNDEPDIRS += fs
endif
KERNDEPDIRS += drivers
endif
else
KERNDEPDIRS += $(FSDIRS)
endif

# Add networking directories to KERNDEPDIRS and CLEANDIRS

ifeq ($(CONFIG_NET),y)
KERNDEPDIRS += net
endif
CLEANDIRS += net

ifeq ($(CONFIG_CRYPTO),y)
KERNDEPDIRS += crypto
endif
CLEANDIRS += crypto

CLEANDIRS += apps
