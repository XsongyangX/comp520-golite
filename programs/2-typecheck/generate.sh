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

