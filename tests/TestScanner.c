#include <stdio.h>
#include "../scanner.h"
#include <stdio.h>
#include <stdlib.h>
void PrintToken(int token)
{

switch((token))
{
case 0:
  {printf("EOL");
   return;}
case 1:
  {printf("EOFILE");
   return;}
case 2:
  {printf("IDENT");
   return;}
case 3:
  {printf("STR");
   return;}
case 4:
  {printf("INT");
   return;}
case 5:
  {printf("FLOAT");
   return;}
case 6:
  {printf("NIL");
   return;}
case 7:
  {printf("LEFT_B");
   return;}
case 8:
  {printf("RIGHT_B");
   return;}
case 9:
  {printf("PLUS");
   return;}
case 10:
  {printf("MINUS");
   return;}
case 11:
  {printf("MULTIPLY");
   return;}
case 12:
  {printf("DIVISION");
   return;}
case 13:
  {printf("LESS");
   return;}
case 14:
  {printf("MORE");
   return;}
case 15:
  {printf("LESSEQ");
   return;}
case 16:
  {printf("MOREEQ");
   return;}
case 17:
  {printf("EQ");
   return;}
case 18:
  {printf("ADDITION");
   return;}
case 19:
  {printf("NOTEQ");
   return;}
case 20:
  {printf("DEF");
   return;}
case 21:
  {printf("DO");
   return;}
case 22:
  {printf("ELSE");
   return;}
case 23:
  {printf("END");
   return;}
case 24:
  {printf("IF");
   return;}
case 25:
  {printf("NOT");
   return;}
case 26:
  {printf("THEN");
   return;}
case 27:
  {printf("WHILE");
   return;}
case 28:
  {printf("COMA");
   return;}
case 29:
  {printf("NEXT");
   return;}
case -1:
  {printf("PROBLEML");
   return;}
case -2:
  {printf("PROBLEMM");
   return;}
case -3:
  {printf("PROBLEMC");
   return;}



}


}

FILE* sourceCode;
int main(int argc, char *argv[])
{


if(argc==1)return 1;
    FILE *ptr=NULL;
    ptr=fopen(argv[1],"r");

    if(ptr==NULL)
     return 0;

    sourceCode=ptr;
    TokenPtr token;
    int i=0;
    while((i<100) ){

        token=(ScannerGetToken(ptr));
        //printf("here\n" );
        printf("Line: %d --Token: ",(token)->line);
        // printf("here\n" );
        PrintToken(token->lexem);//CHYBA
        // printf("here\n" );
        if(token->lexem>-1){
            if(token->name!=NULL){
                int len;
                len=strlen(token->name);

                while(token->name[len]!='\0')
                    len++;
                if(len==0){
                    printf("\n");
                    if(token->lexem==EOFILE){
                        //printf("zatvorim\n");
                        free(token->name);
                        free(token);
                        fclose(ptr);
                        return 0;
                    }
                    //printf("\n");
                    free(token->name);
                    free(token);
                    //printf("\n");
                    //free(token->name);
                    //free(token);

                    continue;
                }
                printf("\t->%s<-\n",(token)->name);
            }
        }
         else
             printf("\n");

        free(token->name);

        //free(token->name);

        if(token->lexem==EOFILE){
            free(token);
            fclose(ptr);
            return 0;
        }
        free(token);
        i++;
    }
    fclose(ptr);

    (void)argv;
    (void)(argc);
    (void)sourceCode;
    return 1;
}

