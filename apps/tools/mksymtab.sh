#!/bin/bash

export LC_ALL=C

usage="Usage: $0 <imagedirpath> <outfilepath>"

# Check for the required directory path

dir=$1
if [ -z "$dir" ]; then
  echo "ERROR: Missing <imagedirpath>"
  echo ""
  echo $usage
  exit 1
fi

if [ ! -d "$dir" ]; then
  echo "ERROR: Directory $dir does not exist"
  echo ""
  echo $usage
  exit 1
fi

# Get the output file name

outfile=$2
if [ -z "$outfile" ]; then
  echo "ERROR: Missing <outfilepath>"
  echo ""
  echo $usage
  exit 1
fi

rm -f $outfile

# Extract all of the undefined symbols from the ELF files and create a
# list of sorted, unique undefined variable names.

execlist=`find ${dir} -type f`
if [ ! -z "${execlist}" ]; then
  for exec in ${execlist}; do
    nm $exec | fgrep ' U ' | sed -e "s/^[ ]*//g" | cut -d' ' -f2  >>_tmplist
  done

  varlist=`cat _tmplist | sort - | uniq -`
  rm -f _tmplist
fi

# Now output the symbol table as a structure in a C source file.  All
# undefined symbols are declared as void* types.  If the toolchain does
# any kind of checking for function vs. data objects, then this could
# faile

echo "#include <ecr/compiler.h>" >$outfile
echo "#include <ecr/binfmt/symtab.h>" >>$outfile
echo "" >>$outfile

for var in $varlist; do
  echo "extern void *${var};" >>$outfile
done

echo "" >>$outfile
echo "#if defined(CONFIG_EXECFUNCS_HAVE_SYMTAB)" >>$outfile
echo "const struct symtab_s CONFIG_EXECFUNCS_SYMTAB_ARRAY[] = " >>$outfile
echo "#elif defined(CONFIG_SYSTEM_NSH_SYMTAB)" >>$outfile
echo "const struct symtab_s CONFIG_SYSTEM_NSH_SYMTAB_ARRAYNAME[] = " >>$outfile
echo "#else" >>$outfile
echo "const struct symtab_s dummy_symtab[] = " >>$outfile
echo "#endif" >>$outfile
echo "{" >>$outfile

for var in $varlist; do
  echo "  {\"${var}\", &${var}}," >>$outfile
done

echo "};" >>$outfile
echo "" >>$outfile
echo "#if defined(CONFIG_EXECFUNCS_HAVE_SYMTAB)" >>$outfile
echo "const int CONFIG_EXECFUNCS_NSYMBOLS_VAR = sizeof(CONFIG_EXECFUNCS_SYMTAB_ARRAY) / sizeof(struct symtab_s);" >>$outfile
echo "#elif defined(CONFIG_SYSTEM_NSH_SYMTAB)" >>$outfile
echo "const int CONFIG_SYSTEM_NSH_SYMTAB_COUNTNAME = sizeof(CONFIG_SYSTEM_NSH_SYMTAB_ARRAYNAME) / sizeof(struct symtab_s);" >>$outfile
echo "#else" >>$outfile
echo "const int dummy_nsymtabs = sizeof(dummy_symtab) / sizeof(struct symtab_s);" >>$outfile
echo "#endif" >>$outfile
