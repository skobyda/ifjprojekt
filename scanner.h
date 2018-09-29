/* IFJ Project 2018/19
 * Implementation of Lexical analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: scanner.h
 *  FILE AUTHOR:
 */

#ifndef SCANNER_H
#define SCANNER_H

/***SYSTEM FILES***/
#include <stdbool.h>

typedef struct {
    int a; //REMOVE THIS LINE
    //TODO
} Token;
typedef Token *TokenPtr;

bool Scanner(FILE *source_code);

TokenPtr ScannerGetToken();

#endif
