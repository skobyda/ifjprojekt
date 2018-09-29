/* IFJ Project 2018/19
 * Implementation of Symbol Table using Hash Table
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: symtable.h
 *  FILE AUTHOR: Simon Kobyda
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

/***SYSTEM FILES***/
#include <stdbool.h>

typedef struct {
    char *symbolName; //string
    int type; //TODO enumerate types
    int scope; //TODO enumerate scopes
} Symbol;

typedef struct {
    Symbol *symbols; //array
    //TODO
} SymTable;

typedef Symbol *SymbolPtr;
typedef SymTable *SymTablePtr;

SymTablePtr SymTableInit();
void SymTableDestroy(SymTablePtr table);
bool SymTableAddSymbol(SymTablePtr table, SymbolPtr symbol);
bool SymTableRemoveSymbol(SymTablePtr table, SymbolPtr symbol);

#endif
