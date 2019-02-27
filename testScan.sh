#!/bin/bash

# Tests the scanner exclusively
# add tokens, if you want to print the tokens

# build the scanner
make clean -C ./src
make scan -C ./src

# run the scanner
# run the tokenizer if specified
echo "----------------VALID-----------------"
for file in programs/1-scan+parse/valid/*.go
do
	echo $file

	if [[ $1 != "tokens" ]]
	then
		./run.sh scan $file
	else
		./run.sh tokens $file
	fi
done

echo "----------------INVALID-----------------"
for file in programs/1-scan+parse/invalid/*.go
do
	echo $file

	if [[ $1 != "tokens" ]]
	then
		./run.sh scan $file
	else
		./run.sh tokens $file
	fi
done


