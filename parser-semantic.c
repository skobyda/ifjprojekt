/* IFJ Project 2018/19
 * Implementation of Semantic Analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: parser-semantic.h
 *  FILE AUTHOR: Denis Filo(xfilod00)
 */

/***SYSTEM FILES***/
#include <stdbool.h>
#include <stdlib.h>

/***LOCAL FILES***/
#include "scanner.h"
#include "parser.h"
#include "parser-semantic.h"
#include "generator.h"
#include "symtable.h"
#include "main.h"

/* Runs Semantic analysis over derivation tree.
 * throws some error if semantic bug was detected.
 * @tree Pointer to derivation tree.
 * Returns: true in case of success, false in case of error
 */

CArray controlA;
static bool ArrayInit = false;
/*0 comparator, 1 left expression, 2 right expression, 3 full control, 4 reset*/
static int condState = 4;
/*0 stands for != and == , 1 stands for others*/
int condComp; 
/*0 stands for strings, 1 for int or float, 2 nil, 3 for unknown, 4 reset*/
static int leftExprComp = 3; 
static int rightExprComp = 3;

/*0 stands for plus(+), 1 for others(*,/,-), 2 for none */
static int exprOperator = 2;
/*0 stands for strings, 1 for int or float,  2 nil, 3 for unkown, 4 reset*/
static int exprAssignCompType = 4;

static char* identVarName = NULL;
static char* identFunName = NULL;
/*-1 means 1 or more params are correct*/
static int numOfParam;
/*-2 means unknown num of parameters, not defined yet*/
static int paramCount;


static void SemanticInitArray (CArray *a, size_t initSize) {
    
    a->arrayI = (FunIdent *)malloc (initSize * sizeof(FunIdent));
    a->used = 0;
    a->size = initSize;
}

static void SemanticInsertArray (CArray *a, unsigned line, char *name) {


    if (a->used == a->size) {
        a->size += ARRAYSIZE;
        a->arrayI = (FunIdent *)realloc (a->arrayI, a->size * sizeof(FunIdent));
    }
    a->arrayI[a->used].line = line;
    a->arrayI[a->used].name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(a->arrayI[a->used].name, name);
    a->arrayI[a->used].numOfParam = -2; //unknown yet
    a->used++;
   
}

static void SemanticArrayParamInsert(char *name, int numOfParam) {

    if (controlA.used > 0) {
        for (unsigned i = 0; i < controlA.used; i++) {
            if (strcmp(controlA.arrayI[i].name, name) == 0 &&
                controlA.arrayI[i].numOfParam == -2) {
                    controlA.arrayI[i].numOfParam = numOfParam;
                    break;
            }  
        }
    }
}
static void SemanticFreeArray (CArray *a) {
    free(a->arrayI);
    a->arrayI = NULL;
    a->used = 0;
    a->size = 0;
}

/*Funtion checks if all used variables and functions are already defined.
 *If they are not defined, function returns false.
 *If function is not defined, it will be checked again at the end of parsing.
 *varOrFun -> 0 = variable, 1 = function
 *block: 0 = global, 1 = function definition, 2 = no matter(looking for variable)
 */
static bool SemanticDefinedControl(SymTablePtr currTable, unsigned line, char *name, int varOrFun, int block){
    
    bool defined = false;

    
    SymbolPtr symbol = NULL;
    symbol = SymTableFind(currTable, name);

	//if identifier is variable name
    if (symbol != NULL && symbol->iType == VARIABLE && varOrFun == 0) 
        defined = true;
   
	//if identifier is function name 
	else if (symbol != NULL && symbol->iType == FUNCTION && varOrFun == 1)
		defined = true;
    else if (block == 1) {
        if ((symbol == NULL && varOrFun == 1) ||
	        (symbol != NULL && symbol->iType == VARIABLE && varOrFun == 1)){ 
            if (!ArrayInit){
                SemanticInitArray (&controlA, ARRAYSIZE);
                ArrayInit = true;
            } 
        SemanticInsertArray (&controlA, line, name);
    
        }
    }
    
    return defined;
}
             
/*Function checks if all used functions in code are defined.
 *It checks only those functions that were called before their definition in other function definition.
 */
void Semantic2ndDefControl() {

    if (controlA.used > 0) {
    SymbolPtr symbol = NULL;
        for (unsigned i = 0; i < controlA.used; i++) {
            symbol = SymTableFind(globalTable, controlA.arrayI[i].name);
            if (symbol == NULL || (symbol != NULL && symbol->iType == VARIABLE)) {
                PrintError(3, controlA.arrayI[i].line,"Called function '%s' is not defined.", controlA.arrayI[i].name);
            }
            else if (symbol->numOfParameters != controlA.arrayI[i].numOfParam)
                PrintError(5, controlA.arrayI[i].line,"Wrong number of arguments in function call '%s'.", controlA.arrayI[i].name);
        }
    }
    SemanticFreeArray(&controlA);
    free(identVarName);
    free(identFunName);
}

/*Sets type of comparator for conditions*/
static void SemanticCondCompSet(TokenPtr token) {

    if (token->lexem >= 13 && token->lexem <= 16)
        condComp = 1;

    else if (token->lexem == EQ || token->lexem == NOTEQ)
        condComp = 0;
}

/*Sets data type of expression in conditon or expression in variable assignment*/
static bool SemanticExprCompSet(SymTablePtr currTable, TokenPtr token, int *exprComp) {
   
    bool ok = true; 
    switch (token->lexem) {
        case STR:
            *exprComp = 0;
            break;
        case INT: case FLOAT:
            *exprComp = 1;
            break;
        case NIL:
            *exprComp = 2;
            break;
        case IDENT:
            break;
        default:
            *exprComp = 3;
            break;
        }
        
        if (token->lexem == IDENT) {
            bool defined = SemanticDefinedControl(currTable, token->line, token->name, 0, 2);
          
            if (defined) {
                SymbolPtr symbol = SymTableFind(currTable, token->name);
                switch (symbol->dType) {
                    case typeString:
                        *exprComp = 0;
                        break;
                    case typeNumeric:
                        *exprComp = 1;
                        break;
                    case typeNil:
                        *exprComp = 2;
                        break;
                    case typeUnknown:
                        *exprComp = 3;
                        break;
                    default:
                        *exprComp = 3;
                        break;
                }
            }
            else {
                PrintError(3,token->line, "Using undefined variable '%s'", token->name);
            ok = false;
        }
    }
    return ok; 
}

/*condComp 0 stands for != and == , 1 stands for others
 *exprComp 0 stands for strings, 1 for int or float, 2 for unknown
 */
static bool SemanticCondControl() {
    bool condOK = false;

    if (condComp == 0)
        condOK = true;
    else if (leftExprComp == rightExprComp && (leftExprComp != 2 && rightExprComp !=2))
        condOK = true;
    else if ((leftExprComp == 3 && rightExprComp !=2) ||
             (leftExprComp != 2 && rightExprComp == 3))
        condOK = true;
    
    return condOK;            
}    

/*Makes full control of condition in if or while statement
 *now in parser called in parserExpression like SemanticFullCondControl(currentTable, tokenToPrint);
 *calling: parser have to decide if expression is condition
 */
void SemanticFullCondControl(SymTablePtr currTable, TokenPtr token) {

    bool condOK, flag1, flag2;

    if ((token->lexem >= 13 && token->lexem <= 16) ||
         token->lexem == EQ ||   
         token->lexem == NOTEQ)
        condState = 0;
    switch (condState) {
        case 0: 
            SemanticCondCompSet(token);
            condState = 1;
            break;
        case 1:
            flag1 = SemanticExprCompSet(currTable, token, &leftExprComp);
            if (!flag1) 
                condOK = false;   
            condState = 2;
            break;
        case 2:
            flag2 = SemanticExprCompSet(currTable, token, &rightExprComp);
            if (flag2)
                condOK = SemanticCondControl();
            else
                condOK = false;
            condState = 3;
            break;
    }
    
    if (condState == 3) {
        if(!condOK)
            PrintError(4,token->line, "Using invalid operator or operands in condition.");

        condState = 4;
    }
    
}

/*Sets type of operator in assignment expression control*/
static void SemanticOperatorSet (TokenPtr token) {

    if (token->lexem == PLUS && exprOperator != 1)
        exprOperator = 0;
    else if (token->lexem == MINUS ||
             token->lexem == MULTIPLY ||
             token->lexem == DIVISION)
        exprOperator = 1;

}
        
/*Sets type of variable when variable assignment expression
 *if OK is false, error in expression
 *if OK is true, so far no errors in expression
 */
static void SemanticVarAssignTypeSet(SymTablePtr currTable, bool ok) {

    SymbolPtr symbol = NULL;
    symbol = SymTableFind(currTable, identVarName);
    if (symbol != NULL) {

        if (ok) {
            switch (exprAssignCompType) {
                case 0:
                    symbol->dType = typeString;
                    break;
                case 1:
                    symbol->dType = typeNumeric;
                    break;
                case 2:
                    symbol->dType = typeNil;
                    break;
                case 3:
                    symbol->dType = typeUnknown;
                    break;
                default:
                    break;

                }
        }
        else
            symbol->dType = typeNil;
    }
    
}

/*Makes control of assignment expression
 *now in parser called in parserExpression like SemanticExprAssignCotrol(currentTable, tokenToPrint);
 *calling: before expression was variable assignment
 */
bool SemanticExprAssignCotrol (SymTablePtr currTable, TokenPtr token) {

    bool ok = true;

    if (token->lexem >= 9 && token->lexem <= 12) {
        if (exprAssignCompType != 2) 
            SemanticOperatorSet (token);
        else {
            PrintError(4,token->line,"Invalid operation with nil in expression."); 
            SemanticVarAssignTypeSet(currTable, false);
            return false; 
        }
    }
    else if (token->lexem >= 2 && token->lexem <= 6) {
        if (token->lexem == NIL && exprAssignCompType != 4) {
            PrintError(4,token->line,"Invalid operation with nil in expression");
            SemanticVarAssignTypeSet(currTable, false);
            return false;
        } 
        if ((token->lexem == NIL && exprAssignCompType == 4) ||
            (token->lexem != NIL && exprAssignCompType >= 3)) {
                bool flag1 = SemanticExprCompSet (currTable, token, &exprAssignCompType);
                if (flag1)
                    SemanticVarAssignTypeSet(currTable, true);
                else {
                    SemanticVarAssignTypeSet(currTable, false);
                    return false;
                }
        }
        else {
            int currentExprType;
            bool flag2 = SemanticExprCompSet (currTable, token, &currentExprType);
            if (!flag2) {
               SemanticVarAssignTypeSet(currTable, false);
               return false;
            } 
            else if (currentExprType == 2 || exprAssignCompType == 2) {
                PrintError(4, token->line, "Invalid operation with nil variable in expression.");
                SemanticVarAssignTypeSet(currTable, false);
                return false;
            }
            else if (currentExprType != exprAssignCompType &&
                currentExprType != 3 &&
                exprAssignCompType != 3) {
                SemanticVarAssignTypeSet(currTable, false);
                PrintError(4, token->line,"Incompatible operands in expression.");
                return false;
            }
            else if (currentExprType == 0 && exprOperator == 1) {
                SemanticVarAssignTypeSet(currTable, false);
                PrintError(4, token->line, "Invalid operator in expression with string.");
                return false;
            }
        }
    }
    return ok;
}
/*Called when expression in assignment is fully checked or when is semantic error in expression
 *calling: when expression stack is empty
 */
void SemanticExpAssignReset () {

    free(identFunName);
    identFunName = NULL;
    exprOperator = 2;
    exprAssignCompType = 4;
}    
/*Sets name of variable or functions and then used in other functions*/
static void SemanticNameSet (char *name, int varOrFun) {

    if (varOrFun == 1) {//function name
        identFunName = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(identFunName, name);
    }
    else {
        identVarName = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(identVarName, name);
    }
}
/*Function controls name of variable in variable assignment
 *returns true if it is able to use such name
 *return false if it is not able to use such name
 */
bool SemanticVarNameAssignControl (TokenPtr token, char *name) {

    SemanticNameSet (name, 0);
    SymbolPtr symbol = NULL;
    symbol = SymTableFind(globalTable, identVarName);

    if (symbol != NULL && symbol->iType == FUNCTION) {
        PrintError(3,token->line,"Cannot define variable with name '%s', already defined as function.",identVarName);
        return false;
    }
    return true;
}
/*Function controls name of function in definition
 *if it is able to use, returns true
 *if name alredy used, returns false
 */                  
bool SemanticFunNameDefControl(TokenPtr token, char *name) {

    SemanticNameSet (name, 1);
    SymbolPtr symbol = NULL;
    symbol = SymTableFind(globalTable, identFunName);

    if (symbol != NULL && symbol->iType == VARIABLE) {
        PrintError(3,token->line,"Cannot define function with name '%s', already defined as variable.",identFunName);
        return false;
    }
    else if (symbol != NULL && symbol->iType == FUNCTION) {
        PrintError(3,token->line,"Cannot redefine function '%s'.",identFunName);
        return false;
    }
    else 
        return true;
}
/*Function sets type of variable by called function - its return type*/
static void SemanticVarTypeSetByFunCall (SymTablePtr currTable, bool funIsDefined) {
    
    if (identVarName != NULL) {
	    SymbolPtr symbol = NULL;
        symbol = SymTableFind (currTable, identVarName);
	    if (symbol != NULL) {
                if (funIsDefined) { //function is already defined
                    if (strcmp(identFunName, "print") == 0)
                        symbol->dType = typeNil;
                    else if (strcmp(identFunName, "inputs") == 0)
                        symbol->dType = typeString;
                    else if (strcmp(identFunName, "inputi") == 0) 
                        symbol->dType = typeNumeric;
                    else if (strcmp(identFunName, "inputf") == 0)
                        symbol->dType = typeNumeric;
                    else if (strcmp(identFunName, "length") == 0)
                        symbol->dType = typeNumeric;
                    else if (strcmp(identFunName, "substr") == 0)
                        symbol->dType = typeString;
                    else if (strcmp(identFunName, "ord") == 0)
                        symbol->dType = typeNumeric;
                    else if (strcmp(identFunName, "chr") == 0)
                        symbol->dType = typeString;
                    else
                        symbol->dType = typeUnknown;
                }
                else
                    symbol->dType = typeUnknown;
            free(identVarName);
            identVarName = NULL; 
        }
    }
}
             

/*Function controls if called function is already defined
 *sets correct number of parameters for called function if its already defined
 *called in parser when there is function call
 *block: 0 = global, 1 = definition of function
 */
void SemanticFunNameCallControl(SymTablePtr currTable, TokenPtr token, char *name, int block){
    
//TODO simon este musi davat to tabulky pocet parametrov definovanych fcii
    SemanticNameSet (name, 1);
    bool defined = SemanticDefinedControl(globalTable,token->line, name, 1, block);
    if (defined) {
        SemanticVarTypeSetByFunCall (currTable, true);
        SymbolPtr symbol = SymTableFind(globalTable, name);
        numOfParam = symbol->numOfParameters;
        paramCount = 0;
    }
    else if (block == 1){
        SemanticVarTypeSetByFunCall (currTable, false); 
        numOfParam = -2; //unknown num of paramaters, not defined yet
        paramCount = 0;
    }
    else if (block == 0){
        PrintError(3,token->line,"Called function '%s' is not defined.", name);
        identFunName = NULL;
    }
    
}
/*Function controls when called function has no arguments*/
void SemanticNoParamControl(TokenPtr token) {

    if (identFunName != NULL) {

       if (numOfParam == -2)
           SemanticArrayParamInsert(identFunName, paramCount);

        if (numOfParam != 0 && numOfParam != -2) 
            PrintError(5, token->line,"Wrong number of arguments in function call '%s'.", identFunName);
        else if (numOfParam == -1)
            PrintError(5, token->line,"Wrong number of arguments in function call '%s'.", identFunName);
    }

}
/*Function controls if number of arguments in called function is correct
 *called in parser when there is no more arguments
 */
void SemanticNoMoreParam(TokenPtr token) {

    if (identFunName != NULL) {

        if (numOfParam == -2) 
            SemanticArrayParamInsert(identFunName, paramCount);
    
        if (numOfParam > paramCount && numOfParam != -1 && numOfParam != -2) 
            PrintError(5, token->line,"Wrong number of arguments in function call '%s'.", identFunName);
    }
    
}
/*Function controls if ident used as argument is ok*/
static bool SemanticArgIdentControl(SymTablePtr currTable, TokenPtr token) {

    bool ok = true;
    if (token->lexem == IDENT) {
        SymbolPtr symbol = NULL;
        symbol = SymTableFind(globalTable, token->name);

        if (symbol != NULL && symbol->iType == FUNCTION) {
            PrintError(6,token->line,"Function cannot be used as argument in function call. Argument '%s'.", token->name);
            ok = false;
        }
        else {
            symbol = SymTableFind(currTable, token->name);
             if (symbol == NULL) {
             PrintError(3,token->line,"Using undefined variable '%s'.",token->name);
            ok = false;
            }
        }
    }
    return ok;
}
/*Function checks expected string argument*/
static void SemanticExpectsArgString(SymTablePtr currTable, TokenPtr token) {
    
     if (token->lexem != STR && token->lexem != IDENT)
       PrintError(6,token->line,"Wrong type of argument '%s' in function call '%s'. Expecting string.", token->name, identFunName);
     if (token->lexem == IDENT) {
        bool flag = SemanticArgIdentControl(currTable, token);
        if (flag) {
            SymbolPtr symbol = NULL;
            symbol = SymTableFind(currTable, token->name);
            if (symbol->dType != typeString && symbol->dType != typeUnknown)
               PrintError(6,token->line,"Wrong type of argument '%s' in function call '%s'. Expecting string.", token->name, identFunName);
        }
    }    
 
}
/*Function checks expected int argument*/
static void SemanticExpectsArgInt(SymTablePtr currTable, TokenPtr token) {

    if (token->lexem != INT && token->lexem != IDENT)
       PrintError(6,token->line,"Wrong type of argument '%s' in function call '%s'. Expecting integer.", token->name, identFunName);
    if (token->lexem == IDENT) {
        bool flag = SemanticArgIdentControl(currTable, token);
        if (flag) {
            SymbolPtr symbol = SymTableFind(currTable, token->name);
            if (symbol->dType != typeNumeric && symbol->dType != typeUnknown)
               PrintError(6,token->line,"Wrong type of argument '%s' in function call '%s'. Expecting integer.", token->name, identFunName);
        }
    }    
 
}
/*Function checks arguments of print function*/
static void SemanticPrintCallControl(SymTablePtr currTable, TokenPtr token) {
    
    //expects only string, int, float, nil or indent as correct argument
    if (token->lexem < 2 && token->lexem < 6) 
        PrintError(6,token->line,"Wrong type of argument in function call '%s'.", identFunName);
    if (token->lexem == IDENT)
        SemanticArgIdentControl(currTable, token);
}

/*Function checks arguments of length function*/
static void SemanticLengthCallControl(SymTablePtr currTable, TokenPtr token){

    //expects only string or string variable
    SemanticExpectsArgString(currTable, token);
} 
 /*Function checks arguments of substr function*/    
static void SemanticSubstrCallControl(SymTablePtr currTable, TokenPtr token){

    switch (paramCount) {
        case 1:
            SemanticExpectsArgString(currTable, token);
            break;
        case 2: case 3:
            SemanticExpectsArgInt(currTable, token);
            break;
        default:
            break;
    }
}
/*Function checks arguments of ord function*/            
static void SemanticOrdCallControl(SymTablePtr currTable, TokenPtr token) {

    switch (paramCount) {
        case 1:
            SemanticExpectsArgString(currTable, token);
            break;
        case 2:
            SemanticExpectsArgInt(currTable, token); 
            break;
        default:
            break;
    }

}
/*Function checks arguments of chr function*/           
static void SemanticChrCallControl(SymTablePtr currTable, TokenPtr token) {

    //expects only integer or numeric(int) variable
    SemanticExpectsArgInt(currTable, token);
    
}
/*Function checks arguments of user defined function*/
static void SemanticOwnFunCallControl(SymTablePtr currTable, TokenPtr token) {

    //expects only string, int, float, nil or indent as correct argument
    if (token->lexem < 2 && token->lexem < 6) 
        PrintError(6,token->line,"Wrong type of argument in function call '%s'.", identFunName);
    if (token->lexem == IDENT)
        SemanticArgIdentControl(currTable, token);
}

/*Controls if argument is ok in current funnction call*/
void SemanticFunParamControl(SymTablePtr currTable, TokenPtr token) {

    if (identFunName != NULL) {
    
        paramCount++;

        if (strcmp(identFunName, "inputs") == 0 ||
            strcmp(identFunName, "inputi") == 0 ||
            strcmp(identFunName, "inputf") == 0) {

            if (paramCount == 1)//1 and more is wrong, error print only once
                PrintError(5,token->line,"Too many arguments in function call '%s'.", identFunName);
        }
        else if (strcmp(identFunName, "print") == 0)
            SemanticPrintCallControl(currTable, token);

        else if (strcmp(identFunName, "length") == 0) {

            if (paramCount == 1)
                SemanticLengthCallControl(currTable, token);
            if (paramCount == 2) //2 and more is wrong, error print only once
                PrintError(5,token->line,"Too many arguments in function call '%s'.", identFunName);
        }
        else if (strcmp(identFunName, "substr") == 0) {

            if (paramCount <= 3)
                SemanticSubstrCallControl(currTable, token);
            if (paramCount == 4) //4 and more is wrong, error print only once
                PrintError(5,token->line,"Too many arguments in function call '%s'.", identFunName);
        }      
        else if (strcmp(identFunName, "ord") == 0) {

            if (paramCount <= 2)
                SemanticOrdCallControl(currTable, token);
            if (paramCount == 3)//3 and more is wrong, error print only once
                    PrintError(5,token->line,"Too many arguments in function call '%s'.", identFunName);
        }
        else if (strcmp(identFunName, "chr") == 0) {

            if (paramCount == 1)
                SemanticChrCallControl(currTable, token);
            if (paramCount == 2) //2 and more is wrong, error print only once
                PrintError(5,token->line,"Too many arguments in function call '%s'.", identFunName);
        }
        else {
            if (paramCount <= numOfParam || numOfParam == -2)
                SemanticOwnFunCallControl(currTable, token);
            if (paramCount == numOfParam + 1)
                PrintError(5,token->line,"Too many arguments in function call '%s'.", identFunName);
        }
    }
    
}

