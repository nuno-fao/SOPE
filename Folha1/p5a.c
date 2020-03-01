#include <stdio.h>
#include <string.h>

int main(int argc, char **argv, char **envp)
{
  char comp[] = {'U','S','E','R','='};
  for (char **env = envp; *env != 0; env++)
  {
    char *thisEnv = *env;
    if(!strncmp(comp,thisEnv,5)){
    	thisEnv+=5;
    	printf("Hello %s!\n", thisEnv); 
    	break; 
    }  
  }
  return 0;
}