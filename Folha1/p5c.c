#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
  char comp[] = {'U','S','E','R','_','N','A','M','E'}; //na shell export USER_NAME="etc"
  char *out;
  out=getenv(comp);
  printf("Hello %s!\n", out); 
  return 0;
}