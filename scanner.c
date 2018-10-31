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

#include <stdbool.h>

/***LOCAL FILES***/
#include "../scanner.h"

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
void ScannerWhite(FILE *sourceCode)
{
  char c=0;
  do{
  c=(char)fgetc(sourceCode);}
  while(c==' ');
  ungetc( c,  sourceCode);
  return;
}

/*
 * Returns: The next token.
 * If there are no more tokens, returns NULL
 */
 int ScannerTestKeyWord(FILE *sourceCode)
 {int res;
  ScannerWhite(sourceCode);
  res=ScannerTestWord("def ",sourceCode);
  if(res==1)return 20;
  res=ScannerTestWord("do ",sourceCode);
  if(res==1)return 21;
  res=ScannerTestWord("else ",sourceCode);
  if(res==1)return 22;
  res=ScannerTestWord("end ",sourceCode);
  if(res==1)return 23;
  res=ScannerTestWord("if ",sourceCode);
  if(res==1)return 24;
  res=ScannerTestWord("not ",sourceCode);
  if(res==1)return 25;
  res=ScannerTestWord("then ",sourceCode);
  if(res==1)return 26;
  res=ScannerTestWord("while ",sourceCode);
  if(res==1)return 27;
  return 0;//!!!ATTENTION HERE IS RETURN 0
 }
/*static*/ int ScannerTestWord(char *str, FILE *sourceCode)
  {
      ScannerWhite(sourceCode);
    char cmp;
    int lenght1=strlen(str);
    if(str==NULL)
      return -1;
    int step=0;
    while(step!=lenght1){
      cmp=(char)fgetc(sourceCode);
      if(cmp==str[step])
        step++;
      else {
        ungetc(cmp,sourceCode);
        while(step-1)
          ungetc(str[step-1],sourceCode);
          step--;
        return 0;
      }
    }
  return 1;

  }
/* static*/ lexems ScannerGetLex(FILE *sourceCode) {
   ScannerWhite(sourceCode);
   while(1)
     {char c;
       c=(char)fgetc(sourceCode);
       switch(c) {
        case '=': {
        int res=ScannerTestWord("begin ",sourceCode);
        if(res==1)
          {
            int state=0;
            while(state!=2)
            {
              if(ScannerTestWord("end ",sourceCode)==1 && state==1)
                state++;
              else
                state=0;
              while(state!=1)
              {
                c=(char)fgetc(sourceCode);
                if(c=='=')
                  state++;
              }
            }
            if(state==2) continue;

          }

        char next=(char)fgetc(sourceCode);
        if(next=='=')
          return EQ;//==
        else {
          ungetc( next,  sourceCode);
          return ADDITION;//=
          }
        }

        case '!': {
          char next=(char)fgetc(sourceCode);
          if(next=='=')
            return NOTEQ;//!=
          else
            return -1;//CHYBA
        }

        case '+':
          return PLUS;

        case '-':
          return MINUS;

        case '*':
          return MULTIPLY;

        case '/': {
          char next=(char)fgetc(sourceCode);
          if(next !=' ' || next <'1' || next>'9'){
          ungetc( next,  sourceCode);
            return -1;//CHYBA
          }
          return DIVISION ;//DIVISION
        }

        case '\n':
          return EOL;

        case ',':
          return COMA;
        case '(':
          return LEFT_B;
        case ')':
          return RIGHT_B;
        case '<': {
          char next=(char)fgetc(sourceCode);
          if(next=='='){
            return LESSEQ;
          }
          else{
            ungetc( next,  sourceCode);
            return LESS;
          }

        case '>': {
          char next=(char)fgetc(sourceCode);
          if(next=='='){
            return MOREEQ;
          }
          else {
          ungetc( next,  sourceCode);
            return MORE;
          }
        }
        case ' ':
          continue;
        case EOF:
          return EOFile;
        case '#': {//COMMENT
          char next=0;
          while(next!='\n')
            c=(char)fgetc(sourceCode);
          continue;
          }
        case '"':{return STRING;//!!!!!!!!!!!!!!!!ATTENTION!!!!!!!!!!!! U must know that in scanner get token and u should get the string there to the  " " "
        }


        default : {
          //ocividne to bude cosi ine ako jedno/dvoj znakove debiliny
      ungetc( c,  sourceCode);
          return PROBLEM;

         }
}}
}

}


TokenPtr
ScannerGetToken(FILE *sourceCode) {
    TokenPtr Token = NULL;

int token=ScannerGetLex(sourceCode);

if(token==NEXT)
  return NULL;






    //TODO

    return Token;
}
