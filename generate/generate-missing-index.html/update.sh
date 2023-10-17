#!/bin/sh

./compile.sh || exit 1

dir='../../docs/user-manuals'
./generate-list "${dir}" > "${dir}/index.html"

dir='../../docs/developer-docs'
./generate-list "${dir}" > "${dir}/index.html"

dir='../../docs/developer-docs/extra'
./generate-list "${dir}" > "${dir}/index.html"

dir='../../docs/developer-docs/extra/libgedit-gtksourceview'
./generate-list "${dir}" > "${dir}/index.html"
