#!/bin/bash

# Invalidation script that checks with the reference compiler
for dir in ./*/
do
	for file in $dir*.go
	do
		cat $file | ~cs520/golitec typecheck 2> /dev/null
		if [[ $? != 1 ]]
		then
			echo -e "\033[0;31m[Error]\033[0m: $file is valid"
		fi
	done
done
