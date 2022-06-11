#include "threadPool.h"

//args for thread_routine
typedef struct struct_thread_args{
    ThreadPool threadPool;
} *ThreadArgs;

void* thread_routine(void* args) {
    ThreadArgs thread_args = (ThreadArgs)args;
    ThreadPool threadPool = thread_args->threadPool;
    int static_requests_counter = 0;
    int dynamic_requests_counter = 0;
    while(1) {
        pthread_mutex_lock(&threadPool->mutex);
        while(listSize(threadPool->waiting_tasks) == LIST_EMPTY) {
            pthread_cond_wait(&threadPool->listNotEmpty, &threadPool->mutex);
        }
        Task* curr_task = removeTail(threadPool->waiting_tasks);
        pthread_mutex_unlock(&threadPool->mutex);
        struct timeval arrival = (*curr_task)->headers.stat_req_arrival;
        struct timeval diff;
        gettimeofday(&diff, NULL);
        if(diff.tv_usec < arrival.tv_usec) {
            int nanoSec = (arrival.tv_usec - diff.tv_usec) / 1000000 + 1;
            arrival.tv_usec -= 1000000 * nanoSec;
            arrival.tv_sec += nanoSec;
        }
        if (diff.tv_usec - arrival.tv_usec > 1000000) {
            int nanoSec = (diff.tv_usec - arrival.tv_usec) / 1000000;
            arrival.tv_usec += 1000000 * nanoSec;
            arrival.tv_sec -= nanoSec;
        }
        diff.tv_sec = diff.tv_sec - arrival.tv_sec;
        diff.tv_usec = diff.tv_usec - arrival.tv_usec;
        (*curr_task)->headers.stat_req_dispatch = diff;
        (*curr_task)->headers.stat_thread_count = threadPool->handled_tasks->size;
        (*curr_task)->headers.stat_thread_static = static_requests_counter;
        (*curr_task)->headers.stat_thread_dynamic = dynamic_requests_counter;
        addToList(threadPool->handled_tasks, curr_task);
        pthread_mutex_unlock(&threadPool->mutex);
        (*curr_task)->handler(*(*curr_task)->args);
        Close(*(*curr_task)->args);
        pthread_mutex_lock(&threadPool->mutex);
        pthread_cond_signal(&threadPool->taskFinished);
        pthread_mutex_unlock(&threadPool->mutex);
    }
}

ThreadPool ThreadPoolInit(int threads_number, int q_size) {
    ThreadPool threadPool = malloc(sizeof(struct struct_thread_pool));
    threadPool->threads_num = threads_number;
    threadPool->list_size = q_size;
    threadPool->threads = malloc((sizeof(pthread_t))*threads_number);
    List waiting_list = createList(q_size-threads_number);
    threadPool->waiting_tasks = waiting_list;
    List handled_tasks = createList(threads_number);
    threadPool->handled_tasks = handled_tasks;
    threadPool->handled_tasks = malloc((sizeof(struct struct_task))*threads_number);
    pthread_mutex_init(&threadPool->mutex,NULL);
    pthread_cond_init(&(threadPool->listNotEmpty), NULL);
    pthread_cond_init(&(threadPool->taskFinished), NULL);

    //creating threads
    for(int i = 0; i<threads_number; i++){
        ThreadArgs thread_args = malloc(sizeof(struct struct_thread_args));
        thread_args->threadPool = threadPool;
        pthread_create(&(threadPool->threads[i]), NULL, thread_routine, thread_args);
    }
    return threadPool;
}

void ThreadPoolAddTask(ThreadPool threadPool, Task task, SchedAlg schedAlg){
    pthread_mutex_lock(&threadPool->mutex);
    if(ThreadIsFull(threadPool)){
        if (schedAlg == DropHead) {
            removeHead(threadPool->waiting_tasks);
            pthread_mutex_unlock(&threadPool->mutex);
            return;
        }
        if (schedAlg == DropTail) {
            Close(*(task->args));
            pthread_mutex_unlock(&threadPool->mutex);
            return;
        }
        if (schedAlg == DropRandom){
            removeRand(threadPool->waiting_tasks);
            pthread_mutex_unlock(&threadPool->mutex);
        }
    }
    if(addToList(threadPool->waiting_tasks,&task) == LIST_OK){
        pthread_cond_signal(&threadPool->listNotEmpty);
        pthread_mutex_unlock(&threadPool->mutex);
    }

}

bool ThreadIsFull(ThreadPool threadPool) {
    return (listSize(threadPool->waiting_tasks) == LIST_FULL);
}