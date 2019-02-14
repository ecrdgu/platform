#!/bin/bash
# Get the input parameter list

USAGE="USAGE: mkkconfig.sh [-d] [-h] [-m <menu>] [-o <kconfig-file>]"
KCONFIG=Kconfig
unset MENU

while [ ! -z "$1" ]; do
  case $1 in
    -d )
      set -x
      ;;
    -m )
      shift
      MENU=$1
      ;;
    -o )
      shift
      KCONFIG=$1
      ;;
    -h )
      echo $USAGE
      exit 0
      ;;
    * )
      echo "ERROR: Unrecognized argument: $1"
      echo $USAGE
      exit 1
      ;;
    esac
  shift
done


if [ -f ${KCONFIG} ]; then
  rm ${KCONFIG} || { echo "ERROR: Failed to remove $PWD/${KCONFIG}"; exit 1; }
fi

echo mkkconfig in $PWD

KCONFIG_LIST=`ls -1 $PWD/*/Kconfig`

echo "#" > ${KCONFIG}
echo "# For a description of the syntax of this configuration file," >> ${KCONFIG}
echo "# see the file kconfig-language.txt in the tools repository." >> ${KCONFIG}
echo "#" >> ${KCONFIG}
echo "" >> ${KCONFIG}

if [ ! -z "${MENU}" ]; then
  echo "menu \"${MENU}\"" >> ${KCONFIG}
fi

for FILE in ${KCONFIG_LIST}; do
  echo "source \"${FILE}\"" >> ${KCONFIG}
done

if [ ! -z "${MENU}" ]; then
  echo "endmenu # ${MENU}" >> ${KCONFIG}
fi

