#!/bin/sh

./compile.sh || exit 1

dir='../../docs/user-manuals'
./generate-list "${dir}" > "${dir}/index.html"
