#include "tree.h"

void prettyTabs(int n);
void prettyPROG(Prog my_prog);
void prettyFctn(Fctn *fn, int t);
void prettyFctnDecl(SDecl *sd, int printComma);
void prettyType(type *t);
void prettyDecl(Decl *d, int t);
void prettySDecl(Decl *d);
void prettySDeclId(Decl *d);
void prettyStmt(Stmt *s, int t);
void prettyAssign(Stmt *s, int t);
void prettyBlock(Stmt *s, int t);
void prettyIf(Stmt *s, int t);
void prettyElif(Stmt *s,int t);
void prettyElse(Stmt *s,int t);
void prettyFor(Stmt *s, int t);
void prettyWhile(Stmt *s, int t);
void prettyPrint(Stmt *s, int t);
void prettyReturn(Stmt *s, int t);
void prettyBreak(int t);
void prettyContinue(int t);
void prettySwitch(Stmt *s, int t);
void prettyCase(Stmt *s, int t);
void prettyExp(Exp *e);
void prettyStmtFn(Stmt *s);