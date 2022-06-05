#include "segel.h"
#include "request.h"
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
void getargs(int *port,int* threads_num, int* q_size, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threads_num = atoi(argv[2]);
    *q_size = atoi(argv[3]);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int listenfd, connfd, port, clientlen, queue_size, threads_num;
    struct sockaddr_in clientaddr;

    getargs(&port, &threads_num, &queue_size, argc, argv);
    ThreadPool threadPool = ThreadPoolInit(threads_num, queue_size);

    // 
    // HW3: Create some threads...
    //
    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t *) &clientlen);
        int* handlerArgs = malloc(sizeof(int));
        *handlerArgs = connfd;
        while (threadPool->handled_tasks_num +threadPool->waiting_tasks_num==threadPool->threads_num) { //thread pool is full
            pthread_cond_wait(&threadPool->taskFinished, &threadPool->mutex);
        }
        Task to_add = TaskInit(requestHandle, handlerArgs);
        ThreadPoolAddWaitingTask(threadPool,to_add);
    }
}




    


 
