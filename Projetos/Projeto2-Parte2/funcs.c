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


Uflags_t save_Uflags(char **argv) {
    Uflags_t Uflags;

    Uflags.nsecs = atoi(argv[2]);
    Uflags.fifoname = argv[3];

    return Uflags;
}

Qflags_t save_Qflags(int argc, char **argv) {
    
    Qflags_t Qflags;

    Qflags.nthreads = 0; 
	Qflags.nplaces = 0;
    Qflags.nsecs = atoi(argv[2]);
    Qflags.nplaces = 0;
    Qflags.nthreads = 0;

    for (int i = 3; i < argc-1; i++) {
        if (strcmp("-l", argv[i]) == 0)
            Qflags.nplaces = atoi(argv[++i]);
        else if (strcmp("-n", argv[i]) == 0)
            Qflags.nthreads = atoi(argv[++i]);
    }


    Qflags.fifoname = argv[argc-1];


    return Qflags;
	
}

double elapsedTime(struct timeval *start, struct timeval *now){
	struct timeval result;
	gettimeofday(now,NULL);
	timersub(now,start,&result);

	return (double)((double)result.tv_sec + (double)result.tv_usec/1000000);
}

double timer() {
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

stalls_t create_stalls(int max_size) {
    stalls_t stalls;
    stalls.size = 0;
    stalls.max_size = max_size;
    stalls.top = 0;
    stalls.bottom = max_size - 1;
    stalls.queue = (int*) malloc(max_size * sizeof(int));
    return stalls;
}

int full(stalls_t* stalls) {
    return stalls->size == stalls->max_size;
}

int empty(stalls_t* stalls) {
    return stalls->size == 0;
}

int push(stalls_t* stalls, int val) {
    if (!full(stalls)) {
        stalls->bottom = (stalls->bottom + 1) % stalls->max_size;
        stalls->queue[stalls->bottom] = val;
        stalls->size++;
        return 0;
    }
    return 1;
}

int pop_top(stalls_t* stalls) {
    int value = -1;
    if (!empty(stalls)) {
        value = stalls->queue[stalls->top];
        stalls->top = (stalls->top + 1) % stalls->max_size;
        stalls->size--;
    }
    return value;
}

int pack(stalls_t* stalls) {
    for (int i = 1; i <= stalls->max_size; i++) {
        if (push(stalls, i) != 0)
            return 1;
    }
    return 0;
}

