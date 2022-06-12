//
// Created by student on 6/4/22.
//

#ifndef WEBSERVER_FILES_THREADPOOL_H
#define WEBSERVER_FILES_THREADPOOL_H

#include "task.h"
#include "list.h"
#include "segel.h"

typedef struct struct_thread_pool {
    int threads_num;
    pthread_t* threads; //array of threads
    int list_size; //given size of requests
    List waiting_tasks;
    List handled_tasks;
    pthread_mutex_t mutex;
    pthread_cond_t listNotEmpty;
    pthread_cond_t taskFinished;
}* ThreadPool;

typedef enum struct_sched_alg {
    Block = 0,
    DropTail = 1,
    DropRandom = 2,
    DropHead = 3
} SchedAlg;

ThreadPool ThreadPoolInit(int threads_number, int l_size);
void ThreadPoolAddTask(ThreadPool, Task, SchedAlg);
bool ThreadIsFull(ThreadPool);

#endif //WEBSERVER_FILES_THREADPOOL_H
