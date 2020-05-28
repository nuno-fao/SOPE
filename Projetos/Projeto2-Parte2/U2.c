#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include "funcs.h"
#include "registers.h"

struct timeval startTime;
int i=0;


int server;
struct timespec start;

void* thread_function(void* arg) {

    struct timeval execTime;
    pthread_t tid;
    pthread_detach(tid = pthread_self());

    comms_t request;
    memcpy(&request, ((comms_t*) arg), sizeof(comms_t));

    request.tid = tid;
    request.pid = getpid();

    char privFIFOPath[256];
    sprintf(privFIFOPath, "/tmp/%d.%ld", getpid(), tid);

    if (mkfifo(privFIFOPath, 0660) != 0) {
        perror("Error Creating fifo");
        exit(1);
    }

    write(server, &request, sizeof(comms_t));

    int user;
    if ((user = open(privFIFOPath, O_RDONLY)) < 0) {
        char* error = (char*) malloc (128 * sizeof(char));
        sprintf(error, "Cannot open FIFO %d ", request.i);
        perror(error);
        free(error);
        op_reg_message(elapsedTime(&startTime,&execTime),request.i, request.pid, request.tid, request.dur, request.pl, "FAILD");

        if (unlink(privFIFOPath) < 0) { perror("Error Unlinking FIFO"); }
        return NULL;
    }

    op_reg_message(elapsedTime(&startTime,&execTime),request.i, request.pid, request.tid, request.dur, request.pl, "IWANT");

    int ct = 0;
    comms_t reply;
    
    while (read(user, &reply, sizeof(comms_t)) <= 0 && ct < 15) {
        usleep(10000);
        ct++;
    }
    if (ct == 15) {
        op_reg_message(elapsedTime(&startTime,&execTime),request.i, request.pid, request.tid, request.dur, request.pl, "FAILD");
        close(user);
        if (unlink(privFIFOPath) < 0) { perror("Error Unlinking FIFO"); }
        return NULL;
    }
    
    op_reg_message(elapsedTime(&startTime,&execTime),reply.i, getpid(), tid, reply.dur, reply.pl, (reply.pl == -1) ? "CLOSD" : "IAMIN");

    if (unlink(privFIFOPath) < 0) { perror("Error Unlinking FIFO"); }
    close(user);
    return NULL;
}


int main(int argc, char** argv) {

    int WcTime=0;

  if (argc != 4) {
        write(STDOUT_FILENO,"Bad arguments. Usage: Un <-t nsecs> fifoname\n", strlen("Bad arguments. Usage: Un <-t nsecs> fifoname\n"));
        exit(1);
  }

    Uflags_t args = save_Uflags(argv);
    
    do {
        server = open(args.fifoname, O_WRONLY);
        if (server == -1) {
            printf("Waiting for Q2\n");
            sleep(1);
        }
    } while(server == -1);
    

    int request_i = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    while (timer() < (args.nsecs * 1000)) {
        pthread_t tid;
        comms_t request;
        WcTime = (rand() % (100 - 30 + 1)) + 30;
        request.dur = WcTime;
        request.i = request_i++;
        request.pl = -1;

        pthread_create(&tid, NULL, thread_function, &request);
        usleep(50000); 
    }


    exit(0);
}
