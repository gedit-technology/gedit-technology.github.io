#!/bin/sh

if test $# -ne 2
then
	echo "$0: wrong number of arguments." 1>&2
	exit 1
fi

directory="$1"
src_file="$2"

dest="${directory}/thumbnail-${src_file}"

# It uses ImageMagick and keeps the aspect ratio (only the width is given).
convert "${directory}/${src_file}" -resize 250 "${dest}"
