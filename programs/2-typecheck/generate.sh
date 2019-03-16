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

python3 initialization.py

python3 short_initialization.py

python3 literal_assignment.py

python3 return.py

python3 call.py