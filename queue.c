#include "queue.h"

Queue createQueue(int size){
    Task* new_tasks= malloc(sizeof(Task)*size);
    if(!new_tasks)
        return NULL;
    Queue que = malloc(sizeof (*que));
    if(!que)
        return NULL;
    que->tasks = new_tasks;
    que->size = size;
    que->front=-1;
    que->tail=-1;
    return que;
};
errorType enqueue(Queue que, Task task){
    if(que->tail == que->size-1)
        return QUE_FULL;
    if(que->front == -1){
        que->front =0;
    }
    que->tail++;
    que->tasks[que->tail]=task;
    return QUE_OK;
}
Task dequeue(Queue que){
    if(que->front == -1)
        return NULL;
    que->front++;
    if(que->front > que->tail)
        que->front = que->tail = -1;
    return que->tasks[que->tail];
}