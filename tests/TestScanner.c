
#include <stdio.h>

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

FILE *ptr;
ptr=fopen(argv[1],"r");
int res1=ScannerTestWord("begin",ptr);
int res2=ScannerTestKeyWord(ptr);
int res=ScannerGetLex(ptr);


printf("Scanner testWord poslal na slovo begin :%d\nScannerTestKeyWord poslal : %d\n ScannerGetLex poslal: %d\n",res1,res2,res);


  return argc;
}
