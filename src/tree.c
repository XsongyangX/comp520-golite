/*Constructors for AST nodes*/
#include <stdlib.h>
#include "tree.h"
#include <stdio.h>

/* Helper error printing */
void printUnequalAssignError(int lineno){
	fprintf(stderr, 
	"Error: (line %d) unequal number of variables and assignment values\n", 
	lineno);
	exit(1);
}

/*Not ever used, emptyExp = 0 acts as a safety net in later stages*/
EXP *makeEXP_empty()
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = emptyExp;
    return e;
}
EXP *makeEXP_int(int literal)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = intExp;
    e->val.intLiteral = literal;
    return e;
}
EXP *makeEXP_rune(char literal)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = runeExp;
    e->val.runeLiteral = literal;
    return e;
}
EXP *makeEXP_float(float literal)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = floatExp;
    e->val.floatLiteral = literal;
    return e;
}
EXP *makeEXP_str(char *literal)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = strExp;
    e->val.strLiteral = literal;
    return e;
}
/*We distinguish raw strings because the pretty printing phase asks for this
Otherwise it will behave like a string*/
EXP *makeEXP_rawstr(char *literal)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = rawstrExp;
    e->val.strLiteral = literal;
    return e;
}

EXP *makeEXP_bool(int literal)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = boolExp;
    e->val.intLiteral = literal;
    return e;
}
EXP *makeEXP_plus(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = plusExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_minus(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = minusExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_times(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = timesExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_div(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = divExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_mod(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = modExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_pos(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = posExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_neg(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = negExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_par(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = parExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_eq(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = eqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_neq(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = neqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_geq(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = geqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_leq(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = leqExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_gt(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = gtExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_lt(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = ltExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_or(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = orExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_and(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = andExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_andnot(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = andnotExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_id(char *identifier)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = idExp;
    e->val.identifier = identifier;
    return e;
}
EXP *makeEXP_band(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = bitAndExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_bor(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = bitOrExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_xor(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = xorExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_lshift(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = lshiftExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_rshift(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = rshiftExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_index(EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = indexExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_element(EXP *e1, EXP *e2)//e1 should be an identifier, e2 should be an index exp
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = elementExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;

}
EXP *makeEXP_invoc(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = invocExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_append(EXP *e1, EXP *e2)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = appendExp;
    e->val.binary.lhs = e1;
    e->val.binary.rhs = e2;
    return e;
}
EXP *makeEXP_len(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = lenExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_cap(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = capExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_not(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = notExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_uxor(EXP *e1)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = uxorExp;
    e->val.binary.lhs = NULL;
    e->val.binary.rhs = e1;
    return e;
}
EXP *makeEXP_func(char *identifier, int size, DECLARATION *args)
{//format of args(params) subject to change
    EXP *e = malloc(sizeof(EXP));
    e->kind = funcExp;
    //make a dummy function struct to represent the function call
    FUNCTION *tmpf = malloc(sizeof(FUNCTION));
    tmpf->identifier = identifier;
    tmpf->paramCount = size;
    //setup the linked list of arguments
    tmpf->body = NULL;
    tmpf->params = args;
    tmpf->returnt = NULL;
    e->val.fn = tmpf;
    return e;
}
/*inserts a funcExp node into an existing tree*/
void makeEXP_func_access(EXP *identifier, int size, DECLARATION *args)
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = funcExp;
    //make a dummy function struct to represent the function call
    FUNCTION *tmpf = malloc(sizeof(FUNCTION));
    tmpf->identifier = "";
    tmpf->paramCount = size;
    //setup the linked list of arguments
    tmpf->body = NULL;
    tmpf->params = args;
    tmpf->returnt = NULL;
    e->val.fn = tmpf;
    identifier = makeEXP_invoc(identifier,e);
}
/*makes a declaration node with a rhs value
the rhs value should be null in case of a type decl*/
DECLARATION *makeDECL(int isVar, char *identifier, TYPE *t, EXP *rhs)
{
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = isVar;
    d->identifier = identifier;
    d->t = t;
    d->val.right = rhs;
    d->next = NULL;
    return d;
}
/*makes a declaration node with no rhs
a type should be provided*/
DECLARATION *makeDECL_norhs(int isVar, char *identifier, TYPE *t)
{
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = isVar;
    d->identifier = identifier;
    d->t = t;
    d->val.right = NULL;
    d->next = NULL;
    return d;
}
/*makes a declaration with no type declared
a rhs should be provided*/
DECLARATION *makeDECL_notype(int isVar, char *identifier, int gtype, int arraysize,  EXP *rhs)
{
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = isVar;
    d->identifier = identifier;
    d->t = malloc(sizeof(TYPE));
    d->t->name = NULL;
    d->t->gType = gtype;
    d->t->size = arraysize;
    d->val.right = rhs;
    d->next = NULL;
    return d;
}
/*makes a declaration node for a struct definition
the struct body is made of a list of declarations*/
DECLARATION *makeDECL_struct( char *identifier, DECLARATION *body)
{
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = structDecl;
    d->identifier = identifier;
    d->t = malloc(sizeof(TYPE));
    d->t->name = identifier;
    d->t->gType = structType;
    d->val.body = body;
    d->next = NULL;
    return d;
}
/*makes a declaration node for a function
simply wraps around the function*/
DECLARATION *makeDECL_fn(DECLARATION *next, FUNCTION *f)
{
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = funcDecl;
    d->val.f = f;
    d->next = next;
    return d;
}

FUNCTION *makeFCTN(int lineno, char *identifier, int size, DECLARATION *params, TYPE *returnType, STATEMENT *body)
{
    FUNCTION *f = malloc(sizeof(FUNCTION));
    f->lineno = lineno;
    f->identifier = identifier;
    f->paramCount = size;
    f->params = params;
    f->body = body;
    f->returnt = returnType;
    return f;
}
/*makes an assignment statement of the form exps = exps
note that some variables may be declared in this*/
STATEMENT *makeSTMT_assmt(int lineno, EXP *identifier, EXP *val)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = assignS;
    s->val.assignment.identifier = identifier;
    s->val.assignment.value = val;
    s->val.assignment.chain = NULL;
    s->next = NULL;
    return s;
}
/*makes an assignment statement of the form exps := exps
note that some variables should be declared in this*/
STATEMENT *makeSTMT_qdecl(int lineno, EXP *identifier, EXP *val)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = quickDeclS;
    s->val.assignment.identifier = identifier;
    s->val.assignment.value = val;
    s->val.assignment.chain = NULL;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_if(int lineno, EXP *condition, STATEMENT *optDecl, STATEMENT *body, STATEMENT *elif)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = ifS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = optDecl;
    s->val.conditional.elif = elif;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_elif(int lineno, EXP *condition, STATEMENT *optDecl, STATEMENT *body, STATEMENT *elif)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = elifS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = optDecl;
    s->val.conditional.elif = elif;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_else(int lineno, STATEMENT *body)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = elseS;
    s->val.conditional.body = body;
    s->val.conditional.condition = NULL;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_while(int lineno, EXP *condition, STATEMENT *body)//empty expression for inf loop
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = whileS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = NULL;
    s->val.conditional.elif = NULL;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_for(int lineno, STATEMENT *optDecl, EXP *condition, STATEMENT *body, STATEMENT *action)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = forS;
    s->val.conditional.body = body;
    s->val.conditional.condition = condition;
    s->val.conditional.optDecl = optDecl;
    s->val.conditional.elif = action;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_decl(int lineno, DECLARATION *declaration)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = declS;
    s->val.declaration = declaration;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_exp(int lineno, EXP *expression)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = exprS;
    s->val.expression = expression;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_switch(int lineno, EXP *condition, STATEMENT *optDecl, STATEMENT *cases)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = switchS;
    s->val.switchBody.condition = condition;
    s->val.switchBody.optDecl = optDecl;
    s->val.switchBody.cases = cases;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_case(int lineno, EXP *condition, STATEMENT *body)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = caseS;
    s->val.caseBody.condition = condition;
    s->val.caseBody.body = body;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_block(int lineno, STATEMENT *body)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = blockS;
    s->val.body = body;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_print(int lineno, EXP *expression, int hasNewLine)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = printS;
    s->val.iostmt.value = expression;
    s->val.iostmt.hasNewLine = hasNewLine;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_break(int lineno)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = breakS;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_continue(int lineno)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = continueS;
    s->next = NULL;
    return s;
}
STATEMENT *makeSTMT_return(int lineno, EXP *expression)
{
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = returnS;
    s->next = NULL;
    s->val.expression = expression;
    return s;
}

PROGRAM *makePROG(char* package, DECLARATION *declList)
{
    PROGRAM *p = malloc(sizeof(PROGRAM));
    p->declList = declList;
    p->package = package;
     
    
}

/*Stuff written by Greg starts here */


TYPE *makeTYPE(int gtype, int size, char *name){
    TYPE *t = malloc(sizeof(TYPE));
    t->size = size;
    t->gType = gtype;
    t->name = name;
    return t; 
}

// TYPE *makeTYPE_struct(int size, char *name, EXP *args){
//     TYPE *t = malloc(sizeof(TYPE));
//     t->gType = structType;
//     t->name = name;
//     t->val.args = args;
//     return t; 
// }

/* A constructor for a block of identifiers
Used e.g. in multiple assignments, or in function headers */
EXP *makeEXP_idblock(char *identifier, EXP *next){
    EXP *e = malloc(sizeof(EXP));
    e->t = NULL;
    e->kind = idblockExp;
    e->val.idblock.next = next;
    e->val.idblock.identifier = identifier;
}
/*used for exps*/
EXP *makeEXP_expblock(EXP *e1, EXP *next){
    EXP *e = malloc(sizeof(EXP));
    e->t = NULL;
    e->kind = expblockExp; // was set to 0 (?!?)
    e->val.expblock.next = next;
    e->val.expblock.value = e1;
}
/*specific declaration node constructor for type declarations*/
DECLARATION *makeDECL_type(char* identifier, TYPE *typeNode){
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = typeDecl;
    d->t = typeNode;
    d->identifier = identifier;
    d->val.right = NULL;
    d->next = NULL;
    return d;
}

/*Called upon reaching the end of a block norhs_assignment, e.g. var a, b, c int
Takes the linked list of identifiers and makes each of them into a declaration statement,
propogating the TYPE to all statements*/
DECLARATION *makeDECL_blocknorhs(int lineno, EXP *ids, TYPE *t){
    DECLARATION *d = malloc(sizeof(DECLARATION));
    if(ids == NULL)
    {
        d = NULL;
        return d;   
    }
    if(ids->kind == idblockExp){
        // If there is no next id, create a dec for the last id 
        if(ids->val.idblock.next == NULL){
            d = makeDECL_norhs(varDecl, ids->val.idblock.identifier, t);
            return d;
        }
        //If there is a next id, recurse onto that id. 
        //Then create a dec for this level and a pointer this dec to the subtree 
        else{
            DECLARATION *nextD;
            nextD = makeDECL_blocknorhs(lineno, ids->val.idblock.next, t);
            d = makeDECL_norhs(varDecl, ids->val.idblock.identifier, t);
            d->next = nextD;
            return d;
        }
    }
}

/*Called for block declarations, but with a right hand side. Does basically the same thing
As makeDECL_blocknorhs(), but each dec actually assigns too.
Throws an error if there is an unequal number of ids and exps on either side.*/
DECLARATION *makeDECL_block(int lineno, EXP *ids, TYPE *t, EXP *exps){
    DECLARATION *d = malloc(sizeof(DECLARATION));
    if(ids->val.idblock.next == NULL && exps->val.expblock.next == NULL){
        d = makeDECL(varDecl, ids->val.idblock.identifier, t, exps->val.expblock.value);
        return d;
    }
    else if(ids->val.idblock.next != NULL && exps->val.expblock.next != NULL){
        DECLARATION *nextD = malloc(sizeof(DECLARATION));
        nextD = makeDECL_block(lineno, ids->val.idblock.next, t, exps->val.expblock.next);
        d = makeDECL(varDecl, ids->val.idblock.identifier,t, exps->val.expblock.value);
        d->next = nextD;
        return d;
    }
    else{
        printUnequalAssignError(lineno);
    }
}

DECLARATION *makeDECL_blocknotype(int lineno, EXP *ids, EXP *exps){
    DECLARATION *d = malloc(sizeof(DECLARATION));
    if(ids->val.expblock.next == NULL && exps->val.expblock.next == NULL){
        d = makeDECL_notype(varDecl, ids->val.idblock.identifier, 0, 0, exps->val.expblock.value);
        return d;
    }
    else if(ids->val.expblock.next != NULL && exps->val.expblock.next != NULL){
        DECLARATION *nextD = malloc(sizeof(DECLARATION));
        nextD = makeDECL_blocknotype(lineno, ids->val.expblock.next, exps->val.expblock.next);
        d = makeDECL_notype(varDecl, ids->val.idblock.identifier, 0, 0, exps->val.expblock.value);
        d->next = nextD;
        return d;
    }
    else{
        printUnequalAssignError(lineno);
    }
}
/*A similar function to block declarations, but for STMTs*/
STATEMENT *makeSTMT_blockassign(int lineno, EXP *ids, EXP *exps){
    STATEMENT *s = malloc(sizeof(STATEMENT));
    if(ids->val.expblock.next == NULL && exps->val.expblock.next == NULL){
        s = makeSTMT_assmt(lineno, ids->val.expblock.value, exps->val.expblock.value);
        return s;
    }
    else if(ids->val.expblock.next != NULL && exps->val.expblock.next != NULL){
        STATEMENT *nextS = malloc(sizeof(STATEMENT));
        nextS = makeSTMT_blockassign(lineno, ids->val.expblock.next, exps->val.expblock.next);
        s = makeSTMT_assmt(lineno, ids->val.expblock.value, exps->val.expblock.value);
        s->val.assignment.chain = nextS;
        return s;
    }
    else{
        printUnequalAssignError(lineno);
    }
}
/*Used for quick := declarations*/
STATEMENT *makeSTMT_blockqassign(int lineno, EXP *ids, EXP *exps){
    STATEMENT *s = malloc(sizeof(STATEMENT));
    if(ids->val.expblock.next == NULL && exps->val.expblock.next == NULL){
        s = makeSTMT_qdecl(lineno, ids->val.expblock.value, exps->val.expblock.value);
        return s;
    }
    else if(ids->val.expblock.next != NULL && exps->val.expblock.next != NULL){
        STATEMENT *nextS = malloc(sizeof(STATEMENT));
        nextS = makeSTMT_blockassign(lineno, ids->val.expblock.next, exps->val.expblock.next);
        s = makeSTMT_qdecl(lineno, ids->val.expblock.value, exps->val.expblock.value);
        s->val.assignment.chain = nextS;
        return s;
    }
    else{
        printUnequalAssignError(lineno);
    }
}

/*only called for function calls*/
DECLARATION *makeDECL_fnCallArgs(EXP *args)
{
    DECLARATION *d = malloc(sizeof(DECLARATION));
    d->d = funcCall;
    d->val.fnCallBlock = args;
    return d;
}
/*Simple functions to get to the bottom of subtrees.
Made to address a problem with block declarations.
e.g. For block TYPE norhsdeclarations, a linked list of exps (idents) and a TYPE 
Must be converted to a linked list of decs, each with one ident and one type.
The problem is the bottom of this new subtree must be pointed to the next dec.
This requires going to the bottom in the parser at another point*/
STATEMENT *findBottomSTMT(STATEMENT *s){
    if(s->next == NULL){
        return s;
    }
    else{
        return findBottomSTMT(s->next);
    }
}

DECLARATION *findBottomDECL(DECLARATION *d){
    if(d->next == NULL){
        return d;
    }
    else{
        return findBottomDECL(d->next);
    }
}


