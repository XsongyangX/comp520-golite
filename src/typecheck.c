#include "typecheck.h"
//the goal of this call is to make sure variables are used properly within the program
bool typeProgram(PROGRAM *prog){
    DECLARATION *dec = prog->declList;
    while (dec != NULL){
        typeCheckDeclaration(dec, prog->globalScope);
        dec = dec->next;
    }
    return true;

}
void typeCheckDeclaration(DECLARATION *decl, symTable *table)
{//transmit the call to stmts in function bodies
    if(decl == NULL)
        return;
    typeCheckDeclaration(decl->chain, table);
    typeCheckDeclaration(decl->next, table);
    if(decl->d == funcDecl)
    {
        typeCheckStatement(decl->val.f->body, getSymbol(table, decl->val.f->identifier, funcSym));
    }
    else if(decl->d == varDecl)
    {
        if(decl->t->gType == nilType)
        {
            SYMBOL *declSym = getSymbol(table, decl->identifier, varSym);
            SYMBOL *declType = typecheckExp(decl->val.right, table, decl->lineno);
            declSym->t = declType->t;
            decl->t = declType->t;
            if(declType->kind == typeSym || declType->kind == structSym)
                declSym->val.parentSym = declType;
            else
                declSym->val.parentSym = declType->val.parentSym;
        }
        else if(decl->val.right != NULL)
        {
            SYMBOL *declSym = getSymbol(table, decl->identifier, varSym);
            SYMBOL *declVal = typecheckExp(decl->val.right, table, decl->lineno);
            if(!MatchingTypes(declSym, declVal, decl->lineno, false))
            {
                fprintf(stderr, "Error: (line %d) trying to assign type %s to %s variable.\n", decl->lineno, shortTypeStr(declVal), shortTypeStr(declSym));
                exit(1);
            }
        }
    }
}
/*  -assignments, make sure lhs and rhs match in type
    -quick declareations, make sure the lhs and rhs match, need to also infer the type of new vars
        and make sure they have the same type if they were declared in a higher scope   
    -conditionals, condition should have bool type, optDecl should be valid (see above 2) or a function call;
        for loops: make sure the post statement (stored in elif) is a valid post statement (either an assignment, quick decl, or fn call)
    -switch: the "condition" should have the same type as the case statement conditions
        remember that a null condition in a case stmt represents the default keyword
    -expr: should be a fn call
    -decl: just propagate the type checking call
    -print: not sure about this one
    */
void typeCheckStatement(STATEMENT *stmt, SYMBOL *func){
    if(stmt == NULL)
        return;
    typeCheckStatement(stmt->next, func);
    SYMBOL *symRHS, *symLHS;
    EXP *expList;
    switch(stmt->kind){
        case incrementS:
        case decrementS:
            symLHS = typecheckExp(stmt->val.expression, stmt->localScope, stmt->lineno);
            incdecHelper(symLHS, stmt->lineno);
            break;
        case assignS:
            typeCheckStatement(stmt->val.assignment.chain, func);
            symRHS = typecheckExp(stmt->val.assignment.value, stmt->localScope, stmt->lineno);
            symLHS = typecheckExp(stmt->val.assignment.identifier, stmt->localScope, stmt->lineno);
            if(symLHS->isConstant)
            {
                fprintf(stderr, "Error: (line %d) trying to assign to constant.\n", stmt->lineno);
                exit(1);
            }
            if(symLHS == BLANK_SYMBOL)
            {
                //OK
            }
            else if(MatchingTypes(symLHS, symRHS, stmt->lineno, false))
            {
                //OK
            }
            else{
                fprintf(stderr, "Error: (line %d) mismatch between assignment lhs (%s) and rhs (%s).\n", stmt->lineno, shortTypeStr(symLHS), shortTypeStr(symRHS));
                exit(1);
            }
            break;
        case quickDeclS:
            typeCheckStatement(stmt->val.assignment.chain, func);
            symRHS = typecheckExp(stmt->val.assignment.value, stmt->localScope, stmt->lineno);
            symLHS = typecheckExp(stmt->val.assignment.identifier, stmt->localScope, stmt->lineno);
            if(symLHS == BLANK_SYMBOL)
            {
                //OK
            }
            else if(symLHS->t != nilType && MatchingTypes(symLHS, symRHS, stmt->lineno, false))
            {
                //OK
            }
            else if(symLHS->t->gType == nilType)
            {
                symLHS->t = symRHS->t;
                // if(stmt->localScope->next != NULL && getSymbol(stmt->localScope->next, stmt->val.assignment.identifier->val.identifier, varSym) != NULL)//check for type consistency across scopes
                //     MatchingTypes(symLHS, getSymbol(stmt->localScope->next, stmt->val.assignment.identifier->val.identifier, varSym), stmt->lineno, true); //HANDLED IN SYMBOL.C
                if(symRHS->kind == typeSym || symRHS->kind == structSym)
                {
                    symLHS->val.parentSym = symRHS;
                }
                else if(symRHS->kind == varSym)
                {
                    if(symRHS->kind == varstructSym)
                    {
                        symLHS->val.structFields = symRHS->val.structFields;
                        symLHS->kind = varstructSym;
                    }
                    else{
                        symLHS->val.parentSym = symRHS->val.parentSym;
                    }
                }
            }
            else{
                fprintf(stderr, "Error: (line %d) mismatch between assignment lhs (%s) and rhs (%s).\n", stmt->lineno, shortTypeStr(symLHS), shortTypeStr(symRHS));
                exit(1);
            }
            break;
        case ifS:
        case elifS:
            typeCheckStatement(stmt->val.conditional.optDecl, func);
            symLHS = typecheckExp(stmt->val.conditional.condition, stmt->localScope, stmt->lineno);
            symRHS = resolveBaseType(symLHS);
            if(symRHS == NULL)
            {
                fprintf(stderr,"Error: (line %d) expecting boolean typed conditional in if/elif statement. Got %s.\n", stmt->lineno, shortTypeStr(symLHS));
                exit(1);
            }
            if(MatchingTypes(symRHS, BOOL_SYMBOL, stmt->lineno, false))
            {
                typeCheckStatement(stmt->val.conditional.body, func);
                typeCheckStatement(stmt->val.conditional.elif, func);
            }
            else{
                
                fprintf(stderr, "Error: (line %d) condition does not have bool type, has type %s.\n", stmt->lineno, shortTypeStr(symLHS));
                exit(1);
            }
            break;
        case elseS:
            typeCheckStatement(stmt->val.conditional.body, func);
            break;
        case forS:
            typeCheckStatement(stmt->val.conditional.optDecl, func);
            if(stmt->val.conditional.condition != NULL && stmt->val.conditional.condition->kind != emptyExp)
            {
                symLHS = typecheckExp(stmt->val.conditional.condition, stmt->localScope, stmt->lineno);
                
                symRHS = resolveBaseType(symLHS);
                if(symRHS == NULL)
                {
                    fprintf(stderr,"Error: (line %d) expecting boolean typed conditional in for statement. Got %s.\n", stmt->lineno, shortTypeStr(symLHS));
                    exit(1);
                }
                if(MatchingTypes(symRHS, BOOL_SYMBOL, stmt->lineno, false))
                {
                    typeCheckStatement(stmt->val.conditional.body, func);
                    typeCheckStatement(stmt->val.conditional.elif, func);
                }
                else{
                    
                    fprintf(stderr, "Error: (line %d) condition does not have bool type, has type %s.\n", stmt->lineno, shortTypeStr(symLHS));
                    exit(1);
                }
            }
            else{                
                typeCheckStatement(stmt->val.conditional.body, func);
                typeCheckStatement(stmt->val.conditional.elif, func);
            }
            break;
        case whileS:
            if(stmt->val.conditional.condition != NULL)
            {
                symLHS = typecheckExp(stmt->val.conditional.condition, stmt->localScope, stmt->lineno);
                
                symRHS = resolveBaseType(symLHS);
                if(symRHS == NULL)
                {
                    fprintf(stderr,"Error: (line %d) expecting boolean typed conditional in for statement. Got %s.\n", stmt->lineno, shortTypeStr(symLHS));
                    exit(1);
                }
                if(MatchingTypes(symRHS, BOOL_SYMBOL, stmt->lineno, false))
                {
                    typeCheckStatement(stmt->val.conditional.body, func);
                }
                else{
                    
                    fprintf(stderr, "Error: (line %d) condition does not have bool type, has type %s.\n", stmt->lineno, shortTypeStr(symLHS));
                    exit(1);
                }
            }
            else{                
                typeCheckStatement(stmt->val.conditional.body, func);
            }
            break;
        case switchS:
            switchHelper(stmt, func);
            break;
        case caseS:
            break;
        case printS:
            expList = stmt->val.iostmt.value;
            while(expList != NULL)
            {
                if(expList->val.expblock.value == NULL)
                {
                    expList = expList->val.expblock.next;
                    continue;
                }
                symLHS = typecheckExp(expList->val.expblock.value, stmt->localScope, stmt->lineno);
                
                symRHS = resolveBaseType(symLHS);
                if(symRHS == NULL)
                {
                    fprintf(stderr,"Error: (line %d) expecting base types in print statement. Got %s.\n", stmt->lineno, shortTypeStr(symLHS));
                    exit(1);
                }
                if(!(MatchingTypes(symRHS, INT_SYMBOL, 0, false) || 
                    MatchingTypes(symRHS, BOOL_SYMBOL, 0, false) || 
                    MatchingTypes(symRHS, FLOAT_SYMBOL, 0, false) || 
                    MatchingTypes(symRHS, RUNE_SYMBOL, 0, false) || 
                    MatchingTypes(symRHS, STR_SYMBOL, 0, false)))
                    {
                        fprintf(stderr,"Error: (line %d) expecting base types in print statement. Got %s.\n", stmt->lineno, shortTypeStr(symLHS));
                        exit(1);
                    }

                expList = expList->val.expblock.next;
            }
            break;
        case returnS:
            if(stmt->val.expression != NULL)
                symLHS = typecheckExp(stmt->val.expression, stmt->localScope, stmt->lineno);
            else if(func->t->gType == nilType)
            {
                //OK
                return;
            }
            if(func->t->gType != nilType && MatchingTypes(symLHS, func->val.func.returnSymRef, stmt->lineno, false))
            {
                //OK
            }
            else{                    
                    fprintf(stderr, "Error: (line %d) incompatible return type %s in function %s.\n", stmt->lineno, shortTypeStr(symLHS), func->name);
                    exit(1);
            }
            break;
        case blockS:
            typeCheckStatement(stmt->val.body, func);
            break;
        case continueS:
        case breakS:
            break;
        case exprS:
            typecheckExp(stmt->val.expression, stmt->localScope, stmt->lineno);
            break;
        case declS:
            typeCheckDeclaration(stmt->val.declaration, stmt->localScope);
            break;
    }
}
void incdecHelper(SYMBOL *sym, int lineno)
{
            SYMBOL *refParent = sym;
            
            refParent = resolveBaseType(sym);
            if(refParent == NULL)
            {
                fprintf(stderr, "Error: (line %d) type %s incremented/decremented. Expecting (int), (float64), (rune).\n", lineno, shortTypeStr(sym));
                exit(1);
            }
            if(!(MatchingTypes(refParent, INT_SYMBOL, 0, false) || 
                    MatchingTypes(refParent, FLOAT_SYMBOL, 0, false) || 
                    MatchingTypes(refParent, RUNE_SYMBOL, 0, false)))
                    {
                       fprintf(stderr, "Error: (line %d) type %s incremented/decremented. Expecting (int), (float64), (rune).\n", lineno, shortTypeStr(sym));
                        exit(1);
                    }
}
void switchHelper(STATEMENT *stmt, SYMBOL *func)
{
    typeCheckStatement(stmt->val.switchBody.optDecl, func);
    SYMBOL *symLHS = BOOL_SYMBOL;
    if(stmt->val.switchBody.condition != NULL)
        symLHS = typecheckExp(stmt->val.switchBody.condition, stmt->localScope, stmt->lineno);
    caseHelper(stmt->val.switchBody.cases, func, symLHS);
}
void caseHelper(STATEMENT *stmt, SYMBOL *func, SYMBOL *key)
{
    if(stmt == NULL)
        return;
    caseHelper(stmt->next, func, key);
    EXP *expList = stmt->val.caseBody.condition;
    while(expList != NULL)
    {
        if(expList->val.expblock.value == NULL)
        {
            expList = expList->val.expblock.next;
            continue;
        }
        SYMBOL *symExp = typecheckExp(expList->val.expblock.value, stmt->localScope, stmt->lineno);
        if(!MatchingTypes(key, symExp, stmt->lineno, false))
        {
            fprintf(stderr, "Error: (line %d) incompatible comparison between switch and case.\n", stmt->lineno);
            exit(1);
        }
        
        expList = expList->val.expblock.next;
    }
    typeCheckStatement(stmt->val.caseBody.body, func);
}
/*verify the type of expression, might need to make this return the type.
See shortTypeStr, longTypeStr, and lookup methods to do this.
Arguments passed to function calls must be verified.
Accessing struct elements should be verified as well.*/
SYMBOL *typecheckExp(EXP *exp, symTable *table, int lineno){
    SYMBOL *dummy1, *dummy2, *tmp;
    char* name;
    EXP *expList;
    switch (exp->kind){
        case intExp:
            exp->t = INT_SYMBOL->t;
            return INT_SYMBOL;
        case floatExp:
            exp->t = FLOAT_SYMBOL->t;
            return FLOAT_SYMBOL;
        case strExp:
        case rawstrExp:
            exp->t = STR_SYMBOL->t;
            return STR_SYMBOL;
        case boolExp:
            exp->t = BOOL_SYMBOL->t;
            return BOOL_SYMBOL;
        case runeExp:
            exp->t = RUNE_SYMBOL->t;
            return RUNE_SYMBOL;
        case plusExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);
            dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for + operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false)||
                MatchingTypes(dummy2, FLOAT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, STR_SYMBOL, lineno, false))
            {
                exp->t = dummy1->t;
                return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) Invalid types (%s), (%s) for + operator\n", lineno, shortTypeStr(dummy1), shortTypeStr(dummy2));
                exit(1);
            }
        case minusExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);
            dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for - operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false)||
                MatchingTypes(dummy2, FLOAT_SYMBOL, lineno, false))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) Invalid types (%s), (%s) for - operator\n", lineno, shortTypeStr(dummy1), shortTypeStr(dummy2));
                exit(1);
            }
        case timesExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);
            dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for * operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false)||
                MatchingTypes(dummy2, FLOAT_SYMBOL, lineno, false))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) Invalid types (%s), (%s) for * operator\n", lineno, shortTypeStr(dummy1), shortTypeStr(dummy2));
                exit(1);
            }
        case divExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for / operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false)||
                MatchingTypes(dummy2, FLOAT_SYMBOL, lineno, false))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) Invalid types (%s), (%s) for / operator\n", lineno, shortTypeStr(dummy1), shortTypeStr(dummy2));
                exit(1);
            }
        case modExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for %% operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) || 
            MatchingTypes(dummy2, RUNE_SYMBOL,  lineno, false) ||
            MatchingTypes(dummy2, FLOAT_SYMBOL,  lineno, false))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) Invalid types (%s), (%s) for %% operator\n", lineno, shortTypeStr(dummy1), shortTypeStr(dummy2));
                exit(1);
            }
        case eqExp:
        case neqExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);
            if (isComparable(dummy1, table, lineno))
            {
                exp->t = BOOL_SYMBOL->t;
                return BOOL_SYMBOL; 
            }
            
            else{
                fprintf(stderr,"Error: (line %d) invalid type %s for comparable.\n", lineno, dummy1->name);
                exit(1);
            }
        case geqExp:
        case leqExp:
        case gtExp:
        case ltExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for ordered operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false)||
                MatchingTypes(dummy2, FLOAT_SYMBOL, lineno, false) || 
                MatchingTypes(dummy2, STR_SYMBOL, lineno, false))
            {
                exp->t = BOOL_SYMBOL->t;
                return BOOL_SYMBOL; 
            }
            else{
                fprintf(stderr,"Error: (line %d) unexpected type %s in oredered expression. Expecting (int), (float64), (rune), (str).\n", lineno, dummy1->name);
                exit(1);
            }
        case orExp:
        case andExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for + operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if ( MatchingTypes(dummy2, BOOL_SYMBOL, lineno, false))
            {
                exp->t = dummy1->t;
                return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) unexpected type %s in boolean expression. Expecting (bool).\n", lineno, dummy1->name);
                exit(1);
            }
        case andnotExp:
        case bitAndExp:
        case bitOrExp:
        case xorExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);
            dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for bitwise operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) || MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false))
            {
                exp->t = dummy1->t;
                return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) unexpected type %s in comparison expression. Expecting (int), (rune).\n", lineno, dummy1->name);
                exit(1);
            }
        case lshiftExp:
        case rshiftExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno, true);dummy2 = resolveBaseType(dummy2);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for + operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) || MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false))
            {
                exp->t = dummy1->t;
                return dummy1; 
            }
            else{
                fprintf(stderr,"Error: (line %d) unexpected type %s in shift expression. Expecting (int), (rune).\n", lineno, dummy1->name);
                exit(1);
            }
        case elementExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            if(dummy1 == BLANK_SYMBOL)
            {
                fprintf(stderr, "Error: (line %d) Attempting to access blank arraytype.\n", lineno);
                exit(1);
            }
            dummy2 = elementHelper(dummy1, lineno);
            exp->t = dummy2->t;
            return dummy2;
            
        case invocExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            if(dummy1 == BLANK_SYMBOL)
            {
                
                fprintf(stderr, "Error: (line %d) Attempting to access blank struct.\n", lineno);
                exit(1);
            }
            if(dummy2 == BLANK_SYMBOL)
            {
                
                fprintf(stderr, "Error: (line %d) Attempting to access blank struct field.\n", lineno);
                exit(1);
            }
            dummy2 = structAccessHelper(dummy1, exp->val.binary.rhs->val.identifier, lineno);
            exp->t = dummy2->t;
            return dummy2;
            break;
        case notExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            if (MatchingTypes(dummy1, BOOL_SYMBOL, lineno, false))
            {
                    exp->t = dummy1->t;
                    return dummy1;
                
            }
            
            fprintf(stderr,"Error: (line %d) unexpected type %s in not expression. Expecting (bool).\n", lineno, dummy1->name);
            exit(1);
            
        case posExp:
        case negExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            dummy2 = resolveBaseType(dummy1);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for + operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, FLOAT_SYMBOL, lineno, false) ||
                MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false))
            {
                exp->t = dummy1->t;
                return dummy1;
            }
            
            fprintf(stderr,"Error: (line %d) unexpected type %s in unary expression. Expecting (int), (rune).\n", lineno, dummy1->name);
            exit(1);
        case uxorExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            dummy2 = resolveBaseType(dummy1);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for + operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if (MatchingTypes(dummy2, INT_SYMBOL, lineno, false) || MatchingTypes(dummy2, RUNE_SYMBOL, lineno, false))
            {
                exp->t = dummy1->t;
                return dummy1;
            }
            
            fprintf(stderr,"Error: (line %d) unexpected type %s in unary expression. Expecting (int), (rune).\n", lineno, dummy1->name);
            exit(1);
            
        case parExp:
            dummy1 =  typecheckExp(exp->val.binary.rhs, table, lineno);
            exp->t = dummy1->t;
            return dummy1;
        case indexExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            dummy2 = resolveBaseType(dummy1);
            if(dummy2 == NULL)
            {
                fprintf(stderr,"Error: (line %d) Invalid types (%s) for + operator\n", lineno, shortTypeStr(dummy1));
                exit(1);
            }
            if(MatchingTypes(dummy2, INT_SYMBOL, lineno, false) )
            {
                    exp->t = dummy1->t;
                    return dummy1;
                
            }
            fprintf(stderr, "Error: (line %d) Attempting to access an array or slice with a non-integer index\n", lineno);
            exit(1);
            
        case idExp:
            if(strcmp(exp->val.identifier, "_") == 0)
            {
                exp->t = NULL;
                return BLANK_SYMBOL;
            }
            dummy1 = getSymbol(table, exp->val.identifier, varSym);
            if(dummy1 != NULL)//case rhs of invoc
                exp->t = dummy1->t;
            return dummy1;

       

        case funcExp:
            if(strcmp(exp->val.fn->identifier, "_") == 0)
            {
                fprintf(stderr, "Error: (line %d) Attempting to access blank function.\n", lineno);
                exit(1);
            }
            if(strlen(lookupFunc(table, exp->val.fn->identifier, lineno, 0)) == 0)//function not found
            {
            
                if(strlen(lookupType(table,exp->val.fn->identifier, lineno, 0)) != 0) {
                    //type found: check for valid cast
                    dummy1 = getSymbol(table, exp->val.fn->identifier, typeSym);
                    investigateType(dummy1->t, lineno);
                    expList = exp->val.fn->params->val.fnCallBlock;
                    if(expList->val.expblock.next != NULL && expList->val.expblock.next->val.expblock.value != NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too many arguments to type cast %s.\n", lineno, exp->val.fn->identifier);
                        exit(1);
                    }
                    dummy2 = typecheckExp(expList->val.expblock.value,table, lineno);
                    SubTypes(dummy1, dummy2, lineno);
                    exp->t = dummy1->t;
                    return dummy1;
                }
                else{
                    fprintf(stderr, "Error: (line %d) undefined function %s.\n", lineno, exp->val.fn->identifier);
                    exit(1);
                }
            }
            else{//function found
                dummy1 = getSymbol(table, exp->val.fn->identifier, funcSym);
                dummy2 = dummy1->val.func.returnSymRef;
                dummy1 = dummy1->val.func.funcParams;
                if( exp->val.fn->params != NULL )
                    expList = exp->val.fn->params->val.fnCallBlock;
                while(dummy1 != NULL)
                {//type check each passed argument
                    if(expList == NULL || expList->val.expblock.value == NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too few arguments for function %s.\n", lineno, exp->val.fn->identifier);
                        exit(1);
                    }
                    tmp = typecheckExp(expList->val.expblock.value,table, lineno);
                    MatchingTypes(dummy1, tmp, lineno, true);
                    expList = expList->val.expblock.next;
                    dummy1 = dummy1->next;
                }
                if(expList != NULL && expList->val.expblock.value != NULL )
                {
                    fprintf(stderr, "Error: (line %d) too many arguments for function %s.\n", lineno, exp->val.fn->identifier);
                    exit(1);
                }
                exp->t = dummy2->t;
                return dummy2;
            }
            break;
        case funcBlockExp:
            expList = exp->val.fnblock.identifier;
            while(expList->kind == parExp)
            {
                expList = expList->val.binary.rhs;
            }
            name = expList->val.identifier;
            if(strcmp(name, "_") == 0)
            {
                fprintf(stderr, "Error: (line %d) Attempting to access blank function.\n", lineno);
                exit(1);
            }
            if(strlen(lookupFunc(table, name, lineno, 0)) == 0)//function not found
            {
            
                if(strlen(lookupType(table,name, lineno, 0)) != 0) {
                    //type found: check for valid cast
                    dummy1 = getSymbol(table, name, typeSym);
                    investigateType(dummy1->t, lineno);
                    expList = exp->val.fnblock.fn->params->val.fnCallBlock;
                    if(expList->val.expblock.next != NULL && expList->val.expblock.next->val.expblock.value != NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too many arguments to type cast %s.\n", lineno, name);
                        exit(1);
                    }
                    dummy2 = typecheckExp(expList->val.expblock.value,table, lineno);
                    checkDefaultCasts(dummy1, dummy2, lineno);
                    exp->t = dummy1->t;
                    return dummy1;
                }
                else{
                    fprintf(stderr, "Error: (line %d) undefined function %s.\n", lineno, name);
                    exit(1);
                }
            }
            else{//function found
                dummy1 = getSymbol(table, name, funcSym);
                dummy2 = dummy1->val.func.returnSymRef;
                dummy1 = dummy1->val.func.funcParams;
                if( exp->val.fnblock.fn->params != NULL )
                    expList = exp->val.fnblock.fn->params->val.fnCallBlock;
                while(dummy1 != NULL)
                {//type check each passed argument
                    if(expList == NULL || expList->val.expblock.value == NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too few arguments for function %s.\n", lineno, name);
                        exit(1);
                    }
                    tmp = typecheckExp(expList->val.expblock.value,table, lineno);
                    MatchingTypes(dummy1, tmp, lineno, true);
                    expList = expList->val.expblock.next;
                    dummy1 = dummy1->next;
                }
                if(expList != NULL && expList->val.expblock.value != NULL )
                {
                    fprintf(stderr, "Error: (line %d) too many arguments for function %s.\n", lineno, name);
                    exit(1);
                }
                exp->t = dummy2->t;
                return dummy2;
            }
            break;
             /*

        --------------------------------
        UNFINISHED
        --------------------------------

        case expblockExp://this case occurs from function calls
            symExp(exp->val.expblock.next, table, lineno);
            symExp(exp->val.expblock.value, table, lineno);
            break;
        


        */
        case appendExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            sliceTypes(dummy1, dummy2, lineno);
            exp->t = dummy1->t;
            return dummy1;
        case lenExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            lenHelper(dummy1, lineno);
            exp->t = INT_SYMBOL->t;
            return INT_SYMBOL;
        case capExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            capHelper(dummy1, lineno);
            exp->t = INT_SYMBOL->t;
            return INT_SYMBOL;
            
        default:
            fprintf(stderr, "Unrecognized expression kind %d", exp->kind);
            exit(1); 
    }
}
//determines if 2 types are equivalent
bool MatchingTypes(SYMBOL *s1, SYMBOL *s2, int lineno, bool isVerbose){
    char *type1 = shortTypeStr(s1);
    char *type2 = shortTypeStr(s2);
    if(s1->kind == varSym && s2->kind == varSym)
    {
        if(strcmp(type1, type2) == 0 && strcmp(getName(s1->t), "struct") != 0)//same type declaration, but not "in-line" struct, deals with array/slice cases
        {
            if(s1->val.parentSym == s2->val.parentSym)//types match in their definition "hierarchy"
            {
                return true;
            }
        }
        if(isVerbose)
        {fprintf(stderr, "Error: (line %d) type %s does not match with type %s.\n", lineno, type1, type2);
        exit(1);}
    }
    //a type symbol would occur in the case of a cast/function call
    else if(s1->kind == varSym && (s2->kind == typeSym || s2->kind == structSym))
    {
        if(strcmp(type1, s2->name) == 0)//same type declaration, deals with array/slice cases
        {
            if(s1->val.parentSym == s2)//types match in their definition "hierarchy"
            {
                return true;
            }
        }
        if(isVerbose)
        {fprintf(stderr, "Error: (line %d) type %s does not match with type %s.\n", lineno, type1, s2->name);
        exit(1);}
    }
    else if((s1->kind == typeSym || s1->kind == structSym) && s2->kind == varSym)
    {
        if(strcmp(type2, s1->name) == 0)//same type declaration, deals with array/slice cases
        {
            if(s2->val.parentSym == s1)//types match in their definition "hierarchy"
            {
                return true;
            }
        }
        if(isVerbose)
        {fprintf(stderr, "Error: (line %d) type %s does not match with type %s.\n", lineno, s1->name, type2);
        exit(1);}
    }
    else if(s1->kind == typeSym && s2->kind == typeSym || s1->kind == structSym && s2->kind == structSym)
    {
        if(s1 == s2)//same type declaration, deals with array/slice cases
        {
             return true;
        }
        if(isVerbose)
        {fprintf(stderr, "Error: (line %d) type %s does not match with type %s.\n", lineno, s1->name, s2->name);
        exit(1);}
    }
    else if(s1->kind == varstructSym && s2->kind == varstructSym)
    {
        SYMBOL *fields1, *fields2;
        fields1 = s1->val.structFields;
        fields2 = s2->val.structFields;
        while(fields1 != NULL && fields2 != NULL)
        {
            if(MatchingTypes(fields1, fields2, lineno, false))
            {
                fields1 = fields1->next;
                fields2 = fields2->next;
            }
            else{
                if(isVerbose)
                {fprintf(stderr, "Error: (line %d) field %s does not match with field %s in structs.\n", lineno, fields1->name, fields2->name);
                exit(1);}
                return false;
            }
        }
        return (fields1 == NULL && fields2 == NULL);
    }
    return false;
}
//determines if 2 types can be interchanged, when cast to the other
bool SubTypes(SYMBOL *s1, SYMBOL *s2, int lineno)
{
    char *type1 = longTypeStr(s1);
    char *type2 = longTypeStr(s2);
    if( strstr(type1, type2) != NULL || strstr(type2,type1) != NULL)//true when one is a substring of the other
    {
        return true;
    }
    fprintf(stderr, "Error: (line %d) type %s cannot be cast to type %s.\n", lineno, s2->name, s1->name);
    exit(1);
    return false;
}
char *typeStr(TYPE *tmp)
{
    char *typename = malloc(128);
    strcpy(typename, "");
    TYPE *cur = tmp;
    while(1)
    {
        if(cur == NULL || cur->gType == nilType)
        {
            return typename;
        }     
        else if(cur->gType == arrayType){
            char tmpstr[32];
            sprintf(tmpstr, "[%d]", cur->size);
            strcat(typename, tmpstr);
        }
        else if(cur->gType == sliceType)
        {
            strcat(typename, "[]");
        }
        else{
            strcat(typename, cur->name);
        }
        cur = cur->val.arg;//get next
    }
}
//determines if the first type is an array type (slice or array) of the second
bool sliceTypes(SYMBOL *s1, SYMBOL *s2, int lineno){
    int offset = 0;
    int size = 0;
    TYPE *tmpType = s1->t;
    SYMBOL *refParent;
    if(s1->kind != structSym) 
        refParent = s1->val.parentSym;
    while(tmpType != NULL)
    {
        if(tmpType->gType == sliceType)
            break;
        else if(tmpType->gType == arrayType || tmpType->gType == structType)
        {
            fprintf(stderr, "Error: (line %d) First argument of append must have slice type.\n", lineno);
            exit(1);
        }
        else{
            tmpType = tmpType->val.arg;
            if(tmpType == NULL || tmpType->gType == nilType) 
            {
                tmpType = refParent->t;
                if(refParent->kind != structSym)
                    refParent = refParent->val.parentSym;
                else{
                    fprintf(stderr, "Error: (line %d) First argument of append must have slice type.\n", lineno);
                    exit(1);
                }
            }    

        }
    }
    if(tmpType == NULL || tmpType->gType == nilType)
    {
        fprintf(stderr, "Error: (line %d) First argument of append must have slice type.\n", lineno);
        exit(1);
    }
    char *type1 = typeStr(tmpType);
    char *type2;
    if(s2->kind == varSym)
    {    type2 = shortTypeStr(s2);}
    else
    {
        type2 = s2->name;
    }
    
    // if(strcmp(getName(s1->t), "struct") == 0 || strcmp(getName(s2->t), "struct") == 0 )
    // {
    //     fprintf(stderr, "Error: (line %d) elements of type %s are not proper elements of type %s.\n", lineno, type2, type1);
    //     exit(1);
    // }
        type1 += 2;//shift by 2
        if(strcmp(type1, type2) == 0)
        {//s1 is always a varSym, since we cannot typecast to a slice type
            if(s2->kind == varSym)
            {
                if(refParent == s2->val.parentSym)
                    return true;
            }
            if(s2->kind == typeSym || s2->kind == structSym)
            {
                if(refParent == s2)
                    return true;
            }
        }
        type1-=2;
    
    fprintf(stderr, "Error: (line %d) elements of type %s are not proper elements of type %s.\n", lineno, type2, type1);
    exit(1);

    return false;
    
}
//check for type casts
void investigateType(TYPE *t, int lineno)
{
    TYPE *tmp = t;
    while(tmp!=NULL)
    {
        if(tmp->gType == structType)
        {
            fprintf(stderr, "Error: (line %d) cannot cast to struct type.\n", lineno);
            exit(1);
        }
        if(tmp->gType == sliceType)
        {
            fprintf(stderr, "Error: (line %d) cannot cast to slice type.\n", lineno);
            exit(1);
        }
        if(tmp->gType == arrayType)
        {
            fprintf(stderr, "Error: (line %d) cannot cast to array type.\n", lineno);
            exit(1);
        }
        if(tmp != NULL && tmp->gType != nilType)//descend hierarchy
        {
            tmp = tmp->val.arg;
        }
        else{
            break;
        }
    }
}

//check for type validity for print stmts
void investigateTypePrint(TYPE *t, int lineno)
{
    TYPE *tmp = t;
    while(tmp != NULL)
    {
        if(tmp->gType == structType)
        {
            fprintf(stderr, "Error: (line %d) cannot print struct type, expecting base types.\n", lineno);
            exit(1);
        }
        if(tmp->gType == sliceType)
        {
            fprintf(stderr, "Error: (line %d) cannot print slice type, expecting base types.\n", lineno);
            exit(1);
        }
        if(tmp->gType == arrayType)
        {
            fprintf(stderr, "Error: (line %d) cannot print array type, expecting base types.\n", lineno);
            exit(1);
        }
        if(tmp != NULL && tmp->gType != nilType)//descend hierarchy
        {
            tmp = tmp->val.arg;
        }
        else{
            break;
        }
    }
}

bool checkDefaultCasts(SYMBOL *sym1, SYMBOL *sym2, int lineno)
{
    SYMBOL *s1, *s2;
    s1 = sym1;
    s2 = sym2;
    TYPE *tmpType = sym1->t;
    if(sym1->kind == structSym || sym2->kind == structSym)
    {
        fprintf(stderr, "Error: (line %d) expecting base types for type cast.\n", lineno);
        exit(1);
    }
    while(tmpType != NULL)
    {
        if(tmpType->gType == sliceType || tmpType->gType == arrayType || tmpType->gType == structType)
        {
            fprintf(stderr, "Error: (line %d) expecting base types for type cast.\n", lineno);
            exit(1);
        }
        else{
            tmpType = tmpType->val.arg;
            if(tmpType == NULL || tmpType->gType == nilType) 
            {
                if(s1->kind != structSym && s1->val.parentSym == BLANK_SYMBOL)
                    break;
                if(s1->kind != structSym)
                {    s1 = s1->val.parentSym;
                    tmpType = s1->t;
                }
                else{
                    fprintf(stderr, "Error: (line %d) First argument of append must have slice type.\n", lineno);
                    exit(1);
                }
            }    

        }
    }
    tmpType = s2->t;
    while(tmpType != NULL)
    {
        if(tmpType->gType == sliceType || tmpType->gType == arrayType || tmpType->gType == structType)
        {
            fprintf(stderr, "Error: (line %d) expecting base types for type cast.\n", lineno);
            exit(1);
        }
        else{
            tmpType = tmpType->val.arg;
            if(tmpType == NULL || tmpType->gType == nilType) 
            {
                if(s2->kind != structSym && s2->val.parentSym == BLANK_SYMBOL)
                    break;
                if(s2->kind != structSym)
                {    s2 = s2->val.parentSym;
                    tmpType = s2->t;
                }
                else{
                    fprintf(stderr, "Error: (line %d) First argument of append must have slice type.\n", lineno);
                    exit(1);
                }
            }    

        }
    }
    if(MatchingTypes(s1, INT_SYMBOL, 0, false))
    {
        if(MatchingTypes(s2, INT_SYMBOL, 0, false) ||
            MatchingTypes(s2, FLOAT_SYMBOL, 0, false) ||
            MatchingTypes(s2, RUNE_SYMBOL, 0, false))
            {
                return true;
            }
    }
    else if(MatchingTypes(s1, FLOAT_SYMBOL, 0, false))
    {
        if(MatchingTypes(s2, INT_SYMBOL, 0, false) ||
            MatchingTypes(s2, FLOAT_SYMBOL, 0, false) ||
            MatchingTypes(s2, RUNE_SYMBOL, 0, false))
            {
                return true;
            }
    }
    else if(MatchingTypes(s1, RUNE_SYMBOL, 0, false))
    {
        if(MatchingTypes(s2, INT_SYMBOL, 0, false) ||
            MatchingTypes(s2, FLOAT_SYMBOL, 0, false) ||
            MatchingTypes(s2, RUNE_SYMBOL, 0, false))
            {
                return true;
            }
    }
    else if(MatchingTypes(s1, STR_SYMBOL, 0, false))
    {
        if(MatchingTypes(s2, INT_SYMBOL, 0, false) ||
            MatchingTypes(s2, STR_SYMBOL, 0, false) ||
            MatchingTypes(s2, RUNE_SYMBOL, 0, false))
            {
                return true;
            }
    }
    else if(MatchingTypes(s1, BOOL_SYMBOL, 0, false))
    {
        if(MatchingTypes(s2, BOOL_SYMBOL, 0, false))
        {
            return true;
        }
    }
    fprintf(stderr, "Error: (line %d) invalid type cast.\n", lineno);
    exit(1);
}

SYMBOL *structAccessHelper(SYMBOL *sym, char *id, int lineno)
{
            SYMBOL *refParent = sym;
            if( sym->kind != structSym && sym->kind != varstructSym) 
                refParent = sym->val.parentSym;
            TYPE *tmpType = sym->t;
            while(tmpType != NULL)
            {
                if(tmpType->gType == structType)
                {
                    break;
                }
                else if(tmpType->gType == sliceType || tmpType->gType == arrayType)
                {
                    fprintf(stderr, "Error: (line %d) type %s has no field %s.\n", lineno, shortTypeStr(sym), id);
                    exit(1);
                }
                else{
                    tmpType = tmpType->val.arg;
                    if(tmpType == NULL || tmpType->gType == nilType) 
                    {
                        tmpType = refParent->t;
                        if(refParent != NULL && refParent->kind != structSym)
                        {
                            tmpType = refParent->t;
                            refParent = refParent->val.parentSym;
                        }
                        else if(refParent == BLANK_SYMBOL)
                        {
                            fprintf(stderr, "Error: (line %d) type %s is not a struct.\n", lineno, shortTypeStr(sym));
                            exit(1);
                        }
                        else{
                            tmpType = refParent->t;
                        }
                    }    
                }
            }
            if (tmpType->gType==structType){
                SYMBOL *fields = refParent->val.structFields;
                while(fields != NULL){
                    if (strcmp(fields->name, id)==0){
                        return fields;
                    }
                    fields = fields->next;
                }
                fprintf(stderr, "Error: (line %d) Attempting to access struct field which does not exist\n", lineno);
                exit(1);
            }
            else{
                printf("Attempting to access field of non-struct variable\n");
                exit(1);
            }
}

void lenHelper(SYMBOL *sym, int lineno)
{
            SYMBOL *refParent = sym;
            if(MatchingTypes(sym, STR_SYMBOL, lineno, false))
                return;
            if( sym->kind != structSym && sym->kind != varstructSym) 
                refParent = sym->val.parentSym;
            TYPE *tmpType = sym->t;
            while(tmpType != NULL)
            {
                if(tmpType->gType == structType)
                {
                    fprintf(stderr, "Error: (line %d) type %s has no length.\n", lineno, shortTypeStr(sym));
                    exit(1);
                }
                else if((refParent != NULL && refParent->t->gType != nilType && MatchingTypes(refParent, STR_SYMBOL, lineno, false))
                         || tmpType->gType == sliceType || tmpType->gType == arrayType)
                {
                    return;
                }
                else{
                    tmpType = tmpType->val.arg;
                    if(tmpType == NULL || tmpType->gType == nilType) 
                    {
                        tmpType = refParent->t;
                        if(refParent != NULL && refParent->kind != structSym && refParent->kind != varstructSym)
                        {
                            tmpType = refParent->t;
                            refParent = refParent->val.parentSym;
                        }
                        else if(refParent == BLANK_SYMBOL)
                        {
                            fprintf(stderr, "Error: (line %d) type %s is not a slice, array, or string.\n", lineno, shortTypeStr(sym));
                            exit(1);
                        }
                        else{
                            tmpType = refParent->t;
                        }
                    }    
                }
            }
            fprintf(stderr, "Error: (line %d) type %s has no length.\n", lineno, shortTypeStr(sym));
            exit(1);
}
void capHelper(SYMBOL *sym, int lineno)
{
            SYMBOL *refParent = sym;
            if( sym->kind != structSym && sym->kind != varstructSym) 
                refParent = sym->val.parentSym;
            TYPE *tmpType = sym->t;
            while(tmpType != NULL)
            {
                if(tmpType->gType == structType)
                {
                    fprintf(stderr, "Error: (line %d) type %s has no capacity.\n", lineno, shortTypeStr(sym));
                    exit(1);
                }
                else if(tmpType->gType == sliceType || tmpType->gType == arrayType)
                {
                    return;
                }
                else{
                    tmpType = tmpType->val.arg;
                    if(tmpType == NULL || tmpType->gType == nilType) 
                    {
                        tmpType = refParent->t;
                        if(refParent != NULL && refParent->kind != structSym && refParent->kind != varstructSym)
                        {
                            tmpType = refParent->t;
                            refParent = refParent->val.parentSym;
                        }
                        else if(refParent == BLANK_SYMBOL)
                        {
                            fprintf(stderr, "Error: (line %d) type %s is not a slice or array.\n", lineno, shortTypeStr(sym));
                            exit(1);
                        }
                        else{
                            tmpType = refParent->t;
                        }
                    }    
                }
            }
            fprintf(stderr, "Error: (line %d) type %s has no capacity.\n", lineno, shortTypeStr(sym));
            exit(1);
}
SYMBOL *elementHelper(SYMBOL *sym, int lineno)
{
            SYMBOL *refParent = sym;
            if( sym->kind != structSym && sym->kind != varstructSym) 
                refParent = sym->val.parentSym;
            TYPE *tmpType = sym->t;
            while(tmpType != NULL)
            {
                if(tmpType->gType == structType)
                {
                    fprintf(stderr, "Error: (line %d) type %s cannot be indexed.\n", lineno, shortTypeStr(sym));
                    exit(1);
                }
                else if(tmpType->gType == sliceType || tmpType->gType == arrayType)
                {
                    SYMBOL *returnSym = makeSymbol(" ", varSym);
                    returnSym->val.parentSym = refParent;
                    returnSym->t = tmpType->val.arg;
                    return returnSym;
                }
                else{
                    tmpType = tmpType->val.arg;
                    if(tmpType == NULL || tmpType->gType == nilType) 
                    {
                        tmpType = refParent->t;
                        if(refParent != NULL && refParent->kind != structSym && refParent->kind != varstructSym)
                        {
                            tmpType = refParent->t;
                            refParent = refParent->val.parentSym;
                        }
                        else if(refParent == BLANK_SYMBOL)
                        {
                            fprintf(stderr, "Error: (line %d) type %s cannot be indexed.\n", lineno, shortTypeStr(sym));
                            exit(1);
                        }
                        else{
                            tmpType = refParent->t;
                        }
                    }    
                }
            }
            fprintf(stderr, "Error: (line %d) type %s cannot be indexed.\n", lineno, shortTypeStr(sym));
            exit(1);
}

SYMBOL *resolveBaseType(SYMBOL *sym1)
{
    SYMBOL *s1;
    s1 = sym1;
    TYPE *tmpType = sym1->t;
    if(sym1->kind == structSym || sym1->kind == varstructSym)
    {
        return NULL;
    }
    while(tmpType != NULL)
    {
        if(tmpType->gType == sliceType || tmpType->gType == arrayType || tmpType->gType == structType)
        {
            return NULL;
        }
        else{
            tmpType = tmpType->val.arg;
            if(tmpType == NULL || tmpType->gType == nilType) 
            {
                if(s1->kind != structSym && s1->kind != varstructSym && s1->val.parentSym == BLANK_SYMBOL)
                    break;
                if(s1->kind != structSym && s1->kind != varstructSym)
                {    s1 = s1->val.parentSym;
                    tmpType = s1->t;
                }
                else{
                    return NULL;
                }
            }    

        }
    }
    return s1;

}

bool isComparable(SYMBOL *sym, symTable *table, int lineno)
{
    SYMBOL *s1;
    s1 = sym;
    TYPE *tmpType = sym->t;
    if(sym->kind == structSym || sym->kind == varstructSym)
    {
        s1 = sym->val.structFields;
        while(s1 != NULL)
        {
            if(!isComparable(s1, table, lineno))
            {
                return false;
            }
            s1 = s1->next;
        }
        return true;
    }

    while(tmpType != NULL)
    {
        if(tmpType->gType == arrayType)
        {
            tmpType = tmpType->val.arg;
        }
        else if(tmpType->gType == sliceType)
        {
            fprintf(stderr, "Error: (line %d) expected comparable not slice %s.\n", lineno, shortTypeStr(s1));
            exit(1);
        }
        else{
            if(tmpType->name != NULL) 
            {
                if(s1->kind == structSym || s1->kind == varstructSym)
                {
                    s1 = s1->val.structFields;
                    while(s1 != NULL)
                    {
                        if(!isComparable(s1, table, lineno))
                        {
                            return false;
                        }
                        s1 = s1->next;
                    }
                    return true;
                }

                if(s1->val.parentSym == BLANK_SYMBOL)
                {
                    return (s1 == INT_SYMBOL || s1 == FLOAT_SYMBOL || s1 == STR_SYMBOL || s1 == BOOL_SYMBOL || s1 == RUNE_SYMBOL);
                }
                s1 = s1->val.parentSym;
                tmpType = s1->t;
            }    

        }
    }
    return true;
}
// //checks that a type is a basic type "target"
// bool InDepthTypeCheck(SYMBOL *sym, char *target){
//     char *name = shortTypeStr(sym);
//     if(strcmp(target, name) == 0)
//     {
//         name = longTypeStr(sym);
//         if(strcmp(target, name) == 0)
//         {
//             if(sym->kind == varSym && strcmp(target, sym->val.parentSym->name)==0)
//             {
//                 return true;
//             }
            
//         }
//     }
//     return false;
// }
