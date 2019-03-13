#ifndef TYPECHECK_H
#define TYPECHECK_H

#include <stdbool.h>
#include "symbol.h"

typedef struct SYMTABLE symTable;


bool typeProgram(PROGRAM *prog);
SYMTABLE typeCheckDeclaration(DECLARATION *decl, SYMTABLE *table);
SYMTABLE typeCheckStatement(STATEMENT *stmt, SYMTABLE *table);
SYMBOL *typecheckExp(EXP *exp, SYMTABLE *table);



















#endif
