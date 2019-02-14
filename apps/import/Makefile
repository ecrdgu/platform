-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

SUBDIRS = arch build include libs startup tmp
FILES   = .config System.map User.map

ifneq (,$(wildcard ./symtab.c))
define MKSYMTAB
	$(Q) $(MAKE) -f Makefile.symtab $1 TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"
endef
else
define MKSYMTAB
endef
endif

all: symtab
.PHONY: symtab context depend clean distclean
.PRECIOUS: ../libapps$(LIBEXT)

symtab:
	$(call MKSYMTAB, all)

install:
	$(call MKSYMTAB, install)

context:
	$(call MKSYMTAB, context)

depend:
	$(call MKSYMTAB, depend)

clean:
	$(call MKSYMTAB, clean)

distclean:
	$(foreach SDIR, $(SUBDIRS), $(call DELDIR, $(SDIR)))
	$(foreach FILE, $(FILES), $(call DELFILE, $(FILE)))
	$(call MKSYMTAB, distclean)
	$(call DELFILE, symtab.c)

# Library path

LIBPATH = $(TOPDIR)/build/libraries
