//PROGRAMA p2.c

#include <unistd.h>
#include <stdio.h>

int main(void){
	//write(STDOUT_FILENO,"1",1);
	printf("0\n");
	printf("1");  //SEM OS \n O PROGRAMA FILHO TEM TAMBEM A ULTIMA LINHA NO OUTPUT BUFFER
	if(fork()>0){
		//write(STDOUT_FILENO,"2",1);
		//write(STDOUT_FILENO,"3",1);
		printf("2\n");
		printf("3");
	}
	else{
		//write(STDOUT_FILENO,"4",1);
		//write(STDOUT_FILENO,"5",1);
		printf("4\n");
		printf("5\n");
	}
	//write(STDOUT_FILENO,"\n",1);
	printf("\n");
	return 0;
}