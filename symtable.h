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

#define TABLESIZE 100

typedef struct Symbol *SymbolPtr;
typedef struct SymTable *SymTablePtr;

struct Symbol{
    char* name;
    int type;
    unsigned int length;
    bool declared;
    int adress;
    bool used;
    SymbolPtr nextSymbol;
};

struct SymTable{
    SymbolPtr arr[TABLESIZE];
};

SymTablePtr SymTableInit();
void SymTableDestroy(SymTablePtr table);
void SymTableAdd(SymTablePtr table, SymbolPtr symbol);
SymbolPtr SymTableFind(SymTablePtr table, char* name);

#endif
