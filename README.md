# Go Project COMP 520
There's a project task tab and wiki tab. Check them out!

We provide 3 scripts for convenience and two directories for organization:

* `programs`: Test programs are organized by compilation phase and by expected result. Valid programs should output `OK` and status code `0`, while invalid programs should output `Error: <description>` and status code `1`.
  * `Scan+parse`: Runs both the scanner and parser phases
  * `Typecheck`: Runs until the end of the typechecker phase
  * `Codegen`: Runs until your compiler outputs the target code
* `src`: Source code for your assignment
* `build.sh`: Builds your compiler using `Make` or similar. You should replace the commands here if necessary to build your compiler
* `run.sh`: Runs your compiler using two arguments (mode - $1 and input file - $2). You should replace the commands here if necessary to invoke your compiler
* `test.sh`: Automatically runs your compiler against test programs in the programs directory and checks the output
* `testScan.sh`: Automatically runs the scanner against test programs in the `1-scan+parse` directory. You should use the `tokens` argument with this Bash script to make the scanner run on the **entire** program. Currently, this script only runs the `valid` programs in the directory.

Comments found in both files provide more details as to the requirements. Since a large portion of grading is performed automatically, please ensure to follow the input/output specifications **exactly**. You must be able to run the provided scripts on the SOCS Trottier machines.

## Milestone 1
Assignment description: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone1.pdf

Specification: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone1_Specifications.pdf

Rubric: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone1_Rubric.pdf

Report Google doc: https://docs.google.com/document/d/1vYhJTqpWyOOF-wvh8Kg6RB_f8D3YG6cP70nl1RN3q2s/edit?usp=sharing
