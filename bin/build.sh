#!/bin/bash

emcc -Lpcre/.libs -lpcre -Ipcre src/c/empcre.c -o dist/empcre.js -s \
    EXPORTED_FUNCTIONS="[
    `bin/find_exports.sh | sed -e 's/^/\"/' -e 's/$/\",/'`
    ]"
