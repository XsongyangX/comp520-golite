#ifndef WEEDER_H
#define WEEDER_H

#include <stdio.h>
#include "tree.h"

void weedRoot(PROGRAM *root);
void weedProgram(PROGRAM *program);
void weedDeclaration(DECLARATION *declaration);
void weedFunction(FUNCTION *function);
void weedStatement(STATEMENT *statement);

#endif