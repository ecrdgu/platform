#!/bin/bash

#
# Handle command line options
#

progname=$0
usage="USAGE: $progname [-w] [-d] [-h] <compiler-path> <def1>[=val1] [<def2>[=val2] [<def3>[=val3] ...]]"
advice="Try '$progname -h' for more information"

while [ ! -z "$1" ]; do
	case $1 in
	-d )
		set -x
		;;
	-h )
		echo "$progname is a tool for flexible generation of command line pre-processor"
		echo "definitions arguments for a variety of diffent compilers in a variety of"
		echo "compilation environments"
		echo ""
		echo $usage
		echo ""
		echo "Where:"
		echo "	<compiler-path>"
		echo "		The full path to your compiler"
		echo "	<def1> <def2> [<def3> ..."
		echo "		A list of pre-preprocesser variable names to be defined."
		echo "	[=val1] [=val2] [=val3]"
		echo "		optional values to be assigned to each pre-processor variable."
		echo "		If not supplied, the variable will be defined with no explicit value."
		echo "	-d"
		echo "		Enable script debug"
		;;
	* )
		break;
		;;
	esac
	shift
done

ccpath=$1
shift
varlist=$@

if [ -z "$ccpath" ]; then
	echo "Missing compiler path"
	echo $usage
	echo $advice
	exit 1
fi

if [ -z "$varlist" ]; then
	echo "Missing definition list"
	echo $usage
	echo $advice
	exit 1
fi

#
# Most compilers support CFLAG options like '-D<defn>' to add pre-processor
# variable defintions.  Some (like the Zilog tools), do not.  This script
# makes the selection of pre-processor definitions compiler independent.
#
# Below are all known compiler names (as found in the config/*/*/Make.defs
# files).  If a new compiler is used that has some unusual syntax, then
# additional logic needs to be added to this file.
#
#   NAME                        Syntax
#   $(CROSSDEV)gcc              -D<def1> -D<def2> -D<def3> ...
#   sdcc                        -D<def2> -D<def2> -D<def3> ...
#

#
# Let's assume that all GCC compiler paths contain the string gcc and
# no non-GCC compiler pathes include this substring
#
gcc=`echo $ccpath | grep gcc`
sdcc=`echo $ccpath | grep sdcc`
exefile=`basename "$ccpath"`

# Now process each definition in the definition list

unset response
for vardef in $varlist; do

	varname=`echo $vardef | cut -d'=' -f1`
	if [ "X$varname" != "X$vardef" ]; then
		varvalue=`echo $vardef | cut -d'=' -f2`
	else
		unset varvalue
	fi

	# Handle the output depending on if there is a value for the variable or not

	if [ -z "$varvalue" ]; then

		# Treat the first definition differently

		if [ -z "$response" ]; then
			response=-D$varname
		else
			response=$response" -D$varname"
		fi
	else

		# Treat the first definition differently

		if [ -z "$response" ]; then
			response=-D$varname=$varvalue
		else
			response=$response" -D$varname=$varvalue"
		fi
	fi
done

echo $response


