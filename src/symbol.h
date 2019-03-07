#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

enum TABLEID {VARTABLE, TYPETABLE, FUNCTABLE};
void prettyTabs(int n);
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
void printStructFields(symTable *table, SYMBOL *fields);
void printType(TYPE *t);
void printFnArgs(symTable *table, SYMBOL *args);
void printFnScope(symTable *table, SYMBOL *args, int depth);
SYMBOL *symStructHelper(DECLARATION *body, symTable *table);
SYMBOL *symFuncHelper(DECLARATION *params, symTable *table);
void symStmt(STATEMENT *stmt, symTable *table, int depth);
void symQDecl(STATEMENT *cur, symTable *table, int depth);
void printQDeclHelper(symTable *table, STATEMENT *stmt, int depth);

void symExp(EXP *exp, symTable *table);

void typeProg(PROGRAM *prog);




#endif