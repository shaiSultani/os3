#include "threadPool.h"

//args for thread_routine
typedef struct struct_thread_args{
    ThreadPool thread_pool;
    int thread_id;
} *ThreadArgs;

void* thread_routine(void* args) {
    ThreadArgs thread_args = (ThreadArgs)args;
    ThreadPool threadPool = thread_args->thread_pool;
    int threadID = thread_args->thread_id;
    int static_requests_counter = 0;
    int dynamic_requests_counter = 0;
    while(1) {
        pthread_mutex_lock(&threadPool->mutex);
        while(listSize(threadPool->waiting_tasks) == LIST_EMPTY) {
            pthread_cond_wait(&threadPool->listNotEmpty, &threadPool->mutex);
        }
        Task curr_task = removeHead(threadPool->waiting_tasks);
        threadPool->handled_tasks_num++;
        pthread_mutex_unlock(&threadPool->mutex);
        struct timeval arrival = curr_task->headers.stat_req_arrival;
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
        curr_task->headers.stat_thread_id = threadID;
        curr_task->headers.stat_req_dispatch = diff;
        curr_task->headers.stat_thread_count = static_requests_counter + dynamic_requests_counter;
        curr_task->headers.stat_thread_static = static_requests_counter;
        curr_task->headers.stat_thread_dynamic = dynamic_requests_counter;
        curr_task->handler(*curr_task->args, curr_task->headers, &static_requests_counter, &dynamic_requests_counter);
        Close(*(curr_task->args));
        pthread_mutex_lock(&threadPool->mutex);
        threadPool->handled_tasks_num--;
        pthread_cond_signal(&threadPool->taskFinished);
        pthread_mutex_unlock(&threadPool->mutex);
    }
}

void ThreadPoolAddTask(ThreadPool threadPool, Task task, SchedAlg schedAlg){
    pthread_mutex_lock(&threadPool->mutex);
    if(ThreadIsFull(threadPool)){
        if (schedAlg == DropHead) {
            if (listSize((threadPool->waiting_tasks)) == LIST_EMPTY){
                Close(*(task->args));
                pthread_mutex_unlock(&threadPool->mutex);
                return;
            }
            Task to_remove = removeHead(threadPool->waiting_tasks);
            Close(*(to_remove->args));
        }
        else if (schedAlg == DropTail) {
            Close(*(task->args));
            pthread_mutex_unlock(&threadPool->mutex);
            return;
        }
        else if (schedAlg == DropRandom){
            if (listSize((threadPool->waiting_tasks)) == LIST_EMPTY){
                Close(*(task->args));
                pthread_mutex_unlock(&threadPool->mutex);
                return;
            }
            removeRand(threadPool->waiting_tasks);
        }
    }
    addToList(threadPool->waiting_tasks, task);
    pthread_cond_signal(&threadPool->listNotEmpty);
    pthread_mutex_unlock(&threadPool->mutex);
}

ThreadPool ThreadPoolInit(int threads_number, int l_size) {
    ThreadPool threadPool = malloc(sizeof(struct struct_thread_pool));
    threadPool->threads_num = threads_number;
    threadPool->list_size = l_size;
    threadPool->threads = malloc((sizeof(pthread_t))*threads_number);
    threadPool->waiting_tasks = malloc((sizeof(struct struct_task))*(l_size));
    threadPool->waiting_tasks = createList(l_size);
    threadPool->handled_tasks_num = 0;
    pthread_mutex_init(&threadPool->mutex,NULL);
    pthread_cond_init(&(threadPool->listNotEmpty), NULL);
    pthread_cond_init(&(threadPool->taskFinished), NULL);

    //creating threads
    for(int i = 0; i<threads_number; i++){
        ThreadArgs thread_args = malloc(sizeof(struct struct_thread_args));
        thread_args->thread_pool = threadPool;
        thread_args->thread_id = i;
        pthread_create(&(threadPool->threads[i]), NULL, thread_routine, thread_args);
    }
    return threadPool;
}

bool ThreadIsFull(ThreadPool threadPool) {
    return (getCurrSize(threadPool->waiting_tasks) + threadPool->handled_tasks_num >= threadPool->list_size);
}