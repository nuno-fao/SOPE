#include <stdio.h>
#include <stdlib.h>

void first(){
	printf("Executing exit handler 1!\n");
}

void second(){
	printf("Executing exit handler 2!\n");
	//exit(1);				//this overwrites main last return, so this will be the last return
}

int main(int argc,char *argv[]) {   
	atexit(first);
	//atexit(first);		//we can make the same handler call twice
	atexit(second);
	//abort();				//atexit handlers are dumped
	printf("Main done!\n");
	exit(0);
}