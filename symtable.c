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

static unsigned int HashFunction(char* key) {
    int val;
    for (unsigned int i = 0; i < strlen(key); i++)
        val += key[i];

    return val % TABLESIZE;
}

SymTablePtr SymTableInit() {
    SymTablePtr table = NULL;
    table = malloc(sizeof(struct SymTable));
    if (!table) {
        fprintf(stderr, "SymTableInit: Allocation of symTable failed");
        exit(1);
    }

    return table;
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
}

void SymTableAddSymbol(SymTablePtr table, SymbolPtr symbol) {
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

    if (tmp) {
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

