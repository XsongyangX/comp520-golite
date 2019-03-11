#!/bin/bash

for file in *.go
do
	echo $file | ~cs520/golitec typecheck
	if [[ $? != 0 ]]
	then
		echo -e "\033[0;31m[Error]\033[0m: $file is invalid"
	fi
done
