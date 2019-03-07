#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

extern int symbolPrint;
/*taken from the JOOS compiler*/
int Hash(char *str)
{
    unsigned int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % HASHSIZE;
}
/*initializes the hash table and allocates space for each slot*/
symTable *initSymbolTable()
{
    symTable *t = malloc(sizeof(symTable));
    for(int i = 0; i < HASHSIZE; i++)
    {
        t->varTable[i] = malloc(sizeof(SYMBOL));
        t->varTable[i]->kind = nullSym;
        t->typeTable[i] = malloc(sizeof(SYMBOL));
        t->typeTable[i]->kind = nullSym;
        t->funcTable[i] = malloc(sizeof(SYMBOL));
        t->funcTable[i]->kind = nullSym;
    }
    t->next = NULL;
    return t;
}
symTable *initScopeTable(symTable *parent)
{
    symTable *t = initSymbolTable();
    t->next = parent;
    return t;
}
void putVar(SYMBOL *s, symTable *t, int lineno)
{
    int cell = Hash(s->name);
    s->next = t->varTable[cell];
    SYMBOL *tmp = s->next;
    SYMBOL *prev = s;
    while(tmp->kind != nullSym)
    {
        if(strcmp(tmp->name, s->name) == 0){//redeclaration
            fprintf(stderr, "Error: (line %d) redeclaration of var %s.\n", lineno, s->name);
            exit(1);
        }
    }
    t->varTable[cell] = s;
}
void putType(SYMBOL *s, symTable *t, int lineno)
{
    int cell = Hash(s->name);
    s->next = t->typeTable[cell];
    SYMBOL *tmp = s->next;
    SYMBOL *prev = s;
    while(tmp->kind != nullSym)
    {
        if(strcmp(tmp->name, s->name) == 0){//redeclaration
            if(tmp->wasRedefined)
            {
                fprintf(stderr, "Error: (line %d) redeclaration of type %s.\n", lineno, s->name);
                exit(1);
            }
            prev->next = tmp->next;
            break;
        }
    }
    t->typeTable[cell] = s;
}
void putFunc(SYMBOL *s, symTable *t, int lineno)
{
    int cell = Hash(s->name);
    s->next = t->funcTable[cell];
    SYMBOL *tmp = s->next;
    SYMBOL *prev = s;
    while(tmp->kind != nullSym)//redeclaration check
    {
        if(strcmp(tmp->name, s->name) == 0){
            fprintf(stderr, "Error: (line %d) redeclaration of function %s.\n", lineno, s->name);
            exit(1);
        }
    }
    t->funcTable[cell] = s;
}
/*prints the first subtype, use for strict type equality*/
char *shortTypeStr(SYMBOL *tmp)
{
    if(tmp->kind == structSym)
        return "struct";
    char *typename = malloc(128);
    strcpy(typename, "");
    TYPE *cur = tmp->t;
    if(cur->gType == nilType)
    {
        strcat(typename, " ");//NULL type, i.e. void
        return typename;
    }
    while(1)
    {
        if(cur == NULL || cur->gType != nilType)
        {
            return typename;
        }    
        else if(cur->gType == arrayType){
            char tmpstr[32];
            sprintf(tmpstr, "[%d]", cur->size);
            strcat(typename, tmpstr);
        }
        else if(cur->gType == sliceType)
        {
            strcat(typename, "[]");
        }
        else{
            strcat(typename, cur->name);
        }
        cur = cur->val.arg;//get next
    }
}
/*prints the bottom level type, use for casts to know if a type is a valid cast of another*/
char *longTypeStr(SYMBOL *tmp)
{
    if(tmp->kind == structSym)
                return "struct";
    char *typename = malloc(128);
    strcpy(typename, "");
    SYMBOL *cursym = tmp;
    TYPE *cur = tmp->t;
    if(cur->gType == nilType)
    {
        strcat(typename, " ");//NULL type, i.e. void
        return typename;
    }
    while(1)
    {
        if(cur == NULL || cur->gType == nilType)
        {//move up hierarchy
            if(cursym->kind == funcSym)
            {
                cursym = cursym->val.func.returnTypeRef;
                cur = cursym->t;
            }
            else{
                if(strcmp(cursym->val.parentType->name, " ") == 0){
                    //next is the base type, end recursion now
                    strcat(typename, cursym->name);
                    return typename;
                }
                if(cursym->val.parentType->kind == structSym){
                    strcat(typename, cursym->name);
                    return typename;
                }
                cursym = cursym->val.parentType;
                cur = cursym->t;
            }
            continue;
        }
        else if(cur->gType == arrayType){
            char tmpstr[32];
            sprintf(tmpstr, "[%d]", cur->size);
            strcat(typename, tmpstr);
        }
        else if(cur->gType == sliceType)
        {
            strcat(typename, "[]");
        }
        cur = cur->val.arg;//get next

    }
}
/*returns the type of var*/
char *lookupVar(symTable *t, char* identifier, int lineno, int doesExit)
{
    int cell = Hash(identifier);
    SYMBOL *tmp = t->varTable[cell];
    while(tmp->kind != nullSym)
    {
        if (strcmp(tmp->name, identifier) == 0)
        {
            return shortTypeStr(tmp->val.parentType);
        }
        else
        {
            tmp = tmp->next;
        }
    }
    if(t->next != NULL)
    {
        return lookupVar(t->next, identifier, lineno, doesExit);       
    }
    else if (doesExit){
        fprintf(stderr, "Error: (line %d) identifier \"%s\" not declared\n", lineno, identifier);
        exit(1);
    }
    return "";
}
/*returns the mapping of the type identifier*/
char *lookupType(symTable *t, char* identifier, int lineno, int doesExit)
{
    int cell = Hash(identifier);
    SYMBOL *tmp = t->typeTable[cell];
    while(tmp->kind != nullSym)
    {
        if (strcmp(tmp->name, identifier) == 0)
        {
            return shortTypeStr(tmp);
        }
        else
        {
            tmp = tmp->next;
        }
    }
    if(t->next != NULL)
    {
        return lookupType(t->next, identifier, lineno, doesExit);       
    }
    else if(doesExit){
        fprintf(stderr, "Error: (line %d) identifier \"%s\" not declared\n", lineno, identifier);
        exit(1);
    }
    return "";
}
/*returns the type of var, only looks at the local scope,
and does not exit*/
char *lookupVarLocal(symTable *t, char* identifier, int lineno)
{
    int cell = Hash(identifier);
    SYMBOL *tmp = t->varTable[cell];
    while(tmp->kind != nullSym)
    {
        if (strcmp(tmp->name, identifier) == 0)
        {
            return shortTypeStr(tmp->val.parentType);
        }
        else
        {
            tmp = tmp->next;
        }
    }
    return NULL;
}
/*returns the mapping of the type identifier, only looks at the local scope, 
and does not exit*/
char *lookupTypeLocal(symTable *t, char* identifier, int lineno)
{
    int cell = Hash(identifier);
    SYMBOL *tmp = t->typeTable[cell];
    while(tmp->kind != nullSym)
    {
        if (strcmp(tmp->name, identifier) == 0)
        {
            return shortTypeStr(tmp);
        }
        else
        {
            tmp = tmp->next;
        }
    }
    return NULL;
}
/*returns the function return type name of the function with name identifier*/
char *lookupFunc(symTable *t, char* identifier, int lineno, int doesExit)
{
    int cell = Hash(identifier);
    SYMBOL *tmp = t->funcTable[cell];
    while(tmp->kind != nullSym)
    {
        if (strcmp(tmp->name, identifier) == 0)
        {
            return shortTypeStr(tmp);            
        }
        else
        {
            tmp = tmp->next;
        }
    }
    if(t->next != NULL)
    {
        return lookupFunc(t->next, identifier, lineno, doesExit);       
    }
    else if (doesExit){
        fprintf(stderr, "Error: (line %d) identifier \"%s\" not declared\n", lineno, identifier);
        exit(1);
    }
    return "";
}
/*returns the symbol name identifier*/
SYMBOL *getSymbol(symTable *t, char* identifier, enum SymbolKind kind)
{
    int cell = Hash(identifier);
    if(kind == varSym)
    {
        SYMBOL *tmp = t->varTable[cell];
        while(tmp->kind != nullSym)
        {
            if (strcmp(tmp->name, identifier) == 0)
            {
                return tmp;                
            }
            else
            {
                tmp = tmp->next;
            }
        }
    }
    else if(kind == typeSym){
        SYMBOL *tmp = t->typeTable[cell];
        while(tmp->kind != nullSym)
        {
            if (strcmp(tmp->name, identifier) == 0)
            {
                return tmp;
                
            }
            else
            {
                tmp = tmp->next;
            }
        }
    }
    else{
        SYMBOL *tmp = t->funcTable[cell];
        while(tmp->kind != nullSym)
        {
            if (strcmp(tmp->name, identifier) == 0)
            {
                return tmp;                
            }
            else
            {
                tmp = tmp->next;
            }
        }
    }
    if(t->next != NULL)
    {
        return getSymbol(t->next, identifier, kind);       
    }
    else{
        return NULL;
    }
}
SYMBOL *makeSymbol(char *name, enum SymbolKind kind, int wasRedefined)
{
    SYMBOL *tmp = malloc(sizeof(SYMBOL));
    tmp->name = name;
    tmp->kind = kind;
    tmp->wasRedefined = wasRedefined;
    tmp->next = NULL;
    return tmp;
}
void addPredefinitions(symTable *s)
{//adds predefs for init, main, int, bool, float64, string, rune, len, cap, append
//dummy type: if a type points to this, then we know it is the actual type, not a redefinition
    char *blankname = malloc(sizeof(char)*10);
    strcpy(blankname, " ");
    SYMBOL *base = makeSymbol(blankname, typeSym, 0);
    base->t = NULL;

    char *name = malloc(sizeof(char)*10);
    strcpy(name, "int");
    SYMBOL *tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s, 0);

    name = malloc(sizeof(char)*10);
    strcpy(name, "float64");
    tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s,0);
    
    name = malloc(sizeof(char)*10);
    strcpy(name, "bool");
    tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s,0);
    
    name = malloc(sizeof(char)*10);
    strcpy(name, "string");
    tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s,0);
    
    name = malloc(sizeof(char)*10);
    strcpy(name, "rune");
    tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s,0);

    name = malloc(sizeof(char)*10);
    strcpy(name, "true");
    tmp = makeSymbol(name, varSym, 0);
    tmp->val.parentType = getSymbol(s, "bool", typeSym);
    tmp->t = makeTYPE(baseType, 0, "bool", NULL);
    putType(tmp,s,0);

    name = malloc(sizeof(char)*10);
    strcpy(name, "false");
    tmp = makeSymbol(name, varSym, 0);
    tmp->val.parentType = getSymbol(s, "bool", typeSym);
    tmp->t = makeTYPE(baseType, 0, "bool", NULL);
    putType(tmp,s,0);
    
    // name = malloc(sizeof(char)*10);
    // name = "cap"
    // tmp = makeSymbol(name, funcSym, 1);
    // tmp->val.funcRef = makeFCTN(0, name, 0, NULL, makeTYPE(0, 0, blankname), NULL);
    // tmp->t = NULL;
    // putFunc(tmp,s,0);
    
    // *name = malloc(sizeof(char)*10);
    // name = "len"
    // tmp = makeSymbol(name, funcSym, 1);
    // tmp->val.funcRef = makeFCTN(0, name, 0, NULL, makeTYPE(0, 0, blankname), NULL);
    // tmp->t = NULL;
    // putFunc(tmp,s,0);
    
    // name = malloc(sizeof(char)*10);
    // name = "append"
    // tmp = makeSymbol(name, funcSym, 1);
    // tmp->val.funcRef = makeFCTN(0, name, 0, NULL, makeTYPE(0, 0, blankname), NULL);
    // tmp->t = NULL;
    // putFunc(tmp,s,0);
}
/*First will build a symbol table for the program, then call typechecking functions*/
void checkProg(PROGRAM *prog)
{
    prog->globalScope = initSymbolTable();

    symProg(prog);

    typeProg(prog);

}

void symProg(PROGRAM *prog)
{
    addPredefinitions(prog->globalScope);
    if(symbolPrint == 1)
    {
        printf("{\n");
        prettyTabs(1);
        printf("int[type] = int\n");
        prettyTabs(1);
        printf("float64[type] = float64\n");
        prettyTabs(1);
        printf("bool[type] = bool\n");
        prettyTabs(1);
        printf("string[type] = string]\n");
        prettyTabs(1);
        printf("true[constant] = bool\n");
        prettyTabs(1);
        printf("false[constant] = bool\n");
    }
    symDecl(prog->declList, prog->globalScope, 2);
    if(symbolPrint == 1)
    {
        printf("}\n");
    }
}
char *getName(TYPE *t)
{
    if(t == NULL)
    {
        return "";
    }
    if(t->name == NULL)
    {
        return getName(t->val.arg);
    }
    else
    {
        return t->name;
    }
    
}
void symTypeDecl(DECLARATION *decl, symTable *table, int depth)
{
            SYMBOL *tmp = makeSymbol(decl->identifier, typeSym, 1);
            char *parentname = getName(decl->t);
            SYMBOL *parent = getSymbol(table, parentname, typeSym);
            if(parent == NULL)
            {
                fprintf(stderr, "Error: (line %d) undefined type %s.\n", decl->lineno, parentname);
                exit(1);
            }
            tmp->val.parentType = parent;
            tmp->t = decl->t;
            putType(tmp, table, decl->lineno);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("%s[type] = %s -> %s\n", decl->identifier, decl->identifier, lookupType(table, decl->identifier, decl->lineno, 1));
            }
}
void symVarDecl(DECLARATION *decl, symTable *table, int depth)
{
            SYMBOL *tmp = makeSymbol(decl->identifier, varSym, 1);
            char *parentname = getName(decl->t);
            SYMBOL *parent = getSymbol(table, parentname, typeSym);
            if(parent == NULL)
            {
                fprintf(stderr, "Error: (line %d) undefined type %s.\n", decl->lineno, parentname);
                exit(1);
            }
            tmp->val.parentType = parent;
            tmp->t = decl->t;
            putType(tmp, table, decl->lineno);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("%s[variable] = %s\n", decl->identifier, lookupVar(table, decl->identifier, decl->lineno, 1));
            }
}
void symStructDecl(DECLARATION *decl, symTable *table, int depth)
{
            SYMBOL *tmp = makeSymbol(decl->identifier, structSym, 1);
            SYMBOL *bodyList = symStructHelper(decl->val.body, table);
            tmp->val.structFields = bodyList;
            putType(tmp, table, decl->lineno);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("%s[type] = %s -> struct {\n", decl->identifier, decl->identifier);
                printStructFields(table, bodyList);
                printf("}\n");
            }
}
void symFuncDecl(DECLARATION *decl, symTable *table, int depth)
{
            SYMBOL *tmp = makeSymbol(decl->identifier, funcSym, 1);
            char *typename = getName(decl->val.f->returnt);
            SYMBOL *typeref = getSymbol(table, typename, typeSym);
            if(typeref == NULL && strlen(typename) != 0)
            {
                fprintf(stderr, "Error: (line %d) undefined type %s.\n", decl->lineno, typename);
                exit(1);
            }
            SYMBOL *paramList = symFuncHelper(decl->val.f->params, table);
            tmp->val.func.funcParams = paramList;
            tmp->val.func.returnTypeRef = typeref;
            tmp->t = decl->val.f->returnt;
            putFunc(tmp, table, decl->lineno);

            //main and init check
            if(strcmp(decl->identifier, "main") == 0 || strcmp(decl->identifier, "init") == 0)
            {
                if(tmp->t->gType != nilType || paramList != NULL)
                {
                    fprintf(stderr, "Error: (line %d) function %s must have a void return type and no arguments.\n", decl->lineno, decl->identifier);
                    exit(1);
                }
            }

            symTable *subTable = initScopeTable(table);
            while(paramList != NULL)
            {
                putVar(paramList, subTable, decl->lineno);
                paramList = paramList->next;
            }
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("%s[function] = (", decl->val.f->identifier);
                printFnArgs(table, paramList);
                prettyTabs(depth);
                printf("{\n");
                printFnScope(table, paramList, depth+1);
            }
            symStmt(decl->val.f->body, subTable, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symDecl(DECLARATION *decl, symTable *table, int depth)
{
    if(decl == NULL)
    {
        return;
    }
    if(decl->next != NULL)
    {
        symDecl(decl->next, table, depth);
    }

    switch(decl->d)
    {
        case typeDecl:
            if(decl->chain != NULL)
                symDecl(decl->chain, table, depth);
            symTypeDecl(decl, table, depth);
            break;
        case varDecl:
            if(decl->chain != NULL)
                symDecl(decl->chain, table, depth);
            symVarDecl(decl, table, depth);
            break;
        case structDecl:
            if(decl->chain != NULL)
                symDecl(decl->chain, table, depth);
            symStructDecl(decl, table, depth);
            break;
        case funcDecl:
            if(decl->chain != NULL)
                symDecl(decl->chain, table, depth);
            symFuncDecl(decl, table, depth);
            break;
        case funcCall:
            break; //handled in expr function
    }
}
void printType(TYPE *t)
{
    TYPE *tmp = t;
    while(tmp != NULL && tmp->gType != nilType)
    {
        if(tmp->gType == arrayType){
            printf("[%d]", tmp->size);
        }
        else if(tmp->gType == sliceType)
        {
            printf("[]");
        }
        else{
            printf("%s;", tmp->name);
        }
        tmp = tmp->val.arg;
    }
}
void printFnArgs(symTable *table, SYMBOL *args)
{
    if(args != NULL)
    {
        printFnArgs(table, args->next);
        printf(", ");
        printType(args->t);
    }

}
void printFnScope(symTable *table, SYMBOL *args, int depth)
{
    if(args != NULL)
    {
        printFnScope(table, args->next, depth);
        prettyTabs(depth);
        printf("%s[variable] = ", args->name);
        printType(args->t);
        printf("\n");
    }
}
void printStructFields(symTable *table, SYMBOL *fields)
{
    if(fields != NULL)
    {
        printStructFields(table, fields->next);
        printf(", ");
        printf("%s ", fields->name);
        printType(fields->t);
    }
}
/*helps construct struct body reference, these are all var declarations in GoLite*/
SYMBOL *symStructHelper(DECLARATION *body, symTable *table)
{
    if(body == NULL)
    {
        return NULL;
    }
    SYMBOL *next = symStructHelper(body->next, table);
    SYMBOL *tmp = makeSymbol(body->identifier, varSym, 1);
    SYMBOL *parent = getSymbol(table, body->t->name, varSym);
    tmp->val.parentType = parent;
    tmp->next = next;
    tmp->t = body->t;
    return tmp;
}
/*puts the function arguments in a reversed list*/
SYMBOL *symFuncHelper(DECLARATION *params, symTable *table)
{
    if(params == NULL)
    {
        return NULL;
    }
    SYMBOL *next = symStructHelper(params->next, table);
    SYMBOL *tmp = makeSymbol(params->identifier, varSym, 1);
    SYMBOL *parent = getSymbol(table, params->t->name, varSym);
    tmp->val.parentType = parent;
    tmp->next = next;
    tmp->t = params->t;
    return tmp;
}
void symAssignStmt(STATEMENT *stmt, symTable *table, int depth)
{
            STATEMENT *cur = stmt;
            while(cur != NULL)
            {
                if(cur->val.assignment.identifier->kind == idExp)
                {
                    if(strcmp(cur->val.assignment.identifier->val.identifier, "_")==0)
                    {
                        continue;
                    }
                }
                    
                symExp(cur->val.assignment.identifier, table, stmt->lineno);
                symExp(cur->val.assignment.value, table, stmt->lineno);
                cur = cur->val.assignment.chain;    
            }
}
void symQDeclStmt(STATEMENT *stmt, symTable *table, int depth)
{
            int newLocals = 0;
            STATEMENT *cur = stmt;
            while(cur != NULL)
            {
                if(cur->val.assignment.identifier->kind == idExp)
                {
                    if(strcmp(cur->val.assignment.identifier->val.identifier, "_")==0)
                    {
                        continue;
                    }
                    else if(lookupVarLocal(table, cur->val.assignment.identifier->val.identifier, stmt->lineno) == NULL)
                    {//keep track of new declared variables
                        newLocals++;
                        
                        symQDecl(cur, table);
                        symExp(cur->val.assignment.value, table, stmt->lineno);
                    }  
                    else{
                        symExp(cur->val.assignment.identifier, table, stmt->lineno);
                        symExp(cur->val.assignment.value, table, stmt->lineno);
                    }
                }  
                else{
                    symExp(cur->val.assignment.identifier, table, stmt->lineno);
                    symExp(cur->val.assignment.value, table, stmt->lineno);

                }
                cur = cur->val.assignment.chain;    
            }
            if(newLocals == 0)
            {
                fprintf(stderr, "Error: (line %d) no new variables declared in quick declaration.\n", stmt->lineno);
                exit(1);
            }
            if(symbolPrint == 1)
            {
                printQDeclHelper(table, stmt, depth);
            }
}
void symElifStmt(STATEMENT *stmt, symTable *table, int depth){
            symTable *subtable = initScopeTable(table);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.conditional.optDecl, subtable, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth+1);
                printf("{\n");
            }
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            symStmt(stmt->val.conditional.body, subtable, depth+2);
            if(symbolPrint == 1)
            {
                prettyTabs(depth+1);
                printf("}\n");
            }
            symStmt(stmt->val.conditional.elif, table, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symElseStmt(STATEMENT *stmt, symTable *table, int depth){
            symTable *subtable = initScopeTable(table);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.conditional.body, subtable, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symForStmt(STATEMENT *stmt, symTable *table, int depth){
            symTable *subtable = initScopeTable(table);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.conditional.optDecl, subtable, depth+1);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            if(symbolPrint == 1)
            {
                prettyTabs(depth+1);
                printf("{\n");
            }
            symStmt(stmt->val.conditional.body, subtable, depth+2);
            if(symbolPrint == 1)
            {
                prettyTabs(depth+1);
                printf("}\n");
            }
            symStmt(stmt->val.conditional.elif, subtable, depth+1);//may depend on quick declarations here
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symBlockStmt(STATEMENT *stmt, symTable *table, int depth)
{
            symTable *subtable = initScopeTable(table);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.body, subtable, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symWhileStmt(STATEMENT *stmt, symTable *table, int depth){
            symTable *subtable = initScopeTable(table);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.conditional.body, subtable, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symSwitchStmt(STATEMENT *stmt, symTable *table, int depth){
            symTable *subtable = initScopeTable(table);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.switchBody.optDecl, subtable, depth+1);
            
            symExp(stmt->val.switchBody.condition, subtable, stmt->lineno);
            symStmt(stmt->val.switchBody.cases, subtable, depth+1);
            
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symCaseStmt(STATEMENT *stmt, symTable *table, int depth){
            symExp(stmt->val.caseBody.condition, table, stmt->lineno);
            symTable *subtable = initScopeTable(table);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("{\n");
            }
            symStmt(stmt->val.caseBody.body, subtable, depth+1);
            if(symbolPrint == 1)
            {
                prettyTabs(depth);
                printf("}\n");
            }
}
void symStmt(STATEMENT *stmt, symTable *table, int depth)
{
    if(stmt == NULL)
    {
        return;
    }
    if(stmt->next != NULL)
    {
        symStmt(stmt->next, table, depth);
    }
    switch(stmt->kind){
        case assignS:
            symAssignStmt(stmt, table, depth);
            break;
        case quickDeclS:
            symQDeclStmt(stmt, table, depth);
            break;
        case blockS:
            symBlockStmt(stmt, table, depth);
            break;
        case ifS:
        case elifS:
            symElifStmt(stmt, table, depth);
            break;
        case elseS:
            symElseStmt(stmt, table,depth);
            break;
        case forS:
            symForStmt(stmt, table, depth);
            break; 
        case whileS:
            symWhileStmt(stmt, table, depth);
            break; 
        case printS:
            symExp(stmt->val.iostmt.value, table, stmt->lineno);
            break;
        case exprS:
        case returnS:
            symExp(stmt->val.expression, table, stmt->lineno);
            break;
        case switchS:
            symSwitchStmt(stmt, table, depth);
            break; 
        case caseS:
            symCaseStmt(stmt, table, depth);
            break;
        case breakS://nothing to do
        case continueS://nothing to do
            break;
        case declS:
            symDecl(stmt->val.declaration, table, depth);
            break;
    }
}

void printQDeclHelper(symTable *table, STATEMENT *stmt, int depth)
{
    if(stmt == NULL)
    {
        return;
    }
    printQDeclHelper(table, stmt->val.assignment.chain, depth);
    prettyTabs(depth);
    printf("%s[variable] = <infer>\n", stmt->val.assignment.identifier->val.identifier);
}
void symQDecl(STATEMENT *cur, symTable *table)
{
    SYMBOL *tmp = makeSymbol(cur->val.assignment.identifier->val.identifier, varSym, 1);
    SYMBOL *parent = NULL;
    tmp->val.parentType = parent;
    tmp->t = NULL;
    putType(tmp, table, cur->lineno);
}
void symExp(EXP *exp, symTable *table, int lineno)
{
    if(exp == NULL)
    {
        return;
    }
    switch(exp->kind)
    {
        case intExp:
        case floatExp:
        case strExp:
        case rawstrExp:
        case boolExp:
        case runeExp:
            break;
        case plusExp:
        case minusExp:
        case timesExp:
        case divExp:
        case modExp:
        case eqExp:
        case neqExp:
        case geqExp:
        case leqExp:
        case gtExp:
        case ltExp:
        case orExp:
        case andExp:
        case andnotExp:
        case bitAndExp:
        case bitOrExp:
        case xorExp:
        case lshiftExp:
        case rshiftExp:
        case invocExp:
        case elementExp:
            symExp(exp->val.binary.lhs, table, lineno);
            symExp(exp->val.binary.rhs, table, lineno);
            break;
        case notExp:
        case posExp:
        case negExp:
        case parExp:
        case indexExp:
        case uxorExp:
            symExp(exp->val.binary.rhs, table, lineno);
            break;
        case idExp:
            lookupVar(table, exp->val.identifier, lineno, 1);
            break;
        case funcExp:
            if(strlen(lookupFunc(table, exp->val.fn->identifier, lineno, 0)) == 0)//function 
            {
                if(strcmp(exp->val.fn->identifier, "cap") == 0) {}
                else if(strcmp(exp->val.fn->identifier, "len") == 0) {}
                else if(strcmp(exp->val.fn->identifier, "append") == 0) {}
                else{
                    fprintf(stderr, "Error: (line %d) undefined function %s.\n", lineno, exp->val.fn->identifier);
                    exit(1);
                }
            }
            symExp(exp->val.fn->params->val.fnCallBlock, table, lineno);
            break;
        case expblockExp://this case occurs from function calls
            symExp(exp->val.expblock.value, table, lineno);
            symExp(exp->val.expblock.next, table, lineno);
            break;
        default:
            fprintf(stderr, "Unrecognized expression kind %d", exp->kind);
            exit(1);
    }
}
void typeProg(PROGRAM *prog)
{

}