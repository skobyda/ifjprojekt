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
static int condState = 0;//0 comparator, 1 left expression, 2 right expression
int condComp; //0 stands for != and == , 1 stands for others

/*0 stands for strings, 1 for int or float, 2 for unknown*/
static int leftExprComp; 
static int rightExprComp;


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
    
    SymbolPtr symbol = SymTableFind(currTable, name);

    if (symbol != NULL)
        defined = true;

    else if (varOrFun) { //if identifier is function name
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
                case typeInt:
                    *exprComp = 1;
                    break;
                case typeFloat:
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
            printf("ERROR: Using undefined variable: %s on the line: %u\n",token->name, token->line);
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

void SemanticFullCondControl(SymTablePtr currTable, TokenPtr token) {

    bool condOK;    

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
            break;
    }
    
    if (condState == 2) {
        if(condOK)
            printf("Condition is OK!\n");
        else
            printf("ERROR: Using invalid operator or operands in condition on the line: %u\n", token->line);

        condState = 0;
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
