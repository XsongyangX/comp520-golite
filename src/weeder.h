#ifndef WEEDER_H
#define WEEDER_H

#include "tree.h"
#include <stdbool.h>

void weedRoot(PROGRAM *root);
void weedProgram(PROGRAM *program);
void weedDeclaration(DECLARATION *declaration);
void weedFunction(FUNCTION *function);
void weedStatement(STATEMENT *statement, bool allowBreak, 
	bool allowContinue, bool lookForDefaultCase);
void weedExpression(EXP *expression, int lineno, 
	bool divBy0, bool funcExpOnly, bool noBlankId);

void notFuncExp(int lineno);

#endif