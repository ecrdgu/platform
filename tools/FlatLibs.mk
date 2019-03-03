# ECRLIBS is the list of NuttX libraries that is passed to the
#   processor-specific Makefile to build the final NuttX target.
#   Libraries in FSDIRS are excluded if file descriptor support
#   is disabled.
# USERLIBS is the list of libraries used to build the final user-space
#   application
# EXPORTLIBS is the list of libraries that should be exported by
#   'make export' is

ECRLIBS = build$(DELIM)libraries$(DELIM)libsched$(LIBEXT)
USERLIBS =

# Driver support.  Generally depends on file descriptor support but there
# are some components in the drivers directory that are needed even if file
# descriptors are not supported.

ECRLIBS += build$(DELIM)libraries$(DELIM)libdrivers$(LIBEXT)

# Add libraries for board support

ECRLIBS += build$(DELIM)libraries$(DELIM)libconfigs$(LIBEXT)

# Add libraries for syscall support.

ECRLIBS += build$(DELIM)libraries$(DELIM)libc$(LIBEXT)
ECRLIBS += build$(DELIM)libraries$(DELIM)libmm$(LIBEXT)
ECRLIBS += build$(DELIM)libraries$(DELIM)libarch$(LIBEXT)
ifeq ($(CONFIG_LIB_SYSCALL),y)
ECRLIBS += build$(DELIM)libraries$(DELIM)libstubs$(LIBEXT)
USERLIBS  += build$(DELIM)libraries$(DELIM)libproxies$(LIBEXT)
endif

# Add libraries for C++ support.  CXX, CXXFLAGS, and COMPILEXX must
# be defined in Make.defs for this to work!

ifeq ($(CONFIG_HAVE_CXX),y)
ECRLIBS += build$(DELIM)libraries$(DELIM)$(LIBXX)$(LIBEXT)
endif

# Add library for application support.

ECRLIBS += build$(DELIM)libraries$(DELIM)libapps$(LIBEXT)

# Add libraries for network support

ifeq ($(CONFIG_NET),y)
ECRLIBS += build$(DELIM)libraries$(DELIM)libnet$(LIBEXT)
endif

# Add libraries for Crypto API support

ifeq ($(CONFIG_CRYPTO),y)
ECRLIBS += build$(DELIM)libraries$(DELIM)libcrypto$(LIBEXT)
endif

# Add libraries for file system support

ifeq ($(CONFIG_NFILE_DESCRIPTORS),0)
ifneq ($(CONFIG_NSOCKET_DESCRIPTORS),0)
ECRLIBS += build$(DELIM)libraries$(DELIM)libfs$(LIBEXT)
endif
else
ECRLIBS += build$(DELIM)libraries$(DELIM)libfs$(LIBEXT) build$(DELIM)libraries$(DELIM)libbinfmt$(LIBEXT)
endif

# Add libraries for the Wireless sub-system

ifeq ($(CONFIG_WIRELESS),y)
ECRLIBS += build$(DELIM)libraries$(DELIM)libwireless$(LIBEXT)
endif

# Add C++ library

ifeq ($(CONFIG_HAVE_CXX),y)
ECRLIBS += build$(DELIM)libraries$(DELIM)$(LIBXX)$(LIBEXT)
endif

# Export all libraries

EXPORTLIBS = $(ECRLIBS)
