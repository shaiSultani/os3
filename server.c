#include "segel.h"
#include "threadPool.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
void getargs(int *port, int* threads_num, int* q_size, SchedAlg* sched_alg, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threads_num = atoi(argv[2]);
    *q_size = atoi(argv[3]);
    if (strcmp(argv[4], "block") == 0) {
        *sched_alg = Block;
    } else if (strcmp(argv[4], "dt") == 0) {
        *sched_alg = DropTail;
    } else if (strcmp(argv[4], "dh") == 0) {
        *sched_alg = DropHead;
    } else if (strcmp(argv[4], "random") == 0) {
        *sched_alg = DropRandom;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int listenfd, connfd, port, clientlen, queue_size, threads_num;
    struct sockaddr_in clientaddr;
    SchedAlg schedAlg;
    getargs(&port, &threads_num, &queue_size, &schedAlg, argc, argv);
    ThreadPool threadPool = ThreadPoolInit(threads_num, queue_size);
    listenfd = Open_listenfd(port);
    //int i;
    while (1) {
        //rintf("request %d\n", i);
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t *) &clientlen);
        int* handlerArgs = malloc(sizeof(int));
        *handlerArgs = connfd;
//        pthread_mutex_lock(&threadPool->mutex);
//        while (ThreadIsFull(threadPool) && schedAlg == Block) { //thread thread_pool is full
//            pthread_cond_wait(&threadPool->taskFinished, &threadPool->mutex);
//        }
        //pthread_mutex_unlock(&threadPool->mutex);
        Task to_add = TaskInit(requestHandle, handlerArgs);
        ThreadPoolAddTask(threadPool, to_add, schedAlg);
    }
}




    


 
