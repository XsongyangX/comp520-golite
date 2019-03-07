#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

extern int typecheck;
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
    t->table[cell] = s;
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
    t->table[cell] = s;
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
    t->table[cell] = s;
}
/*prints the first subtype, use for strict type equality*/
char *shortTypeStr(SYMBOL *tmp)
{
    if(tmp->kind == structSym)
                return "struct";
    char typename[128];
    typename = strcpy(typename, "");
    TYPE *cur = tmp->t;
    while(true)
    {
        if(cur == NULL)
        {
            strcat(typename, tmp->name);
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
        cur = cur->val.arg;//get next
    }
}
/*prints the bottom level type, use for casts to know if a type is a valid cast of another*/
char *longTypeStr(SYMBOL *tmp)
{
    if(tmp->kind == structSym)
                return "struct";
    char typename[128];
    typename = strcpy(typename, "");
    SYMBOL *cursym = tmp;
    TYPE *cur = tmp->t;
    while(true)
    {
        if(cur == NULL)
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
    while(tmp->kind != nullK)
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
    while(tmp->kind != nullK)
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
    while(tmp->kind != nullK)
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
        while(tmp->kind != nullK)
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
    else if(kind == typeSym){
        SYMBOL *tmp = t->typeTable[cell];
        while(tmp->kind != nullK)
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
        while(tmp->kind != nullK)
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
    *blankname = " "
    SYMBOL *base = makeSymbol(blankname, typeSym, 0);
    base->t = NULL;

    char *name = malloc(sizeof(char)*10);
    *name = "int"
    SYMBOL *tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s);

    *name = malloc(sizeof(char)*10);
    *name = "float64"
    *tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s);
    
    *name = malloc(sizeof(char)*10);
    *name = "bool"
    *tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s);
    
    *name = malloc(sizeof(char)*10);
    *name = "string"
    *tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s);
    
    *name = malloc(sizeof(char)*10);
    *name = "rune"
    *tmp = makeSymbol(name, typeSym, 0);
    tmp->val.parentType = base;
    tmp->t = NULL;
    putType(tmp,s);
    
    *name = malloc(sizeof(char)*10);
    *name = "cap"
    *tmp = makeSymbol(name, funcSym, 1);
    tmp->val.funcRef = makeFCTN(0, name, 0, NULL, makeTYPE(0, 0, blankname), NULL);
    tmp->t = NULL;
    putFunc(tmp,s);
    
    *name = malloc(sizeof(char)*10);
    *name = "len"
    *tmp = makeSymbol(name, funcSym, 1);
    tmp->val.funcRef = makeFCTN(0, name, 0, NULL, makeTYPE(0, 0, blankname), NULL);
    tmp->t = NULL;
    putFunc(tmp,s);
    
    *name = malloc(sizeof(char)*10);
    *name = "append"
    *tmp = makeSymbol(name, funcSym, 1);
    tmp->val.funcRef = makeFCTN(0, name, 0, NULL, makeTYPE(0, 0, blankname), NULL);
    tmp->t = NULL;
    putFunc(tmp,s);
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
    symDecl(prog->declList, prog->globalScope);
}

void symDecl(DECLARATION *decl, symTable *table)
{
    if(decl == NULL)
    {
        return;
    }
    if(decl->next != NULL)
    {
        symDecl(decl->next, table)
    }

    switch(decl->d)
    {
        case typeDecl:
            SYMBOL *tmp = makeSymbol(decl->identifier, typeSym, 1);
            SYMBOL *parent = getSymbol(table, decl->t->name, typeSym);
            tmp->val.parentType = parent;
            tmp->t = decl->t;
            putType(tmp, table, decl->lineno);
            break;
        case varDecl:
            SYMBOL *tmp = makeSymbol(decl->identifier, varSym, 1);
            SYMBOL *parent = getSymbol(table, decl->t->name, varSym);
            tmp->val.parentType = parent;
            tmp->t = decl->t;
            putType(tmp, table, decl->lineno);
            break;
        case structDecl:
            SYMBOL *tmp = makeSymbol(decl->identifier, structSym, 1);
            SYMBOL *bodyList = symStructHelper(decl->val.body, table);
            tmp->val.structFields = bodyList;
            putType(tmp, table, decl->lineno);
            break;
        case funcDecl:
            SYMBOL *tmp = makeSymbol(decl->identifier, funcSym, 1);
            SYMBOL *typeref = getSymbol(table, decl->val.f->returnt->name, typeSym);
            SYMBOL *paramList = symFuncHelper(decl->val.f->params, table);
            tmp->val.func.funcParams = paramList;
            tmp->val.func.returnTypeRef = typeref;
            tmp->t = decl->val.f->returnt;
            putFunc(tmp, table, decl->lineno);

            symTable *subTable = initScopeTable(table);
            while(paramList != NULL)
            {
                putVar(paramList, subTable, decl->lineno);
                paramList = paramList->next;
            }
            symStmt(decl->val.f->body, subTable);
            break;
        case funcCall:
            break; //handled in expr function
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

void symStmt(STATEMENT *stmt, symTable *table)
{
    if(stmt == NULL)
    {
        return;
    }
    if(stmt->next != NULL)
    {
        symStmt(stmt->next, table);
    }
    switch(stmt->kind){
        case assignS:
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
            break;
        case quickDeclS:
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
                    else if(lookupVarLocal(cur->val.assignment.identifier->val.identifier) == NULL)
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
            break;
        case blockS:
            symTable *subtable = initScopeTable(table);
            symStmt(stmt->val.body, subtable, stmt->lineno);
            break;
        case ifS:
        case elifS:
            symTable *subtable = initScopeTable(table);
            symStmt(stmt->val.conditional.optDecl, subTable);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            symStmt(stmt->val.conditional.body, subtable);
            symStmt(stmt->val.conditional.elif, table);
            break;
        case elseS:
            symTable *subtable = initScopeTable(table);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            symStmt(stmt->val.conditional.body, subtable);
            break;
        case forS:
            symTable *subtable = initScopeTable(table);
            symStmt(stmt->val.conditional.optDecl, subTable);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            symStmt(stmt->val.conditional.body, subtable);
            symStmt(stmt->val.conditional.elif, subtable);//may depend on quick declarations here
            break; 
        case whileS:
            symTable *subtable = initScopeTable(table);
            symExp(stmt->val.conditional.condition, subtable, stmt->lineno);
            symStmt(stmt->val.conditional.body, subtable);
            break; 
        case printS:
            symExp(stmt->val.iostmt.condition, table, stmt->lineno);
            break;
        case exprS:
        case returnS:
            symExp(stmt->val.expression, table, stmt->lineno);
            break;
        case switchS:
            symTable *subtable = initScopeTable(table);
            symStmt(stmt->val.switchBody.optDecl, subTable);
            symExp(stmt->val.switchBody.condition, subtable, stmt->lineno);
            symStmt(stmt->val.switchBody.cases, subtable);
            break; 
        case caseS:
            symExp(stmt->caseBody.condition, table, stmt->lineno);
            symTable *subtable = initScopeTable(table);
            symStmt(stmt->caseBody.body, subTable);
            break;
        case breakS://nothing to do
        case continueS://nothing to do
            break;
        case declS:
            symDecl(stmt->val.declaration, table);
            break;
    }
}

void symQDecl(STATEMENT *cur, symTable *table)
{
    SYMBOL *tmp = makeSymbol(cur->val.assignment.identifier->val.identifier, varSym, 1);
    SYMBOL *parent = NULL;
    tmp->val.parentType = parent;
    tmp->t = NULL;
    putType(tmp, table, decl->lineno);
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
            char *name = lookupFunc(table, exp->val.fn->identifier, lineno, 0);
            if(strlen(name) == 0)
            {
                if(strcmp(exp->val.fn->identifier, "cap") == 0) {}
                else if(strcmp(exp->val.fn->identifier, "len") == 0) {}
                else if(strcmp(exp->val.fn->identifier, "append") == 0) {}
                else{
                    sprintf(stderr, "Error: (line %d) undefined function %s.\n", lineno, exp->val.fn->identifier);
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