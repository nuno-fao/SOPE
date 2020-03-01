#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>

int main(int argc,char *argv[]) {
	clock_t start=clock();

	if(argc!=3){
		printf("Error!! Usage: p8 UpperLimit Iterations\n");
		exit(0);
	}

	long clock_times;
   	struct tms begintime;

	srand(time(NULL));

	int n1, n2;

	n1=atoi(argv[1]);
	n2=atoi(argv[2]);

	for(int i=0;i<n2;i++){
		printf("It %i: %i\t",i+1,rand()%n1);
	}
	clock_t end=clock();

	printf("\n");

	printf("%li %ld %ld\n", end-start,(long)begintime.tms_utime, (long)begintime.tms_stime);  //real time measure in microseconds i think, the other two just need to divide by clocks_per_sec
	exit(0);
}