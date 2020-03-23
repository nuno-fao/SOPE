// PROGRAMA p2a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signo){
	printf("Entering SIGINT handler ...\n");
	sleep(10);
	printf("Exiting SIGINT handler ...\n");
}

void sigterm_handler(int signo){
	printf("Entering SIGTERM handler ...\n");
	sleep(10);
	printf("Exiting SIGTERM handler ...\n");
}

int main(void){
	struct sigaction action1;
	struct sigaction action2;

	action1.sa_handler = sigint_handler;
	sigemptyset(&action1.sa_mask);
	action1.sa_flags = 0;

	action2 = action1;
	action2.sa_handler = sigterm_handler;

	if (sigaction(SIGINT,&action1,NULL) < 0){
		fprintf(stderr,"Unable to install SIGINT handler\n");
		exit(1);
	}

	if (sigaction(SIGTERM,&action2,NULL) < 0){
		fprintf(stderr,"Unable to install SIGTERM handler\n");
		exit(1);
	}
	printf("Try me with CTRL-C ...\n");
	while(1) pause();

	exit(0);
}