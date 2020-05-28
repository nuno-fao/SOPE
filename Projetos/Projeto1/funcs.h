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

struct FLAGS{

	bool all;
	bool bytes;
	bool link;
	bool dereference;
	bool separate;
	int blockSize;
	int maxDepth;
	char* dir;
};

//reads flags and checks if they are valid or not
bool readFlags(char **argv, int argc, struct FLAGS* flags);

//will list files and directories in the directory in flags.dir
long int du(struct FLAGS* flags,char* path, int depth);

//will print an item and its size
void printItem(char* path, long int size);

//will add to the size variable the corresponding value in the statBuffer according to the flags
void getSizeFlagged(long int *size,struct FLAGS* flags,struct stat statBuffer);

//handles SIGINT signal, asks the user for confirmation
void sigint_handler(int sig); 