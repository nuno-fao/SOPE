#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

struct FLAGS{
	int nsecs;
	int nplaces;
	int nthreads;
	char* fifoname;
};

extern struct timespec start;

typedef struct { //struct where U2 specific flags are saved
    int nsecs;
    char* fifoname;
} Uflags_t;

typedef struct { //struct where Q2 Specific flags are saved
    int nsecs;
    int nplaces;
    int nthreads;
    char* fifoname;
} Qflags_t;


typedef struct{ //struct for communications between U and Q
	int i;
	pid_t pid;
	pthread_t tid;
	int dur;
	int pl;
}comms_t;

bool readFlags(char **argv, int argc, struct FLAGS* flags);	//reads flags and checks if they are valid or not, useful for Q2 check

/*void printFlags(struct FLAGS* flags);	//debug purposes */

Uflags_t save_Uflags(char **argv); //store flag values specific to U2 program

Qflags_t save_Qflags(int argc, char **argv); //store flag values specific to Q2 program

double elapsedTime(struct timeval *start, struct timeval *now);	//start must have a value and now will be overwritten 

double timer();

typedef struct { //struct with bathroom stalls info
    int top;
    int bottom;
    int size;
    int max_size;
    int* queue;
} stalls_t;

stalls_t create_stalls(int max_size); //create bathroom stall grid

int full(stalls_t* stalls); //checks if bathroom is full

int empty(stalls_t* stalls); // checks if there are available stalls

int push(stalls_t* stalls, int val); //occupies specified bathroom stall

int pop_top(stalls_t* stalls); //empties bathroom space

int pack(stalls_t* stalls); //fills up bathroom stalls







