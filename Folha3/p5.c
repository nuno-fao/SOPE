//PROGRAMA p5.c

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void){
	pid_t pid, pidSon;
	int status;

	pid=fork();	

	switch(pid){
		case -1:
			perror("fork");
			break;
		case 0:	//filho
			pid=fork();

			switch(pid){
				case-1:
					perror("son fork");
					break;
				case 0: //filho do filho
					printf("Hello");
					break;
				default:	//filho original
					pidSon=wait(&status);
					printf(" my");
					break;
			}

			break;
		default: //pai
			pidSon = wait(&status);
			printf(" friends!\n");
			break;
	}
}