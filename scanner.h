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
    EOL=0,//OK        //token for end of the line
    EOFILE=1,         //token for end of the file
    IDENT=2,          //token for identifier of function or identifier     abc,
    STR=3,            //token for string value
    INT=4,            //token for integer value
    FLOAT=5,          //token for float value
    NIL=6,            //token for nil
    //SYMBOLS
    LEFT_B=7,//(      //token for left bracket  (
    RIGHT_B=8,//)     //token for right bracket )
    PLUS=9,           //token for sign PLUS     +
    MINUS=10,         //token for sign MINUS    -
    MULTIPLY=11,      //token for sign MULTIPLY *
    DIVISION=12,// /  //token for sign DIVISION /
    LESS=13,//<       //token for sign LESS     <
    MORE=14,//>       //token for sign MORE     >
    LESSEQ=15,//<=    //token for sign lessequal <=
    MOREEQ=16,//>=    //token for sign moreequal >=
    EQ=17,//==        //token for sign EQUAL    ==
    ADDITION=18, //=  //token for ADDITION      =
    NOTEQ=19, // !=   //token for NOTEQ         !=
    //keywords
    DEF=20,           //token for keyword DEF
    DO=21,            //token for keyword DO
    ELSE=22,          //token for keyword ELSE
    END=23,           //token for keyword END
    IF=24,            //token for keyword IF
    NOT=25,           //token for keyword NOT
    THEN=26,          //token for keyword THEN
    WHILE=27,         //token for keyword WHILE
    COMA=28,//,       //token for COMA
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
ZERO,
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
