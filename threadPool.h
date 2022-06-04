//
// Created by student on 6/4/22.
//

#ifndef WEBSERVER_FILES_THREADPOOL_H
#define WEBSERVER_FILES_THREADPOOL_H

#include <pthread.h>
#include <stdbool.h>
#include "task.h"

typedef struct struct_thread_pool {
    int threads_num;
    pthread_t* threads; //array of threads
    int queue_size; //given size of requests
    Task* waiting_tasks;
    Task* handled_tasks;
    int waiting_tasks_num;
    int handled_tasks_num;
    pthread_mutex_t mutex;
    pthread_cond_t qNotEmpty;
    //pthread_cond_t qNotFull;
}* ThreadPool;

ThreadPool ThreadPoolInit(int threads_number, int q_size);
void ThreadPoolAddWaitingTask(ThreadPool threadPool, Task task);

#endif //WEBSERVER_FILES_THREADPOOL_H
