#!/bin/bash

WD=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOPDIR="${WD}/.."

function discover() {
    echo "Select one of the following options for <board-name>:"
    configlist=`find ${TOPDIR}/configs -name defconfig`
    for defconfig in ${configlist}; do
      config=`dirname ${defconfig} | sed -e "s,${TOPDIR}/configs/,,g"`
      echo "  ${config}"
    done

}

USAGE="
USAGE: ${0} [-d] [-l|m|c|u|g|n] [-a <app-dir>] <board-name>/<config-name>

Where:
  -l selects the Linux (l) host environment.
  -m selects the macOS (m) host environment.
  Default: Use host setup in the defconfig file
  <board-name> is the name of the board in the configs directory
  <config-name> is the name of the board configuration sub-directory
  <app-dir> is the path to the apps/ directory, relative to the nuttx
     directory

"

# A list of optional files that may be installed

OPTFILES="\
  .gdbinit\
  .cproject\
  .project\
"

# Parse command arguments

unset boardconfig
unset host

while [ ! -z "$1" ]; do
  case "$1" in
    -d ) set -x ;;
    -h ) echo "$USAGE"; exit 0 ;;
    -l ) host=linux ;;
    -m ) host=macos ;;
    *)
      if [ ! -z "${boardconfig}" ]; then
        echo ""
        echo "<board/config> defined twice"
        echo "$USAGE"
        exit 1
      fi
      boardconfig=$1
      ;;
  esac
  shift
done

# Sanity checking

if [ -z "${boardconfig}" ]; then
  echo ""
  echo "Missing <board/config> argument"
  echo "$USAGE"
  exit 2
fi

configpath=${TOPDIR}/configs/${boardconfig}
if [ ! -d "${configpath}" ]; then
  # Try direct path for convenience.

  configpath=${TOPDIR}/${boardconfig}
  if [ ! -d "${configpath}" ]; then
    echo "Directory ${configpath} does not exist.  Options are:"
	discover
	echo "$USAGE"
    exit 3
  fi
fi

src_makedefs="${configpath}/Make.defs"
dest_makedefs="${TOPDIR}/Make.defs"

if [ ! -r "${src_makedefs}" ]; then
  boardpath=`dirname $configpath`
  src_makedefs="${boardpath}/scripts/Make.defs"

  if [ ! -r "${src_makedefs}" ]; then
    echo "File Make.defs could not be found"
    exit 4
  fi
fi

src_config="${configpath}/defconfig"
dest_config="${TOPDIR}/.config"

if [ ! -r "${src_config}" ]; then
  echo "File \"${src_config}\" does not exist"
  exit 6
fi

if [ -r ${dest_config} ]; then
  echo "Already configured!"
  echo "Do 'make distclean' and try again."
  exit 7
fi

# If no application directory was provided on the command line and we are
# switching between a windows native host and some other host then ignore the
# path to the apps/ directory in the defconfig file.  It will most certainly
# not be in a usable form.

appdir="apps"

echo "  Copy files"
install -m 644 "${src_makedefs}" "${dest_makedefs}" || \
  { echo "Failed to copy \"${src_makedefs}\"" ; exit 7 ; }
install -m 644 "${src_config}" "${dest_config}" || \
  { echo "Failed to copy \"${src_config}\"" ; exit 9 ; }

# Install any optional files

for opt in ${OPTFILES}; do
  test -f "${configpath}/${opt}" && install "${configpath}/${opt}" "${TOPDIR}/"
done

if [ ! -z "$host" ]; then
  sed -i -e "/CONFIG_HOST_LINUX/d" ${dest_config}
  sed -i -e "/CONFIG_HOST_WINDOWS/d" ${dest_config}
  sed -i -e "/CONFIG_HOST_MACOS/d" ${dest_config}
  sed -i -e "/CONFIG_HOST_OTHER/d" ${dest_config}
   case "$host" in
    "linux")
      echo "  Select CONFIG_HOST_LINUX=y"
      echo "CONFIG_HOST_LINUX=y" >> "${dest_config}"
      ;;

    "macos")
      echo "  Select CONFIG_HOST_MACOS=y"
      echo "CONFIG_HOST_MACOS=y" >> "${dest_config}"
      ;;
  esac
fi

# The saved defconfig files are all in compressed format and must be
# reconstitued before they can be used.

echo "  Refreshing..."
cd ${TOPDIR} || { echo "Failed to cd to ${TOPDIR}"; exit 1; }

MAKE_BIN=make
if [ ! -z `which gmake 2>/dev/null` ]; then
  MAKE_BIN=gmake
fi

${MAKE_BIN} olddefconfig 1>/dev/null
