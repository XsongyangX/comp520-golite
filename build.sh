#!/bin/bash

# Build the compiler
#
# You MUST replace the following commands with the commands for building your compiler

# $1 is the Makefile rule
make clean -C ./src
make -C ./src $1
