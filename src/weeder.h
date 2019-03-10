#ifndef WEEDER_H
#define WEEDER_H

#include "tree.h"
#include <stdbool.h>

void weedRoot(PROGRAM *root);
void weedProgram(PROGRAM *program);
void weedDeclaration(DECLARATION *declaration, int lineno);
void weedFunction(FUNCTION *function);
bool weedStatement(STATEMENT *statement, 
	bool allowBreak, bool allowContinue, bool lookForDefaultCase,
	bool encounteredReturn, bool needReturn);
void weedExpression(EXP *expression, 
	int lineno, bool divBy0, bool funcExpOnly, 
	bool lookForBlankId);
void weedFnCall(FUNCTION *fn, int lineno);

void notFuncExp(int lineno);

#endif