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

//THIS macro will check if we have enough space for saving next character, if not it will make it :)
#define SAVENEW do { \
    strlenght=strlenght+1; \
    if((strlenght%50)==0){ \
        token->name= realloc(token->name,(strlenght+50)*sizeof(char)); \
    } \
    if(ScannerSaveNew(token,n_lines,&c)==0) \
        return token; \
} while(0)

//this macro will convert ascii values 0-32, 35,95 because of generating code into \number number and save it into our token string
#define SAVENEWCHAR do { \
    if((c<=32)|| (c==92)||(c==35)) \
        {char var=c; \
            int out=0; \
            c='\\'; \
            SAVENEW; \
            out=((var%100)-(var%10))/10; \
            if(out!=0){ \
                c='0'; \
                SAVENEW; \
                c=out+'0'; \
                SAVENEW; \
                out=(var%10); \
                c=out+'0'; \
                SAVENEW; \
            } \
            else{ \
                out=(var%10); \
                c='0'; \
                SAVENEW; \
                c='0'; \
                SAVENEW; \
                c=out+'0'; \
                SAVENEW; \
            } \
        } \
    else{ \
        SAVENEW; \
    } \
} while(0)

FILE *sourceCode = NULL;

/* Firstly, file with source code is passed to Scanner
 *
 * @sourceCode File with code written in IFJ18 language
 *
 * Returns: true in case of success, false in case of error
 */
bool
Scanner(FILE *file) {
    sourceCode = file;

    return true;
}

//THIS will skip the rest of the line
int ScannerSkipLineE(){
    char c=0;
    while(c!='\n')
     c=(char)fgetc(sourceCode);

    if(c==EOF){
        return 1;
    }
    ungetc(c,sourceCode);
    return 0;
}

//this will check if here are some white spaces and if they are it will throw them away
void ScannerWhite(){
    char c=0;
    do
        c=(char)fgetc(sourceCode);
    while(c==' '|| c==9);
    ungetc( c,  sourceCode);
    return;
}

//this function converts hex value into decimal
int ConvertHextoDec(char c){
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

//this function save only one char into string
char *ScannerStradd( char *s, char* c ){
    int length=0;
    if(s==NULL|| c==NULL)
        return NULL;
    while(s[length]!='\0')
        length++;
    if(length==0)
        {
            s[length]=*c;
            s[length+1]='\0';

            return s;
        }
    s[length+1]=s[length];
    s[length]=*c;
    return s;
}

//this function will check if the ident token if the keyword
int ScannerTestW(char*str){
    if(str==NULL)
        return 0;
    if(strcmp(str,"def")==0)
        return 20;
    if(strcmp(str,"do")==0)
        return 21;
    if(strcmp(str,"else")==0)
        return 22;
    if(strcmp(str,"end")==0)
        return 23;
    if(strcmp(str,"if")==0)
        return 24;
    if(strcmp(str,"not")==0)
        return 25;
    if(strcmp(str,"then")==0)
        return 26;
    if(strcmp(str,"while")==0)
        return 27;
    if(strcmp(str,"nil")==0)
        return 6;
    return 0;
}

//check if the function ScannerStradd is okay with no error
int ScannerSaveNew(TokenPtr token,int lines,char* c){
    if(ScannerStradd((token)->name,c)==0){
        (token)->lexem=PROBLEMC;
        PrintError(99, lines, "Concatenation of the strings failed");
        (token)->line=lines;
        return 0;
    }

    return 1;
}
//this check the word, char by char
int ScannerTestWord(char *str){
    char c;
     c=fgetc(sourceCode);
     ungetc(c,sourceCode);
    if(str==NULL)
        return 0;
    if((c==' ')||(c==9))
        return 0;
    if(str==NULL)
        return -1;
    //int lenght1=strlen(str);
    int lenght1=0;
    while(str[lenght1]!='\0')
        lenght1++;
    char cmp[lenght1+1];
    int step=0;
    while(step!=lenght1){
        cmp[step]=fgetc(sourceCode);
        if(cmp[step]==EOF){
            return 0;
        }
        while(step==0 && ((cmp[step]==' ')||(cmp[step]==9)))
            cmp[step]=fgetc(sourceCode);
        if(cmp[step]==EOF){
            return 0;
        }
        if(cmp[step]==EOF)
            return 0;
        if(cmp[step]==str[step])
            step++;
        else if((cmp[step]=='\n' || cmp[step]=='#' || cmp[step]==' ' || cmp[step]==9)&& (step==lenght1-1)){
            if(cmp[step]=='\n'){
                ungetc(cmp[step],sourceCode);
            }
            return 1;
        }
        else {
            for(int i=0;i<=step;step--){
                ungetc(cmp[step],sourceCode);
            }
            return 0;
        }
    }
    return 1;
}

//function which call parser, if he wants new token, most of the code is here
TokenPtr ScannerGetToken(){
    static int strlenght=0; //lenght of the string
    strlenght=0;    //initialization of the lenght
    static int n_lines=1; //number of the line where we are
    static states state=NEWLINE;    //state of the our FSM
    static  char c; //char that we actualy have
    TokenPtr token=NULL;    //token that we will sent
    token=malloc(sizeof(Token));    //allocation of the token
    if(token==NULL){
        (token)->lexem=PROBLEMM;
        PrintError(99, 0, "Problem with allocation of memory");
        (token)->line=n_lines;
        return token;
    }
    (token)->name=malloc((sizeof(char))*50);    //here we save our chars
    if(token->name==NULL){//
        (token)->lexem=PROBLEMM;
        PrintError(99, 0, "Problem with allocation of memory");
        (token)->line=n_lines;
        return token;
    }
    token->name[0]='\0';    //first char is this, initialization
    ScannerWhite(); //check for white spaces
    c =(char)fgetc(sourceCode);
    if(c==EOF){
        token->lexem=EOFILE;
        state=ENDFILE;
        token->line=n_lines;
        return token;
    }
    while(1){   //FSM will run while it will find token, correct or bad
        switch(state){
            case START:{
                if(c=='#'){
                    state=BCOMMENT;
                    continue;
                }
                else if(c=='\n'){
                    state=NEWLINE;
                    (token)->lexem=EOL;
                    (token)->line=n_lines;
                    n_lines++;
                    return token;
                }
                else if((c>='a' &&c<='z')|| c=='_'){
                    state=IDKEY;
                    continue;
                }
                else if(c>='1' && c<='9'){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=NUMBER;
                    continue;
                }
                else if(c=='0'){
                    SAVENEW;
                    state=ZERO;
                        c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='+'){
                    (token)->lexem=PLUS;
                    (token)->line=n_lines;
                    return token;
                }
                else if(c=='-'){
                    (token)->lexem=MINUS;
                    (token)->line=n_lines;
                    return token;
                }
                else if(c=='*'){
                    (token)->lexem=MULTIPLY;
                    (token)->line=n_lines;
                    return token;
                }
                else if((c==' ') || (c==9)){ // 9 is TAB
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='/'){
                    (token)->lexem=DIVISION;
                    (token)->line=n_lines;
                    return token;
                }
                else if(c==')'){
                    (token)->lexem=RIGHT_B;
                    (token)->line=n_lines;
                    return token;
                }
                else if(c==','){
                    (token)->lexem=COMA;
                    (token)->line=n_lines;
                    return token;
                }
                else if(c=='('){
                    (token)->lexem=LEFT_B;
                    (token)->line=n_lines;
                    return token;
                }
                else if(c=='>'){
                    state=BIGGER;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='<'){
                    state=SMALLER;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='!'){
                    state=NEQ;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='='){
                    if(ScannerTestWord("begin ")==1){

                        state=PROBLEM;
                        PrintError(1, n_lines, "Problem that Begin of the comment \"=begin\" is not at the new line");
                        continue;
                    }

                    state=EQUAL;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='\"'){
                    state=STRING;
                    (token)->lexem=STR;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else{
                    (token)->lexem=PROBLEML;
                    PrintError(1, n_lines, "Problem unknown token");
                    (token)->line=n_lines;
                    return token;
                }
            }
            case IDKEY:{
                if((c==' ') || (c==9) || c=='!' || c=='?'||c==','){//check for end of the string
                    (token)->lexem=IDENT;
                    (token)->line=n_lines;
                    if((c!=' ') && (c!=9))
                        SAVENEW;
                    if(c==' '||c==','|| c==9)
                        ungetc(c,sourceCode);
                    state=START;
                    return token;
                }
                if((c>='A'&& c<='Z')||((c>='a')&& (c<='z'))||(c>='0'&& c<='9')||c=='_'){
                    state=ID;
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else{
                    state=PROBLEML;
                    PrintError(1, n_lines, "Problem with name of function,identifier or keyword");
                    continue;
                }
            }
            case ID:{
                (token)->lexem=IDENT;
                (token)->line=n_lines;
                if(c==' '||c=='?'|| c=='!' || (c==9)){
                    if((c=='?'|| c=='!' )&&( (c!=' ') && (c!=9)))
                        SAVENEW;
                    if(c==' ' || c==9)
                        ungetc(c,sourceCode);
                    int smth=ScannerTestW(token->name);
                    if(smth!=0)
                        token->lexem=smth;
                    state=START;
                    return token;
                }
                else if(((c>='a') && (c<='z'))||((c>='A') && (c<='Z'))||((c>='0') && (c<='9'))||(c=='_')){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    continue;
                }
                else if((c==',')||(c==' ')||(c==9)||(c=='\n')||(c=='#')||(c=='(')||(c=='=')||(c=='!')||(c=='>')||(c=='<')||(c==')')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){
                    ungetc(c,sourceCode);
                    int smth=ScannerTestW(token->name);
                    if(smth!=0)
                        token->lexem=smth;
                    state =START;
                    return token;
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem with name of function,identifier or keyword");
                    continue;
                }
            }
            case ENDFILE:{
                return NULL;
            }
            case NEWLINE:{
                if(c=='\n'){
                    state=NEWLINE;
                    n_lines++;
                    (token)->lexem=EOL;
                    (token)->line=n_lines-1;
                    return token;
                }
                else if((c>='a' &&c<='z')|| c=='_'){
                    state=IDKEY;
                    continue;
                }
                else if(c>='1' && c<='9'){
                    SAVENEW;
                        c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=NUMBER;
                    continue;
                }
                else if(c=='0'){
                    SAVENEW;
                    state=ZERO;
                        c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='#'){
                    state=BCOMMENT;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='='){
                    if(ScannerTestWord("begin ")==1){
                        while(c!='\n'){
                            c=(char)fgetc(sourceCode);
                        }
                        token->line++;

                        state=LCOMMENT;
                        continue;
                    }
                    else{
                        state=PROBLEM;
                        PrintError(1, n_lines, "Problem that token \"=\" cannot be at the begining of the line");
                        continue;
                    }
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem Unknown token at the begining of the line");
                    continue;
                }
            }
            case LCOMMENT:{
                if(c=='\n'){
                    n_lines++;
                    state=LCOMMENTEND;
                }
                c=(char)fgetc(sourceCode);
                if(c==EOF){
                    token->lexem=EOFILE;
                    state=ENDFILE;
                    token->line=n_lines;
                    return token;
                }
                continue;
            }
            case LCOMMENTEND:{
                ScannerWhite();
                if(c=='='){
                    if(ScannerTestWord("end ")==1){
                        while(c!='\n'){
                            c=(char)fgetc(sourceCode);
                        }
                        n_lines--;
                        ungetc(c,sourceCode);
                        state=START;
                        continue;
                    }
                    continue;
                }
                else {
                    state=LCOMMENT;
                    continue;
                }
            }
            case STRING:{
                if(c!='\"' && c!='\\'){
                    SAVENEWCHAR;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c=='\"'){
                    //printf("-hereee-\n" );
                    (token)->lexem=STR;
                    (token)->line=n_lines;
                    state=START;
                    return token;
                }
                else{
                    state=CHARCODEF;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
            }
            case CHARCODEF:{
                char code[2];
                if(c=='s'){
                    c=' ';
                    SAVENEWCHAR;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=STRING;
                    continue;
                }
                if(c=='\\' || c=='\"'){
                    SAVENEWCHAR;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=STRING;
                    continue;
                }
                if(c=='n'){
                    c='\n';
                    SAVENEWCHAR;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=STRING;
                    continue;
                }
                if(c=='t'){
                    c='\t';
                    SAVENEWCHAR;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=STRING;
                    continue;
                }
                if(c=='x'){ //if there is \x
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    if((c>='0' && c<='9')||(c>='A'&&c<='F')||((c>='a')&&(c<='f'))){
                        if(((c>='a')&&(c<='f')))
                            c=c-32;
                        code[0]=c;
                    }
                    else{
                        state=PROBLEM;
                        PrintError(1, n_lines, "Problem that after \\x is something out of range 0-f");
                        continue;
                    }
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    if(((c>='0') && (c<='9'))||((c>='A')&&(c<='F'))||((c>='a')&&(c<='f'))){
                        if(((c>='a')&&(c<='f')))
                            c=c-32;
                        code[1]=c;
                        int a=ConvertHextoDec(code[0])*16+ConvertHextoDec(code[1]);
                        c=a;
                        SAVENEWCHAR;
                        state=STRING;
                        c=(char)fgetc(sourceCode);

                        if(c==EOF){
                            token->lexem=EOFILE;
                            state=ENDFILE;
                            token->line=n_lines;
                            return token;
                        }
                        continue;
                    }
                    else{
                        //convert 0 , c vratit
                        int a=ConvertHextoDec(code[0]);
                        //char b=a;
                        char b=c;
                        c=a;
                        SAVENEWCHAR;
                        c=b;
                        if(c!='\"' && c!='\\'){
                            SAVENEWCHAR;
                            c=(char)fgetc(sourceCode);
                        }
                        state=STRING;
                        continue;
                    }
                }
                    else{
                        state=STRING;
                        continue;
                    }
            }
            case BCOMMENT:{
                while(c!='\n'){
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                }
                n_lines++;
                token->lexem=EOL;
                token->line=n_lines-1;
                state=NEWLINE;
                return token;
            }
            case BIGGER:{
                if(c=='=')
                    (token)->lexem=MOREEQ;
                else{
                    (token)->lexem=MORE;
                    ungetc(c,sourceCode);
                }
                (token)->line=n_lines;
                state=START;
                return token;
            }
            case SMALLER:{
                if(c=='=')
                    (token)->lexem=LESSEQ;
                else{
                    (token)->lexem=LESS;
                    ungetc(c,sourceCode);
                }
                (token)->line=n_lines;
                state=START;
                return token;
            }
            case EQUAL:{
                if(c=='='){
                    (token)->lexem=EQ;
                    state=START;
                }
                else{
                    (token)->lexem=ADDITION;
                    state=START;ungetc(c,sourceCode);
                }
                (token)->line=n_lines;
                return token;
            }
            case NEQ:{
                if(c=='=')
                    (token)->lexem=NOTEQ;
                else{
                    (token)->lexem=NOT;
                    ungetc(c,sourceCode);
                }
                (token)->line=n_lines;
                state=START;
                return token;
            }
            case ZERO:{
                    if(c=='.'){
                        state=DOUBLECOM;
                        SAVENEW;
                        c=(char)fgetc(sourceCode);
                        if(c==EOF){
                            token->lexem=EOFILE;
                            state=ENDFILE;
                            token->line=n_lines;
                            return token;
                        }
                        continue;
                    }//next
                    else if(c=='e'||c=='E'){
                        state=DEXP;
                        SAVENEW;
                        c=(char)fgetc(sourceCode);
                        if(c==EOF){
                            token->lexem=EOFILE;
                            state=ENDFILE;
                            token->line=n_lines;
                            return token;
                        }
                        continue;
                    }
                    else if((c=='#')||(c==' ')||(c==9)||(c=='\n')||(c==')')||(c==',')||(c=='(')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){
                        (token)->lexem=INT;
                        (token)->line=n_lines;
                        if(c=='\n')
                            state=NEWLINE;
                        else
                            state=START;
                        ungetc(c,sourceCode);
                        return token;
                    }
                    else{
                        state=PROBLEM;
                        PrintError(1, n_lines, "Problem that after 0 is not valid character");
                        continue;
                    }
            }
            case NUMBER:{
                if(c=='.'){
                    state=DOUBLECOM;
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }//next
                else if(c=='e'||c=='E'){
                    state=DEXP;
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if(c>='0' && c<='9'){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }

                else if((c=='#')||(c==' ')||(c==9)||(c=='\n')||(c==')')||(c==',')||(c=='(')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){
                    (token)->lexem=INT;
                    (token)->line=n_lines;
                    if(c=='\n')
                        state=NEWLINE;
                    else
                        state=START;
                    ungetc(c,sourceCode);
                    return token;
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem with number");
                    continue;
                }
            }
            case DOUBLECOM:{
                if(c>='0'&&c<='9'){
                    state=DCOMD;
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem with number that after \".\" is not character from range 0-9");
                    continue;
                }
            }
            case DEXP:{
                if((c=='+')||(c=='-')){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=DEXPS;
                    continue;
                }
                if(c>='0'&&c<='9'){
                    state=DEXPD;
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem with number that after character of exponent which is \"e or E\" is not + nor - nor value from range 0-9");
                    continue;
                }
                }
            case DEXPD:{
                if(c>='0'&& c<='9'){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    continue;
                }
                else if((c=='#')||(c=='\n')||(c=='\n')||(c==' ')||(c==9)||(c==',')||(c==')')||(c=='(')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){

                    (token)->lexem=FLOAT;
                    (token)->line=n_lines;
                    ungetc(c,sourceCode);
                    if(c=='\n')
                        state=NEWLINE;
                    else
                        state=START;
                    return token;
                }
                else{
                state=PROBLEM;
                    PrintError(1, n_lines, "Problem with number that after number e/E number->here is not valid character");
                    continue;
                }
            }
            case DEXPS:{
                if(c>='0'&& c<='9'){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=DEXPD;
                    continue;
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem with number that after number e/E +/- -> here is not character from range 0-9");
                    continue;
                }
            }
            case DCOMD:{
                if(c=='e'||c=='E'){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                    state=DEXP;
                continue;
                }
                if(c<='9'&&c>='0'){
                    SAVENEW;
                    c=(char)fgetc(sourceCode);
                    if(c==EOF){
                        token->lexem=EOFILE;
                        state=ENDFILE;
                        token->line=n_lines;
                        return token;
                    }
                continue;
                }
                else if((c=='#')||(c=='\n')||(c=='\n')||(c==' ')||(c==',')||(c==9)||(c==')')||(c=='(')||(c=='+')||(c=='-')||(c=='*')||(c=='/')){
                    (token)->lexem=FLOAT;
                    (token)->line=n_lines;
                    ungetc(c,sourceCode);
                    if(c=='\n')
                        {
                            state=NEWLINE;}
                    else
                        state=START;
                    return token;
                }
                else{
                    state=PROBLEM;
                    PrintError(1, n_lines, "Problem with number that after number.number->here is not valid character");
                    continue;
                }
            }
            case PROBLEM:{
                if(c=='\n')
                    n_lines++;
                if(ScannerSkipLineE()==1){
                    state=ENDFILE;

                    continue;
                }
                token->lexem=PROBLEML;
                token->line=n_lines;
                state=NEWLINE;
                return token;
            }
            default:{
                (token)->lexem=-1;
                (token)->line=n_lines;
                return token;
            }
        }
    }
    return token;
}
