#!/bin/bash

set -eufo

aclocal
autoconf
libtoolize
automake --add-missing

mkdir build
cd build
../configure
make
