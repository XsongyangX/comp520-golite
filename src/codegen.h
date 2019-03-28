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
void codegenStructHelper(SYMBOL *field, int depth, symTable *table);
void genStructureHelper(char *tName, char *tMods, SYMBOL *sym, int depth, bool isTypeDef, symTable *table);
void codegenAssign(STATEMENT *stmt, symTable *table, int depth);
void codegenElse(STATEMENT *stmt, symTable *table, int depth);
void codegenIf(STATEMENT *stmt, symTable *table, int depth);
void codegenElif(STATEMENT *stmt, symTable *table, int depth);
void codegenQuickDecl(STATEMENT *stmt, symTable *table, int depth);
SYMBOL *makeSymbolCopy(SYMBOL *sym);
void lookForPlusString(EXP *e, int tabs);
char *getFullStr(EXP *e, symTable *table);
void eqExpArrays(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table);
SYMBOL *findExistingBindingSym(SYMBOL *sym, symTable *table);
void appendCheck(EXP *e, symTable *table, int depth);
SYMBOL *findExistingBindingSym(SYMBOL *sym, symTable *table);
void codegenFuncDeclArgsCheck(SYMBOL *args, symTable *table);
void codegenFuncDeclArgs(SYMBOL *args, symTable *table);
char *eqExpStructsStr(SYMBOL *s1, SYMBOL *s2, char *nameSoFar1, char *nameSoFar2, symTable *table);
char *eqExpArraysStr(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table);


void structAssignHelper(SYMBOL *sym1, SYMBOL *sym2, char *nameSoFar1, char *nameSoFar2, int depth, symTable *table);
void sliceAssignHelper(char *nameSoFar1, char *nameSoFar2, int depth);
void arrayAssignHelper(SYMBOl *sym1, SYMBOL *sym2, char *tMods, int depth, char *nameSoFar1, char *nameSoFar2);
void sliceAppendHelper(EXP *target, EXP *value, symTable *table, int depth);






#endif
