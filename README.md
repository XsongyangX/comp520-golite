# Go Project COMP 520
There's a project task tab and wiki tab. Check them out!

There are many scripts from the instructor and some come purely from us:

* `programs`: Test programs are organized by compilation phase and by expected result. Valid programs should output `OK` and status code `0`, while invalid programs should output `Error: <description>` and status code `1`.
  * `Scan+parse`: Runs both the scanner and parser phases
  * `Typecheck`: Runs until the end of the typechecker phase
  * `Codegen`: Runs until your compiler outputs the target code
* `programs-solution`: Test programs from the solution set of Milestone 1, organized like in `programs`
* `src`: Source code for your assignment
* `build.sh`: Builds your compiler using `Make` or similar. You should replace the commands here if necessary to build your compiler
* `prettyChecker.py`: Checks whether two files are identical in content. If not, it reports the line where they differ. Although named for the pretty printer, it can be used to compare any two files. Requires `python3`.
* `run.sh`: Runs your compiler using two arguments (mode - $1 and input file - $2). You should replace the commands here if necessary to invoke your compiler
* `test.sh`: Automatically runs your compiler against test programs in the programs directory and checks the output
* `test-solution.sh`: Automatically runs your compiler against **Milestone 1 solution test programs** given by the instructor
* `testScan.sh`: Automatically runs the scanner against test programs in the `1-scan+parse` directory. You should use the `tokens` argument with this Bash script to make the scanner run on the **entire** program. Otherwise, no arguments given just builds the scanner and runs it for one token.
* `testPretty.sh`: Automatically runs the pretty printer against test programs in the `1-scan+parse` directory. The test is divided in 2 phases: pretty printing twice and comparison with the reference pretty printer and pretty identity. To run the second phase, use the `compare` argument. The script will create a directory called `pretty` in the same directory. It contains the following sub-directories. All these directories are ignored by `git`.
  * `first`: Contains the first pretty printed output of valid `.go` files
  * `reference`: Contains the pretty printed output from the reference compiler of the same valid `.go` files
  * `second`: Contains the second pretty printed output of valid `.go` files. Requires the first pretty printed output be successfully printed.

Comments found in both files provide more details as to the requirements. Since a large portion of grading is performed automatically, please ensure to follow the input/output specifications **exactly**. You must be able to run the provided scripts on the SOCS Trottier machines.

## Milestone 1
Assignment description: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone1.pdf

Specifications: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone1_Specifications.pdf

Rubric: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone1_Rubric.pdf

Report Google doc: https://docs.google.com/document/d/1vYhJTqpWyOOF-wvh8Kg6RB_f8D3YG6cP70nl1RN3q2s/edit?usp=sharing

## Milestone 2
Assignment description: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone2.pdf

Specifications: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone2_Specifications.pdf

Rubric: https://www.cs.mcgill.ca/~cs520/2019/project/Milestone2_Rubric.pdf
