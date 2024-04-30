#!/bin/sh
set -e
tmpFile=$(mktemp)
gcc -ggdb -lcurl app/*.c -o $tmpFile
gdb -q -ex "" "$tmpFile" "$@" 
