#!/bin/bash

# this file performs a rigorous test of the pretty printer
# using 2 phases
# 	phase 1: Check for validity with reference compiler
#	phase 2: Check for the pretty identity

# arguments: $1 (optional)
# 	if $1 is "compare", the python compare script will be called
#   otherwise, the script will only attempt to pretty print

# check for build script existence
if [ ! -f build.sh ]
then
	echo -e "\033[0;31m[Error]\033[0m: Missing build.sh script"
	exit 1
fi

# attempt a pretty build
./build.sh pretty

if [[ $? != 0 ]]
then
	echo -e "\033[0;31m[Error]\033[0m: Pretty build failed"
	exit 1
fi

# check for run script existence
if [ ! -f run.sh ] 
then
	echo -e "\033[0;31m[Error]\033[0m: Missing run.sh script"
    exit 1
fi

# check for python script existence
if [ ! -f prettyChecker.py ]
then
	echo -e "\033[0;31m[Error]\033[0m: Missing prettyChecker.py script"
	exit 1
fi

# check for test directory for pretty printer
if [ ! -d pretty ]
then
	mkdir pretty
fi
# check for the output directory for the pretty printer (1st pass)
if [ ! -d pretty/first ]
then
	mkdir pretty/first
fi
# check for the output directory for the reference compiler
if [ ! -d pretty/reference ]
then
	mkdir pretty/reference
fi
# check for the output directory for the pretty printer (2nd pass)
if [ ! -d pretty/second ]
then
	mkdir pretty/second
fi

# generate files
for file in programs/1-scan+parse/valid/*.go
do
	name=$(basename $file)
	cat $file | ~cs520/golitec pretty > pretty/reference/$name
	./run.sh pretty $file > pretty/first/$name
	if [ $? != 0 ]
	then
		echo -e "\033[0;31m[Failed to pretty print]\033[0m: $file\n"
	fi
done
for file in pretty/first/*.go 
do 
	name=$(basename $file)
	./run.sh pretty $file > pretty/second/$name
	if [ $? != 0 ]
	then
		echo -e "\033[0;31m[Failed to pretty print a second time]\033[0m: $file\n"
	fi
done

# if the 'compare' argument is given
if [[ $1 == "compare" ]]
then
	# compare files
	
	# first pass vs reference
	echo "===="
	echo "First pass vs. reference"
	echo "===="
	for file in pretty/first/*.go
	do 
		name=$(basename $file)
		python3 prettyChecker.py $file pretty/reference/$name
		
		if [[ $? == 1 ]]
		then
			echo -e "\033[0;31m[Comparison failed]\033[0m: $name\n"
		fi
	done

	# first pass vs second pass
	echo "===="
	echo "First pass vs. second pass"
	echo "===="
	for file in pretty/first/*.go
	do
		name=$(basename $file)
		python3 prettyChecker.py $file pretty/second/$name
		
		if [[ $? == 1 ]]
		then 
			echo -e "\033[0;31m[Identity failed]\033[0m: $name\n"
		fi
	done
fi
