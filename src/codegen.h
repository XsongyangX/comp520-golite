#ifndef CODEGEN_H
#define CODEGEN_H

#include "typecheck.h"
#include <string.h>

void codegenProgram(PROGRAM *p);
void codegenDeclaration(DECLARATION *decl, symTable *table, int depth);
void codegenStatement(STATEMENT *stmt, int depth);
void codegenExpression(EXP *e, symTable *table);
void eqExpStructs(SYMBOL *s1, SYMBOL *s2, char *nameSoFar1, char *nameSoFar2, symTable *table)
void codegenVarDecl(DECLARATION *decl, symTable *table, int depth);
void codegenTypeDecl(DECLARATION *decl, symTable *table, int depth);
void codegenFuncDecl(DECLARATION *decl, symTable *table, int depth);
void codegenFuncCall(EXP *e, SYMBOL* sym, symTable *table);
char *strFuncCall(EXP *e, SYMBOL* sym, symTable *table);
void codegenStructHelper(SYMBOL *field, int depth);
void genStructureHelper(char *tName, char *tMods, SYMBOL *sym, int depth, bool isTypeDef);
SYMBOL *makeSymbolCopy(SYMBOL *sym);
void lookForPlusString(EXP *e, int tabs);
char *getFullStr(EXP *e, symTable *table);
void eqExpArrays(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table);






#endif
