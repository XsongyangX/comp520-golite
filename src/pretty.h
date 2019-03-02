#include "tree.h"

void prettyTabs(int n);
void prettyPROG(PROGRAM *my_prog);
void prettyFctn(FUNCTION *fn, int t);
void prettyFctnDecl(DECLARATION *sd, int printComma);
void prettyType(TYPE *t);
void prettyDecl(DECLARATION  *d, int t);
void prettySDecl(STATEMENT  *d);
void prettySDeclId(STATEMENT  *d);
void prettySDeclVal(STATEMENT *d);
void prettySDeclFn(EXP *s);
void prettyStmt(STATEMENT *s, int t);
void prettyAssign(STATEMENT *s, int t);
void prettyBlock(STATEMENT *s, int t);
void prettyIf(STATEMENT *s, int t);
void prettyElif(STATEMENT *s,int t);
void prettyElse(STATEMENT *s,int t);
void prettyFor(STATEMENT *s, int t);
void prettyWhile(STATEMENT *s, int t);
void prettyPrintS(STATEMENT *s, int t);
void prettyReturn(STATEMENT *s, int t);
void prettyBreak(int t);
void prettyContinue(int t);
void prettySwitch(STATEMENT *s, int t);
void prettyCase(STATEMENT *s, int t);
void prettyExp(EXP *e);
void prettyStmtFn(STATEMENT *s);