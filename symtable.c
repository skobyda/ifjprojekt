/* IFJ Project 2018/19
 * Implementation of Symbol Table using Hash Table
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: symtable.c
 *  FILE AUTHOR: Simon Kobyda
 */

/***SYSTEM FILES***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/***LOCAL FILES***/
#include "symtable.h"

SymTablePtr globalTable = NULL; //table contains all identifiers of functions

static bool SymTableInsertFunctions(SymTablePtr table) {
    char* functions[8];
    functions[0] = malloc(sizeof(char) * 7);
    functions[1] = malloc(sizeof(char) * 7);
    functions[2] = malloc(sizeof(char) * 7);
    functions[3] = malloc(sizeof(char) * 7);
    functions[4] = malloc(sizeof(char) * 7);
    functions[5] = malloc(sizeof(char) * 7);
    functions[6] = malloc(sizeof(char) * 7);
    functions[7] = malloc(sizeof(char) * 7);

    strcpy (functions[0], "print");
    strcpy (functions[1], "inputs");
    strcpy (functions[2], "inputi");
    strcpy (functions[3], "inputf");
    strcpy (functions[4], "substr");
    strcpy (functions[5], "length");
    strcpy (functions[6], "ord");
    strcpy (functions[7], "chr");

    for (int i = 0; i < 8; i++) {
        SymbolPtr symbol = malloc(sizeof(struct Symbol));
        if (!symbol) {
            printf("ERROR: malloc of symbol\n");
            return false;
        }
        symbol->name = functions[i];
        symbol->nextSymbol = NULL;
        symbol->iType = FUNCTION;
        switch (i) {
            case 0:  
                symbol->numOfParameters = -1;
                break;
            case 1: case 2: case 3:
                symbol->numOfParameters = 0;
                break;
            case 4:
                symbol->numOfParameters = 3;
                break;
            case 5:
                symbol->numOfParameters = 1;
                break;
            case 6:
                symbol->numOfParameters = 2;
                break;
            case 7:
                symbol->numOfParameters = 1;
                break;
            default:
                break;
        }
        SymTableAdd(table, symbol);
    }

    return true;
}

static unsigned int HashFunction(char* key) {
    int val = 0;
    for (unsigned int i = 0; i < strlen(key); i++)
        val += key[i];

    return val % TABLESIZE;
}

SymTablePtr SymTableInit(SymTablePtr ParentTable) {
    SymTablePtr table = NULL;
    table = malloc(sizeof(struct SymTable));
    if (!table) {
        fprintf(stderr, "SymTableInit: Allocation of symTable failed");
        exit(1);
    }
    table->parentTable = ParentTable;
    for (int i = 0; i < TABLESIZE; i++)
        table->arr[i] = NULL;

    if(SymTableInsertFunctions(table))
        return table;
    else
        return NULL;
}

void SymTableDestroy(SymTablePtr table) {
    for (int i = 0; i < TABLESIZE; i++) {
        SymbolPtr symbol = table->arr[i];
        while(symbol) {
            SymbolPtr tmp = symbol;
            symbol = tmp->nextSymbol;
            free(tmp->name);
            free(tmp);
        }
    }
    free(table);
}

void SymTableAdd(SymTablePtr table, SymbolPtr symbol) {
    if (!symbol) {
        fprintf(stderr, "SymTableAdd symbol: symbol passed as argument is NULL");
        exit(1);
    }

    if (!table) {
        fprintf(stderr, "SymTableAddSymbol: table passed as argument is NULL");
        exit(1);
    }

    unsigned int index = HashFunction(symbol->name);
    SymbolPtr tmp = table->arr[index];

    if (!tmp) {
        table->arr[index] = symbol;
    } else {
        while(tmp->nextSymbol)
            tmp = tmp->nextSymbol;

        tmp->nextSymbol = symbol;
    }

}

SymbolPtr SymTableFind(SymTablePtr table, char *name) {
    if (!name) {
        fprintf(stderr, "SymTableFind: string passed as argument is NULL");
        exit(1);
    }

    unsigned int index = HashFunction(name);
    SymbolPtr symbol = table->arr[index];

    while (symbol) {
        if (!strcmp(symbol->name, name))
            break;
        symbol = symbol->nextSymbol;
    }

    return symbol;
}

