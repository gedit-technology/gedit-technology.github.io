#!/bin/sh

# Checks that the directory contains only regular files and directories, not
# symbolic links or other special files.

directory="$1"

echo "Sanity check of directory '${directory}'..."

tmp_file=/tmp/check-directory-content.$$

find "${directory}" '(' -not -type f ')' -and '(' -not -type d ')' > "${tmp_file}"
if test "$?" -ne 0
then
	echo 'Error: the find command has returned a non-zero exit status.' 1>&2
	exit 1
fi

if test -s "${tmp_file}"
then
	echo "Error: the directory '${directory}' contains the following non-regular files:" 1>&2
	cat "${tmp_file}"
	rm "${tmp_file}"
	exit 1
fi

rm "${tmp_file}"
echo "Sanity check of directory '${directory}': done."
