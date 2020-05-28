#include "funcs.h"
#include "registers.h"

bool readFlags(char **argv, int argc, struct FLAGS* flags){
	char *secs = "-t\0";
	char *places = "-l\0";
	char *threads = "-n\0";
	bool Un = false;
	bool Qn = false;

	if(strncmp(*argv,"./Q1\0",5)==0 || strncmp(*argv,"./Q2\0",5)==0){
		Qn=true;
	}
	else if (strncmp(*argv,"./U1\0",5)==0 || strncmp(*argv,"./U2\0",5)==0){
		Un=true;
	}

	if(Un && (argc != 4)){
		return true;
	}
	if(Qn && (argc != 4)){
		return true;
	}

	flags->nsecs=flags->nplaces=flags->nthreads=0;
	flags->fifoname="";

	for(char **arg=argv+1;*arg!=0;arg++){
		char *thisarg=*arg;

		//will check each flag and if it has been already used
		if(strncmp(secs,thisarg,3)==0 && flags->nsecs==0){
			arg++;
			flags->nsecs=atoi(*arg);
			continue;
		}
		else if(strncmp(places,thisarg,3)==0 && flags->nplaces==0){
			if(Un) return true;
			arg++;
			flags->nplaces=atoi(*arg);
			continue;
		}
		else if(strncmp(threads,thisarg,3)==0 && flags->nthreads==0){
			if(Un) return true;
			arg++;
			flags->nthreads=atoi(*arg);
			continue;
		}
		else if(strlen(flags->fifoname)==0){
			flags->fifoname=thisarg;
			continue;
		}
		return true;	//returns true if any input is wrong
	}

	return false; //read flags with no errors
}

void printFlags(struct FLAGS* flags){
	printf("nsecs = %i\n",flags->nsecs);
	printf("nplaces = %i\n",flags->nplaces);
	printf("nthreads = %i\n",flags->nthreads);
	printf("fifoname = %s\n",flags->fifoname);
}

double elapsedTime(struct timeval *start, struct timeval *now){
	struct timeval result;
	gettimeofday(now,NULL);
	timersub(now,start,&result);

	return (double)((double)result.tv_sec + (double)result.tv_usec/1000000);
}
