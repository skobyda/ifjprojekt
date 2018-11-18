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
    EOFILE=1,         //ok
    IDENT=2,             //
    STR=3,         //nok but little ok
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
    NEXT=29, //for some functions only in lex. analyser
//    QUOTMARK,//"

    //EOL_ENUM,//HOW MUCH ITEMS IS THERE FOR TESTING
	PROBLEML = -1,//PROBLEM WITH LEXEM
  PROBLEMM=-2,//PROBLEM WITH MEMORY
  PROBLEMC=-3//problem with char operations
} lexems;
typedef enum{
START=0,//ok
IDKEY,//ok
ID,//ok
NEWLINE,//ok
LCOMMENT,//ok
LCOMMENTEND,//ok
STRING,//ok
CHARCODEF,//ok
CHARCODEL,//ok
BCOMMENT,//ok
BIGGER,//ok
SMALLER,//ok
EQUAL,//ok
NEQ,//ok
DOUBLECOM,//ok
NUMBER=15,//ok
DEXP,//ok
DEXPD,//ok
DEXPS,//ok
DCOMD,
PROBLEM,
ENDFILE,
}states;
//lexems ScannerGetLex(FILE *sourceCode);
char* ScannerStradd(char* s,char *c);
int ConvertHextoDec(char c);
int ScannerTestW(char*str);
int ScannerTestKeyWord();
int ScannerSkipLineE();
/*Structure for work with strings*/
typedef struct {//DANGER SHOULD NOT BE IN STRINGPTR []?
	lexems lexem;   // type of token
	unsigned line;  // number of line
	char *name;   // if it is ID, contains it's name, if it is  number it's a value. The same with a string.
} Token;
typedef Token *TokenPtr;
int ScannerSaveNew(TokenPtr token,int lines, char*c);

bool Scanner();
/*static*/

/*static*/
int ScannerTestWord(char *str);
TokenPtr ScannerGetToken();

#endif
