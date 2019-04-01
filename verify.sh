#!/bin/bash

# run test.sh to codegen first
if [[ ! -f programs/3-semantics+codegen/valid/identifiers/blank_param.c ]]
then
	./test.sh
fi

# call verify
for folder in programs/3-semantics+codegen/valid/*/
do
	for file in $folder*.go
	do
		echo "##### $file  #####"
		programs/3-semantics+codegen/valid/verify.sh $file
	done
done
