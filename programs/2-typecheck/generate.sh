#!/bin/bash

# look for valid/invalid directories
if [ ! -d valid ]
then
	mkdir valid
fi
if [ ! -d invalid ]
then
	mkdir invalid
fi

# generate test programs

# initialization
python3 initialization.py

python3 short_initialization.py
