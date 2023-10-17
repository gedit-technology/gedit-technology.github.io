#!/bin/sh

src='../../gedit-technology-website/src/'
dest='../docs/'

./check-directory-content.sh "${src}" || exit 1

# --archive equals -rlptgoD (keep only what's necessary).
rsync -rlp --delete --force --progress --stats	\
	--filter '- /developer-docs/'		\
	--filter '- /user-manuals/'		\
	"${src}" "${dest}"
