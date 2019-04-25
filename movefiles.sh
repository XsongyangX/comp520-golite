#!/bin/bash

# Move the test programs around the phase folders
# so that proper error messages are caught in the 
# right place

# Our test programs
our_scan="programs/1-scan+parse/invalid/moved"
# Soln test programs
soln_scan="programs-solution/1-scan+parse/invalid/moved"
soln_type="programs-solution/2-typecheck/invalid/moved"

mkdir $our_scan
mkdir $soln_scan
mkdir $soln_type

# Move our programs
our_blank="programs/2-typecheck/invalid/blank_id"
mv $our_blank/blank.go $our_scan
mv $our_blank/blank2.go $our_scan
mv $our_blank/blank3.go $our_scan

our_builtin="programs/2-typecheck/invalid/built_in"
mv $our_builtin/append.go $our_scan
mv $our_builtin/append1.go $our_scan
mv $our_builtin/append2.go $our_scan

# Move the solution programs
mv programs-solution/2-typecheck/invalid/2-1-var_decls/var_decl9.go $soln_scan

soln_return="programs-solution/2-typecheck/invalid/3-4-return_stmt"
mv $soln_return/return_stmt8.go $soln_scan
mv $soln_return/return_stmt16.go $soln_scan
mv $soln_return/return_stmt17.go $soln_scan
mv $soln_return/return_stmt9.go $soln_scan
mv $soln_return/return_stmt15.go $soln_scan
mv $soln_return/return_stmt14.go $soln_scan
mv $soln_return/return_stmt10.go $soln_scan
mv $soln_return/return_stmt11.go $soln_scan
mv $soln_return/return_stmt13.go $soln_scan
mv $soln_return/return_stmt12.go $soln_scan
mv $soln_return/return_stmt6.go $soln_scan
mv $soln_return/return_stmt7.go $soln_scan
