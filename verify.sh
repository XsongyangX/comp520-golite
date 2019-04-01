#!/bin/bash

# run test.sh to codegen first
./test.sh

# call verify
for folder in programs/3-semantics+codegen/valid/*/
do
	for file in $folder*.go
	do
		echo "##### $file  #####"
		echo ""
		programs/3-semantics+codegen/valid/verify.sh $file
		echo ""
	done
done
