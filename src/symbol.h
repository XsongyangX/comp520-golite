#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

enum TABLEID {VARTABLE, TYPETABLE, FUNCTABLE};
int Hash(char *str);
symTable *initSymbolTable();
symTable *initScopeTable(symTable *parent);
void putVar(SYMBOL *s, symTable *t, int lineno);
void putType(SYMBOL *s, symTable *t, int lineno);
void putFunc(SYMBOL *s, symTable *t, int lineno);
SYMBOL *makeSymbol(char *name, enum SymbolKind kind, int wasRedefined);
char *lookupVar(symTable *t, char* identifier, int lineno, int doesExit);
char *lookupType(symTable *t, char* identifier, int lineno, int doesExit);
char *lookupFunc(symTable *t, char* identifier, int lineno, int doesExit);
void addPredefinitions(symTable *s);
SYMBOL *getSymbol(symTable *t, char* identifier, enum SymbolKind kind);
char *shortTypeStr(SYMBOL *tmp);
char *longTypeStr(SYMBOL *tmp);


void symProg(PROGRAM *prog);
void symDecl(DECLARATION *decl, symTable *table);
SYMBOL *symStructHelper(DECLARATION *body, symTable *table);
SYMBOL *symFuncHelper(DECLARATION *params, symTable *table);
void symStmt(STATEMENT *stmt, symTable *table);
void symQDecl(STATEMENT *cur, symTable *table);

void symExp(EXP *exp, symTable *table);

void typeProg(PROGRAM *prog);




#endif