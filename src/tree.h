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
                    bitAndExp, //.&.
                    bitOrExp, //.|.
                    xorExp, //.^.
                    lshiftExp, //.<<.
                    rshiftExp, //.>>.
                    rangeExp, //[.:.]
                    appendExp, //built-in
                    lenExp, //built-in
                    capExp, //built-in
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
enum SymbolType{    intType,
                    floatType,
                    runeType,
                    strType
                    };
enum GroupingType{  nilType; //just a type, e.g. int, string, T
                    arrayType,
                    sliceType,
                    ptrType
                    };
enum DeclarationType{longDecl, shortDecl};
/*fakeDecl are a construct that use the declaration
 data structure, but represent something else*/
struct TYPE{
    enum SymbolType;
    enum GroupingType;
};
struct EXP{
    enum ExpressionKind kind;
    type t;
    union {
        char *identifier;
        char *strLiteral;
        char runeLiteral;
        int intLiteral;
        float floatLiteral;
        Fctn *fn;
        struct{EXP *lhs; EXP *rhs;} binary;
    }val;
};

struct DECLARATION{//compound declarations should be broken down into individual declarations
    type t;
    char *identifier;
    char *declType;
    Exp *right;
    Decl *next;
};
struct SHORT_DECL{//Struct for function declaration parameters for space efficiency
    Exp *identifier;
    char *declType;
    SDecl *next;
}
struct FUNCTION{//parameters are referred to as a list of declarations where the 'right' field will be nil
    int lineno;
    symTable *localSym;
    char *identifier;
    int paramCount;
    SDecl *params;
    char *returnType;
    Stmt *body;
};
struct STATEMENT{
    enum StatementKind kind;
    int lineno;
    symTable *localSym;
    union{
        struct{char *identifier; Exp *value;} assignment;
        struct{Exp *condition; Decl *optDecl; Stmt *body; Stmt *elif;} conditional;//IF(,ELIF,ELSE) and FOR
        struct{Exp *value; int hasNewLine;} iostmt;//PRINT
        Decl *declaration;
        Stmt *body;
        Exp *expression;//used for return stmts and expr stmts
        struct{Exp *condition; Stmt *cases;} switchBody;
        struct{Exp *condition, Stmt *body; Stmt *next;} caseBody;//null condition represents default
    } val;
    STATEMENT *next;
};
struct PROGRAM{
    char *package;
    Decl *declList;
    Fctn *fnList;
};



Exp makeEXP_empty();
Exp makeEXP_int(int literal);
Exp makeEXP_float(float literal);
Exp makeEXP_str(char *literal);
Exp makeEXP_bool(int literal);
Exp makeExp_plus(Exp *e1, Exp *e2);
Exp makeExp_minus(Exp *e1, Exp *e2);
Exp makeExp_times(Exp *e1, Exp *e2);
Exp makeExp_div(Exp *e1, Exp *e2);
Exp makeExp_mod(Exp *e1, Exp *e2);
Exp makeExp_pos(Exp *e1);
Exp makeExp_neg(Exp *e1);
Exp makeExp_ptr(Exp *e1);
Exp makeExp_addr(Exp *e1);
Exp makeExp_par(Exp *e1);
Exp makeExp_eq(Exp *e1, Exp *e2);
Exp makeExp_neq(Exp *e1, Exp *e2);
Exp makeExp_geq(Exp *e1, Exp *e2);
Exp makeExp_leq(Exp *e1, Exp *e2);
Exp makeExp_gt(Exp *e1, Exp *e2);
Exp makeExp_lt(Exp *e1, Exp *e2);
Exp makeExp_or(Exp *e1, Exp *e2);
Exp makeExp_and(Exp *e1, Exp *e2);
Exp makeExp_andnot(Exp *e1, Exp *e2);
Exp makeExp_id(char *identifier);
Exp makeExp_band(Exp *e1, Exp *e2);
Exp makeExp_bor(Exp *e1, Exp *e2);
Exp makeExp_xor(Exp *e1, Exp *e2);
Exp makeExp_lshift(Exp *e1, Exp *e2);
Exp makeExp_rshift(Exp *e1, Exp *e2);
Exp makeExp_range(Exp *e1, Exp *e2);
Exp makeExp_appendExp(Exp *e1. Exp *e2);
Exp makeExp_len(Exp *e1);
Exp makeExp_cap(Exp *e1);
Exp makeExp_func(char *identifier, int size, SDecl *args);

Decl makeDECL(char *identifier, char *declType, Exp *rhs);
Decl makeDECL(char *identifier, char *declType);
Decl makeDECL(char *identifier,  Exp *rhs);
SDecl makeSDecl(Exp *e, char* declType)

Fctn makeFCTN(int lineno, char *identifier, int size, SDecl *params, char *returnType, Stmt *body);

Stmt makeSTMT_assmt(int lineno, char *identifier, Exp *val);
Stmt makeSTMT_if(int lineno, Exp *condition, Decl *optDecl, Stmt *body, Stmt *elif);
Stmt makeSTMT_elif(int lineno, Exp *condition, Stmt *body, Stmt *elif);
Stmt makeSTMT_else(int lineno, Exp *condition, Stmt *body);
Stmt makeSTMT_while(int lineno, Exp *condition, Stmt *body);
Stmt makeSTMT_for(int lineno, Decl *optDecl, Exp *condition, Stmt *body, Stmt *action);
Stmt makeSTMT_decl(int lineno, Decl *declaration);
Stmt makeSTMT_exp(int lineno, Exp *expression);
Stmt makeSTMT_switch(int lineno, Exp *condition, Stmt *cases);
Stmt makeSTMT_case(int lineno, Exp *condition, Stmt *body. Stmt *next);
Stmt makeSTMT_block(int lineno, Stmt *body);
Stmt makeSTMT_print(int lineno, Exp *expression, int hasNewLine);
Stmt makeSTMT_break(int lineno);
Stmt makeSTMT_continue(int lineno);


Prog makePROG(char* package, Decl *declList, Fctn *fnList);













#endif