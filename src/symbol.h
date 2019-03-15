#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

#include "tree.h"

enum SymbolKind{
	nullSym, 
	varSym, 
	funcSym, 
	typeSym, 
	structSym
};

enum TABLEID {VARTABLE, TYPETABLE, FUNCTABLE};

struct SYMBOL{
    char *name;
    enum SymbolKind kind;
    TYPE *t;
    int wasRedefined; //used to deal with int redeclerations, for instance
    union{
        SYMBOL *parentSym;
        SYMBOL *returnType;
        SYMBOL *structFields;
        struct {SYMBOL *funcParams; SYMBOL *returnSymRef;} func;
    } val;
    struct SYMBOL *next;
};
struct symTable {
    SYMBOL *varTable[HASHSIZE];
    SYMBOL *typeTable[HASHSIZE];
    SYMBOL *funcTable[HASHSIZE];
    symTable *next;
};

SYMBOL *INT_SYMBOL, *FLOAT_SYMBOL, *RUNE_SYMBOL, *STR_SYMBOL, *BOOL_SYMBOL, *BLANK_SYMBOL;

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
char *getName(TYPE *t);

SYMBOL *makeSymbolCopy(SYMBOL *template);

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
void funcBlockHelper(EXP *exp, symTable *table, int lineno);

/* The following four functions will be in separate .h and .c files. */
// bool typeCheckProgam(PROGRAM *prog);
// bool typeCheckDeclaration(DECLARATION *decl);
// bool typeCheckStatement(STATEMENT *stmt);
// bool typeCheckExpression(EXP *exp);




#endif