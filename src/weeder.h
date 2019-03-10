#ifndef WEEDER_H
#define WEEDER_H

#include "tree.h"
#include <stdbool.h>

typedef struct {
	bool foundTerminating;
	bool foundDefault;
	bool foundBreak;
} Traversal;

void weedRoot(PROGRAM *root);
void weedProgram(PROGRAM *program);
void weedDeclaration(DECLARATION *declaration, int lineno);
void weedFunction(FUNCTION *function);

Traversal weedStatement(STATEMENT *statement, 
	bool allowBreak, 
	bool allowContinue);
void weedExpression(EXP *expression, 
	int lineno, bool divBy0, bool funcExpOnly, 
	bool lookForBlankId);
void weedFnCall(FUNCTION *fn, int lineno);

void notFuncExp(int lineno);
#endif