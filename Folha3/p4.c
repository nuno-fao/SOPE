//PROGRAMA p4.c

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void){
	pid_t pid, pidSon;
	int status;

	write(STDOUT_FILENO,"Hello",5);

	pid=fork();	

	switch(pid){
		case -1:
			perror("fork");
			break;
		case 0:	//filho
			printf(" World !\n");
			break;
		default: //pai
			//pidSon = wait(&status);
			//printf(" World !\n");
			break;
	}
}