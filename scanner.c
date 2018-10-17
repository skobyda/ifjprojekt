/* IFJ Project 2018/19
 * Implementation of Lexical analyser
 *
 * TEAM:
 *  Simon Kobyda
 *  Michal Zelenak
 *  Juraj Samuel Salon
 *  Denis Filo
 *
 * FILE: scanner.c
 *  FILE AUTHOR:
 */

/***SYSTEM FILES***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/***LOCAL FILES***/
#include "scanner.h"

/* Firstly, file with source code is passed to Scanner
 *
 * @sourceCode File with code written in IFJ18 language
 *
 * Returns: true in case of success, false in case of error
 */
bool
Scanner(FILE *sourceCode) {
    (void)(sourceCode); //REMOVE THIS LINE

    //TODO

    return true;
}


/*
 * Returns: The next token.
 * If there are no more tokens, returns NULL
 */

 static lexems ScannerGetLex(FILE *sourceCode) {
   while(1)
     {char c;
       c=getchar();
       switch(c)
{
        case '=': {
          char next=getchar();
          if(next=='=')
            return 25;//==
          else {
          ungetc( next,  sourceCode);
            return 26;//=
          }
        }

        case '!': {
          char next=getchar();
          if(next=='=')
            return 27;//!=
          else
            return -1;//CHYBA
        }

        case '+':
          return 9;

        case '-':
          return 10;

        case '*':
          return 20;

        case '/': {
          char next=getchar();
          if(next !=' ' || next <'1' || next>'9'){
          ungetc( next,  sourceCode);
            return -1;//CHYBA
          }
          return 21 ;//DIVISION
        }

        case '\n':
          return 0;

        case ',':
          return 17;
        case '(':
          return 7;
        case ')':
          return 8;
        case '<': {
          char next=getchar();
          if(next=='='){
            return 23;
          }
          else{
            ungetc( next,  sourceCode);
            return 21;
          }

        case '>': {
          char next=getchar();
          if(next=='='){
            return 24;
          }
          else {
          ungetc( next,  sourceCode);
            return 22;
          }
        }
        case ' ':
          continue;
        case EOF:
          return 1;
        default : {
          //ocividne to bude cosi ine ako jedno/dvoj znakove debiliny
      //    ungetchar();
          return 29;

         }
}}
}

}


TokenPtr
ScannerGetToken() {
    TokenPtr Token = NULL;










    //TODO

    return Token;
}
