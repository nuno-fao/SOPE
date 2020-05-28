#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "funcs.h"
#include "registers.h"
#include <semaphore.h>

struct timeval startTime;

static int limited_threads = 0;  // if number of threads is limited by user, flag will be turned on
static sem_t nthreads; // thread handling semaphore
static int limited_stalls = 0;  //if number of places is limited by user, flag will be turned on
static stalls_t stalls; //available stalls
static sem_t nplaces;   //semaphore that handles number of occupied bathroom stalls
static pthread_mutex_t pl_mut = PTHREAD_MUTEX_INITIALIZER;

struct timespec start;
long int timeout;

void* thread_function(void* arg) {
    pthread_t tid;
    pthread_detach(tid = pthread_self());

    comms_t request;
    memcpy(&request, ((comms_t*) arg), sizeof(comms_t));
    struct timeval execTime;

    op_reg_message(elapsedTime(&startTime,&execTime),request.i, getpid(), tid, request.dur, request.pl, "RECVD");

    char pvFifoname[256];
    sprintf(pvFifoname, "/tmp/%d.%ld", request.pid, request.tid);

    int user;
    if ((user = open(pvFifoname, O_WRONLY)) < 0) {
        fprintf(stderr, "Error opening private fifo with request %d\n", request.i);
        op_reg_message(elapsedTime(&startTime,&execTime),request.i, getpid(), tid, request.dur, request.pl, "GAVUP");

        if (limited_threads) { sem_post(&nthreads); }
        return NULL;
    }

    comms_t reply;
    reply.i = request.i;
    reply.pid = getpid();
    reply.tid = tid;
    reply.dur = request.dur;


    int stall = 1;
    if (limited_stalls) {
        sem_wait(&nplaces);
        pthread_mutex_lock(&pl_mut);
        stall = pop_top(&stalls);
        pthread_mutex_unlock(&pl_mut);
    }
    reply.pl = stall;

    if (timer() + request.dur < timeout) {
        if (write(user, &reply, sizeof(comms_t)) < 0) { 
            fprintf(stderr, "Error with request %d to FIFO\n", request.i);
            op_reg_message(elapsedTime(&startTime,&execTime),reply.i, reply.pid, reply.tid, reply.dur, reply.pl, "GAVUP");
            close(user); 
            if (limited_threads) { sem_post(&nthreads); } 
            return NULL;
        }
        close(user); 
        op_reg_message(elapsedTime(&startTime,&execTime),reply.i, getpid(), tid, reply.dur, reply.pl, "ENTER");
        usleep(reply.dur * 1000);
        op_reg_message(elapsedTime(&startTime,&execTime),reply.i, getpid(), tid, reply.dur, reply.pl, "TIMUP");
    }
    
    else {
        reply.pl = -1;
        if (write(user, &reply, sizeof(comms_t)) < 0) {
            fprintf(stderr, "Error with request %d to FIFO\n", request.i);
            op_reg_message(elapsedTime(&startTime,&execTime),reply.i, reply.pid, reply.tid, reply.dur, reply.pl, "GAVUP");
            close(user);

            if (limited_threads) { sem_post(&nthreads); } 
            return NULL;
        }
        op_reg_message(elapsedTime(&startTime,&execTime),reply.i, getpid(), tid, reply.dur, -1, "2LATE");
    }

    if (limited_threads) { sem_post(&nthreads); }
    if (limited_stalls) {
        pthread_mutex_lock(&pl_mut);
        push(&stalls, stall);
        pthread_mutex_unlock(&pl_mut);
        sem_post(&nplaces);
    }
    
    close(user);
    return NULL;
}


int main(int argc, char** argv) {

  struct FLAGS flags;

  if(readFlags(argv,argc, &flags)){		//reads arguments passed on function call
  	write(STDOUT_FILENO,"Bad arguments. Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n",strlen("Bad arguments. Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n"));
    exit(1);
  }

    
  Qflags_t args = save_Qflags(argc, argv);

  if (mkfifo(args.fifoname, 0660) != 0) {
      perror("Error making FIFO");
      exit(1);
  }
    int fd = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    if (args.nthreads) { limited_threads = 1; }
    if (args.nplaces) { limited_stalls = 1; }


    if (limited_threads) {
        sem_init(&nthreads, 0, args.nthreads);
    }
    if (limited_stalls) {
        sem_init(&nplaces, 0, args.nplaces);
        stalls = create_stalls(args.nplaces);
        pack(&stalls);
    }

    timeout = args.nsecs * 1000;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    while (timer() < timeout) {
        comms_t request;
        while (read(fd, &request, sizeof(comms_t)) <= 0 && timer() < timeout) { usleep(1000); }
        if (timer() >= timeout) break;
        if (limited_threads) { sem_wait(&nthreads); }
        pthread_t tid;
        pthread_create(&tid, NULL, thread_function, &request);
    }

    close(fd);
    if (unlink(args.fifoname) < 0) {
        perror("Error unlinking public FIFO");
    }

    exit(0);
}
