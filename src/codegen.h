#ifndef CODEGEN_H
#define CODEGEN_H

#include "typecheck.h"
#include <string.h>

void codegenProgram(PROGRAM *p);
void codegenDeclaration(DECLARATION *decl, symTable *table, int depth);
void codegenStatement(STATEMENT *stmt, int depth);
void codegenExpression(EXP *e, symTable *table);
void eqExpStructs(SYMBOL *s1, SYMBOL *s2, int mode, char *nameSoFar);
void codegenVarDecl(DECLARATION *decl, symTable *table, int depth);
void codegenTypeDecl(DECLARATION *decl, symTable *table, int depth);
void codegenFuncDecl(DECLARATION *decl, symTable *table, int depth);
void codegenFuncCall(DECLARATION *decl, symTable *table);
void codegenStructHelper(SYMBOL *field, int depth);
void genStructureHelper(char *tName, char *tMods, SYMBOL *sym, int depth, bool isTypeDef);
SYMBOL *makeSymbolCopy(SYMBOL *sym);





#endif
