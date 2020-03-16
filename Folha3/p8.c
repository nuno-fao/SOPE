#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[], char *envp[]){
	pid_t pid;
	if(argc != 2){
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();
	if(pid>0){
		printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]); 
	}
	else if(pid == 0){
		execl("/bin/ls","/bin/ls","-laR",NULL);
		printf("command not executed !\n");
		exit(1);
	}
	exit(0);
}