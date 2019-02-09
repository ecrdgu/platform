#!/bin/bash
#set -x

src=$1
dest=$2

# Verify that arguments were provided

if [ -z "${src}" -o -z "${dest}" ]; then
	echo "Missing src and/or dest arguments"
	exit 1
fi

# Check if something already exists at the destination path replace it with
# the new link (which might be different).  Note that we check for the
# the link (-h) before we check for existence (-e) because a bad link will
# report that it does not exist.

if [ -h "${dest}" ]; then
	rm -f "${dest}"
else

	# If the path exists and is a directory that contains the "fake link"
	# mark, then treat it like a soft link (i.e., remove the directory)

	if [ -d "${dest}" -a -f "${dest}/.fakelnk" ]; then
		rm -rf "${dest}"
	else

		# Does anything exist at the destination path?

		if [ -e "${dest}" ]; then

			# It is something else (like a file) or directory that does
			# not contain the "fake link" mark

			echo "${dest} already exists but is not a symbolic link"
			exit 1
		fi
	fi
fi


# Verify that a directory exists at the source path

if [ ! -d "${src}" ]; then
	echo "No directory at ${src}"
	exit 1
fi

# Copy the directory

cp -a "${src}" "${dest}" || \
	{ echo "Failed to create link: $dest" ; rm -rf ${dest} ; exit 1 ; }
touch "${dest}/.fakelnk" || \
	{ echo "Failed to touch ${dest}/.fakelnk" ; rm -rf ${dest} ; exit 1 ; }

