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
void typeCheckStatement(STATEMENT *stmt, symTable *table){

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
            MatchingTypes(dummy1, dummy2, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64") || InDepthTypeCheck(dummy1, "string"))
            {
                exp->t = dummy1->t;
                return dummy1; 
            }
            else{
                fprintf(stderr,"Invalid type %s for + operator\n", name);
                exit(1);
            }
        case minusExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64"))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Invalid type %s for - operator\n", name);
                exit(1);
            }
        case timesExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64"))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Invalid type %s for * operator\n", name);
                exit(1);
            }
        case divExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64"))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Invalid type %s for / operator\n", name);
                exit(1);
            }
        case modExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64"))
            {
                 exp->t = dummy1->t;
                 return dummy1; 
            }
            else{
                fprintf(stderr,"Invalid type %s for %% operator\n", name);
                exit(1);
            }
        case eqExp:
        case neqExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64") || InDepthTypeCheck(dummy1, "string"))
            {
                exp->t = BOOL_SYMBOL->t;
                return BOOL_SYMBOL; 
            }
            else if(dummy1->kind==structSym && dummy2->kind==structSym){
                if(dummy1 != dummy2)
                {
                    printf("Error: (line %d) incompatible struct types %s and %s.\n", lineno, dummy1->name, dummy2->name);
                    exit(1);
                }
                else{
                    exp->t = BOOL_SYMBOL->t;
                    return BOOL_SYMBOL; 
                }
            }
            else if(dummy1->t->gType==arrayType && dummy2->t->gType==arrayType){
                exp->t = BOOL_SYMBOL->t;
                return BOOL_SYMBOL; 
            }
            else if(dummy1->t->gType == sliceType && dummy2->t->gType == sliceType)
            {
                fprintf(stderr,"Error: (line %d) slice type is not a valid comparable.\n", lineno);
                exit(1);
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
            MatchingTypes(dummy1, dummy2, lineno);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune")||InDepthTypeCheck(dummy1, "float64") || InDepthTypeCheck(dummy1, "string"))
            {
                exp->t = BOOL_SYMBOL->t;
                return BOOL_SYMBOL; 
            }
            else{
                fprintf(stderr,"Error: (line %d) unexpected type %s in comparison expression. Expecting (int), (float64), (rune), (str).\n", lineno, dummy1->name);
                exit(1);
            }
        case orExp:
        case andExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            MatchingTypes(dummy1, dummy2, lineno);
            if ( InDepthTypeCheck(dummy1, "rune"))
            {
                exp->t = BOOL_SYMBOL->t;
                return BOOL_SYMBOL; 
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
            MatchingTypes(dummy1, dummy2, lineno);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune"))
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
            MatchingTypes(dummy1, dummy2, lineno);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune"))
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
            if (dummy1->t->gType==arrayType || dummy1->t->gType == sliceType){
                    dummy2 = makeSymbol(" ", typeSym, 1);
                    dummy2->t = dummy1->t->val.arg;
                    exp->t = dummy2->t;
                    return dummy2;
            }
            else{
                fprintf(stderr, "Error: (line %d) Attempting to access element of non-array and non-slice variable\n", lineno);
                exit(1);
            }
        case invocExp:
            dummy1 = typecheckExp(exp->val.binary.lhs, table, lineno);
            dummy2 = typecheckExp(exp->val.binary.rhs, table, lineno);
            if (dummy1->t->gType==structType){
                dummy1 = dummy1->val.structFields;
                while(dummy1 != NULL){
                    if (strcmp(dummy1->name, dummy2->name)==0){
                        return dummy1;
                    }
                    dummy1 = dummy1->next;
                }
                fprintf(stderr, "Error: (line %d) Attempting to access struct field which does not exist\n", lineno);
                exit(1);
            }
            else{
                printf("Attempting to access field of non-struct variable\n");
                exit(1);
            }
        case notExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "bool"))
            {
                    exp->t = dummy1->t;
                    return dummy1;
                
            }
            
            fprintf(stderr,"Error: (line %d) unexpected type %s in not expression. Expecting (bool).\n", lineno, dummy1->name);
            exit(1);
            
        case posExp:
        case negExp:
        case uxorExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            if (InDepthTypeCheck(dummy1, "int") || InDepthTypeCheck(dummy1, "rune"))
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
            if(InDepthTypeCheck(dummy1, "int"))
            {
                    exp->t = dummy1->t;
                    return dummy1;
                
            }
            fprintf(stderr, "Error: (line %d) Attempting to access an array or slice with a non-integer index\n", lineno);
            exit(1);
            
        case idExp:
            dummy1 = getSymbol(table, exp->val.identifier, varSym);
            exp->t = dummy1->t;
            return dummy1;

       

        case funcExp:
            if(strlen(lookupFunc(table, exp->val.fn->identifier, lineno, 0)) == 0)//function not found
            {
            
                if(strlen(lookupType(table,exp->val.fn->identifier, lineno, 0)) != 0) {
                    //type found: check for valid cast
                    dummy1 = getSymbol(table, exp->val.fn->identifier, typeSym);
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
                expList = exp->val.fn->params->val.fnCallBlock;
                while(dummy1 != NULL)
                {//type check each passed argument
                    if(expList == NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too few arguments for function %s.\n", lineno, exp->val.fn->identifier);
                        exit(1);
                    }
                    tmp = typecheckExp(expList->val.expblock.value,table, lineno);
                    MatchingTypes(dummy1, tmp, lineno);
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
            if(strlen(lookupFunc(table, name, lineno, 0)) == 0)//function not found
            {
            
                if(strlen(lookupType(table,name, lineno, 0)) != 0) {
                    //type found: check for valid cast
                    dummy1 = getSymbol(table, name, typeSym);
                    expList = exp->val.fnblock.fn->params->val.fnCallBlock;
                    if(expList->val.expblock.next != NULL && expList->val.expblock.next->val.expblock.value != NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too many arguments to type cast %s.\n", lineno, name);
                        exit(1);
                    }
                    dummy2 = typecheckExp(expList->val.expblock.value,table, lineno);
                    SubTypes(dummy1, dummy2, lineno);
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
                expList = exp->val.fn->params->val.fnCallBlock;
                while(dummy1 != NULL)
                {//type check each passed argument
                    if(expList == NULL)
                    {
                        fprintf(stderr, "Error: (line %d) too few arguments for function %s.\n", lineno, name);
                        exit(1);
                    }
                    tmp = typecheckExp(expList->val.expblock.value,table, lineno);
                    MatchingTypes(dummy1, tmp, lineno);
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
            ArrayTypes(dummy1, dummy2, lineno);
            exp->t = dummy1->t;
            return dummy1;
        case lenExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            name = longTypeStr(dummy1);
            if (InDepthTypeCheck(dummy1, "string") || dummy1->t->gType==arrayType || dummy1->t->gType==sliceType){
                exp->t = INT_SYMBOL->t;
                return INT_SYMBOL;
            }
            else{
                fprintf(stderr, "Error: (line %d) Invalid expression in cap() function. Expecting slice, array, or type string.\n", lineno);
                exit(1);
            }
        case capExp:
            dummy1 = typecheckExp(exp->val.binary.rhs, table, lineno);
            if (dummy1->t->gType==arrayType || dummy1->t->gType==sliceType){
                exp->t = INT_SYMBOL->t;
                return INT_SYMBOL;
            }
            else{
                fprintf(stderr, "Error: (line %d) Invalid expression in cap() function. Expecting slice or array.\n", lineno);
                exit(1);
            }
        default:
            fprintf(stderr, "Unrecognized expression kind %d", exp->kind);
            exit(1); 
    }
}
//determines if 2 types are equivalent
bool MatchingTypes(SYMBOL *s1, SYMBOL *s2, int lineno){
    if(strcmp(s1->name, s2->name) == 0){//same type name
        if(strcmp(shortTypeStr(s1), shortTypeStr(s2)) == 0)//same type declaration
        {
            if(strcmp(longTypeStr(s1), longTypeStr(s2)) == 0)//types match in their definition "hierarchy"
            {
                return true;
            }
        }
    }
    fprintf(stderr, "Error: (line %d) type %s does not match with type %s.\n", lineno, s1->name, s2->name);
    exit(1);
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
//determines if the first type is an array type (slice or array) of the second
bool ArrayTypes(SYMBOL *s1, SYMBOL *s2, int lineno){
    int offset = 0;
    int size = 0;
    
    char *type1 = shortTypeStr(s1);
    char *type2 = shortTypeStr(s2);
    if(s1->t->gType == sliceType)
    {
        type1 += 2;//shift by 2
        if(strcmp(type1, type2) == 0)
        {
            type1 = longTypeStr(s1);
            type1 += 2;
            type2 = longTypeStr(s2);
            if(strcmp(type1, type2) == 0)
            {
                return true;
            }
        }
    }
    else if(s1->t->gType == arrayType)
    {
        size = s1->t->size;
        //calculate number of chars to shift over
        while(size >= 1)
        {
            size = size / 10;
            offset++;
        }
        type1 += 2 + offset;//shift by 2
        if(strcmp(type1, type2) == 0)
        {
            type1 = longTypeStr(s1);
            type1 += 2 + offset;
            type2 = longTypeStr(s2);
            if(strcmp(type1, type2) == 0)
            {
                return true;
            }
        }
    }
    fprintf(stderr, "Error: (line %d) elements of type %s are not proper elements of type %s.\n", lineno, s2->name, s1->name);
    exit(1);

    return false;
    
}
//checks that a type is a basic type "target"
bool InDepthTypeCheck(SYMBOL *sym, char *target){
    char *name = shortTypeStr(sym);
    if(strcmp(target, name) == 0)
    {
        name = longTypeStr(sym);
        if(strcmp(target, name) == 0)
        {
            if(sym->kind == varSym && strcmp(target, sym->val.parentSym->name)==0)
            {
                return true;
            }
            
        }
    }
    return false;
}
