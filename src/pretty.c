#include <stdio.h>
#include <string.h>
#include "pretty.h"
#include <stdlib.h>

//prints n tabs
void prettyTabs(int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("\t");
    }
}
//prints a program
void prettyPROG(PROGRAM *my_prog)
{
    if(strlen(my_prog->package) != 0)
    {
        printf("package %s;\n", my_prog->package);
    }
    prettyDecl(my_prog->declList, 0);
}
//prints a function list
void prettyFctn(FUNCTION *fn, int t)
{
    if(fn == NULL)
	return;
    prettyTabs(t);
    printf("func %s(", fn->identifier);
    if(fn->params != NULL)
    	prettyFctnDecl(fn->params, 0);
    printf(") ");
    if(fn->returnt != NULL)
    	prettyType(fn->returnt);
    printf("{\n");
    if(fn->body != NULL)
    	prettyStmt(fn->body, t+1);
    prettyTabs(t);
    printf("}\n");
}

//prints a list of function arguments
void prettyFctnDecl(DECLARATION *sd, int printComma)
{
    if(sd->next != NULL)
    {
        prettyFctnDecl(sd->next,1);
    }
    printf("%s ", sd->identifier);
    prettyType(sd->t);
    
    if(printComma)
        printf(", ");
}

//prints a type
void prettyType(TYPE *t)
{
    if(t == NULL)
	return;
    switch(t->gType){
        case nilType:
            break;
        case arrayType:
            printf("[%d]%s", t->size, t->name);
            break;
        case sliceType:
            printf("[]%s", t->name);
            break;
        case structType:
        case userType:
        default:
            printf("%s", t->name);
    }
}

//prints a list of Declarations
void prettyDecl(DECLARATION *d, int t)
{
    if(d->next != NULL)
    {
        prettyDecl(d->next, t);
    }
    switch(d->d){//apologies for this:switch on enum DeclarationType
        case typeDecl:
	    if(d->chain != NULL)
		prettyDecl(d->chain, t);
            printf("type %s ", d->identifier);
            prettyType(d->t);
            printf("\n");
            break;
        case varDecl:
            printf("var %s ", d->identifier);
            if(strlen(d->t->name) != 0)
            {
                prettyType(d->t);
            }
            if(d->val.right != NULL && d->val.right->kind != emptyExp)
            {
                printf(" = ");
                prettyExp(d->val.right);
            }
	    printf("\n");
            break;
        case structDecl:
	    if(d->chain != NULL)
		prettyDecl(d->chain, t);
            printf("type %s struct {\n", d->identifier);
	    
            prettyDecl(d->val.body, t+1);
            prettyTabs(t);
            printf("}\n");
            break;
        case funcDecl:
            prettyFctn(d->val.f, t);
    }
}

//prints a short declaration e.g. in conditionals
void prettySDecl(STATEMENT *d)
{
    
    if(d->kind == quickDeclS)
    {
        if(d->val.assignment.chain != NULL){
            prettySDeclId(d->val.assignment.chain);
        }
        prettyExp(d->val.assignment.identifier);
        printf(" := ");
        if(d->val.assignment.chain != NULL){
            prettySDeclVal(d->val.assignment.chain);
        }
        prettyExp(d->val.assignment.value);
        printf("; ");
    }
    else if(d->kind == assignS)
    {
        if(d->val.assignment.chain != NULL){
            prettySDeclId(d->val.assignment.chain);
        }
        prettyExp(d->val.assignment.identifier);
        printf(" = ");
        if(d->val.assignment.chain != NULL){
            prettySDeclVal(d->val.assignment.chain);
        }
        prettyExp(d->val.assignment.value);
        printf("; ");
    }
    else if(d->kind == exprS){
        prettyExp(d->val.expression);
        printf("; ");
    }
    else{
        prettyStmt(d,0);
    }

}
void prettySDeclId(STATEMENT *d)
{
    if(d->val.assignment.chain != NULL){
        prettySDeclId(d->val.assignment.chain);
    }
    
    prettyExp(d->val.assignment.identifier);
    printf(", ");
}
void prettySDeclVal(STATEMENT *d)
{
    if(d->val.assignment.chain != NULL){
        prettySDeclId(d->val.assignment.chain);
    }
    prettyExp(d->val.assignment.value);
    printf(", ");
}

//prints a list of statements
void prettyStmt(STATEMENT *s, int t)
{
    if(s == NULL)
	return;
    if(s->next != NULL)
    {
        prettyStmt(s->next, t);
    }
    switch(s->kind)
    {
        case emptyS:
            break;
        case assignS:
            prettyAssign(s,t);
            break;
        case quickDeclS:
            prettyAssign(s,t);
	    break;
        case blockS:
            prettyBlock(s,t);
            break;
        case ifS:
            prettyIf(s,t);
            break;
        case elifS:
            prettyElif(s,t);
            break;
        case elseS:
            prettyElse(s,t);
            break;
        case forS:
            prettyFor(s,t);
            break;
        case whileS:
            prettyWhile(s,t);
            break;
        case printS:
            prettyPrintS(s,t);
            break;
        case exprS:
            prettyTabs(t);
            prettyExp(s->val.expression);
	    printf("\n");
            break;
        case returnS:
            prettyReturn(s,t);
            break;
        case switchS:
            prettySwitch(s,t);
            break;
        case breakS:
            prettyBreak(t);
            break;
        case continueS:
            prettyContinue(t);
            break;
        case declS:
            prettyDecl(s->val.declaration,t);
	    printf("\n");
            break;
        case caseS://this should never be executed
            prettyCase(s,t);
            break;

    }
}

void prettyAssignHelper(STATEMENT *s)
{
    if(s->val.assignment.chain != NULL)
    {
        prettyAssignHelper(s->val.assignment.chain);
    }
    prettyExp(s->val.assignment.identifier);
}
void prettyAssignHelper2(STATEMENT *s)
{
    if(s->val.assignment.chain != NULL)
    {
        prettyAssignHelper(s->val.assignment.chain);
    }
    prettyExp(s->val.assignment.value);
}

//prints an assign statement
void prettyAssign(STATEMENT *s, int t)
{
    if(s->val.assignment.chain != NULL)
    {
        prettyAssignHelper(s->val.assignment.chain);
    }
    else{
        prettyTabs(t);
        prettyExp(s->val.assignment.identifier);
        if(s->kind == assignS)
        {    printf(" = ");}
        else
        {
            printf(" := ");
        }
        
    }
    if(s->val.assignment.chain != NULL)
    {
        prettyAssignHelper2(s->val.assignment.chain);
    }

    prettyExp(s->val.assignment.value);
    printf("\n");
}


//prints a block statement
void prettyBlock(STATEMENT *s, int t)
{
    prettyTabs(t);
    printf("{\n");
    prettyStmt(s->val.body, t+1);
    prettyTabs(t);
    printf("}\n");
}

//prints an if statement
void prettyIf(STATEMENT *s, int t)
{
    prettyTabs(t);
    printf("if ");
    if(s->val.conditional.optDecl != NULL)
    {
        prettySDecl(s->val.conditional.optDecl);
        printf("; ");
    }
    prettyExp(s->val.conditional.condition);
    printf("{\n");
    prettyStmt(s->val.conditional.body, t+1);
    prettyTabs(t);
    printf("}");
    if(s->val.conditional.elif != NULL)
        prettyStmt(s->val.conditional.elif, t);
    else{ printf("\n");}
}
void prettyElif(STATEMENT *s,int t)
{
    prettyTabs(t);
    printf("else if ");
    if(s->val.conditional.optDecl != NULL)//TODO: can we even declare things here???
    {
        prettySDecl(s->val.conditional.optDecl);
        printf("; ");
    }
    prettyExp(s->val.conditional.condition);
    printf("{\n");
    prettyStmt(s->val.conditional.body, t+1);
    prettyTabs(t);
    printf("}");
    if(s->val.conditional.elif != NULL)
        prettyStmt(s->val.conditional.elif, t);    
    else{ printf("\n");}

}
void prettyElse(STATEMENT *s,int t)
{
    prettyTabs(t);
    printf("else");
    printf("{\n");
    prettyStmt(s->val.conditional.body, t+1);
    prettyTabs(t);
    printf("}\n");
}

//prints a std for loop
void prettyFor(STATEMENT *s, int t)
{
    prettyTabs(t);
    printf("for ");
    if(s->val.conditional.optDecl != NULL){
        prettySDecl(s->val.conditional.optDecl);//print loop var
    }
    else{ printf("; ");}//print missing semi colon
    if(s->val.conditional.condition != NULL)
    {prettyExp(s->val.conditional.condition);}//print loop condition
    printf("; ");
    if(s->val.conditional.elif != NULL)
    {prettyStmt(s->val.conditional.elif, t);}//elif should be an expression stmt, typically an increment
    printf("{\n");
    prettyStmt(s->val.conditional.body, t+1);
    prettyTabs(t);
    printf("}\n");
}

//prints a "while" loop
void prettyWhile(STATEMENT *s, int t)
{
    prettyTabs(t);
    printf("for ");
    if(s->val.conditional.optDecl != NULL){
        prettySDecl(s->val.conditional.optDecl);//print loop var
    }
    if(s->val.conditional.condition != NULL)
    {prettyExp(s->val.conditional.condition);}//print loop condition
    printf("{\n");
    prettyStmt(s->val.conditional.body, t+1);
    prettyTabs(t);
    printf("}\n");
}

void prettyPrintHelper(EXP *e)
{
    if(e->val.expblock.next != NULL)
    {
        prettyPrintHelper(e->val.expblock.next);
	printf(", ");
    }
    prettyExp(e->val.expblock.value);
    
}
//prints a print stmt
void prettyPrintS(STATEMENT *s, int t)
{
    prettyTabs(t);
    if(s->val.iostmt.hasNewLine)
    {
        printf("println(");
    }
    else{
        printf("print(");
    }
    
    prettyPrintHelper(s->val.iostmt.value);
    printf(")\n");
}


//prints return stmts
void prettyReturn(STATEMENT *s, int t)
{
    prettyTabs(t);
    printf("return ");
    prettyExp(s->val.expression);
    printf("\n");
}
void prettyBreak(int t)
{
    prettyTabs(t);
    printf("break\n");
}
void prettyContinue(int t)
{
    prettyTabs(t);
    printf("continue\n");
}
//prints a switch statement
void prettySwitch(STATEMENT *s, int t)
{
    prettyTabs(t);
    printf("switch ");
    if (s->val.switchBody.optDecl != NULL)
    {
        prettySDecl(s->val.switchBody.optDecl);
    }
    if (s->val.switchBody.condition)
    {
        prettyExp(s->val.switchBody.condition);
    }
    printf(" {\n");
    prettyCase(s->val.switchBody.cases, t+1);
    prettyTabs(t);
    printf("}\n");
}
//prints a list of case statements
void prettyCase(STATEMENT *s, int t)
{
    if(s->next != NULL)
    {
        prettyCase(s->next, t);
    }
    prettyTabs(t);
    if(s->val.caseBody.condition == NULL )
    {
        printf("default:\n");
    }
    else{
        printf("case ");
        prettyExp(s->val.caseBody.condition);
        printf(":\n");
    }
    prettyStmt(s->val.caseBody.body, t+1);
}
void prettyRawStr(char *s)
{
	int len = strlen(s);
	char *tmp = malloc(len*sizeof(char));
	strcpy(tmp,s);
	tmp[len-1] = '\0';
	printf("`");
	printf("%s", (tmp+1));
	printf("`");
}

//prints expressions
void prettyExp(EXP *e)
{
    if(e == NULL)
	return;
    switch(e->kind){
        case emptyExp:
            break;
        case intExp:
            printf("%d", e->val.intLiteral);
            break;
        case floatExp:
            printf("%f", e->val.floatLiteral);
            break;
	case rawstrExp:
	    prettyRawStr(e->val.strLiteral);
	    break;
        case strExp:
            printf("%s", e->val.strLiteral);
            break;
        case boolExp:
            if(e->val.intLiteral){ printf("true");}
            else{printf("false");}
            break;
        case idExp:
            printf("%s", e->val.identifier);
            break;
        case plusExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" + ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case minusExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" - ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case timesExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" * ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case divExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" / ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case modExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" %% ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case notExp:
            printf("( ! ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case posExp:
            printf("( +");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case negExp:
            printf("( -");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case ptrExp:
            printf("( *");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case addrExp:
            printf("( &");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case parExp:
            printf("(");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case uxorExp:
            printf("( ^");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case eqExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" == ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case neqExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" != ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case geqExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" >= ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case leqExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" <= ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case gtExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" > ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case ltExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" < ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case orExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" || ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case andExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" && ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case andnotExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" &^ ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case bitAndExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" & ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case bitOrExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" | ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case xorExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" ^ ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case lshiftExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" << ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case rshiftExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(" >> ");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case rangeExp:
            printf("[");
            prettyExp(e->val.binary.lhs);
            printf(":");
            prettyExp(e->val.binary.rhs);
            printf("]");
            break;
        case indexExp:
            printf("[");
            prettyExp(e->val.binary.rhs);
            printf("]");
            break;
        case elementExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case invocExp:
            printf("(");
            prettyExp(e->val.binary.lhs);
            printf(".");
            prettyExp(e->val.binary.rhs);
            printf(")");
            break;
        case appendExp:
            printf("append(");
            prettyExp(e->val.binary.rhs);
            printf(")\n ");
            break;
        case lenExp:
            printf("(len(");
            prettyExp(e->val.binary.rhs);
            printf("))");
            break;
        case capExp:
            printf("(cap(");
            prettyExp(e->val.binary.rhs);
            printf("))");
            break;
        case funcExp:
            printf("%s(", e->val.fn->identifier);
	    if(e->val.fn->params->val.fnCallBlock != NULL)
            	prettySDeclFn(e->val.fn->params->val.fnCallBlock);
            printf(")");
            break;
	case runeExp:
	    printf("'%c'",e->val.runeLiteral);
    }
}
//helper for function case in prettyExp
void prettySDeclFn(EXP *s)
{
    if(s->val.expblock.next != NULL)
    {
        prettySDeclFn(s->val.expblock.next);
        printf(", ");//separate arguments
    }
    prettyExp(s->val.expblock.value);
}
