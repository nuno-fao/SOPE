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


struct Reply{
	int i;
	pid_t pid;
	int tid;
	int dur;
	int pl;
};

bool readFlags(char **argv, int argc, struct FLAGS* flags);	//reads flags and checks if they are valid or not

void printFlags(struct FLAGS* flags);	//debug purposes

double elapsedTime(struct timeval *start, struct timeval *now);	//start must have a value and now will be overwritten 
