#!/bin/bash

# validate all test files
for dir in ./*/
do
	
	# skip over milestone 1 and weeder files
	if [[ $dir == "./milestone1/" || $dir == "./weeder/" ]]
	then
		continue
	fi

	for file in $dir*.go
	do 
		cat $file | ~cs520/golitec typecheck
		if [[ $? != 0 ]]
		then
			echo -e "\033[0;31m[Error]\033[0m: $file is invalid"
		fi
	done
done

