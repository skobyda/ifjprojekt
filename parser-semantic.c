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
/*0 stands for strings, 1 for int or float, 2 for unknown, 3 reset*/
static int leftExprComp = 2; 
static int rightExprComp = 2;

/*0 stands for plus(+), 1 for others(*,/,-), 2 for none */
int exprOperator = 2;
/*0 stands for strings, 1 for int or float, 2 for unkown, 3 reset(outside from parser)*/
int exprAssignCompType = 3;

char* identVarName;
char* identFunName;
/*-1 means 1 or more params are correct*/
int numOfParam;
/*-2 means unknown num of parameters, not defined yet*/
int paramCount;


void SemanticInitArray (CArray *a, size_t initSize) {
    
    a->arrayI = (FunIdent *)malloc (initSize * sizeof(FunIdent));
    a->used = 0;
    a->size = initSize;
}

void SemanticInsertArray (CArray *a, unsigned line, char *name) {

    if (a->used == a->size) {
        a->size += ARRAYSIZE;
        a->arrayI = (FunIdent *)realloc (a->arrayI, a->size * sizeof(FunIdent));
    }
    a->arrayI[a->used].line = line;
    a->arrayI[a->used].name = name;
    a->arrayI[a->used].defined = false;
    a->used++;
}

void freeArray (CArray *a) {
    free(a->arrayI);
    a->arrayI = NULL;
    a->used = 0;
    a->size = 0;
}

/*Funtion checks if all used variables and functions are already defined.
 *If they are not defined, function returns false.
 *If function is not defined, it will be checked again at the end of parsing.
 *varOrFun -> 0 = variable, 1 = function
 */
//TODO dokoncit pre vstavane funkcie, mozno aj na to upravit symtab
bool SemanticDefinedControl(SymTablePtr currTable, unsigned line, char *name, int varOrFun){
    
    bool defined = false;
    
    SymbolPtr symbol = NULL;
    symbol = SymTableFind(currTable, name);

	//if identifier is variable name
    if (symbol != NULL && symbol->iType == VARIABLE && varOrFun == 0) 
        defined = true;
   
	//if identifier is function name 
	else if (symbol != NULL && symbol->iType == FUNCTION && varOrFun == 1)
		defined = true;
    else if ((symbol == NULL && varOrFun == 1) ||
			 (symbol != NULL && symbol->iType == VARIABLE && varOrFun == 1)) { 
        if (!ArrayInit){
            SemanticInitArray (&controlA, ARRAYSIZE);
            ArrayInit = true;
        } 
        SemanticInsertArray (&controlA, line, name);
    }
    
    return defined;
}
             
/*Function checks if all used functions in code are defined.
 *It checks only those functions that were called before their definition.
 */
bool Semantic2ndDefControl() {

    bool ok = false; //if all functions are defined, ok will be true

    if (controlA.used > 0) {
    unsigned found = 0;
    SymbolPtr symbol = NULL;
        for (unsigned i = 0; i < controlA.used; i++) {
            symbol = SymTableFind(globalTable, controlA.arrayI[i].name);
            if (symbol != NULL) {
                controlA.arrayI[i].defined = true;
                found++;
            }
        }
    if (found == controlA.used)
        ok = true;
    }
    return ok;
}

/*Sets type of comparator for conditions*/
void SemanticCondCompSet(TokenPtr token) {

    if (token->lexem >= 13 && token->lexem <= 16)
        condComp = 1;

    else if (token->lexem == EQ || token->lexem == NOTEQ)
        condComp = 0;
}
//TODO vsade pre expression este aj null
/*Sets data type of expression in codition*/
void SemanticExprCompSet(SymTablePtr currTable, TokenPtr token, int *exprComp) {

    if (token->lexem == INT || token->lexem == FLOAT)
        *exprComp = 1;
    
    else if (token->lexem == STR)
        *exprComp = 0;

    else if (token->lexem == IDENT) {
        bool defined = SemanticDefinedControl(currTable, token->line, token->name, 0);
      
        if (defined) {
            SymbolPtr symbol = SymTableFind(currTable, token->name);
            switch (symbol->dType) {
                case typeUnknown:
                    *exprComp = 2;
                    break;
                case typeNumeric:
                    *exprComp = 1;
                    break;
                case typeString:
                    *exprComp = 0;
                    break;
                default:
                    *exprComp = 2;
            }
        }
        else {
            printf("ERROR: Using undefined variable '%s' on the line: %u\n",token->name, token->line);
        }
    } 
}

/*condComp 0 stands for != and == , 1 stands for others
 *exprComp 0 stands for strings, 1 for int or float, 2 for unknown
 */
bool SemanticCondControl() {
    bool condOK = false;

    if (leftExprComp == rightExprComp || leftExprComp == 2 || rightExprComp == 2)
        condOK = true;
    else if (condComp == 0)
        condOK = true;
    
    return condOK;            
}    

/*Makes full control of condition in if or while statement
 *now in parser called in parserExpression like SemanticFullCondControl(currentTable, tokenToPrint);
 *calling could change a bit in parser
 */
void SemanticFullCondControl(SymTablePtr currTable, TokenPtr token) {

    bool condOK;

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
            SemanticExprCompSet(currTable, token, &leftExprComp);
            condState = 2;
            break;
        case 2:
            SemanticExprCompSet(currTable, token, &rightExprComp);
            condOK = SemanticCondControl();
            condState = 3;
            break;
    }
    
    if (condState == 3) {
        if(condOK)
            printf("Condition is OK!\n");
        else
            printf("ERROR: Using invalid operator or operands in condition on the line: %u\n", token->line);

        condState = 4;
    }
    
}

/*Sets type of operator in assignment expression control*/
void SemanticOperatorSet (TokenPtr token) {

    if (token->lexem == PLUS && exprOperator != 1)
        exprOperator = 0;
    else if (token->lexem == MINUS ||
             token->lexem == MULTIPLY ||
             token->lexem == DIVISION)
        exprOperator = 1;

}
        
/*Sets data type of expression in assignment*/
void SemanticExprAssignTypeSet(SymTablePtr currTable, TokenPtr token, int *exprAssignType) {

    if (token->lexem == INT || token->lexem == FLOAT)
        *exprAssignType = 1;
    
    else if (token->lexem == STR)
        *exprAssignType = 0;

    else if (token->lexem == IDENT) {
        bool defined = SemanticDefinedControl(currTable, token->line, token->name, 0);
        if (defined) {
            SymbolPtr symbol = SymTableFind(currTable, token->name);
            switch (symbol->dType) {
                case typeUnknown:
                    *exprAssignType = 2;
                    break;
                case typeNumeric:
                    *exprAssignType = 1;
                    break;
                case typeString:
                    *exprAssignType = 0;
                    break;
                default:
                    *exprAssignType = 2;
            }
        }
        else {
            printf("ERROR: Using undefined variable '%s' on the line: %u\n",token->name, token->line);
        }
    }
}

/*Makes control of assignment expression
 *now in parser called in parserExpression like SemanticExprAssignCotrol(currentTable, tokenToPrint);
 *calling could change a bit in parser
 *TODO parser has to "reset" exprAssignCompType with value 3 after full Control
 */
void SemanticExprAssignCotrol (SymTablePtr currTable, TokenPtr token) {

    if (token->lexem >= 9 && token->lexem <= 12)
        SemanticOperatorSet (token);

    else if (token->lexem >= 2 && token->lexem <= 5 && exprAssignCompType >= 2)
        SemanticExprAssignTypeSet (currTable, token, &exprAssignCompType);

    else {
        int currentExprType;
        SemanticExprAssignTypeSet (currTable, token, &currentExprType);
        if (currentExprType != exprAssignCompType &&
            currentExprType != 2 &&
            exprAssignCompType != 2)
            printf("ERROR: Incompatible operands in expression on the line: %u\n", token->line);
        else if (currentExprType == 0 && exprOperator == 1)
            printf("ERROR: Invalid operator in expression with string on the line: %u\n", token->line);
    }
}

void SemanticNameSet (char *name, int varOrFun) {

		if (varOrFun == 1) {//function name
        	identFunName = malloc(sizeof(char) * (strlen(name) + 1));
        	strcpy(identFunName, name);
		}
		else {
			identVarName = malloc(sizeof(char) * (strlen(name) + 1));
        	strcpy(identVarName, name);
		}
}

//TODO might have changed to fit parser
//mozno budem musiet si zapamat meno a kontrolu zavolat az potom
bool SemanticVarNameAssignControl (SymTablePtr currTable, TokenPtr token, char *name) {

    SemanticNameSet (name, 0);
    SymbolPtr symbol = NULL;
    symbol = SymTableFind(globalTable, identVarName);

    if (symbol != NULL && symbol->iType == FUNCTION) {
        printf("ERROR: On the line: %u. Cannot define variable with name '%s', already defined as function.\n",token->line, identVarName);
        return false;
    }
    else {
        symbol = SymTableFind(currTable, identVarName);
        symbol->dType = typeUnknown;
    }
    return true;
}
                  
bool SemanticFunNameDefControl(TokenPtr token, char *name) {

    SemanticNameSet (name, 1);
    SymbolPtr symbol = NULL;
    symbol = SymTableFind(globalTable, identFunName);

    if (symbol != NULL && symbol->iType == VARIABLE) {
        printf("ERROR: On the line: %u. Cannot define function with name '%s', already defined as variable.\n",token->line, identFunName);
        return false;
    }
    else if (symbol != NULL && symbol->iType == FUNCTION) {
        printf("ERROR: On the line: %u. Function with name '%s' already exists.\n",token->line, identFunName);
        return false;
    }
    else 
        return true;
}

void SemanticFunNameCallControl(TokenPtr token, char *name){
//TODO simon este musi davat to tabulky pocet parametrov definovanych fcii
    SemanticNameSet (name, 1);
    bool defined = SemanticDefinedControl(globalTable,token->line, name, 1);
    if (defined) {
		SymbolPtr symbol = SymTableFind(globalTable, name);
    	numOfParam = symbol->numOfParameters;
        paramCount = 0;
	}
	else { 
		numOfParam = -2; //unknown num of paramaters, not defined yet
        paramCount = 0;
    }
}	


void SemanticTreeInit (ATreeNodePtr *RootPtr) {
    *RootPtr = NULL;
}

bool ParserSemantic(ParTreePtr tree) {
    (void)(tree); //DELETE THIS LINE

    //TODO

    return NULL;
}

