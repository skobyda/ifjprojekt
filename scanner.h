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
	lexems lexem;   // type of token
	unsigned line;  // number of line
	string value;   // if it is ID, contains it's name, if it is  number it's a value. The same with a string.
} Token;
typedef Token *TokenPtr;

bool Scanner(FILE *source_code);

TokenPtr ScannerGetToken();

#endif
