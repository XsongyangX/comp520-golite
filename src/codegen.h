#ifndef CODEGEN_H
#define CODEGEN_H

#include "typecheck.h"
#include <string.h>

typedef struct continueLabel continueLabel;
typedef struct breakLabel breakLabel;

struct continueLabel{
    int id;
    continueLabel *next;
};
struct breakLabel{
    int id;
    breakLabel *next;
};

void codegenProgram(PROGRAM *p);
void codegenDeclaration(DECLARATION *decl, symTable *table, int depth);
void codegenStatement(STATEMENT *stmt, int depth);
void codegenExpression(EXP *e, symTable *table);
void eqExpStructs(SYMBOL *s1, SYMBOL *s2, char *nameSoFar1, char *nameSoFar2, symTable *table);

char *getTypeName(SYMBOL *tmp);
char *getTypeModifiers(SYMBOL *tmp);
bool structCompatible(SYMBOL *sym1, SYMBOL *sym2);

void codegenVarDecl(DECLARATION *decl, symTable *table, int depth);
void codegenTypeDecl(DECLARATION *decl, symTable *table, int depth);
void codegenFuncDecl(DECLARATION *decl, symTable *table, int depth);
void codegenFuncCall(EXP *e, SYMBOL* sym, symTable *table);
char *strFuncCall(EXP *e, SYMBOL* sym, symTable *table);
void codegenStructHelper(SYMBOL *field, int depth, symTable *table);
void genStructureHelper(char *tName, char *tMods, SYMBOL *sym, int depth, char *typeName, bool isTypeDef, symTable *table);
void codegenAssign(STATEMENT *stmt, int depth);
void codegenAssignLast(STATEMENT *stmt, int depth);
void codegenElse(STATEMENT *stmt, int depth);
void codegenIf(STATEMENT *stmt, int depth);
void codegenElif(STATEMENT *stmt, int depth);
void codegenQuickDecl(STATEMENT *stmt, int depth);
void codegenPrint(EXP *e, symTable *table, bool hasNewline, int depth);
void codegenWhile(STATEMENT *stmt, int depth);
void codegenFor(STATEMENT *stmt, int depth);
void codegenSwitch(STATEMENT *stmt, int depth);
void codegenCase(STATEMENT *stmt, EXP *ref, int depth);
SYMBOL *makeSymbol_Copy(SYMBOL *sym);
void lookForPlusString(EXP *e, int tabs);
char *getFullStr(EXP *e, symTable *table);
void eqExpArrays(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table);
SYMBOL *findExistingBindingSym(SYMBOL *sym, symTable *table);
void appendCheck(EXP *e, symTable *table, int depth);
void codegenFuncDeclArgsCheck(SYMBOL *args, symTable *table);
void codegenFuncDeclArgs(SYMBOL *args, symTable *table);
char *eqExpStructsStr(SYMBOL *s1, SYMBOL *s2, char *nameSoFar1, char *nameSoFar2, symTable *table);
char *eqExpArraysStr(SYMBOL *s1, SYMBOL *s2, char *tMods, char *nameSoFar1, char *nameSoFar2, symTable *table);


void structAssignHelper(SYMBOL *sym1, SYMBOL *sym2, char *nameSoFar1, char *nameSoFar2, int depth, symTable *table);
void sliceAssignHelper(char *nameSoFar1, char *nameSoFar2, int depth);
void arrayAssignHelper(SYMBOL *sym1, SYMBOL *sym2, char *tMods, int depth, char *nameSoFar1, char *nameSoFar2, symTable *table);
void sliceAppendHelper(EXP *target, EXP *value, symTable *table, int depth);
char *findExistingBinding(SYMBOL *sym, symTable *table);






#endif
