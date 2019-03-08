#ifndef TREE_H
#define TREE_H
typedef struct EXP EXP;
typedef struct SYMBOL SYMBOL;
typedef struct STATEMENT STATEMENT;
typedef struct FUNCTION FUNCTION;
typedef struct DECLARATION DECLARATION;
//typedef struct SHORT_DECL SDecl;
typedef struct PROGRAM PROGRAM;
typedef struct TYPE TYPE;
#define HASHSIZE 317
typedef struct symTable symTable;

//prints n tabs
void prettyTabs(int n);
enum ExpressionKind{emptyExp, //NULL
                    intExp, 
                    floatExp, 
                    strExp, 
					rawstrExp,
                    boolExp, 
                    runeExp,
                    plusExp, //.+.
                    minusExp, //.-.
                    timesExp, //.*.
                    divExp, //./.
                    modExp, //.%.
                    notExp, //!.
                    posExp, //+.
                    negExp, //-.
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
                    expblockExp, //Blocks of expressions 
                    bitAndExp, //.&.
                    bitOrExp, //.|.
                    xorExp, //.^.
                    lshiftExp, //.<<.
                    rshiftExp, //.>>.
                    indexExp, //[.]  e.g. arrays
                    elementExp, //for arrays and slices, e.g. identifier[x]
                    invocExp, //x.y
                    appendExp, //built-in
                    lenExp, //built-in
                    capExp, //built-in
		            uxorExp, // unary ^
                    funcExp //function call
                    };
enum StatementKind{ emptyS, //NULL // UNUSED
                    assignS,
                    quickDeclS,
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
enum DeclarationType{typeDecl,
					varDecl,
					structDecl,
					funcDecl,
					funcCall
					};

enum SymbolKind{nullSym, varSym, funcSym, typeSym, structSym};
struct SYMBOL{
    char *name;
    enum SymbolKind kind;
    TYPE *t;
    int wasRedefined; //used to deal with int redeclerations, for instance
    union{
        SYMBOL *parentType;
        SYMBOL *returnType;
        SYMBOL *structFields;
        struct {SYMBOL *funcParams; SYMBOL *returnTypeRef;} func;
    } val;
    struct SYMBOL *next;
};
struct symTable {
    SYMBOL *varTable[HASHSIZE];
    SYMBOL *typeTable[HASHSIZE];
    SYMBOL *funcTable[HASHSIZE];
    symTable *next;
};


/*By convention, simple values have size 1.
Slices start with size 0.
Name is NULL unless the type is user-defined, in which case it reflects
the user-specificed name of the type.
If the type is just a rename of another type, the val is one arg with a pointer to that type.
If the type is a struct, the val is a linked list of Exps representing the members of the struct.*/
struct TYPE{
    int size;
    char *name;
    enum GroupingType gType;
    union {
        TYPE *arg;
        EXP *args;
    }val;
};
struct EXP{
    enum ExpressionKind kind;
    TYPE *t;
    union {
        char *identifier;
        char *strLiteral;
        char runeLiteral;
        int intLiteral;
        float floatLiteral;
        FUNCTION *fn;
        struct{EXP *lhs; EXP *rhs;} binary;
        struct{EXP *next; char *identifier;} idblock;
        struct{EXP *next; EXP *value;} expblock; 
    }val;
};

struct DECLARATION{//compound declarations should be broken down into individual declarations
    enum DeclarationType d;
    TYPE *t;
    int lineno;
    char *identifier;
    union {
        EXP *right;
        EXP *fnCallBlock;
        FUNCTION *f;
        DECLARATION *body;
    }val;
    DECLARATION *next;    DECLARATION *chain;
};
// struct SHORT_DECL{//Struct for function declaration parameters for space efficiency
//     EXP *identifier;
//     TYPE *t;
//     SDECLARATION *next;
// };
struct FUNCTION{//parameters are referred to as a list of declarations where the 'right' field will be nil
    int lineno;
    symTable *localScope;
    char *identifier;
    int paramCount;
    DECLARATION *params;
    TYPE *returnt;
    STATEMENT *body;
};
struct STATEMENT{
    enum StatementKind kind;
    int lineno;
    symTable *localScope;
    union{
        struct{EXP *identifier; EXP *value; STATEMENT *chain;} assignment;
        struct{EXP *condition; STATEMENT *optDecl; STATEMENT *body; STATEMENT *elif;} conditional;//IF(,ELIF,ELSE) and FOR
        struct{EXP *value; int hasNewLine;} iostmt;//PRINT
        DECLARATION *declaration;
        STATEMENT *body;
        EXP *expression;//used for return stmts and expr stmts
        struct{EXP *condition; STATEMENT *optDecl; STATEMENT *cases;} switchBody;
        struct{EXP *condition; STATEMENT *body;} caseBody;//null condition represents default
    } val;
    STATEMENT *next;
};
struct PROGRAM{
    char *package;
    symTable *globalScope;
    DECLARATION *declList;
};



EXP *makeEXP_empty();
EXP *makeEXP_int(int literal);
EXP *makeEXP_float(float literal);
EXP *makeEXP_str(char *literal);
EXP *makeEXP_rawstr(char *literal);
EXP *makeEXP_bool(int literal);
EXP *makeEXP_rune(char literal);
EXP *makeEXP_plus(EXP *e1, EXP *e2);
EXP *makeEXP_minus(EXP *e1, EXP *e2);
EXP *makeEXP_times(EXP *e1, EXP *e2);
EXP *makeEXP_div(EXP *e1, EXP *e2);
EXP *makeEXP_mod(EXP *e1, EXP *e2);
EXP *makeEXP_pos(EXP *e1);
EXP *makeEXP_neg(EXP *e1);
EXP *makeEXP_par(EXP *e1);
EXP *makeEXP_eq(EXP *e1, EXP *e2);
EXP *makeEXP_neq(EXP *e1, EXP *e2);
EXP *makeEXP_geq(EXP *e1, EXP *e2);
EXP *makeEXP_leq(EXP *e1, EXP *e2);
EXP *makeEXP_gt(EXP *e1, EXP *e2);
EXP *makeEXP_lt(EXP *e1, EXP *e2);
EXP *makeEXP_or(EXP *e1, EXP *e2);
EXP *makeEXP_and(EXP *e1, EXP *e2);
EXP *makeEXP_andnot(EXP *e1, EXP *e2);
EXP *makeEXP_id(char *identifier);
EXP *makeEXP_band(EXP *e1, EXP *e2);
EXP *makeEXP_bor(EXP *e1, EXP *e2);
EXP *makeEXP_xor(EXP *e1, EXP *e2);
EXP *makeEXP_lshift(EXP *e1, EXP *e2);
EXP *makeEXP_rshift(EXP *e1, EXP *e2);
EXP *makeEXP_index(EXP *e2);
EXP *makeEXP_element(EXP *e1, EXP *e2);
EXP *makeEXP_invoc(EXP *e1, EXP *e2);
EXP *makeEXP_append(EXP *e1, EXP *e2);
EXP *makeEXP_len(EXP *e1);
EXP *makeEXP_cap(EXP *e1);
EXP *makeEXP_uxor(EXP *e1);
EXP *makeEXP_func(char *identifier, int size, DECLARATION *args);
void makeEXP_func_access(EXP *identifier, int size, DECLARATION *args);
EXP *makeEXP_expblock(EXP *e, EXP *next);
EXP *makeEXP_idblock(char *identifier, EXP *next);
EXP *makeEXP_not(EXP *e1);

DECLARATION *makeDECL(int isVar, char *identifier, TYPE *t, EXP *rhs, int lineno);
DECLARATION *makeDECL_norhs(int isVar, char *identifier, TYPE *t, int lineno);
DECLARATION *makeDECL_notype(int isVar, char *identifier, int gtype, int arraysize,  EXP *rhs, int lineno);
DECLARATION *makeDECL_struct( char *identifier, DECLARATION *body, int lineno);
//SDECLARATION *makeSDecl(EXP *e, char* declType, int gtype, int arraysize);
DECLARATION *makeDECL_block(int lineno, EXP *ids, TYPE *t, EXP *exps);
DECLARATION *makeDECL_blocknorhs(int lineno, EXP *ids, TYPE *t);
DECLARATION *makeDECL_type(char *identifier, TYPE *typeNode, int lineno);
DECLARATION *makeDECL_blocknotype(int lineno, EXP *ids, EXP *exps);
DECLARATION *makeDECL_fn(DECLARATION *next, FUNCTION *f, int lineno);
DECLARATION *makeDECL_fnCallArgs(EXP *args);

FUNCTION *makeFCTN(int lineno, char *identifier, int size, DECLARATION *params, TYPE *returnType, STATEMENT *body);

STATEMENT *makeSTMT_assmt(int lineno, EXP *identifier, EXP *val);
STATEMENT *makeSTMT_if(int lineno, EXP *condition, STATEMENT *optDecl, STATEMENT *body, STATEMENT *elif);
STATEMENT *makeSTMT_elif(int lineno, EXP *condition, STATEMENT *optDecl, STATEMENT *body, STATEMENT *elif);
STATEMENT *makeSTMT_else(int lineno, STATEMENT *body);
STATEMENT *makeSTMT_while(int lineno, EXP *condition, STATEMENT *body);
STATEMENT *makeSTMT_for(int lineno, STATEMENT *optDecl, EXP *condition, STATEMENT *body, STATEMENT *action);
STATEMENT *makeSTMT_decl(int lineno, DECLARATION *declaration);
STATEMENT *makeSTMT_exp(int lineno, EXP *expression);
STATEMENT *makeSTMT_switch(int lineno, EXP *condition, STATEMENT *optDecl, STATEMENT *cases);
STATEMENT *makeSTMT_case(int lineno, EXP *condition, STATEMENT *body);
STATEMENT *makeSTMT_block(int lineno, STATEMENT *body);
STATEMENT *makeSTMT_print(int lineno, EXP *expression, int hasNewLine);
STATEMENT *makeSTMT_break(int lineno);
STATEMENT *makeSTMT_continue(int lineno);
STATEMENT *makeSTMT_return(int lineno, EXP *expression);
STATEMENT *makeSTMT_blockassign(int lineno, EXP *ids, EXP *exps);
STATEMENT *makeSTMT_qdecl(int lineno, EXP *identifier, EXP *val);
STATEMENT *makeSTMT_blockqassign(int lineno, EXP *ids, EXP *exps);

TYPE *makeTYPE(int gtype, int size, char *name, TYPE *ref);
//TYPE *makeTYPE_struct(int size, char *name, DECLARATION *args);



PROGRAM *makePROG(char* package, DECLARATION *declList);

DECLARATION *findBottomDECL(DECLARATION *d);
STATEMENT *findBottomSTMT(STATEMENT *s);













#endif
