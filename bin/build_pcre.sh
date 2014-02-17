#!/bin/bash

cd pcre
autoreconf -fi
emconfigure ./configure
emmake make
cd ..

