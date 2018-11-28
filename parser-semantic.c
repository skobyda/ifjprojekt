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
bool ArrayInit = false;

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
    a->used++;
}

void freeArray (CArray *a) {
    free(a->arrayI);
    a->arrayI = NULL;
    a->used = 0;
    a->size = 0;
}

//varOrFun -> 0 = variable, 1 = function
bool SemanticDefinedControl(SymTablePtr currTable, unsigned line, char *name, int varOrFun){
    
    bool defined = false;
    
    SymbolPtr symbol = SymTableFind(currTable, name);

    if (!symbol)
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

/*bool Semantic2ndDefControl() {

    bool ok = false; //if all functions are defined, ok will be true

    if (controlA.used > 0) {
    unsigned found = 0;
    SymbolPtr symbol;
        for (int i = 0; i < controlA.used; i++) {
            symbol = SymTableFind(*/
    

void SemanticTreeInit (ATreeNodePtr *RootPtr) {
    *RootPtr = NULL;
}

bool ParserSemantic(ParTreePtr tree) {
    (void)(tree); //DELETE THIS LINE

    //TODO

    return NULL;
}
