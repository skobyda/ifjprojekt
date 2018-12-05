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
 *  FILE AUTHOR: Denis Filo
 */

#ifndef PARSERSEMANTIC_H
#define PARSERSEMANTIC_H

/***SYSTEM FILES***/
#include <stdbool.h>

/***LOCAL FILES***/
#include "parser.h"
#include "scanner.h"
#include "symtable.h"

#define ARRAYSIZE 5


typedef struct {
    unsigned line;
    char *name;
    int numOfParam;		
} FunIdent;

typedef struct {
    FunIdent *arrayI;
    size_t used;
    size_t size;
} CArray;//array of undefined functions that were called function definition


void Semantic2ndDefControl();
void SemanticFullCondControl(SymTablePtr currTable, TokenPtr token);
bool SemanticExprAssignCotrol (SymTablePtr currTable, TokenPtr token);
void SemanticExpAssignReset ();
bool SemanticVarNameAssignControl (TokenPtr token, char *name);
bool SemanticFunNameDefControl(TokenPtr token, char *name);
void SemanticFunNameCallControl(SymTablePtr currTable, TokenPtr token, char *name, int block);
void SemanticFunParamControl(SymTablePtr currTable, TokenPtr token);
void SemanticNoMoreParam(TokenPtr token);
void SemanticNoParamControl(TokenPtr token);


#endif
