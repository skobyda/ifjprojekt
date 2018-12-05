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
    IDENT=2,          //token for identifier of function or identifier     abc,ab_,ab_A,ab_A!,ab_A?
    STR=3,            //token for string value  "abcd\s\t\"\n\x[0-F][0-F],\x[0-f][0-f]"
    INT=4,            //token for integer value 5 15 165
    FLOAT=5,          //token for float value   1e0,1.0,1e+0,0e-0,1.0e-15
    NIL=6,            //token for nil           nil
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
    DEF=20,           //token for keyword DEF   def
    DO=21,            //token for keyword DO    do
    ELSE=22,          //token for keyword ELSE  else
    END=23,           //token for keyword END   end
    IF=24,            //token for keyword IF    if
    NOT=25,           //token for keyword NOT   not
    THEN=26,          //token for keyword THEN  then
    WHILE=27,         //token for keyword WHILE while
    COMA=28,//,       //token for COMA          ,
    NEXT=29, //for some functions only in lex. analyser
	PROBLEML = -1,//PROBLEM WITH LEXEM
    PROBLEMM=-2,//PROBLEM WITH MEMORY
    PROBLEMC=-3//problem with char operations
} lexems;//ENUMERATION OF TOKENS THAT WE CAN RECOGNIZE, AND SEND some of them

typedef enum{
START=0,    //state where automat will be every time where it will not be reading character after \n, so the first time is the first state newline
IDKEY,      //state for resolving if characters are keyword or not
ID,         //same as IDKEY, but there is also end state when ? or ! came
NEWLINE,    //FIRST STATE , and also state that is after every EOL
LCOMMENT,   //state when we get =begin
LCOMMENTEND,//state when we get new line and we are waitng for =end
STRING,     /*state when we get " and also after until we get \ */
CHARCODEF,  /*state when we are in string after*/
CHARCODEL,  //UNUSED state
BCOMMENT,   //when we get # and we are waiting for \n
BIGGER,     //when we get > and we are not sure if next character is =
SMALLER,    //when we get < and we are not sure if next character is =
EQUAL,      //when we get = and we are not sure if next character is =
NEQ,        //when we get ! and we are not sure if next character is =
DOUBLECOM,  //when we get number and .,and we are expecting number there
NUMBER=15,  //when we get 0 or 1-9 and then 0-9 as many times as we can
DEXP,       //whe we in state number get exponent(e/E),and we are waiting for +/- or 0-9
DEXPD,      //whe we get number exponent(e/E) number, and we can get another number to the after exponent(e/E) part
DEXPS,      //when we get +/- and we are waitng number from range 0-9
DCOMD,      //when we get number point number and we can get another character from range 0-9
ZERO,       //when we get zero "0", and we can get exponent(e/E), or point "."
PROBLEM,    //when we found some problem, we will skip the rest of the line
ENDFILE,    //when we once find EOF, we will save the state ENDFILE and next time someone call function ScannerGetToken we will send null
}states;    //STATES OF OUR FINITE STATE MACHINGE THAT IS REPRESENTING OUR LEXICAL ANALYSER

//
char* ScannerStradd(char* s,char *c);

//Function for converting Hexadecimal value to decimal value
int ConvertHextoDec(char c);

//Function that test if str will not immediately came from file by reading it
int ScannerTestW(char*str);

//Function that skips rest of the line after some problems or =begin or =end
int ScannerSkipLineE();

/*Structure for Token, token is the structure we are sending back to parser*/
typedef struct {
	lexems lexem;   // type of token
	unsigned line;  // number of line where we found the token
	char *name;   // if the token is ID, name of the ID is here, if the token is int or float, there is same format we get, and if the token is string there is the string
} Token;

typedef Token *TokenPtr;//Pointer to Token, because parser will free our space THEN

//Basicaly calling function ScannerStradd and check if everything is okay
int ScannerSaveNew(TokenPtr token,int lines, char*c);

//Function that test if some of the keywords are not string we have
int ScannerTestWord(char *str);

//Fuction that use all of above functions, and the function that reads the file and sending the tokens,
TokenPtr ScannerGetToken();
#endif
