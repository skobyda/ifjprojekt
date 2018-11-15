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
void ScannerSkipLineE(FILE * sourceCode)
  {char c;
    while(c!='\n')
     c=(char)fgetc(sourceCode);
    ungetc(c,sourceCode);
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
    // printf("fail is here\n");
    // printf("%s , char je %c",s,c);
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
  //*c='s';
  return s;
  }

////////////////////////


int ScannerSaveNew(TokenPtr token,FILE *source,int lines,char* c,int next)
{
//*c='x';
  if(ScannerStradd((token)->stringPtr,c)==0)//NOT SUCESFULl, returned value not used
    {(token)->lexem=PROBLEMC;
    (token)->line=lines;
    return 0;}
  if(next==1)
    {
      char x=(char)fgetc(source);
      *c=x;

    }
    (void)(c);
    //
  return 1;
}

/*
 * Returns: The next (token)->
 * If there are no more tokens, returns NULL
 */
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
  // for(int i=0;i<5; i++)
  // {char c=(char)fgetc(sourceCode);
  // printf("%c\n",c );}
  return 0;//!!!ATTENTION HERE IS RETURN 0
 }

int ScannerTestWord(char *str, FILE *sourceCode)
  {
     //printf("TUTOTOOOO %s\n",str);
    //  ScannerWhite(sourceCode);
      if(str==NULL)
        return -1;
    int lenght1=strlen(str);
    char cmp[lenght1+1];

    int step=0;
    while(step!=lenght1){
      //printf("step=%d,lenght=%d",step,lenght1);
    //  getchar();
      cmp[step]=fgetc(sourceCode);
      while(step==0 && cmp[step]==' ')
      cmp[step]=fgetc(sourceCode);
      if(cmp[step]==EOF)return 0;
    //  printf("dostal som : %c\n",cmp[step]);
    //  printf("step= %d, cmp= %s, str[step]=%c\n",step,cmp,str[step] );

      if(cmp[step]==str[step])
        step++;
      else if((cmp[step]=='\n' || cmp[step]=='#' || cmp[step]==' ')&& (step==lenght1-1)){
          if(cmp[step]=='\n')
            {
              ungetc(cmp[step],sourceCode);
            }
        //    printf("som vo else\n" );

          //free(cmp);
          return 1;


      }
      else {
    //    printf("som vo else\n" );
        for(int i=0;i<=step;step--)
          {
            ungetc(cmp[step],sourceCode);
          //  printf("2vraciam %c\n",cmp[step]);
          }
        //free(cmp);
        return 0;
      }
      //printf("cyclus \n" );

    }

  //free(cmp);
  return 1;
  }

TokenPtr ScannerGetToken(FILE *sourceCode) {
  getchar();
  static int n_lines=1;
  static int one=0;
  static states state=NEWLINE;
  static  char c;
  static char *chaPtr;
  chaPtr=malloc(sizeof(char));
//  lexems lexem=0;
  TokenPtr token=NULL;
  token=malloc(sizeof(Token));
  (token)->stringPtr=malloc(sizeof(char));
    if(token==NULL || (token)->stringPtr==NULL|| chaPtr==NULL)
      {(token)->lexem=PROBLEMM;
      (token)->line=n_lines;
      return token;}
ScannerWhite(sourceCode);
printf("%d\n",one);
if(one==0)
  c =(char)fgetc(sourceCode);
//if(one==1)one=0;
//  printf("PRAVE SOM DOSTAL %c state= %d\n",c,state);

while(1){
  switch(state){
    case START:
    {
      //printf("22PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
      //printf("!!!->%c<-!!!\n",c);
      //ScannerWhite(sourceCode);
      if(c=='#')
        {
          state=BCOMMENT;
          //c=(char)fgetc(sourceCode);
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
          ungetc(c,sourceCode);
        //if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)return token;
          continue;
        }
      else if(c>='0' && c<='9')
        {if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
        c=(char)fgetc(sourceCode);
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
        {//printf("here it si??\n");
          c=(char)fgetc(sourceCode);
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
        continue;
        }
      else if(c=='<')
        {state=SMALLER;
        c=(char)fgetc(sourceCode);
        continue;
        }
      else if(c=='!')
        {
          state=NEQ;
        c=(char)fgetc(sourceCode);
        continue;
        }
      else if(c=='=')
        {if(ScannerTestWord("begin ",sourceCode)==1)
          {state=PROBLEM;
          continue;}
        state=EQUAL;
        c=(char)fgetc(sourceCode);
        continue;
        }
      else if(c=='\"')
        {
        (token)->lexem=STR;
        c=(char)fgetc(sourceCode);
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
      int res=ScannerTestKeyWord(sourceCode);//biele miesta v tejto funkcii o3etrit
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
        if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)return token;

        if(c==' ')
        {ungetc(c,sourceCode);}
        state=START;
        return token;
        }
      if((c>='A'&& c<='Z')||((c>='a')&& (c<='z'))||(c>='0'&& c<='9')||c=='_')
        {//printf("hre i ammm\n");
        state=ID;
        //printf("IN IDKEY before %s,%c\n",token->stringPtr,c);
        if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)return token;
        c=(char)fgetc(sourceCode);
          //      printf("IN IDKEY after %s,%c\n",token->stringPtr,c);
        continue;
        }
      else
      {
        state=ID;//ZMENA
        continue;//ZMENA
      }
      }
    case ID:
      {//printf("HERE too\n");
        //printf("am i here\n" );
       printf("%s,%c\n",token->stringPtr,c);

        //printf("som v id\n" );

      (token)->lexem=IDENT;
      (token)->line=n_lines;
        if(c==' '||c=='?'|| c=='!' )
          {printf("nie je sance\n" );
            //printf("som v if\n" );
            if(c=='?'|| c=='!' )
              if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            if(c==' ')ungetc(c,sourceCode);
          state =START;
          return token;
          }
        else if(((c>='a') && (c<='z'))||((c>='A') && (c<='Z'))||((c>='0') && (c<='9')))
                {printf("am i fucking here\n" );
                //printf("v eleseif %s,%c\n",token->stringPtr,c);
                if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)
                //printf("v pici eleseif %s,%c\n",token->stringPtr,c);
                  return token;//TUNA VRATILO B?? PRECO???
                  continue;
                }
        else if((c==' ')||(c=='\n')||(c=='#')||(c=='(')||(c=='=')||(c=='!')||(c=='>')||(c=='<')||(c==')')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){
          printf("->%c<-\n",c );
          ungetc(c,sourceCode);
          printf("here %c\n",c );
          // printf("haaalooo %c",c);
          // for(int i=0;i<15; i++)
          // {char c=(char)fgetc(sourceCode);
          // printf("%c\n",c );}
      //  printf("som v else\n" );
          state =START;
          return token;
        }
      else{
        state=PROBLEM;//ZMENA
        continue;//ZMENA
      }
      }
    case NEWLINE:
    {if(one==1)one=0;
      //printf("mame newline state\n" );
        ScannerWhite(sourceCode);
        if(c=='\n')
          {//printf("tramtada je riadok\n" );
          state=NEWLINE;
          n_lines++;
          (token)->lexem=EOL;
          (token)->line=n_lines-1;
          return token;
          }
        else if((c>='a' &&c<='z')|| c=='_')
          {//printf("AZ TERAZ SOM V NEWLINE A MAM %c\n",c);
             ungetc(c,sourceCode);
          //   for(int i=0;i<14;i++)
          //     {printf("%c",fgetc(sourceCode));}
          //   printf("\n");
            state=IDKEY;
        //if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          continue;
          }

        else if(c=='#')
          {
            state=BCOMMENT;
            c=(char)fgetc(sourceCode);
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
        { //printf("here\n");
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
        continue;
      }
    case LCOMMENTEND:
      {ScannerWhite(sourceCode);
      if(c=='=')
        {if(ScannerTestWord("end ",sourceCode)==1)
          {
          state=START;
          c=(char)fgetc(sourceCode);
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
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)return token;
          c=(char)fgetc(sourceCode);
          continue;
          }
        else if(c=='\"')
          {
            (token)->lexem=STR;
            (token)->line=n_lines;
            return token;
          }
        else
          {
          state=CHARCODEF;
          c=(char)fgetc(sourceCode);
          if(c!='x')
            state=STRING;
          // else{char b='\\';if(ScannerSaveNew(token,sourceCode,n_lines,&b,0)==0)return token;if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)return token;if(ScannerCon(&token,n_lines,&b,0)==0) return token;if(ScannerCon(&token,n_lines,c,1)==0) return token;continue;}
          continue;
        }
      }
    case CHARCODEF:
      {
        char code[1];
        if((c>='0' && c<='9')||(c>='A'&&c<='Z'))
          code[0]=c;
        else
          {code[0]=c;
          c='\\';
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          c='x';
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          c=code[0];
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==0)return token;//1??
          state=STRING;
          continue;
          }
        if((c>='0' && c<='9')||(c>='A'&&c<='Z'))
          code[1]=c;
        else
          {int a=ConvertHextoDec(code[0]);
          char b=a;
          if(ScannerSaveNew(token,sourceCode,n_lines,&b,0)==1)return token;//next
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==1)return token;//next
          c=(char)fgetc(sourceCode);
          state=STRING;
          continue;
          }
        int a=ConvertHextoDec(code[0]);//code 0 is MSB 0-FF
        char b=a;
        if(ScannerSaveNew(token,sourceCode,n_lines,&b,0)==1)return token;//next
        if(ScannerSaveNew(token,sourceCode,n_lines,&c,1)==1)return token;//next
        c=(char)fgetc(sourceCode);
      }
    case BCOMMENT:
      {printf("am i here\n");
        while(c!='\n')
          {c=(char)fgetc(sourceCode);
            printf("dostal som: %c\n",c);
          }
        n_lines++;
        printf("am i here c is :%c\n<<<<<-",c);
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

          //printf("TUTUTUT %c\n ",c);
        (token)->line=n_lines;
        return token;
      }
    case NEQ:
      {//printf("hereee\n");

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
        //printf("1111PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
        if(c=='.')
          {state=DOUBLECOM;
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          c=(char)fgetc(sourceCode);
          continue;}//next
        else if(c=='e'||c=='E')
          {state=DEXP;
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          c=(char)fgetc(sourceCode);
          continue;}
        else if(c>='0' && c<='9')
          {//printf("22PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
            if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          //  printf("44PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
            c=(char)fgetc(sourceCode);
          //  printf("55PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
            continue;}
        else if((c=='#')||(c==' ')|(c=='\n'))
          {//printf("33PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
            //if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            //one=1;
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
      {//printf("1111PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
         if(c>='0'&&c<='9')
          {state=DCOMD;
          if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
          c=(char)fgetc(sourceCode);
          continue;}//next
        else {

          state=PROBLEM;
          continue;
          }
      }
    case DEXP:
      {
          // printf("PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
        if((c=='+')||(c=='-'))
          {//printf("here<<<<\n" );
            if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            //printf("here<<<<\n" );
            c=(char)fgetc(sourceCode);
            state=DEXPS;
            continue;
          }
        if(c>='0'&&c<='9')
          {
            state=DEXPD;
            if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            c=(char)fgetc(sourceCode);
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
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            c=(char)fgetc(sourceCode);
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
        // printf("55PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);

        if(c>='0'&& c<='9')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            c=(char)fgetc(sourceCode);
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
        //printf("55PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
        if(c=='e'||c=='E')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            c=(char)fgetc(sourceCode);
          continue;
          }
        // printf("66PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
        if(c<='9'&&c>='0')
          {if(ScannerSaveNew(token,sourceCode,n_lines,&c,0)==0)return token;
            c=(char)fgetc(sourceCode);
            printf("here\n" );
          continue;}
        // printf("77PRAVE SOM DOSTAL %c state= %d ulozene mam:%s\n",c,state,token->stringPtr);
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

      ScannerSkipLineE(sourceCode);
      token->lexem=PROBLEML;
      token->line=n_lines;

      state=NEWLINE;
    //  one=1;
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
