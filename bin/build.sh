#!/bin/bash

emcc -Lpcre/.libs -lpcre -Ipcre src/c/empcre.c -o dist/empcre.js -s \
    EXPORTED_FUNCTIONS="[
        '_empcre_make_re',
        '_empcre_match',
    ]"
