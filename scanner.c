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
int ScannerSkipLineE(FILE * sourceCode)
  {char c;
    while(c!='\n')
     c=(char)fgetc(sourceCode);
     if(c==EOF){
       return 1;
     }
    ungetc(c,sourceCode);
    return 0;
  }
void ScannerWhite(FILE *sourceCode)
{
  char c=0;
  do{
  c=(char)fgetc(sourceCode);
  }
  while(c==' ');
  ungetc( c,  sourceCode);
  return;
}

int ConvertHextoDec(char c)
{
  switch(c){
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'A':
      return 10;
    case 'B':
      return 11;
    case 'C':
      return 12;
    case 'D':
      return 13;
    case 'E':
      return 14;
    case 'F':
      return 15;
    default:
      return 16;
    }
}

//////////////////////
char *ScannerStradd( char *s, char* c )
  {
  char *s2;
  int length= strlen(s);
  s2=(char*)realloc(s,(length+2)*sizeof(char));
  if(!s2) {
    free(s);
    return NULL;
    }
  s=s2;
  s[length]=*c;
  s[length+1]=0;
  return s;
  }

int ScannerSaveNew(TokenPtr token,FILE *source,int lines,char* c)//UPRAV TO NA  int NAVRAT NA KONTROLU EOF
{
  if(ScannerStradd((token)->stringPtr,c)==0)//NOT SUCESFULl, returned value not used
    {(token)->lexem=PROBLEMC;
    (token)->line=lines;
    return 0;}
    (void)(source);
  return 1;
}
int ScannerTestKeyWord(FILE *sourceCode)
 {int res;
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
  return 0;
 }

int ScannerTestWord(char *str, FILE *sourceCode)
  {char c;
    c=fgetc(sourceCode);
    ungetc(c,sourceCode);
    if(c==' ')
      return 0;
      if(str==NULL)
        return -1;
    int lenght1=strlen(str);
    char cmp[lenght1+1];
    int step=0;
    while(step!=lenght1){
      cmp[step]=fgetc(sourceCode);
      if(cmp[step]==EOF){
      }
      while(step==0 && cmp[step]==' ')
      cmp[step]=fgetc(sourceCode);
      if(cmp[step]==EOF){
      }
      if(cmp[step]==EOF)return 0;
      if(cmp[step]==str[step])
        step++;
      else if((cmp[step]=='\n' || cmp[step]=='#' || cmp[step]==' ')&& (step==lenght1-1)){
          if(cmp[step]=='\n')
            {
              ungetc(cmp[step],sourceCode);
            }
          return 1;
      }
      else {
        for(int i=0;i<=step;step--)
          {
            ungetc(cmp[step],sourceCode);
          }
        return 0;
      }
    }
  return 1;
  }

TokenPtr ScannerGetToken(FILE *sourceCode) {
  //getchar();
  static int n_lines=1;
  static int one=0;
  static states state=NEWLINE;
  static  char c;
  TokenPtr token=NULL;
  token=malloc(sizeof(Token));
  (token)->stringPtr=malloc(sizeof(char));
    if(token==NULL || (token)->stringPtr==NULL)
      {(token)->lexem=PROBLEMM;
      (token)->line=n_lines;
      return token;}
ScannerWhite(sourceCode);
if(one==0)
  c =(char)fgetc(sourceCode);
  if(c==EOF){
    token->lexem=EOFILE;
    token->line=n_lines;
    return token;
  }

while(1){
  switch(state){
    case START:
    {
      if(c=='#')
        {
          state=BCOMMENT;
          continue;
        }
      else if(c=='\n')
        {state=NEWLINE;
        (token)->lexem=EOL;
        (token)->line=n_lines;
        n_lines++;
        return token;
        }
      else if((c>='a' &&c<='z')|| c=='_')
        {state=IDKEY;
          continue;
        }
      else if(c>='0' && c<='9')
        {if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
          state=NUMBER;
        continue;}
      else if(c=='+')
        {(token)->lexem=PLUS;
        (token)->line=n_lines;
        return token;
        }
      else if(c=='-')
        {(token)->lexem=MINUS;
        (token)->line=n_lines;
        return token;
        }
      else if(c=='*')
        {(token)->lexem=MULTIPLY;
        (token)->line=n_lines;
        return token;
        }
      else if(c==' ')
        {
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;
        }
      else if(c=='/')
        {(token)->lexem=DIVISION;
        (token)->line=n_lines;
        return token;
        }
      else if(c==')')
        {(token)->lexem=RIGHT_B;
        (token)->line=n_lines;
        return token;
        }
      else if(c=='(')
        {(token)->lexem=LEFT_B;
        (token)->line=n_lines;
        return token;
        }
      else if(c=='>')
        {state=BIGGER;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
        }
      else if(c=='<')
        {state=SMALLER;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
        }
      else if(c=='!')
        {
          state=NEQ;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
        }
      else if(c=='=')
        {if(ScannerTestWord("begin ",sourceCode)==1)
          {state=PROBLEM;
          continue;}
        state=EQUAL;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
        }
      else if(c=='\"')
        {
        state=STRING;
        (token)->lexem=STR;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
        }
      else {
        {(token)->lexem=PROBLEML;
        (token)->line=n_lines;
        return token;}
      }
    }
    case IDKEY:
      {
      int res=ScannerTestKeyWord(sourceCode);
      if(res!=0)
        {(token)->lexem=res;
        (token)->line=n_lines;
        state=START;
        return token;
        }
      if(c==' ' || c=='!' || c=='?')
        {
        (token)->lexem=IDENT;
        (token)->line=n_lines;
        if(c!=' ')
          if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
        if(c==' ')
          ungetc(c,sourceCode);
        state=START;
        return token;
      }
      if((c>='A'&& c<='Z')||((c>='a')&& (c<='z'))||(c>='0'&& c<='9')||c=='_')
        {
        state=ID;
        if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
        }
      else
      {
          state=PROBLEML;
          continue;
      }
      }
    case ID:
      {
      (token)->lexem=IDENT;
      (token)->line=n_lines;
        if(c==' '||c=='?'|| c=='!')
            {
            if((c=='?'|| c=='!' )&&( c!=' '))
              if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            if(c==' ')ungetc(c,sourceCode);
          state=START;
          return token;
          }
        else if(((c>='a') && (c<='z'))||((c>='A') && (c<='Z'))||((c>='0') && (c<='9')))
                {
                if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
                c=(char)fgetc(sourceCode);
                continue;
                }
        else if((c==' ')||(c=='\n')||(c=='#')||(c=='(')||(c=='=')||(c=='!')||(c=='>')||(c=='<')||(c==')')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){
          ungetc(c,sourceCode);
          state =START;
          return token;
        }
      else{
        state=PROBLEM;
        continue;
      }
      }
    case ENDFILE:
    {
      token->lexem=EOFILE;
      token->line=n_lines;
      return token;  }
    case NEWLINE:

      {if(one==1)one=0;
        if(c=='\n')
          {
          state=NEWLINE;
          n_lines++;
          (token)->lexem=EOL;
          (token)->line=n_lines-1;
          return token;
          }
        else if((c>='a' &&c<='z')|| c=='_')
          {
            state=IDKEY;
          continue;
          }
        else if(c=='#')
          {
            state=BCOMMENT;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            continue;
          }
        else if(c=='=')
        {
          if(ScannerTestWord("begin ",sourceCode)==1)
          {state=LCOMMENT;
          continue;}
          else{state=PROBLEM;
              continue;}
        }
        else
        {
        state=PROBLEM;
        continue;
        }
    }
    case LCOMMENT:
      {
        if(c=='\n')
          {n_lines++;
            state=LCOMMENTEND;}
        c=(char)fgetc(sourceCode);
        if(c==EOF){
          token->lexem=EOFILE;
          token->line=n_lines;
          return token;
        }
        continue;
      }
    case LCOMMENTEND:
      {ScannerWhite(sourceCode);
      if(c=='=')
        {if(ScannerTestWord("end ",sourceCode)==1)
          {
          state=START;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;
          }
        }
      else
        state=LCOMMENT;
      }
    case STRING:
      {
        if(c!='\"' && c!='\\')
          {
          if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;
          }
        else if(c=='\"')
          {
            (token)->lexem=STR;
            (token)->line=n_lines;
            state=START;
            return token;
          }
        else
          {
          state=CHARCODEF;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;
        }
      }
    case CHARCODEF:
      {char code[2];
        if(c=='s')
          {
            c=' ';
            if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            state=STRING;
            continue;
          }
        if(c=='\\' || c=='\"')
          {
            if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            state=STRING;
            continue;
          }
        if(c=='n')
          {
            c='\n';
            if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            state=STRING;
            continue;
          }
        if(c=='t')
          {
          c='\t';
          if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          state=STRING;
          continue;
          }
        if(c=='x')
          {
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            if((c>='0' && c<='9')||(c>='A'&&c<='F'))
              code[0]=c;
            else
              {
                code[0]='\\';
                code[1]='x';
                if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
                if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
                if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
                state=STRING;
                continue;
              }
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            if((c>='0' && c<='9')||(c>='A'&&c<='F'))//convert 0.1
                {
                code[1]=c;
                int a=ConvertHextoDec(code[0])*16+ConvertHextoDec(code[1]);
                char b=a;
                if(ScannerSaveNew(token,sourceCode,n_lines,&b)==0)return token;
                state=STRING;
                c=(char)fgetc(sourceCode);
                if(c==EOF){
                  token->lexem=EOFILE;
                  token->line=n_lines;
                  return token;
                }
                continue;
                }
            else{//convert 0 , c vratit
                int a=ConvertHextoDec(code[0]);
                char b=a;
                if(ScannerSaveNew(token,sourceCode,n_lines,&b)==0)return token;
                if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
                state=STRING;
                continue;
            }
          }
      }
    case BCOMMENT:
      {
        while(c!='\n')
          {c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
          }
        n_lines++;
        token->lexem=EOL;
        token->line=n_lines-1;
        state=START;
        return token;
      }
    case BIGGER:
      {
      if(c=='=')
        (token)->lexem=MOREEQ;
      else
        {(token)->lexem=MORE;
        ungetc(c,sourceCode);
        }
      (token)->line=n_lines;
      state=START;
      return token;
      }
    case SMALLER:
      {
        if(c=='=')
          (token)->lexem=LESSEQ;
        else
        {(token)->lexem=LESS;
        ungetc(c,sourceCode);
        }
      (token)->line=n_lines;
      state=START;
      return token;
      }
    case EQUAL:
      {

        if(c=='=')
          {(token)->lexem=EQ;
            state=START;}
        else {
          (token)->lexem=ADDITION;
          state=START;ungetc(c,sourceCode);}
        (token)->line=n_lines;
        return token;
      }
    case NEQ:
      {
          if(c=='=')
            (token)->lexem=NOTEQ;
          else
          {(token)->lexem=NOT;
          ungetc(c,sourceCode);
          }
        (token)->line=n_lines;
        state=START;
        return token;

        }
    case NUMBER:
      {
        if(c=='.')
          {state=DOUBLECOM;
          if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;}//next
        else if(c=='e'||c=='E')
          {state=DEXP;
          if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;}
        else if(c>='0' && c<='9')
          {
            if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            continue;}
        else if((c=='#')||(c==' ')|(c=='\n'))
          {
            (token)->lexem=INT;
            (token)->line=n_lines;
            state=START;
            ungetc(c,sourceCode);
            return token;
          }
        else{
            state=PROBLEM;
            continue;
        }
      }
    case DOUBLECOM:
      {
         if(c>='0'&&c<='9')
          {state=DCOMD;
          if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
          c=(char)fgetc(sourceCode);
          if(c==EOF){
            token->lexem=EOFILE;
            token->line=n_lines;
            return token;
          }
          continue;}
        else {

          state=PROBLEM;
          continue;
          }
      }
    case DEXP:
      {
        if((c=='+')||(c=='-'))
          {
            if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            state=DEXPS;
            continue;
          }
        if(c>='0'&&c<='9')
          {
            state=DEXPD;
            if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
            continue;
          }
          else{
            state=PROBLEM;
            continue;
          }
      }
    case DEXPD:
      {
        if(c>='0'&& c<='9')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
          continue;}
        else if((c=='#')||(c=='\n')||(c=='\n')||(c==' ')) {
          (token)->lexem=FLOAT;
          (token)->line=n_lines;
          ungetc(c,sourceCode);
          state=START;
          return token;
        }
        else{
          state=PROBLEM;
          continue;
          }
      }
    case DEXPS:
      {
        if(c>='0'&& c<='9')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
          continue;}
        else if((c=='#')||(c=='\n')||(c=='\n')||(c==' ')) {
          (token)->lexem=FLOAT;
          (token)->line=n_lines;
          ungetc(c,sourceCode);
          state=START;
          return token;
        }
        else{
          state=PROBLEM;
          continue;
        }
      }
    case DCOMD:
      {
        if(c=='e'||c=='E')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
          continue;
          }
        if(c<='9'&&c>='0')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c)==0)return token;
            c=(char)fgetc(sourceCode);
            if(c==EOF){
              token->lexem=EOFILE;
              token->line=n_lines;
              return token;
            }
          continue;}
        else if((c=='#')||(c=='\n')||(c=='\n')||(c==' ')) {
          (token)->lexem=FLOAT;
          (token)->line=n_lines;
          ungetc(c,sourceCode);
          state=START;
          return token;
        }
        else{
          state=PROBLEM;
          continue;
        }
      }
    case PROBLEM:
    {
      if(ScannerSkipLineE(sourceCode)==1)
        {state=END;
        continue;}
      token->lexem=PROBLEML;
      token->line=n_lines;
      state=NEWLINE;
      return token;
    }
      default:
        {
          (token)->lexem=-1;
          (token)->line=n_lines;
          return token;
        }
      }
    }
 return token;//THIS IS OF ERROR THIS WILL NEVER BE USED
}
