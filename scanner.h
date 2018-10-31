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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    EOL=0,//OK        //ok
    EOFile=1,         //ok
    ID=2,             //
    STRING=3,         //nok but little ok
    INT=4,            //
    FLOAT=5,          //
    NIL=6,            //
    //SYMBOLS
    LEFT_B=7,//(      // ok
    RIGHT_B=8,//)     //ok
    PLUS=9,           //ok
    MINUS=10,         //ok
    MULTIPLY=11,      //ok
    DIVISION=12,// /  //
    LESS=13,//<       //ok
    MORE=14,//>       //ok
    LESSEQ=15,//<=    //ok
    MOREEQ=16,//>=    //ok
    EQ=17,//==        //ok
    ADDITION=18, //=  //ok
    NOTEQ=19, // !=   //ok
    //keywords
    DEF=20,           //ok
    DO=21,            //ok
    ELSE=22,          //ok
    END=23,           //ok
    IF=24,            //ok
    NOT=25,           //ok
    THEN=26,          //ok
    WHILE=27,         //ok
    COMA=28,//,       //ok
    FUNCTION=29,//    //
    NEXT=30, //for some functions only in lex. analyser
//    QUOTMARK,//"

    //EOL_ENUM,//HOW MUCH ITEMS IS THERE FOR TESTING
	PROBLEM = -1
} lexems;
lexems ScannerGetLex(FILE *sourceCode);
int ScannerTestKeyWord(FILE *sourceCode);
/*Structure for work with strings*/
typedef struct {
	lexems lexem;   // type of token
	unsigned line;  // number of line
	char *name;   // if it is ID, contains it's name, if it is  number it's a value. The same with a string.
} Token;
typedef Token *TokenPtr;

bool Scanner(FILE *source_code);
/*static*/

/*static*/
int ScannerTestWord(char *str, FILE *sourceCode);
TokenPtr ScannerGetToken();

#endif
