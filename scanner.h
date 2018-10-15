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
	lexems lexem;   // type of the read word
	unsigned line;  // line counter
	string value;   // if an ID was found, contains it's name, if it was a number it has it's value. The same with a string.
} Token;
typedef Token *TokenPtr;

bool Scanner(FILE *source_code);

TokenPtr ScannerGetToken();

#endif
