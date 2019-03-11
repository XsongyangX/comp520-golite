#!/bin/bash

for file in *.go
do
	cat $file | ~cs520/golitec typecheck
	if [[ $? == 0 ]]
	then
		echo -e "\033[0;31m[Error]\033[0m: $file is valid"
	fi
done
