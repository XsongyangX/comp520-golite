//the goal of this call is to make sure variables are used properly within the program
bool typeProgram(PROGRAM *prog){
    prog->globalScope = initSymbolTable();
    DECL *dec = prog->decList;
    while (dec != NULL){
        prog->globalScope = typeCheckDeclaration(dec, prog->globalScope);
        dec = dec->next;
    }
    return true;

}
SYMTABLE typeCheckDeclaration(DECLARATION *decl, SYMTABLE *table)
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
SYMTABLE typeCheckStatement(STATEMENT *stmt, SYMTABLE *table){

}
/*verify the type of expression, might need to make this return the type.
See shortTypeStr, longTypeStr, and lookup methods to do this.
Arguments passed to function calls must be verified.
Accessing struct elements should be verified as well.*/
SYMBOL *typecheckExp(EXP *exp, SYMTABLE *table){
    switch (exp->kind){
        case intExp:
            TYPE *dummyType = makeType(basetype, 1, "int", NULL);
            SYMBOL *dummySymbol = makeSymbol("DUMMY", nullSym, 0);
            dummySymbol->t = dummyType;
            return dummySymbol;
        case floatExp:
            TYPE *dummyType = makeType(basetype, 1, "float", NULL);
            SYMBOL *dummySymbol = makeSymbol("DUMMY", nullSym, 0);
            dummySymbol->t = dummyType;
            return dummySymbol;
        case strExp:
            TYPE *dummyType = makeType(basetype, 1, "str", NULL);
            SYMBOL *dummySymbol = makeSymbol("DUMMY", nullSym, 0);
            dummySymbol->t = dummyType;
            return dummySymbol;
        case rawstrExp:
            TYPE *dummyType = makeType(basetype, 1, "str", NULL);
            SYMBOL *dummySymbol = makeSymbol("DUMMY", nullSym, 0);
            dummySymbol->t = dummyType;
            return dummySymbol;
        case boolExp:
            TYPE *dummyType = makeType(basetype, 1, "bool", NULL);
            SYMBOL *dummySymbol = makeSymbol("DUMMY", nullSym, 0);
            dummySymbol->t = dummyType;
            return dummySymbol;
        case runeExp:
            TYPE *dummyType = makeType(basetype, 1, "rune", NULL);
            SYMBOL *dummySymbol = makeSymbol("DUMMY", nullSym, 0);
            dummySymbol->t = dummyType;
            return dummySymbol;
        case plusExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0 
            || strcmp(dummy1->t->name, "rune")==0 || strcmp(dummy1->t->name, "str")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of + operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of + operator\n");
                exit(1);
            }
        case minusExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0 || strcmp(dummy1->t->name, "rune")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of - operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of - operator\n");
                exit(1);
            }
        case timesExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0 || strcmp(dummy1->t->name, "rune")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of * operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of * operator\n");
                exit(1);
            }
        case divExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0 || strcmp(dummy1->t->name, "rune")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of / operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of / operator\n");
                exit(1);
            }
        case modExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of \% operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of \% operator\n");
                exit(1);
            }
        /* 

        --------------------------------
        UNFINISHED
        --------------------------------

        case eqExp:
            TYPE *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            TYPE *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            TYPE *dummy3 = makeType(basetype, 1, "bool", NULL);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0
            || strcmp(dummy1->t->name, "bool")==0 || strcmp(dummy1->t->name, "str")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy3;
                }
                else{
                    printf("Arguments of different type in == operator\n");
                    exit(1);
                }   
            }
            else if(dummy1->kind==structSym && dummy2->kind==structSym){
                SYMBOL *dummyStructSym1 = dummy1->val->structFields;
                SYMBOL *dummyStructSym2 = dummy2->val->structFields;
                EXP *dummyLHS = makeEXP_empty();
                EXP *dummyRHS = makeEXP_empty();
                dummyLHS->t = dummyStructSym
                EXP *dummyExp = makeEXP_eq()
                while(dummyStructSym1 != NULL){
                    if (dummyStructSym2 != NULL){
                        
                    }
                    else{
                        printf("Bad comparison of structs\n");
                        exit(1);
                    }
                }
            }
            else if(dummy1->gType==arrayType && dummy2->gType==arrayType){

            }
            else{
                printf("Invalid type in first argument of == operator\n");
                exit(1);
            }
        case neqExp:
        case geqExp:
        case leqExp:
        case gtExp:
        case ltExp:
        */
        case orExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "bool")==0 && strcmp(dummy2->t->name, "bool")==0){
                return dummy1; 
            }
            else{
                printf("Non-Boolean Arguments of || operator\n");
                exit(1);
            }
        case andExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "bool")==0 && strcmp(dummy2->t->name, "bool")==0){
                return dummy1; 
            }
            else{
                printf("Non-Boolean Arguments of && operator\n");
                exit(1);
            }
        case andnotExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of &^ operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of &^ operator\n");
                exit(1);
            }
        case bitAndExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of & operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of & operator\n");
                exit(1);
            }
        case bitOrExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of | operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of | operator\n");
                exit(1);
            }
        case xorExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of ^ operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of ^ operator\n");
                exit(1);
            }
        case lshiftExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of << operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of << operator\n");
                exit(1);
            }
        case rshiftExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0){
                if (strcmp(dummy1->t->name, dummy2->t->name) == 0){
                    return dummy1;
                }
                else{
                    printf("Invalid second argument of >> operator\n");
                    exit(1);
                }   
            }
            else{
                printf("Invalid first argument of >> operator\n");
                exit(1);
            }
        case elementExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (dummy1->t->gType==arrayType || dummy1->t->gType == sliceType){
                if (dummy2->t->name=="int"){
                    return dummy1->val->parentType;
                }
                else{
                    printf("Attempting to access an array or slice with a non-integer index\n");
                    exit(1);
                }
            }
            else{
                printf("Attempting to access element of non-array and non-slice variable\n");
                exit(1);
            }
        case invocExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->lhs, table);
            SYMBOL *dummy2 = typecheckExp(exp->val->binary->rhs, table);
            if (dummy1->t->gType==structType){
                dummy1 = dummy1->val->structFields;
                while(dummy1 != NULL){
                    if (strcmp(dummy1->name, dummy2->name)==0){
                        return dummy1;
                    }
                    dummy1 = dummy1->next;
                }
                printf("Attempting to access struct field which does not exist\n");
                exit(1);
            }
            else{
                printf("Attempting to access field of non-struct variable\n");
                exit(1);
            }
        case notExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "bool")==0){
                return dummy1;
            }
            else{
                printf("Invalid first argument of unary ! operator\n");
                exit(1);
            }
        case posExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0 || strcmp(dummy1->t->name, "rune")==0){
                return dummy1;
            }
            else{
                printf("Invalid first argument of unary + operator\n");
                exit(1);
            }
        case negExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "float")==0 || strcmp(dummy1->t->name, "rune")==0){
                return dummy1;
            }
            else{
                printf("Invalid first argument of unary - operator\n");
                exit(1);
            }
        case parExp:
            return typecheckExp(exp->val->binary->rhs, table);
        case indexExp:
        /*Not exactly clear what the difference is between this and
        elementexp... so I'll leave this blank for now*/
            break;
        case uxorExp:
            SYMBOL *dummy1 = typecheckExp(exp->val->binary->rhs, table);
            if (strcmp(dummy1->t->name, "int")==0 || strcmp(dummy1->t->name, "rune")==0){
                return dummy1;
            }
            else{
                printf("Invalid first argument of unary ^ operator\n");
                exit(1);
            }
        case idExp:
            return getSymbol(table, e->val->identifier, varKind);

        /*

        --------------------------------
        UNFINISHED
        --------------------------------


        case funcExp:
            if(strlen(lookupFunc(table, exp->val.fn->identifier, lineno, 0)) == 0)//function 
            {
            
                if(strlen(lookupType(table,exp->val.fn->identifier, lineno, 0)) != 0) {}
                else{
                    fprintf(stderr, "Error: (line %d) undefined function %s.\n", lineno, exp->val.fn->identifier);
                    exit(1);
                }
            }
            EXP *dummy1 = lookupFunc(table, exp->val.fn->identifier)
            EXP *dummy2 = DECL->val->fnCallBlock->idblock->next
            symExp(exp->val.fn->params->val.fnCallBlock, table, lineno);
            break;
        case expblockExp://this case occurs from function calls
            symExp(exp->val.expblock.next, table, lineno);
            symExp(exp->val.expblock.value, table, lineno);
            break;
        case funcBlockExp:
            funcBlockHelper(exp, table, lineno);
            symExp(exp->val.fnblock.fn->params->val.fnCallBlock, table, lineno);
            break;


        */
        case appendExp:
            SYMBOL *dummy1 = symEXP(exp->val.binary.lhs);
            SYMBOL *dummy2 = symEXP(exp->val.binary.rhs);
            if (dummy1->t->gType==sliceType && dummy1->t->val->arg->gType==dummy2->t->gType 
            && dummy1->t->val->arg->name==dummy2->t->name){
                return dummy1;
            }
            else{
                printf("Invalid expression in append() function\n");
                exit(1);
            }
        case lenExp:
            SYMBOL *dummy = symEXP(exp->val.binary.rhs);
            if (strcmp(dummy->t->name, "str")==0 || dummy->t->gType==arrayType || dummy->t->gType==sliceType){
                dummy = makeSymbol("DUMMY", nullSym, 0);
                TYPE *dummyType = makeType(basetype, 1, "int", NULL);
                dummy->t = dummyType;
                return dummy;
            }
            else{
                printf("Invalid expression in len() function\n");
                exit(1);
            }
        case capExp:
            SYMBOL *dummy = symEXP(exp->val.binary.rhs);
            if (dummy->t->gType==arrayType || dummy->t->gType==sliceType){
                dummy = makeSymbol("DUMMY", nullSym, 0);
                TYPE *dummyType = makeType(basetype, 1, "int", NULL);
                dummy->t = dummyType;
                return dummy;
            }
            else{
                printf("Invalid expression in cap() function\n");
                exit(1);
            }
        default:
            fprintf(stderr, "Unrecognized expression kind %d", exp->kind);
            exit(1); 
    }
}
