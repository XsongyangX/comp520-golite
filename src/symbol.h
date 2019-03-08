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
char *lookupVarLocal(symTable *t, char* identifier, int lineno);
char *lookupTypeLocal(symTable *t, char* identifier, int lineno);
char *lookupFunc(symTable *t, char* identifier, int lineno, int doesExit);
void addPredefinitions(symTable *s);
SYMBOL *getSymbol(symTable *t, char* identifier, enum SymbolKind kind);
char *shortTypeStr(SYMBOL *tmp);
char *longTypeStr(SYMBOL *tmp);

void checkProg(PROGRAM *prog);
void symProg(PROGRAM *prog);
void symTypeDecl(DECLARATION *decl, symTable *table, int depth);
void symVarDecl(DECLARATION *decl, symTable *table, int depth);
void symStructDecl(DECLARATION *decl, symTable *table, int depth);
void symFuncDecl(DECLARATION *decl, symTable *table, int depth);
void symDecl(DECLARATION *decl, symTable *table, int depth);
void printStructFields(symTable *table, SYMBOL *fields);
void printType(TYPE *t);
void printFnArgs(symTable *table, DECLARATION *args);
void printFnScope(symTable *table, DECLARATION *args, int depth);
SYMBOL *symStructHelper(DECLARATION *body, symTable *table);
SYMBOL *symFuncHelper(DECLARATION *params, symTable *table);
void symAssignStmt(STATEMENT *stmt, symTable *table, int depth);
void symQDeclStmt(STATEMENT *stmt, symTable *table, int depth);
void symElifStmt(STATEMENT *stmt, symTable *table, int depth);
void symElseStmt(STATEMENT *stmt, symTable *table, int depth);
void symForStmt(STATEMENT *stmt, symTable *table, int depth);
void symBlockStmt(STATEMENT *stmt, symTable *table, int depth);
void symWhileStmt(STATEMENT *stmt, symTable *table, int depth);
void symCaseStmt(STATEMENT *stmt, symTable *table, int depth);
void symSwitchStmt(STATEMENT *stmt, symTable *table, int depth);
void symStmt(STATEMENT *stmt, symTable *table, int depth);
void symQDecl(STATEMENT *cur, symTable *table);
void printQDeclHelper(symTable *table, STATEMENT *stmt, int depth);

void symExp(EXP *exp, symTable *table, int lineno);

void typeProg(PROGRAM *prog);
void typeDecl(DECLARATION *decl);
void typeStmt(STATEMENT *stmt);
void typeExp(EXP *exp);




#endif