#ifndef REG_H
#define REG_H

#include <stdio.h>      
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void op_reg_message(double time, int i, pid_t pid, pid_t tid, int dur, int pl, char *oper);

#endif /*REG_H*/