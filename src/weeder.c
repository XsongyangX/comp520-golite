#include "weeder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function that serves as an interface with the main file */
void weedRoot(PROGRAM *root){
	weedProgram(root);
}

void weedProgram(PROGRAM *program){
	
}

void weedDeclaration(DECLARATION *declaration, int lineno){

}

void weedFunction(FUNCTION *function){
	
}

/* Things to weed for:
More than one default cases in switch
Break and continue
If, switch, for short declaration
*/
void weedStatement(STATEMENT *s, bool allowBreak, bool allowContinue, 
bool lookForDefaultCase){
	
	// check for null
	if (s == NULL) return;
	
	switch (s->kind) {
		
		// unused token
		case emptyS:
			return;
			
		// assignment
		case assignS:
			weedExpression(s->val.assignment.identifier, false, false, false);
			weedExpression(s->val.assignment.value, false, false, true);
			return;
			
		// quick declaration
		case quickDeclS:
			weedExpression(s->val.assignment.identifier, false, false, false);
			weedExpression(s->val.assignment.value, false, false, true);
			return;
			
		// statement block
		case blockS:
			weedStatement(s->val.body, allowBreak, allowContinue, false);
			return;
			
		// if and else-if statement
		case ifS: 
		case elifS:
			weedExpression(s->val.conditional.condition, false, false, true);
			weedStatement(s->val.conditional.optDecl, false, false, false);
			weedStatement(s->val.conditional.elif, allowBreak, allowContinue, false);
		// else statement
		case elseS:
			weedStatement(s->val.conditional.body, allowBreak, allowContinue, false);
			return;
			
		// for statement
		case forS:
			weedExpression(s->val.conditional.condition, false, false, true);
			weedStatement(s->val.conditional.optDecl, false, false, false);
			weedStatement(s->val.conditional.body, true, true, false);
			return;
			
		// while statement
		case whileS:
			weedExpression(s->val.conditional.condition, false, false, true);
			weedStatement(s->val.conditional.body, true, true, false);
			return;
			
		// print statement
		case printS:
			weedExpression(s->val.iostmt.value, false, false, true);
			return;
			
		// expression statement
		case exprS:
			weedExpression(s->val.expression, false, true, true);
			return;
			
		// return statement
		case returnS:
			weedExpression(s->val.expression, false, false, true);
			return;
		
		// switch statement
		case switchS:
			weedStatement(s->val.switchBody.optDecl, false, false, false);
			weedExpression(s->val.switchBody.condition, false, false, true);
			weedStatement(s->val.switchBody.cases, allowBreak, allowContinue, true);
			return;
		
		// case statement
		case caseS:
			weedExpression(s->val.caseBody.condition, false, false, true);
			weedStatement(s->val.caseBody.body, true, false, false);
			return;
			
		// break statement
		case breakS:
			if (!allowBreak) {
				fprintf(stderr, 
				"Error: (line %d) break used outside of a loop or switch statement\n", 
				s->lineno);
				exit(1);
			}
			return;
			
		// continue statement
		case continueS:
			if (!allowContinue) {
				fprintf(stderr, 
				"Error: (line %d) continue used outside of a loop context\n",
				s->lineno);
				exit(1);
			}
			return;
			
		// declaration statement
		case declS:
			weedDeclaration(s->val.declaration, s->lineno);
			return;
			
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
void weedExpression(EXP *e, int lineno, bool lookForDivisionBy0,
bool lookForFuncExp, bool lookForBlankId){
	
	// null expression
	if (e == NULL) return;
	
	// look for function call only
	if (lookForFuncExp && e->kind != funcExp) notFuncExp(lineno);
	
	switch (e->kind){
	
	// empty and literals
	case emptyExp:
	case strExp:
	case boolExp:
	case runeExp:
		return;
	
	// potentially 0 literals
	case intExp:
		if (lookForDivisionBy0 && e->val.intLiteral == 0){
			fprintf(stderr, "Error: (line %d) division by 0\n", lineno);
			exit(1);
		}
	case floatExp:
		if (lookForDivisionBy0 && e->val.floatLiteral == 0){
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
		weedFunction(e->val.fn); // TODO
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