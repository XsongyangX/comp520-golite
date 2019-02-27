#!/bin/bash

# invalidates all go files with the reference compiler
for file in *.go
do
	echo $file
	cat $file | ~cs520/golitec codegen 
done
