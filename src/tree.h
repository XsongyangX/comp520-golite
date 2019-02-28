#ifndef TREE_H
#define TREE_H
typedef struct EXP Exp;
typedef struct SYMBOL Sym;
typedef struct STATEMENT Stmt;
typedef struct FUNCTION Fctn;
typedef struct DECLARATION Decl;
typedef struct SHORT_DECL SDecl;
typedef struct PROGRAM Prog;
typedef struct TYPE type;
#define HASHSIZE 317
typedef struct symTable symTable;

enum ExpressionKind{emptyExp, //NULL
                    intExp, 
                    floatExp, 
                    strExp, 
                    boolExp, 
                    plusExp, //.+.
                    minusExp, //.-.
                    timesExp, //.*.
                    divExp, //./.
                    modExp, //.%.
                    notExp, //!.
                    posExp, //+.
                    negExp, //-.
                    ptrExp, //*.
                    addrExp, //&.
                    parExp, //(.)
                    eqExp, //.==.
                    neqExp, //.!=.
                    geqExp, //.>=.
                    leqExp, //.<=.
                    gtExp, //.>.
                    ltExp,//.<.
                    orExp,//.||.
                    andExp, //.&&.
                    andnotExp, //.&^.
                    idExp,
                    idblockExp, //Blocks of identifiers
                    explblockExp, //Blocks of expressions 
                    bitAndExp, //.&.
                    bitOrExp, //.|.
                    xorExp, //.^.
                    lshiftExp, //.<<.
                    rshiftExp, //.>>.
                    rangeExp, //[.:.]
                    indexExp, //[.]  e.g. arrays
                    elementExp, //for arrays and slices
                    invocExp, //x.y
                    appendExp, //built-in
                    lenExp, //built-in
                    capExp, //built-in
		            uxorExp,
                    funcExp //function call
                    };
enum StatementKind{ emptyS, //NULL
                    assignS,
                    blockS, //{...}
                    ifS,
                    elifS,
                    elseS,
                    forS,
                    whileS,
                    printS,
                    exprS,
                    returnS,
                    switchS,
                    caseS,
                    breakS,
                    continueS,
                    declS
                    };

enum GroupingType{  nilType,
                    baseType,
                    arrayType,
                    sliceType,
                    structType,
                    userType,
                    };
enum DeclarationType{typeDecl, varDecl, structDecl, funcDecl};
/*fakeDecl are a construct that use the declaration
 data structure, but represent something else*/
/*By convention, simple values have size 1.
Slices start with size 0.
Name is NULL unless the type is user-defined, in which case it reflects
the user-specificed name of the type.*/
struct TYPE{
    int size;
    char *name;
    enum GroupingType gType;
    union {
        type *arg;
        Exp *args;
    }val;
};
struct EXP{
    enum ExpressionKind kind;
    type *t;
    union {
        char *identifier;
        char *strLiteral;
        char runeLiteral;
        int intLiteral;
        float floatLiteral;
        Fctn *fn;
        struct{Exp *lhs; Exp *rhs;} binary;
        struct{Exp *next; char *identifier;} idblock;
        struct{Exp *next; Exp *value;} expblock; 
    }val;
};

struct DECLARATION{//compound declarations should be broken down into individual declarations
    enum DeclarationType d;
    type *t;
    char *identifier;
    union {
        Exp *right;
        Fctn *f;
        Decl *body;
    }val;
    Decl *next;
};
struct SHORT_DECL{//Struct for function declaration parameters for space efficiency
    Exp *identifier;
    type *t;
    SDecl *next;
};
struct FUNCTION{//parameters are referred to as a list of declarations where the 'right' field will be nil
    int lineno;
    symTable *localSym;
    char *identifier;
    int paramCount;
    Decl *params;
    type *returnt;
    Stmt *body;
    Fctn *next;
};
struct STATEMENT{
    enum StatementKind kind;
    int lineno;
    symTable *localSym;
    union{
        struct{Exp *identifier; Exp *value;} assignment;
        struct{Exp *condition; Decl *optDecl; Stmt *body; Stmt *elif;} conditional;//IF(,ELIF,ELSE) and FOR
        struct{Exp *value; int hasNewLine;} iostmt;//PRINT
        Decl *declaration;
        Stmt *body;
        Exp *expression;//used for return stmts and expr stmts
        struct{Exp *condition; Decl *optDecl; Stmt *cases;} switchBody;
        struct{Exp *condition; Stmt *body;} caseBody;//null condition represents default
    } val;
    Stmt *next;
};
struct PROGRAM{
    char *package;
    Decl *declList;
    Fctn *fnList;
};



Exp *makeEXP_empty();
Exp *makeEXP_int(int literal);
Exp *makeEXP_float(float literal);
Exp *makeEXP_str(char *literal);
Exp *makeEXP_bool(int literal);
Exp *makeEXP_plus(Exp *e1, Exp *e2);
Exp *makeEXP_minus(Exp *e1, Exp *e2);
Exp *makeEXP_times(Exp *e1, Exp *e2);
Exp *makeEXP_div(Exp *e1, Exp *e2);
Exp *makeEXP_mod(Exp *e1, Exp *e2);
Exp *makeEXP_pos(Exp *e1);
Exp *makeEXP_neg(Exp *e1);
Exp *makeEXP_ptr(Exp *e1);
Exp *makeEXP_addr(Exp *e1);
Exp *makeEXP_par(Exp *e1);
Exp *makeEXP_eq(Exp *e1, Exp *e2);
Exp *makeEXP_neq(Exp *e1, Exp *e2);
Exp *makeEXP_geq(Exp *e1, Exp *e2);
Exp *makeEXP_leq(Exp *e1, Exp *e2);
Exp *makeEXP_gt(Exp *e1, Exp *e2);
Exp *makeEXP_lt(Exp *e1, Exp *e2);
Exp *makeEXP_or(Exp *e1, Exp *e2);
Exp *makeEXP_and(Exp *e1, Exp *e2);
Exp *makeEXP_andnot(Exp *e1, Exp *e2);
Exp *makeEXP_id(char *identifier);
Exp *makeEXP_band(Exp *e1, Exp *e2);
Exp *makeEXP_bor(Exp *e1, Exp *e2);
Exp *makeEXP_xor(Exp *e1, Exp *e2);
Exp *makeEXP_lshift(Exp *e1, Exp *e2);
Exp *makeEXP_rshift(Exp *e1, Exp *e2);
Exp *makeEXP_range(Exp *e1, Exp *e2);
Exp *makeEXP_index(Exp *e2);
Exp *makeEXP_element(Exp *e1, Exp *e2);
Exp *makeEXP_invoc(Exp *e1, Exp *e2);
Exp *makeEXP_append(Exp *e1, Exp *e2);
Exp *makeEXP_len(Exp *e1);
Exp *makeEXP_cap(Exp *e1);
Exp *makeEXP_uXOR(Exp *e1);
Exp *makeEXP_func(char *identifier, int size, Decl *args);
Exp *makeEXP_expblock(Exp *e, Exp *next);
Exp *makeEXP_idblock(char *identifier, Exp *next);

Decl *makeDECL(int isVar, char *identifier, char *declType, int gtype, int arraysize, Exp *rhs);
Decl *makeDECL_norhs(int isVar, char *identifier, char *declType, int gtype, int arraysize);
Decl *makeDECL_notype(int isVar, char *identifier, int gtype, int arraysize,  Exp *rhs);
Decl *makeDECL_struct( char *identifier, Decl *body);
SDecl *makeSDecl(Exp *e, char* declType, int gtype, int arraysize);
Decl *makeDECL_block(int lineno, Exp *ids, type *t, Exp *exps);
Decl *makeDECL_blocknorhs(int lineno, Exp *ids, type *t);
Decl *makeDECL_type(char *identifier, type *typeNode);

Fctn *makeFCTN(int lineno, char *identifier, int size, Decl *params, type *returnType, Stmt *body);

Stmt *makeSTMT_assmt(int lineno, Exp *identifier, Exp *val);
Stmt *makeSTMT_if(int lineno, Exp *condition, Decl *optDecl, Stmt *body, Stmt *elif);
Stmt *makeSTMT_elif(int lineno, Exp *condition, Decl *optDecl, Stmt *body, Stmt *elif);
Stmt *makeSTMT_else(int lineno, Stmt *body);
Stmt *makeSTMT_while(int lineno, Exp *condition, Stmt *body);
Stmt *makeSTMT_for(int lineno, Decl *optDecl, Exp *condition, Stmt *body, Stmt *action);
Stmt *makeSTMT_decl(int lineno, Decl *declaration);
Stmt *makeSTMT_exp(int lineno, Exp *expression);
Stmt *makeSTMT_switch(int lineno, Exp *condition, Decl *optDecl, Stmt *cases);
Stmt *makeSTMT_case(int lineno, Exp *condition, Stmt *body);
Stmt *makeSTMT_block(int lineno, Stmt *body);
Stmt *makeSTMT_print(int lineno, Exp *expression, int hasNewLine);
Stmt *makeSTMT_break(int lineno);
Stmt *makeSTMT_continue(int lineno);
Stmt *makeSTMT_return(int lineno, Exp *expression);
Stmt *makeSTMT_blockassign(int lineno, Exp *ids, Exp *exps);

type *makeTYPE(int gtype, int size, char *name, type *arg);
//type *makeTYPE_struct(int size, char *name, Decl *args);



Prog *makePROG(char* package, Decl *declList);

Decl *findBottomDECL(Decl *d);
Stmt *findBottomSTMT(Stmt *s);













#endif
