#ifndef TREE_H
#define TREE_H
typedef struct EXP Exp;
typedef struct SYMBOL Sym;
typedef struct STATEMENT Stmt;
typedef struct FUNCTION Fctn;
typedef struct DECLARATION Decl;
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
                    equalsExp, //.==.
                    nequalsExp, //.!=.
                    gequalsExp, //.>=.
                    lequalsExp, //.<=.
                    greaterExp, //.>.
                    lesserExp,//.<.
                    orExp,//.||.
                    andExp, //.&&.
                    andnotExp, //.&^.
                    idExp, 
                    bitAndExp, //.|.
                    bitOrExp, //.&.
                    powExp, //^
                    lshiftExp, //.<<.
                    rshiftExp, //.>>.
                    rangeExp, //[.:.]
                    appendExp, //built-in
                    lenExp, //built-in
                    capExp //built-in
                    };
enum StatementKind{ emptyS, //NULL
                    assignS,
                    blockS, //{...}
                    ifS,
                    elifS,
                    elseS,
                    forS,
                    printS,
                    exprS,
                    returnS,
                    switchS,
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
        struct{EXP *lhs; EXP *rhs;} binary;
    }val;
};

struct DECLARATION{//compound declarations should be broken down into individual declarations
    enum DeclarationType kind;
    type t;
    Exp *left;
    Exp *right;
    Decl *next;
};
struct FUNCTION{//parameters are referred to as a list of declarations where the 'right' field will be nil
    int lineno;
    symTable *localSym;
    char *identifier;
    int paramCount;
    Decl *params;
    type *returnType;
    Stmt *body;
};
struct STATEMENT{
    enum StatementKind kind;
    int lineno;
    symTable *localSym;
    union{
        struct{char *identifier; Exp *value;} assignment;
        struct{Exp *condition; Decl *optDecl; Stmt *body; Stmt *elif;} conditional;//IF(,ELIF,ELSE) and FOR
        struct{Exp *value;} iostmt;//PRINT
        Decl declaration;
        Exp expression;//used for return stmts and expr stmts
        struct{Exp *condition; Stmt *body; Stmt *next;} switchBody;//null condition represents default
    } val;
    STATEMENT *next;
};
struct PROGRAM{
    char *package;
    Decl *declList;
    Fctn *fnList;
};























#endif