# These are configuration variables that are quoted by configuration tool
# but which must be unquoated when used in the build system.

CONFIG_ARCH       := $(patsubst "%",%,$(strip $(CONFIG_ARCH)))
CONFIG_ARCH_CHIP  := $(patsubst "%",%,$(strip $(CONFIG_ARCH_CHIP)))
CONFIG_ARCH_BOARD := $(patsubst "%",%,$(strip $(CONFIG_ARCH_BOARD)))

# Some defaults just to prohibit some bad behavior if for some reason they
# are not defined

OBJEXT ?= .o
LIBEXT ?= .a

# DELIM - Path segment delimiter character

DELIM = $(strip /)

# INCDIR - Convert a list of directory paths to a list of compiler include
#   directories
# Example: CFFLAGS += ${shell $(INCDIR) [options] "compiler" "dir1" "dir2" "dir2" ...}
#
# Note that the compiler string and each directory path string must quoted if
# they contain spaces or any other characters that might get mangled by the
# shell
#
# Depends on this setting passed as a make commaond line definition from the
# toplevel Makefile:
#
#   TOPDIR - The path to the top level NuttX directory in the form
#     appropriate for the current build environment
#

INCDIR = "$(TOPDIR)/tools/incdir.sh"

# PREPROCESS - Default macro to run the C pre-processor
# Example: $(call PREPROCESS, in-file, out-file)
#
# Depends on these settings defined in board-specific Make.defs file
# installed at $(TOPDIR)/Make.defs:
#
#   CPP - The command to invoke the C pre-processor
#   CPPFLAGS - Options to pass to the C pre-processor

define PREPROCESS
	@echo "CPP: $1->$2"
	$(Q) $(CPP) $(CPPFLAGS) $1 -o $2
endef

# COMPILE - Default macro to compile one C file
# Example: $(call COMPILE, in-file, out-file)
#
# Depends on these settings defined in board-specific Make.defs file
# installed at $(TOPDIR)/Make.defs:
#
#   CC - The command to invoke the C compiler
#   CFLAGS - Options to pass to the C compiler

define COMPILE
	@echo "CC: $1"
	$(Q) $(CC) -c $(CFLAGS) $1 -o $2
endef

# COMPILEXX - Default macro to compile one C++ file
# Example: $(call COMPILEXX, in-file, out-file)
#
# Depends on these settings defined in board-specific Make.defs file
# installed at $(TOPDIR)/Make.defs:
#
#   CXX - The command to invoke the C++ compiler
#   CXXFLAGS - Options to pass to the C++ compiler

define COMPILEXX
	@echo "CXX: $1"
	$(Q) $(CXX) -c $(CXXFLAGS) $1 -o $2
endef

# ASSEMBLE - Default macro to assemble one assembly language file
# Example: $(call ASSEMBLE, in-file, out-file)
#
# NOTE that the most common toolchain, GCC, uses the compiler to assemble
# files because this has the advantage of running the C Pre-Processor against
# the assembly language files.  This is not possible with other toolchains;
# platforms using those other tools should define AS and over-ride this
# definition in order to use the assembler directly.
#
# Depends on these settings defined in board-specific Make.defs file
# installed at $(TOPDIR)/Make.defs:
#
#   CC - By default, the C compiler is used to compile assembly language
#        files
#   AFLAGS - Options to pass to the C+compiler

define ASSEMBLE
	@echo "AS: $1"
	$(Q) $(CC) -c $(AFLAGS) $1 -o $2
endef

# INSTALL_LIB - Install a library $1 into target $2
# Example: $(call INSTALL_LIB, libabc.a, $(TOPDIR)/build/libraries/)

define INSTALL_LIB
	@echo "IN: $1 -> $2"
	$(Q) install -m 0644 $1 $2
endef

# MOVEOBJ - Default macro to move an object file to the correct location
# Example: $(call MOVEOBJ, prefix, directory)
#
# This is only used in directories that keep object files in sub-directories.
# Certain compilers (ZDS-II) always place the resulting files in the
# directory where the compiler was invoked with not option to generate objects
# in a different location.

define MOVEOBJ
endef

# ARCHIVE - Add a list of files to an archive
# Example: $(call ARCHIVE, archive-file, "file1 file2 file3 ...")
#
# Note: The fileN strings may not contain spaces or  characters that may be
# interpreted strangely by the shell
#
# Depends on these settings defined in board-specific Make.defs file
# installed at $(TOPDIR)/Make.defs:
#
#   AR - The command to invoke the archiver (includes any options)
#

define ARCHIVE
	@echo "AR: $2"
	$(Q) $(AR) $1 $(2) || { echo "$(AR) $1 FAILED!" ; exit 1 ; }
endef

# PRELINK - Prelink a list of files
# This is useful when files were compiled with fvisibility=hidden.
# Any symbol which was not explicitly made global is invisible outside the
# prelinked file.
#
# Example: $(call PRELINK, prelink-file, "file1 file2 file3 ...")
#
# Note: The fileN strings may not contain spaces or  characters that may be
# interpreted strangely by the shell
#
# Depends on these settings defined in board-specific Make.defs file
# installed at $(TOPDIR)/Make.defs:
#
#   LD - The command to invoke the linker (includes any options)
#    OBJCOPY - The command to invoke the object cop (includes any options)
#

define PRELINK
	@echo "PRELINK: $1"
	$(Q) $(LD) -Ur -o $1 $2 && $(OBJCOPY) --localize-hidden $1
endef

# DELFILE - Delete one file

define DELFILE
	$(Q) rm -f $1
endef

# DELDIR - Delete one directory

define DELDIR
	$(Q) rm -rf $1
endef

# MOVEFILE - Move one file

define MOVEFILE
	$(Q) mv -f $1 $2
endef

# CLEAN - Default clean target

define CLEAN
	$(Q) rm -f *$(OBJEXT) *$(LIBEXT) *~ .*.swp
endef
