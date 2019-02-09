#!/bin/bash

# Handle command line options

progname=$0
pathtype=user
usage="USAGE: $progname [-w] [-d] [-h] <compiler-path> <dir1> [<dir2> [<dir3> ...]]"
advice="Try '$progname -h' for more information"

while [ ! -z "$1" ]; do
	case $1 in
	-d ) set -x ;;
	-s ) pathtype=system ;;
	-h )
		echo "$progname is a tool for flexible generation of include path arguments for a"
		echo "variety of different compilers in a variety of compilation environments"
		echo ""
		echo $usage
		echo ""
		echo "Where:"
		echo "	<compiler-path>"
		echo "		The full path to your compiler"
		echo "	<dir1> [<dir2> [<dir3> ...]]"
		echo "		A list of include directories"
		echo "	-s"
		echo "		Generate standard, system header file paths instead of normal user"
		echo "		header file paths."
		echo "	-d"
		echo "		Enable script debug"
		echo "	-h"
		echo "		Shows this help text and exits."
		exit 0
		;;
	* ) break ;;
	esac
	shift
done

ccpath=$1
shift
dirlist=$@

if [ -z "$ccpath" ]; then
	echo "Missing compiler path"
	echo $usage
	echo $advice
	exit 1
fi

if [ -z "$dirlist" ]; then
	echo "Missing include directory list"
	echo $usage
	echo $advice
	exit 1
fi

#
# Most compilers support CFLAG options like '-I<dir>' to add include
# file header paths.  Some (like the Zilog tools), do not.  This script
# makes the selection of header file paths compiler independent.
#
# Below are all known compiler names (as found in the config/*/*/Make.defs
# files).  If a new compiler is used that has some unusual syntax, then
# additional logic needs to be added to this file.
#
#   NAME                        Syntax
#   $(CROSSDEV)gcc              -I<dir1> -I<dir2> -I<dir3> ...
#   sdcc                        -I<dir2> -I<dir2> -I<dir3> ...

# Let's assume that all GCC compiler paths contain the string gcc or
# g++ and no non-GCC compiler pathes include these substrings

gcc=`echo $ccpath | grep gcc`
if [ -z "${gcc}" ]; then
  gcc=`echo $ccpath | grep g++`
fi

sdcc=`echo $ccpath | grep sdcc`

exefile=`basename "$ccpath"`

# Select system or user header file path command line option

if [ "X$pathtype" = "Xsystem" ]; then
	cmdarg=-isystem
else
	cmdarg=-I
fi

# Now process each directory in the directory list

unset response
for dir in $dirlist; do

	# Verify that the include directory exists

	if [ ! -d $dir ]; then
		echo "Include path '$dir' does not exist"
		echo $showusage
		exit 1
	fi

	path=$dir

	# Handle the output using the selected format

	# Treat the first directory differently

	if [ -z "$response" ]; then
		response="${cmdarg} \"$path\""
	else
		response="${response} ${cmdarg} \"$path\""
	fi
done

echo $response


