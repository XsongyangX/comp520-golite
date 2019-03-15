#ifndef TYPECHECK_H
#define TYPECHECK_H

#include <stdbool.h>
#include "symbol.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


bool typeProgram(PROGRAM *prog);
void typeCheckDeclaration(DECLARATION *decl, symTable *table);
void typeCheckStatement(STATEMENT *stmt, SYMBOL *func);
SYMBOL *typecheckExp(EXP *exp, symTable *table, int lineno);
bool MatchingTypes(SYMBOL *s1, SYMBOL *s2, int lineno, bool isVerbose);
bool SubTypes(SYMBOL *s1, SYMBOL *s2, int lineno);
bool sliceTypes(SYMBOL *s1, SYMBOL *s2, int lineno);
void investigateType(TYPE *t, int lineno);
void investigateTypePrint(TYPE *t, int lineno);
void switchHelper(STATEMENT *stmt, SYMBOL *func);
void caseHelper(STATEMENT *stmt, SYMBOL *func, SYMBOL *key);
bool checkDefaultCasts(SYMBOL *sym1, SYMBOL *sym2, int lineno);
SYMBOL *structAccessHelper(SYMBOL *sym, char *id, int lineno);
void capHelper(SYMBOL *sym, int lineno);
SYMBOL *elementHelper(SYMBOL *sym, int lineno);
void incdecHelper(SYMBOL *sym, int lineno);
void lenHelper(SYMBOL *sym, int lineno);
SYMBOL *resolveBaseType(SYMBOL *sym1);


//bool InDepthTypeCheck(SYMBOL *sym, char *target);


















#endif
