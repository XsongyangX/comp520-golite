#include "tree.h"
#include "string.h"


Exp makeEXP_empty()
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = emptyExp;
    return e;
}
Exp makeEXP_int(int literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = intExp;
    e->val.intLiteral = literal;
    return e;
}
Exp makeEXP_float(float literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = floatExp;
    e->val.floatLiteral = literal;
    return e;
}
Exp makeEXP_str(char *literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = strExp;
    e->val.strLiteral = literal;
    return e;
}
Exp makeEXP_bool(int literal)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = boolExp;
    e->val.intLiteral = literal;
    return e;
}
Exp makeExp_plus(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = plusExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_minus(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = minusExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_times(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = timesExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_div(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = divExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_mod(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = modExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_pos(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = posExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_neg(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = negExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_ptr(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = ptrExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_addr(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = addrExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_par(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = parExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_eq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = eqExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_neq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = neqExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_geq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = geqExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_leq(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = leqExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_gt(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = gtExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_lt(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = ltExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_or(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = orExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_and(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = andExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_andnot(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = andnotExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_id(char *identifier)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = idExp;
    e->val.identifier = identifier;
    return e;
}
Exp makeExp_band(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = bitAndExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_bor(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = bitOrExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_xor(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = xorExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_lshift(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = lshiftExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_rshift(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = rshiftExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_range(Exp *e1, Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = rangeExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_appendExp(Exp *e1. Exp *e2)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = appendExp;
    e->val.binary.lhs = e1;
    e-.val.binary.rhs = e2;
    return e;
}
Exp makeExp_len(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = lenExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_cap(Exp *e1)
{
    Exp *e = malloc(sizeof(Exp));
    e->kind = capExp;
    e->val.binary.lhs = NULL;
    e-.val.binary.rhs = e1;
    return e;
}
Exp makeExp_func(char *identifier, int size, SDecl *args)
{//format of args(params) subject to change
    Exp *e = malloc(sizeof(Exp));
    e->kind = funcExp;
    //make a dummy function struct to represent the function call
    Fctn *tmpf = malloc(sizeof(Fctn));
    tpmf->identifier = identifier;
    tmpf->paramCount = size;
    //setup the linked list of parameters
    tmpf->params = args;
    tmpf->body = NULL;
    tmpf->returnType = NULL;
    e->val.fn = tmpf;
    return e;
}

Decl makeDECL(char *identifier, char *declType, Exp *rhs)
{
    Decl *d = malloc(sizeof(Decl));
    d->identifier = identifier;
    d->declType = declType;
    d->right = rhs;
    d->next = NULL;
    return d;
}
Decl makeDECL(char *identifier, char *declType)
{
    Decl *d = malloc(sizeof(Decl));
    d->identifier = identifier;
    d->declType = declType;
    d->right = NULL;
    d->next = NULL;
    return d;
}
Decl makeDECL(char *identifier,  Exp *rhs)
{
    Decl *d = malloc(sizeof(Decl));
    d->identifier = identifier;
    d->declType = NULL;
    d->right = rhs;
    d->next = NULL;
    return d;
}

SDecl makeSDecl(Exp *e, char* declType)
{
    SDecl *sd = malloc(sizeof(SDecl));
    sd->identifier = e;
    sd->declType = declType;
    sd->next = NULL;
    return sd;
}

Fctn makeFCTN(int lineno, char *identifier, int size, SDecl *params, char *returnType, Stmt *body)
{
    Fctn *f = malloc(sizeof(Fctn));
    f->lineno = lineno;
    f->identifier = identifier;
    f->paramCount = size;
    f->params = params;
    f->body = body;
    f->returnType = returnType;
    return f;
}

Stmt makeSTMT_assmt(int lineno, char *identifier, Exp *val)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = assignS;
    s->next = NULL;
    s->val.assignment.identifier = identifier;
    s->val.assignment.value = val;
    return s;
}
Stmt makeSTMT_if(int lineno, Exp *condition, Decl *optDecl, Stmt *body, Stmt *elif)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = ifS;
    s->next = NULL;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = optDecl;
    s->val.conditional.elif = elif;
    return s;
}
Stmt makeSTMT_elif(int lineno, Exp *condition, Stmt *body, Stmt *elif)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = elifS;
    s->next = NULL;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = elif;
    return s;
}
Stmt makeSTMT_else(int lineno, Exp *condition, Stmt *body)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = elseS;
    s->next = NULL;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    return s;
}
Stmt makeSTMT_while(int lineno, Exp *condition, Stmt *body)//empty expression for inf loop
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = whileS;
    s->next = NULL;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    return s;
}
Stmt makeSTMT_for(int lineno, Decl *optDecl, Exp *condition, Stmt *body, Stmt *action)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = forS;
    s->next = NULL;
    action->next = body;
    s->val.conditional.body = action;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    return s;
}
Stmt makeSTMT_decl(int lineno, Decl *declaration)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = declS;
    s->next = NULL;
    s->val.declaration = declaration;
    return s;
}
Stmt makeSTMT_exp(int lineno, Exp *expression)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = exprS;
    s->next = NULL;
    s->val.expression = expression;
    return s;
}
Stmt makeSTMT_switch(int lineno, Exp *condition, Stmt *cases)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = switchS;
    s->next = NULL;
    s->val.switchBody.condition = condition;
    s->val.switchBody.cases = cases;
    return s;
}
Stmt makeSTMT_case(int lineno, Exp *condition, Stmt *body, Stmt *next)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = caseS;
    s->next = NULL;
    s->val.caseBody.condition = condition;
    s->val.caseBody.body = body;
    s->val.caseBody.next = next;
    return s;
}
Stmt makeSTMT_block(int lineno, Stmt *body)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = blockS;
    s->next = NULL;
    s->val.body = body;
    return s;
}
Stmt makeSTMT_print(int lineno, Exp *expression, int hasNewLine)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = printS;
    s->next = NULL;
    s->val.iostmt.value = expression;
    s->val.iostmt.hasNewLine = hasNewLine;
    return s;
}
Stmt makeSTMT_break(int lineno)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = breakS;
    s->next = NULL;
    return s;
}
Stmt makeSTMT_continue(int lineno)
{
    Stmt *s = malloc(sizeof(Stmt));
    s->lineno = lineno;
    s->kind = continueS;
    s->next = NULL;
    return s;
}

Prog makePROG(char* package, Decl *declList, Fctn *fnList)
{
    Prog *p = malloc(sizeof(Prog));
    p->package = package;
    p->declList = declList;
    p->fnList = fnList;
    return p;
}