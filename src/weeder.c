#include "weeder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
The weeder traverses the AST from root to leaves to check for 
invalid constructs that the parser cannot detect easily. The 
checks are done by keeping track of temporary variables that 
are recursively passed down the nodes.

After annotating the tree header file by looking over the parser,
I want to emphasize on the left-recursive nature of lists in 
the grammar.

The weeder checks for:
	- Division by 0
	- Terminating statements (return and infinite for loop)
	- Too many default cases
	- Break and continue validity
	- If, switch, for: short declaration
	- Blank identifier usage in expressions
*/

/* Function that serves as an interface with the main file */
void weedRoot(PROGRAM *root){
	weedProgram(root);
}

void weedProgram(PROGRAM *p){
	
	if (p == NULL) return;
	
	weedDeclaration(p->declList, 1);
}

void weedDeclaration(DECLARATION *d, int lineno){
	if (d == NULL) return;
	
	// head recursion
	
	// weed next
	weedDeclaration(d->next, lineno);
	// weed chain
	weedDeclaration(d->chain, lineno);
	
	switch (d->d){
		
		// type declaration
		case typeDecl:
			if(d->t->name != NULL && strcmp(d->t->name, "struct") == 0)
			{
				weedDeclaration(d->t->val.args, lineno);
			}
			else if(d->t->val.arg != NULL)
			{
				TYPE *tmp = d->t->val.arg;
				while(true)
				{
					if(tmp->name != NULL && strcmp(tmp->name, "struct") == 0)
					{
						weedDeclaration(tmp->val.args, lineno);
						break;
					}
					else{
						if(tmp->val.arg != NULL)
						{
							tmp = tmp->val.arg;
						}
						else{
							break;
						}
					}
				}
			}
			return;
		
		// variable declaration
		case varDecl:
			weedExpression(d->val.right, lineno, false, false, true);
			return;
		
		// struct declaration
		case structDecl:
			weedDeclaration(d->val.body, lineno);
			return;
			
		// function declaration
		case funcDecl:
			weedFunction(d->val.f);
			return;
		
		// function call
		case funcCall:
			weedExpression(d->val.fnCallBlock, lineno, false, false, true);
			return;		
	}
}

void weedFunction(FUNCTION *f){
	
	if (f == NULL) return;
	
	weedDeclaration(f->params, f->lineno);
	
	// function declaration
	// no return statement, and not void
	if ( !weedStatement(f->body, false, false).foundTerminating
		&& f->returnt->gType != nilType){
		fprintf(stderr, 
		"Error: (line %d) function %s expecting a terminating statement\n",
		f->lineno, f->identifier);
		exit(1);
	}
	
	
}

/* Things to weed for:
More than one default cases in switch
Break and continue
If, switch, for short declaration
Return statement
*/
// Return value is true if the statement and its decendents are terminating
Traversal weedStatement(STATEMENT *s, bool allowBreak, bool allowContinue)
{
	
	// return values
	const Traversal foundNothing = {false, false, false};
	
	// check for null, reached the end of a list of statements
	// this is actually the beginning of the list
	if (s == NULL) return foundNothing;
	
	const Traversal foundTerminating = {true, false, false};
	const Traversal foundDefault = {false, true, false};
	const Traversal foundTerminatingDefault = {true, true, false};
	const Traversal foundBreak = {false, false, true};
	const Traversal foundTerminatingBreak = {true, false, true};
	
	// head recursion
	Traversal foundValues = weedStatement(s->next, allowBreak, 
		allowContinue);
	
	// temporary values
	Traversal returnInBody;
	Traversal returnInElsePart;
	Traversal temp;
	
	
	switch (s->kind) {
		
		case emptyS:
			return foundNothing;
			
		// assignment
		case assignS:
      
			// return values ignored, and the field is not used in grammar
			weedStatement(s->val.assignment.chain, allowBreak, 
				allowContinue); 
		
		// quick declaration
		case quickDeclS:
			weedExpression(s->val.assignment.identifier, s->lineno, false, false, true);
			weedExpression(s->val.assignment.value, s->lineno, false, false, true);

			if(s->val.assignment.identifier->kind != idExp)
			{
				
				fprintf(stderr, 
                "Error: (line %d) expecting identifier expression in assignment\n", 
                s->lineno);
				exit(1);
			}
			
			if (foundValues.foundBreak) return foundBreak;
			
			return foundNothing;

			
		// statement block
		case blockS:
			return weedStatement(s->val.body, allowBreak, allowContinue);	       
			
		// if and else-if statement
		case ifS: 
		case elifS:
			weedExpression(s->val.conditional.condition, s->lineno, false, false, true);
			// return value ignored
			weedStatement(s->val.conditional.optDecl, false, false);
			
			returnInElsePart = 
				weedStatement(s->val.conditional.elif, allowBreak, allowContinue);
				
			returnInBody = 
				weedStatement(s->val.conditional.body, allowBreak, allowContinue);
			
			// no else part
			// do not place this above because there are other weeding procedures
			if (s->val.conditional.elif == NULL) {
				returnInBody.foundTerminating = false;
				return returnInBody;
			}
			
			temp.foundTerminating = returnInElsePart.foundTerminating && returnInBody.foundTerminating;
			temp.foundDefault = false;
			temp.foundBreak = returnInElsePart.foundBreak || returnInBody.foundBreak;
			return temp;
			
		// else statement
		case elseS:
			return weedStatement(s->val.conditional.body, allowBreak, allowContinue);
			
		// for statement
		case forS:
			weedExpression(s->val.conditional.condition, s->lineno, false, false, true);

			weedStatement(s->val.conditional.optDecl, false, false);
      
			returnInBody = 
				weedStatement(s->val.conditional.body, true, true);
			
			weedStatement(s->val.conditional.elif, false, false);
			
			// action field in a for loop, e.g. for ;;i++{}
			if(s->val.conditional.elif != NULL){
				if(s->val.conditional.elif->kind == exprS)
				{
					if (s->val.conditional.elif->val.expression->kind == funcExp)
					{
						//do nothing
					}
					else{
						fprintf(stderr, "Error: (line %d) expression statements in for-loop statements must be function calls\n", 
                    s->lineno);
						exit(1);
					}
				}
				else if(s->val.conditional.elif->kind == assignS)
				{
					//OK
				}
				else{
					fprintf(stderr, "Error: (line %d) for loop post calls must be assignments or function calls\n", 
                  s->lineno);
					exit(1);

				}
			}
			
			// found nothing, because the break statement is confined to the
			// for-loop context
			return foundNothing;

			
		// while statement
		case whileS:
			weedExpression(s->val.conditional.condition, s->lineno, false, false, true);
			
			// infinite loop without breaks
			if (s->val.conditional.condition == NULL
				&& !weedStatement(s->val.conditional.body, true, true)
				.foundBreak)
				
				return foundTerminating;
			
			// found nothing, because the break statement is confined to the
			// for-loop context
			return foundNothing;	
			
		// print statement
		case printS:
			weedExpression(s->val.iostmt.value, s->lineno, false, false, true);
			
			if (foundValues.foundBreak) return foundBreak;
			
			return foundNothing;
			
		// expression statement
		case exprS:
			weedExpression(s->val.expression, s->lineno, false, true, true);
      
			// the following may be redundant, but weedExpression does not check for funcBlockExp
			if(s->val.expression->kind == funcExp || s->val.expression->kind == funcBlockExp)
			{
				//OK
			}
			else{
				fprintf(stderr, "Error: (line %d) expression statements must be function calls.", s->lineno);
					exit(1);
			}
			
			if (foundValues.foundBreak) return foundBreak;
			
			return foundNothing;

			
		// return statement
		case returnS:
			
			weedExpression(s->val.expression, s->lineno, false, false, true);
			
			if (foundValues.foundBreak) return foundTerminatingBreak;
			
			return foundTerminating;
		
		// switch statement
		case switchS:
			weedStatement(s->val.switchBody.optDecl, false, false);
			weedExpression(s->val.switchBody.condition, s->lineno, false, false, true);
			temp = weedStatement(s->val.switchBody.cases, 
				allowBreak, allowContinue);
				
			// no break, a default case, each case is terminating
			if (temp.foundTerminating && temp.foundDefault && !temp.foundBreak) {
				return foundTerminating;
			}
			
			if (foundValues.foundBreak) return foundBreak;
			
			return foundNothing;
			
		// case statement
		case caseS:
			// default case
			if (s->val.caseBody.condition == NULL) {
				
				// second one seen so far
				if (foundValues.foundDefault) { 
					fprintf(stderr, 
					"Error: (line %d) multiple default cases in switch statement\n", 
					s->lineno);
					exit(1);
				}
				
				if (weedStatement(s->val.caseBody.body, 
					true, allowContinue).
					foundTerminating)
					return foundTerminatingDefault;
				
				return foundDefault;
					
				
			}
			weedExpression(s->val.caseBody.condition, 
				s->lineno, false, false, true);
			
			returnInBody = weedStatement(s->val.caseBody.body, true, allowContinue);
			
			// previous case is terminating
			if (foundValues.foundTerminating){
				
				// previous case is default
				if (foundValues.foundDefault) {
					
					// previous case has a break
					if (foundValues.foundBreak){
						temp.foundTerminating = true;
						temp.foundDefault = true;
						temp.foundBreak = true;
						return temp;
					}
					// no previous break
					else {
						temp.foundTerminating = true;
						temp.foundDefault = true;
						temp.foundBreak = returnInBody.foundBreak;
						return temp;
					}
				}
				// no default
				else {
					
					// break present in previous
					if (foundValues.foundBreak){
						return foundTerminatingBreak;
					}
					// no previous break
					else {
						return returnInBody;
					}
				}
				
			}
			// previous case is not terminating
			else {
				return foundNothing;
			}

		// break statement
		case breakS:
			if (!allowBreak) {
				fprintf(stderr, 
				"Error: (line %d) break used outside of a loop or switch statement\n", 
				s->lineno);
				exit(1);
			}
			return foundBreak;
			
		// continue statement
		case continueS:
			if (!allowContinue) {
				fprintf(stderr, 
				"Error: (line %d) continue used outside of a loop context\n",
				s->lineno);
				exit(1);
			}
			
			if (foundValues.foundBreak) return foundBreak;
			
			return foundNothing;
			
		// declaration statement
		case declS:
			weedDeclaration(s->val.declaration, s->lineno);
			
			if (foundValues.foundBreak) return foundBreak;
			
			return foundNothing;
			
		// throw error
		default:
			fprintf(stderr, "Error: (line %d) unknown statement\n", s->lineno);
			exit(1);
	}		
	
}

/* Things to weed for:
Division by 0
Blank identifier
If, switch, for short declaration
*/
void weedExpression(EXP *e, int lineno, bool divBy0, bool funcExpOnly, bool lookForBlankId){
	
	// null expression
	if (e == NULL) return;
	
	// look for function call only
	if (funcExpOnly && (e->kind != funcExp && e->kind != funcBlockExp)) notFuncExp(lineno);
	
	switch (e->kind){
	
	// empty and literals
	case emptyExp:
	case strExp:
	case boolExp:
	case runeExp:
	case rawstrExp:
		return;
	
	// potentially 0 literals
	case intExp:
		if (divBy0 && e->val.intLiteral == 0){
			fprintf(stderr, "Error: (line %d) division by 0\n", lineno);
			exit(1);
		}
	case floatExp:
		if (divBy0 && e->val.floatLiteral == 0){
			fprintf(stderr, "Error: (line %d) division by 0\n", lineno);
			exit(1);
		}
		return;
		
	// division by 0
	case divExp:
	case modExp:
		weedExpression(e->val.binary.lhs, lineno, false, false, true);
		weedExpression(e->val.binary.rhs, lineno, true, false, true);
		return;
		
	// other binary operators
	case plusExp:
	case minusExp:
	case timesExp:
	case eqExp:
	case neqExp:
	case geqExp:
	case leqExp:
	case gtExp:
	case ltExp:
	case orExp:
	case andExp:
	case andnotExp:
	case bitAndExp:
	case bitOrExp:
	case xorExp:
	case lshiftExp:
	case rshiftExp:
		weedExpression(e->val.binary.lhs, lineno, false, false, true);
		weedExpression(e->val.binary.rhs, lineno, false, false, true);
		return;
		
	// unary operators
	case notExp:
	case posExp:
	case negExp:
	case parExp: // parenthesized
	case uxorExp:
		weedExpression(e->val.binary.rhs, lineno, false, false, true);
		return;
		
	// identifier, look for blank
	case idExp:
		if (lookForBlankId && 0 == strcmp(e->val.identifier, "_")){
			fprintf(stderr, "Error: (line %d) blank identifier used in an expression\n", lineno);
			exit(1);
		}
		return;
	
	// identifier block
	case idblockExp:
		if (lookForBlankId && 0 == strcmp(e->val.identifier, "_")){
			fprintf(stderr, "Error: (line %d) blank identifier used in an expression\n", lineno);
			exit(1);
		}
		weedExpression(e->val.idblock.next, lineno, false, false, lookForBlankId);
		return;
		
	// expression block
	case expblockExp:
		weedExpression(e->val.expblock.value, lineno, false, false, lookForBlankId);
		weedExpression(e->val.expblock.next, lineno, false, false, lookForBlankId);	
		return;
	
	// array and slices
	case indexExp:
		weedExpression(e->val.binary.rhs, lineno, false, false, true);
		return;
	case elementExp:
		weedExpression(e->val.binary.lhs, lineno, false, false, false);
		weedExpression(e->val.binary.rhs, lineno, false, false, false);
		return;
	
	// struct member invoke
	case invocExp:
		weedExpression(e->val.binary.lhs, lineno, false, false, false);
		weedExpression(e->val.binary.rhs, lineno, false, false, false);
		return;
	
	// built-in
	case appendExp:
		weedExpression(e->val.binary.lhs, lineno, false, false, false);
		weedExpression(e->val.binary.rhs, lineno, false, false, false);
		return;
	case lenExp:
	case capExp:
		weedExpression(e->val.binary.rhs, lineno, false, false, false);
		return;
	
	case funcExp:
		weedDeclaration(e->val.fn->params, lineno);
		weedFnCall(e->val.fn, lineno);
		return;
	case funcBlockExp:
		weedDeclaration(e->val.fnblock.fn->params, lineno);
		weedExpression(e->val.fnblock.identifier, lineno, false, false, true);
		return;
	// throw errors
	default:
		fprintf(stderr, "Error: (line %d) unknown expression\n", lineno);
		exit(1);
	}
}

void weedFnCall(FUNCTION *fn, int lineno)
{
	EXP *tmp = fn->params->val.fnCallBlock;
	int count = 0;
	while(tmp != NULL && tmp->val.expblock.value != NULL)
	{
		tmp = tmp->val.expblock.next;
		count ++;
	}
	
}
/* Print an error because it was expecting a function call as an expression */
void notFuncExp(int lineno){
	fprintf(stderr, "Error: (line %d) optional declaration as an expression must be a function call\n", lineno);
	exit(1);
}
