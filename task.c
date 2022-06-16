#include "task.h"

Task TaskInit(Handle handler, HandleArgs args){
    Task task = malloc(sizeof(*task));
    task->handler= handler;
    task->args=args;
    return task;
}
