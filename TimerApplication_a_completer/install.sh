#!/bin/bash

export APES_COMPILER="mips-sls-dnaos-gcc"
export APES_LINKER="mips-sls-dnaos-gcc"

export APES_CC_FLAGS="-Wall -Werror -Wno-format -std=c99"
export APES_LINKER_FLAGS="-msoft-float -G0 -EL -T $PWD/ldscript"

APES_CC_OPTIMIZATIONS="-g -EL -O0 -G0 -msoft-float -march=mips32"
export APES_CC_OPTIMIZATIONS
