#!/bin/bash

em++ -O2 --bind \
    -Lpcre/.libs \
    -lpcre -lpcrecpp \
    -Ipcre \
    -o dist/empcre.js \
    -s EXPORTED_FUNCTIONS="[
        '_pcre_compile',
        '_pcre_exec',
        '_pcre_fullinfo',
    ]" \
    src/c/empcre.cpp
