#include <stdio.h> 

int main(int argc, char *argv[])  {
	for(int count=1; count <= *argv[argc-1] - '0';count++){
		printf("Hello");
		for(int i=1;i<argc-1;i++){
			printf(" %s",argv[i]);
		}
		printf("!\n");
	}
	return 0;   
}

