#include "weeder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
The weeder traverses the AST from root to leaves to check for 
invalid constructs that the parser cannot detect easily. The 
checks are done by keeping track of temporary variables that 
are recursively passed down the nodes.

One challenge in this weeder design is the cryptic nature of the
AST nodes. There might be some missing traversals because the
AST is not a good abstraction of the parsed language.

The weeder checks for:
	- Division by 0
	- Terminating statements (return)
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
	
	// weed next
	weedDeclaration(d->next, lineno);
	// weed chain
	weedDeclaration(d->chain, lineno);
	
	switch (d->d){
		
		// type declaration
		case typeDecl:
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
	if ( !weedStatement(f->body, false, false, false, false, true)
		&& f->returnt->gType != nilType){
		fprintf(stderr, 
		"Error: (line %d) expecting a return statement in all control flows\n",
		f->lineno);
		exit(1);
	}
	
	
}

/* Things to weed for:
More than one default cases in switch
Break and continue
If, switch, for short declaration
Return statement
*/
// Return value is true if the statement and its decendents have a return statement
bool weedStatement(STATEMENT *s, bool allowBreak, bool allowContinue, 
bool lookForDefaultCase, bool encounteredReturn, bool needReturn){
	
	// check for null
	if (s == NULL) return false;
	
	// temporary values
	bool returnInBody;
	bool returnInElsePart;
	bool returnChain;
	bool returnNext;
	
	
	switch (s->kind) {
		
		// unused token
		case emptyS:
			return false;
			
		// assignment
		case assignS:
		// quick declaration
		case quickDeclS:
			weedExpression(s->val.assignment.identifier, s->lineno, false, false, false);
			weedExpression(s->val.assignment.value, s->lineno, false, false, true);
			
			returnChain = weedStatement(s->val.assignment.chain, allowBreak, 
				allowContinue, false, encounteredReturn, needReturn);
			returnNext = weedStatement(s->next, allowBreak, allowContinue,
				false, encounteredReturn, needReturn);
			return returnChain || returnNext;
			
		// statement block
		case blockS:
			returnNext = weedStatement(s->next, allowBreak, allowContinue,
				false, encounteredReturn, needReturn);
			returnInBody = weedStatement(s->val.body, allowBreak, allowContinue, false, encounteredReturn, needReturn);	       
			return returnNext || returnInBody;
			
		// if and else-if statement
		case ifS: 
		case elifS:
			weedExpression(s->val.conditional.condition, s->lineno, false, false, true);
			// return value ignored
			weedStatement(s->val.conditional.optDecl, false, false, false, true, false);
			
			returnInElsePart = 
				weedStatement(s->val.conditional.elif, allowBreak, allowContinue, 
				false, encounteredReturn, needReturn);
				
			returnInBody = 
				weedStatement(s->val.conditional.body, allowBreak, allowContinue, 
				false, encounteredReturn, needReturn);
			return returnInElsePart && returnInBody;
			
		// else statement
		case elseS:
			return weedStatement(s->val.conditional.body, allowBreak, allowContinue, 
				false, encounteredReturn, needReturn);
			
		// for statement
		case forS:
			weedExpression(s->val.conditional.condition, s->lineno, false, false, true);
			weedStatement(s->val.conditional.optDecl, false, false, false, true, false);
			returnInBody = 
				weedStatement(s->val.conditional.body, true, true, false,
				encounteredReturn, needReturn);
			return returnInBody;
			
		// while statement
		case whileS:
			weedExpression(s->val.conditional.condition, s->lineno, false, false, true);
			return weedStatement(s->val.conditional.body, true, true, false,
				encounteredReturn, needReturn);	
			
		// print statement
		case printS:
			weedExpression(s->val.iostmt.value, s->lineno, false, false, true);
			return false;
			
		// expression statement
		case exprS:
			weedExpression(s->val.expression, s->lineno, false, true, true);
			return false;
			
		// return statement
		case returnS:
			if (encounteredReturn) {
				fprintf(stderr, "Error: (line %d) too many return statements\n", s->lineno);
				exit(1);
			}
			weedExpression(s->val.expression, s->lineno, false, false, true);
			return true;
		
		// switch statement
		case switchS:
			weedStatement(s->val.switchBody.optDecl, false, false, false, true, false);
			weedExpression(s->val.switchBody.condition, s->lineno, false, false, true);
			return weedStatement(s->val.switchBody.cases, 
				allowBreak, allowContinue, false, false, needReturn);
			
		// case statement
		case caseS:
			// default case
			if (s->val.caseBody.condition == NULL) {
				if (lookForDefaultCase) { // second one seen so far
					fprintf(stderr, 
					"Error: (line %d) multiple default cases in switch statement\n", 
					s->lineno);
					exit(1);
				}
				returnInBody = weedStatement(s->val.caseBody.body, 
					true, allowContinue, false, false, needReturn);
				returnNext = weedStatement(s->next, true, allowContinue, true, 
					false, needReturn);
				return returnInBody && returnNext;
					
				
			}
			weedExpression(s->val.caseBody.condition, 
				s->lineno, false, false, true);

			returnNext = weedStatement(s->next, true, allowContinue, lookForDefaultCase, false, needReturn);

			returnInBody =  weedStatement(s->val.caseBody.body, true, allowContinue, false, false, needReturn);
			return returnNext && returnInBody;

		// break statement
		case breakS:
			if (!allowBreak) {
				fprintf(stderr, 
				"Error: (line %d) break used outside of a loop or switch statement\n", 
				s->lineno);
				exit(1);
			}
			return false;
			
		// continue statement
		case continueS:
			if (!allowContinue) {
				fprintf(stderr, 
				"Error: (line %d) continue used outside of a loop context\n",
				s->lineno);
				exit(1);
			}
			return false;
			
		// declaration statement
		case declS:
			weedDeclaration(s->val.declaration, s->lineno);
			return false;
			
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
	if (funcExpOnly && e->kind != funcExp) notFuncExp(lineno);
	
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
		return;
	
	// throw errors
	default:
		fprintf(stderr, "Error: (line %d) unknown expression\n", lineno);
		exit(1);
	}
}

/* Print an error because it was expecting a function call as an expression */
void notFuncExp(int lineno){
	fprintf(stderr, "Error: (line %d) optional declaration as an expression must be a function call\n", lineno);
	exit(1);
}
