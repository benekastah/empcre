#!/bin/bash

find src -name '*.h' | \
    xargs cat | \
    grep -A 1 '^//@export$' | \
    sed -e '/^\/\/@export$/d' -e '/^--$/d' | \
    perl -pe 's/^(\w+\s+\*?)+(\w+)\s*\(.*$/_$2/'
