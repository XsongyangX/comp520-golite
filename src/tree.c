#include <stdlib.h>
#include "tree.h"
#include <stdio.h>


Exp *makeEXP_empty()
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = emptyExp;
    return e;
}
Exp *makeEXP_int(int literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = intExp;
    e->val.intLiteral = literal;
    return e;
}
Exp *makeEXP_float(float literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = floatExp;
    e->val.floatLiteral = literal;
    return e;
}
Exp *makeEXP_str(char *literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = strExp;
    e->val.strLiteral = literal;
    return e;
}
Exp *makeEXP_bool(int literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = boolExp;
    e->val.intLiteral = literal;
    return e;
}
Exp *makeEXP_plus(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = plusExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_minus(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = minusExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_times(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = timesExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_div(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = divExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_mod(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = modExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_pos(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = posExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_neg(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = negExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_ptr(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = ptrExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_addr(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = addrExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_par(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = parExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_eq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = eqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_neq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = neqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_geq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = geqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_leq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = leqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_gt(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = gtExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_lt(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = ltExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_or(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = orExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_and(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = andExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_andnot(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = andnotExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_id(char *identifier)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = idExp;
    e->val.identifier = identifier;
    return e;
}
Exp *makeEXP_band(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = bitAndExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_bor(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = bitOrExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_xor(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = xorExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_lshift(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = lshiftExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_rshift(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = rshiftExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_range(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = rangeExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_index(Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = indexExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_element(Exp *e1, Exp *e2)//e1 should be an identifier, e2 should be a range or and index exp
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = elementExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;

}
Exp *makeEXP_invoc(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = invocExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_append(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = appendExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
Exp *makeEXP_len(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = lenExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_cap(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = capExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_not(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = notExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_uXOR(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = uxorExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
Exp *makeEXP_func(char *identifier, int size, SDecl *args)
{//format of args(params) subject to change
    Exp *e = malloc(sizeof(Exp));
    e->kind = funcExp;
    //make a dummy function struct to represent the function call
    Fctn *tmpf = malloc(sizeof(Fctn));
    tmpf->identifier = identifier;
    tmpf->paramCount = size;
    //setup the linked list of arguments
    tmpf->body = NULL;
    tmpf->params = args;
    tmpf->returnt = NULL;
    tmpf->next = NULL;
    e->val.fn = tmpf;
    return e;
}

Decl *makeDECL(int isVar, char *identifier, char *declType, int gtype, int arraysize, Exp *rhs)
{
    Decl *d = malloc(sizeof(Decl));
    d->d = isVar;
    d->identifier = identifier;
    d->t = malloc(sizeof(type));
    d->t->SymbolType = declType;
    d->t->gType = gtype;
    d->t->size = arraysize;
    d->val.right = rhs;
    d->next = NULL;
    return d;
}
Decl *makeDECL_norhs(int isVar, char *identifier, char *declType, int gtype, int arraysize)
{
    Decl *d = malloc(sizeof(Decl));
    d->d = isVar;
    d->identifier = identifier;
    d->t = malloc(sizeof(type));
    d->t->SymbolType = declType;
    d->t->gType = gtype;
    d->t->size = arraysize;
    d->val.right = NULL;
    d->next = NULL;
    return d;
}
Decl *makeDECL_notype(int isVar, char *identifier, int gtype, int arraysize,  Exp *rhs)
{
    Decl *d = malloc(sizeof(Decl));
    d->d = isVar;
    d->identifier = identifier;
    d->t = malloc(sizeof(type));
    d->t->SymbolType = NULL;
    d->t->gType = gtype;
    d->t->size = arraysize;
    d->val.right = rhs;
    d->next = NULL;
    return d;
}
Decl *makeDECL_struct( char *identifier, Decl *body, Fctn *fbody)
{
    Decl *d = malloc(sizeof(Decl));
    d->d = structDecl;
    d->identifier = identifier;
    d->t = malloc(sizeof(type));
    d->t->SymbolType = NULL;
    d->t->gType = structType;
    d->val.body.dbody = body;
    d->val.body.fbody = fbody;
    d->next = NULL;
    return d;
}
SDecl *makeSDecl(Exp *e, char* declType, int gtype, int arraysize)
{
    SDecl *sd = malloc(sizeof(SDecl));
    sd->identifier = e;
    sd->t = malloc(sizeof(type));
    sd->t->SymbolType = declType;
    sd->t->gType = gtype;
    sd->t->size = arraysize;
    sd->next = NULL;
    return sd;
}

Fctn *makeFCTN(int lineno, char *identifier, int size, SDecl *params, char *returnType, int gtype, int arraysize, Stmt *body)
{
    Fctn *f = malloc(sizeof(Fctn));
    f->lineno = lineno;
    f->identifier = identifier;
    f->paramCount = size;
    f->params = params;
    f->body = body;
    f->returnt = malloc(sizeof(type));
    f->returnt->SymbolType = returnType;
    f->returnt->gType = gtype;
    f->returnt->size = arraysize;
    f->next = NULL;
    return f;
}

Stmt *makeSTMT_assmt(int lineno, Exp *identifier, Exp *val, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = assignS;
    s->val.assignment.identifier = identifier;
    s->val.assignment.value = val;
    s->next = next;
    return s;
}
Stmt *makeSTMT_multiassmt(int lineno, int depth, SDecl *lhs, SDecl *rhs, Stmt *next)
{
    if(lhs->next == NULL && rhs->next == NULL)
    {
        return makeSTMT_assmt(lineno, lhs->identifier, rhs->identifier, next);
    }
    else if(lhs->next != NULL && rhs->next == NULL)
    {
        fprintf(stderr, "Error: (line %d) too few assignments. %d to the left, %d to the right.\n", lineno, depth+1, depth);
    }
    else if(lhs->next == NULL && rhs->next != NULL)
    {
        fprintf(stderr, "Error: (line %d) too few identifiers. %d to the left, %d to the right.\n", lineno, depth, depth+1);
    }
    else
    {
        next = makeSTMT_multiassmt(lineno, depth+1, lhs->next, rhs->next, next);
        return makeSTMT_assmt(lineno, lhs->identifier, rhs->identifier, next);
    }
    
}
Stmt *makeSTMT_if(int lineno, Exp *condition, Decl *optDecl, Stmt *body, Stmt *elif, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = ifS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = optDecl;
    s->val.conditional.elif = elif;
    s->next = next;
    return s;
}
Stmt *makeSTMT_elif(int lineno, Exp *condition, Decl *optDecl, Stmt *body, Stmt *elif, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = elifS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = optDecl;
    s->val.conditional.elif = elif;
    s->next = next;
    return s;
}
Stmt *makeSTMT_else(int lineno, Stmt *body, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = elseS;
    s->val.conditional.body = body;
    s->val.conditional.condition = NULL;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    s->next = next;
    return s;
}
Stmt *makeSTMT_while(int lineno, Exp *condition, Stmt *body, Stmt *next)//empty expression for inf loop
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = whileS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    s->next = next;
    return s;
}
Stmt *makeSTMT_for(int lineno, Decl *optDecl, Exp *condition, Stmt *body, Stmt *action, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = forS;
    action->next = body;
    s->val.conditional.body = action;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    s->next = next;
    return s;
}
Stmt *makeSTMT_decl(int lineno, Decl *declaration, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = declS;
    s->val.declaration = declaration;
    s->next = next;
    return s;
}
Stmt *makeSTMT_exp(int lineno, Exp *expression, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = exprS;
    s->val.expression = expression;
    s->next = next;
    return s;
}
Stmt *makeSTMT_switch(int lineno, Exp *condition, Decl *optDecl, Stmt *cases, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = switchS;
    s->val.switchBody.condition = condition;
    s->val.switchBody.optDecl = optDecl;
    s->val.switchBody.cases = cases;
    s->next = next;
    return s;
}
Stmt *makeSTMT_case(int lineno, Exp *condition, Stmt *body, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = caseS;
    s->val.caseBody.condition = condition;
    s->val.caseBody.body = body;
    s->next = next;
    return s;
}
Stmt *makeSTMT_block(int lineno, Stmt *body, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = blockS;
    s->val.body = body;
    s->next = next;
    return s;
}
Stmt *makeSTMT_print(int lineno, Exp *expression, int hasNewLine, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = printS;
    s->val.iostmt.value = expression;
    s->val.iostmt.hasNewLine = hasNewLine;
    s->next = next;
    return s;
}
Stmt *makeSTMT_break(int lineno, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = breakS;
    s->next = next;
    return s;
}
Stmt *makeSTMT_continue(int lineno, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = continueS;
    s->next = next;
    return s;
}
Stmt *makeSTMT_return(int lineno, Exp *expression, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = returnS;
    s->next = next;
    s->val.expression = expression;
    return s;
}

Prog *makePROG(char* package, Decl *declList, Fctn *fnList)
{
    Prog *p = malloc(sizeof(Prog));
    p->package = package;
    p->declList = declList;
    p->fnList = fnList;
    return p;
}

/*Stuff written by Greg starts here */


Type *makeTYPE(GroupingType gtype, int size, char *name, type *arg){
    type *t = malloc(sizeof(type));
    t->SymbolType = identifier;
    t->gType = gType;
    t->name = name;
    t->val->arg = arg;
    return t; 
}

Type *makeTYPE_struct(int size, char *name, Decl *args){
    type *t = malloc(sizeof(type));
    t->SymbolType = identifier;
    t->gType = structType;
    t->name = name;
    t->val->args = args;
    return t; 
}

/* A constructor for a block of identifiers
Used e.g. in multiple assignments, or in function headers */
Exp *makeEXP_idblock(char *identifier, Exp *next){
    Exp *e = malloc(sizeof(Exp));
    e->type = NULL;
    e->kind = idblockExp;
    e->val->idblock->next = next;
    e->val->idblock->identifier = identifier;
}

Exp *makeEXP_expblock(Exp *e, Exp *next){
    Exp *e = malloc(sizeof(Exp));
    e->type = NULL;
    e->kind = expblockExp;
    e->val->expblock->next = next;
    e->val->expblock->value = e;
}

Decl *makeDECL_type(type typeNode){
    Decl *d = malloc(sizeof(Decl));
    d->d = typeDecl;
    d->t = typeNode
    d->val.right = NULL;
    d->next = NULL;
    return d;
}

/*Called upon reaching the end of a block norhs_assignment, e.g. var a, b, c int
Takes the linked list of identifiers and makes each of them into a declaration statement,
propogating the type to all statements*/
Decl *makeDECL_blocknorhs(int lineno, Exp *ids, type *t){
    Decl *d = malloc(sizeof(Decl));
    // If there is no next id, create a dec for the last id 
    if(ids->next == NULL){
        d = makeDECL_norhs(typeDecl, ids->val->idblock->identifier, varDecl, nilType, NULL);
        return d;
    }
    //If there is a next id, recurse onto that id. 
    //Then create a dec for this level and a pointer this dec to the subtree 
    else{
        Decl *nextD = malloc(sizeof(Decl));
        nextD = makeDECL_blocknorhs(lineno, ids->next, t);
        d = makeDECL_norhs(typeDecl, ids->val->idblock->identifier, varDecl, nilType, NULL);
        d->next = nextD;
        return d;
    }
}

/*Called for block declarations, but with a right hand side. Does basically the same thing
As makeDECL_blocknorhs(), but each dec actually assigns too.
Throws an error if there is an unequal number of ids and exps on either side.*/
Decl *makeDECL_block(int lineno, Exp *ids, type *t, Exp *exps){
    Decl *d = malloc(sizeof(Decl));
    if(ids->next == NULL && exps->next == NULL){
        d = makeDecl(typeDecl, ids->val->idblock->identifier, varDecl, nilType, NULL, exps->val->expblock->value);
        return d;
    }
    else if(ids->next != NULL && exps->next != NULL){
        Decl *nextD = malloc(sizeof(Decl));
        nextD = makeDECL_block(lineno, ids->next, t, exps->next);
        d = makeDecl(typeDecl, ids->val->idblock->identifier, varDecl, nilType, NULL, exps->val->expblock->value);
        d->next = nextD;
        return d;
    }
    else{
        printf("Error in line %d; unequal number of idents and exps\n", lineno);
        exit(1);
    }
}
/*A similar function to block declarations, but for STMTs*/
Stmt *makeSTMT_blockassign(int lineno, Exp *ids, Exp *exps){
    Stmt *s = malloc(sizeof(Stmt));
    if(ids->next == NULL && exps->next == NULL){
        s = makeSTMT_assmt(lineno, ids->val->idblock->identifier, exps->val->expblock->value, NULL);
        return s;
    }
    else if(ids->next != NULL && exps->next != NULL){
        Stmt *nextS = malloc(sizeof(Stmt));
        nextD = makeSTMT_blockassign(lineno, ids->next, exps->next);
        s = makeSTMT_assmt(lineno, ids->val->idblock->identifier, exps->val->expblock->value, nextD);
        return s;
    }
    else{
        printf("Error in line %d; unequal number of idents and exps\n", lineno);
        exit(1);
    }
}

/*Simple functions to get to the bottom of subtrees.
Made to address a problem with block declarations.
e.g. For block type norhsdeclarations, a linked list of exps (idents) and a type 
Must be converted to a linked list of decs, each with one ident and one type.
The problem is the bottom of this new subtree must be pointed to the next dec.
This requires going to the bottom in the parser at another point*/
Stmt *findBottomSTMT(Stmt *s){
    if(s->next == NULL){
        return s;
    }
    else{
        return findBottomSTMT(s->next);
    }
}

Decl *findBottomDECL(Decl *d){
    if(d->next == NULL){
        return d;
    }
    else{
        return findBottomDECL(d->next);
    }
}


