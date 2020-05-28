#ifndef REG_H
#define REG_H

#include <stdio.h>      
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

struct Register {
    pid_t pid;
    double instant;
};

void initRegisters();
void structRegisterBegin(struct Register *registo);

void writeCreateEvent(int argc, char *argv[]);
void writeExitEvent(int exitStatus);
void writeRecvSignalEvent(int signal);
void writeSendSignalEvent(pid_t pid, int signal);
void writeRecvPipeEvent(long int receivedMessage);
void writeSendPipeEvent(long int sentMessage);
void writeEntryEvent();

#endif /*REG_H*/