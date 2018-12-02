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
 *  FILE AUTHOR: Simon Kobyda, Denis Filo
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

/***SYSTEM FILES***/
#include <stdbool.h>

#define TABLESIZE 100

typedef struct Symbol *SymbolPtr;
typedef struct SymTable *SymTablePtr;

extern SymTablePtr globalTable;

typedef enum {
    VARIABLE,
    FUNCTION,
} identifType;


typedef enum {
    //typeNone,
    //typeBool,
    typeUnknown,
    typeNumeric,
    typeString,
} dataType;
/*TODO REMOVE LATER
typedef union {
    bool bVal;
    int iVal;
    float fVal;
    char *sVal;
} Value; */

struct Symbol{
    char* name;
    identifType iType;
    dataType dType;//contains last assigned data type, only for variables
    unsigned numOfParameters;
    // bool defined; // TODO REMOVE LATER
    SymbolPtr nextSymbol;
};

struct SymTable{
    SymbolPtr arr[TABLESIZE];
    SymTablePtr parentTable; // if NULL, current table is global table
};

SymTablePtr SymTableInit(SymTablePtr ParentTable);
void SymTableDestroy(SymTablePtr table);
void SymTableAdd(SymTablePtr table, SymbolPtr symbol);
SymbolPtr SymTableFind(SymTablePtr table, char* name);

#endif
