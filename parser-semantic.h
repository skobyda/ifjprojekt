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
} FunIdent;

typedef struct {
    FunIdent *arrayI;
    size_t used;
    size_t size;
} CArray;

void SemanticInitArray (CArray *a, size_t initSize);
void SemanticInsertArray (CArray *a, unsigned line, char *name);
void freeArray (CArray *a);

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



/* Runs Semantic analysis over derivation tree.
 * throws some error if semantic bug was detected.
 * @tree Pointer to derivation tree.
 * Returns: true in case of success, false in case of error
 */
bool ParserSemantic(ParTreePtr tree);

#endif
