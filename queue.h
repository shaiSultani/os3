//
// Created by student on 6/4/22.
//

#ifndef WEBSERVER_FILES_QUEUE_H
#define WEBSERVER_FILES_QUEUE_H

#include "task.h"
#include "segel.h"
typedef enum {QUE_EMPTY, QUE_FULL,QUE_OK} errorType;

typedef struct struct_queue {
    int size;
    int front;
    int tail;
    Task* tasks;
}* Queue;

Queue createQueue(int size);
errorType enqueue(Queue que, Task task);
Task dequeue(Queue que);




#endif //WEBSERVER_FILES_QUEUE_H
