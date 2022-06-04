//
// Created by student on 6/4/22.
//
#include "task.h"
#include <stdlib.h>

Task TaskInit(Handle handler, HandleArgs args){
    Task task = malloc(sizeof(*task));
    task->handler= handler;
    task->args=args;
    return task;
}
