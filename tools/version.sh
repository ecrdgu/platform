#!/bin/bash

WD=$(cd `dirname $0` && pwd)

# Get command line parameters

USAGE="USAGE: $0 [-d|-h] [-b <build>] -v <major.minor> <outfile-path>"
ADVICE="Try '$0 -h' for more information"

unset VERSION
unset BUILD
unset OUTFILE

while [ ! -z "$1" ]; do
	case $1 in
	-b )
		shift
		BUILD=$1
		;;
	-d )
		set -x
		;;
	-v )
		shift
		VERSION=$1
		;;
	-h )
		echo "$0 is a tool for generation of proper version files for the NuttX build"
		echo ""
		echo $USAGE
		echo ""
		echo "Where:"
		echo "	-b <build>"
		echo "		Use this build identification string.  Default: use GIT build ID"
		echo "		NOTE: GIT build information may not be available in a snapshot"
		echo "	-d"
		echo "		Enable script debug"
		echo "	-h"
		echo "		show this help message and exit"
		echo "	-v <major.minor>"
		echo "		The NuttX version number expressed as a major and minor number separated"
		echo "		by a period"
		echo " 	<outfile-path>"
		echo "		The full path to the version file to be created"
		exit 0
		;;
	* )
		break;
		;;
	esac
	shift
done

OUTFILE=$1

# Make sure we know what is going on

if [ -z ${VERSION} ] ; then
	echo "Missing versioning information"
	echo $USAGE
	echo $ADVICE
	exit 1
fi

if [ -z ${OUTFILE} ] ; then
	echo "Missing path to the output file"
	echo $USAGE
	echo $ADVICE
	exit 1
fi

# Get the major and minor version numbers

MAJOR=`echo ${VERSION} | cut -d'.' -f1`
if [ "X${MAJOR}" = "X${VERSION}" ]; then
	echo "Missing minor version number"
	echo $USAGE
	echo $ADVICE
	exit 2
fi
MINOR=`echo ${VERSION} | cut -d'.' -f2`

# Get GIT information (if not provided on the command line)

if [ -z "${BUILD}" ]; then
	GITINFO=`git log 2>/dev/null | head -1`
	if [ -z "${GITINFO}" ]; then
		echo "GIT version information is not available"
		exit 3
	fi
	BUILD=`echo ${GITINFO} | cut -d' ' -f2`
	if [ -z "${BUILD}" ]; then
		echo "GIT build information not found"
		exit 4
	fi
fi

# Write a version file into the NuttX directoy.  The syntax of file is such that it
# may be sourced by a bash script or included by a Makefile.

echo "#!/bin/bash" >${OUTFILE}
echo "" >>${OUTFILE}
echo "CONFIG_VERSION_STRING=\"${VERSION}\"" >>${OUTFILE}
echo "CONFIG_VERSION_MAJOR=${MAJOR}" >>${OUTFILE}
echo "CONFIG_VERSION_MINOR=${MINOR}" >>${OUTFILE}
echo "CONFIG_VERSION_BUILD=\"${BUILD}\"" >>${OUTFILE}
