//
// Created by student on 6/4/22.
//
#include "threadPool.h"

//args for thread_routine
typedef struct struct_thread_args{
    ThreadPool pool;
} *ThreadArgs;

void* thread_routine(void* args) {
    ThreadArgs thread_args = (ThreadArgs)args;
    ThreadPool threadPool = thread_args->pool;

    while(1) {
        pthread_mutex_lock(&threadPool->mutex);
        while(threadPool->waiting_tasks_num==0){
            pthread_cond_wait(&threadPool->qNotEmpty, &threadPool->mutex);
        }
        Task curr_task = dequeue(threadPool->waiting_tasks);
        if(curr_task!=NULL){
            threadPool->waiting_tasks_num--;
            threadPool->handled_tasks[threadPool->handled_tasks_num]=curr_task;
            threadPool->handled_tasks_num++;
            pthread_mutex_unlock(&threadPool->mutex);
            //do_work
            curr_task->handler(*curr_task->args);
            Close(*curr_task->args);
            //work_done
            pthread_mutex_lock(&threadPool->mutex);
            threadPool->handled_tasks_num--;
            pthread_cond_signal(&threadPool->taskFinished);
            pthread_mutex_unlock(&threadPool->mutex);
        }
    }
}

ThreadPool ThreadPoolInit(int threads_number, int q_size) {
    ThreadPool threadPool = malloc(sizeof(struct struct_thread_pool));
    threadPool->threads_num = threads_number;
    threadPool->queue_size = q_size;
    threadPool->threads= malloc((sizeof(pthread_t))*threads_number);
    Queue waiting_queue = createQueue(q_size);
    threadPool->waiting_tasks = waiting_queue;
    threadPool->handled_tasks = malloc((sizeof(struct struct_task))*threads_number);
    threadPool->waiting_tasks_num=0;
    threadPool->handled_tasks_num=0;
    pthread_mutex_init(&threadPool->mutex,NULL);
    pthread_cond_init(&(threadPool->qNotEmpty), NULL);
    pthread_cond_init(&(threadPool->taskFinished), NULL);

    //creating threads
    for(int i = 0; i<threads_number; i++){
        ThreadArgs thread_args = malloc(sizeof(struct struct_thread_args));
        thread_args->pool=threadPool;
        pthread_create(&(threadPool->threads[i]), NULL, thread_routine, thread_args);
    }
    return threadPool;
}

void ThreadPoolAddWaitingTask(ThreadPool threadPool, Task task){
    pthread_mutex_lock(&threadPool->mutex);
    if(threadPool->waiting_tasks_num + threadPool->handled_tasks_num >= threadPool->queue_size){ //threadPool is full
        //todo: part 2: overload handling
    }
    if(enqueue(threadPool->waiting_tasks,task) == QUE_OK){
        threadPool->waiting_tasks_num++;
    //if(threadPool->waiting_tasks_num == 1) //waiting_tasks_array was empty
        pthread_cond_signal(&threadPool->qNotEmpty);
        pthread_mutex_unlock(&threadPool->mutex);
    }

}
