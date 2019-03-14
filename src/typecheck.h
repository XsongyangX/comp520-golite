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
void typeCheckStatement(STATEMENT *stmt, symTable *table);
SYMBOL *typecheckExp(EXP *exp, symTable *table, int lineno);
bool MatchingTypes(SYMBOL *s1, SYMBOL *s2, int lineno);
bool SubTypes(SYMBOL *s1, SYMBOL *s2, int lineno);
bool ArrayTypes(SYMBOL *s1, SYMBOL *s2, int lineno);
bool InDepthTypeCheck(SYMBOL *sym, char *target);


















#endif
