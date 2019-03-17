#ifndef CODEGEN_H
#define CODEGEN_H

#include "typecheck.h"

void codegenProgram(PROGRAM *p);
void codegenDeclaration(DECLARATION *d);
void codegenStatement(STATEMENT *s);
void codegenExpression(EXP *e, SYMTABLE *table);
void eqExpStructs(SYMBOL *s1, SYMBOL *s2, int mode, char *nameSoFar);






#endif
