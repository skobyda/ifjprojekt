
#include <stdio.h>

#include "../scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

FILE *ptr;
ptr=fopen(argv[1],"r");
if(ptr==NULL)
{
  printf("Nepodarilo sa otvorit subor\n" );
  return 0;
}
int res1=ScannerTestWord("begin ",ptr);
printf("Scanner testWord poslal na slovo begin :%d\n",res1);
int res2=ScannerTestKeyWord(ptr);
printf("ScannerTestKeyWord poslal : %d\n",res2);

int res=ScannerGetLex(ptr);


printf("ScannerGetLex poslal: %d\n",res);


  return argc;
}
