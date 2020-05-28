#include "registers.h"

void op_reg_message(double timed, int i, pid_t pid, pid_t tid, int dur, int pl, char *oper) {
    char* message;
    message = (char*) malloc (128 * sizeof(char));
    timed = time(NULL);

    sprintf(message, "%f ; %d ; %d ; %d ; %d ; %d ; %s\n", timed, i, pid, tid, dur, pl, oper);
    write(STDOUT_FILENO, message, strlen(message));
}