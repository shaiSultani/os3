//
// Created by student on 6/4/22.
//
#include "threadPool.h"
#include <stdlib.h>

//args for thread_routine
typedef struct struct_thread_args{

} *ThreadArgs;

void* thread_routine(void* args) {

}

ThreadPool ThreadPoolInit(int threads_number, int q_size) {
    ThreadPool threadPool = malloc(sizeof(struct struct_thread_pool));
    threadPool->threads_num = threads_number;
    threadPool->queue_size = q_size;
    threadPool->threads= malloc(sizeof(pthread_t)*threads_number);
    threadPool->waiting_tasks = malloc(sizeof(struct struct_task)*q_size);
    threadPool->handled_tasks = malloc(sizeof(struct struct_task)*threads_number);
    threadPool->waiting_tasks_num=0;
    threadPool->handled_tasks_num=0;
    pthread_mutex_init(&threadPool->mutex,NULL);
    pthread_cond_init(&(threadPool->qNotEmpty), NULL);

    //creating threads
    for(int i = 0; i<threads_number; i++){
        ThreadArgs thread_args = malloc(sizeof(struct struct_thread_args));
        //todo: update thread_args
        pthread_create(&threadPool->threads[i], NULL, thread_routine, thread_args);
    }
    return threadPool;
}

void ThreadPoolAddWaitingTask(ThreadPool threadPool, Task task){
    pthread_mutex_lock(&threadPool->mutex);
    if(threadPool->waiting_tasks_num + threadPool->handled_tasks_num >= threadPool->queue_size){ //threadPool is full
        //todo: part 2: overload handling
    }
    threadPool->waiting_tasks[threadPool->waiting_tasks_num] = task;
    threadPool->waiting_tasks++;
    if(threadPool->waiting_tasks_num == 1) //waiting_tasks_array was empty
        pthread_cond_signal(&threadPool->qNotEmpty);
    pthread_mutex_unlock(&threadPool->mutex);
}
