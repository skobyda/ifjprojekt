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
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    EOL=0,
    EOFile,
    ID,
    STRING,
    INT,
    FLOAT,
    NIL,
    //SYMBOLS
    LEFT_B,//(
    RIGHT_B,//)
    PLUS,
    MINUS,
    //keywords
    DEF,
    DO,
    ELSE,
    END,
    IF,
    NOT,
    SEMICOL,//SEMICOLON ';'
    COMA,//,
    THEN,
    WHILE,
    MULTIPLY,
    DIVISION,// /
    LESS,//<
    MORE,//>
    LESSEQ,//<=
    MOREEQ,//>=
    EQ,//==
    ADDITION, //=
    NOTEQ, // !=
    FUNCTION,//
    QUOTMARK,//"
    EOL_ENUM,//HOW MUCH ITEMS IS THERE FOR TESTING
	PROBLEM = -1
} lexems;

/*Structure for work with strings*/
typedef struct {
	lexems lexem;   // type of token
	unsigned line;  // number of line
	char *name;   // if it is ID, contains it's name, if it is  number it's a value. The same with a string.
} Token;
typedef Token *TokenPtr;

bool Scanner(FILE *source_code);

TokenPtr ScannerGetToken();

#endif
