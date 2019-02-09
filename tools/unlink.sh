#!/bin/bash

link=$1

# Verify that arguments were provided

if [ -z "${link}" ]; then
	echo "Missing link argument"
	exit 1
fi

# Check if something already exists at the link path

if [ -e "${link}" ]; then

	# Yes, is it a symbolic link?  If so, then remove it

	if [ -h "${link}" ]; then
		rm -f "${link}"
	else

		# It is something else (like a file) or directory that does

		echo "${link} already exists but is not a symbolic link"
		exit 1
	fi
fi
