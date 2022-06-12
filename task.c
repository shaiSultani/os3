#include "task.h"

Task TaskInit(Handle handler, HandleArgs args){
    Task task = malloc(sizeof(*task));
    task->handler= handler;
    gettimeofday(&task->headers.stat_req_arrival, NULL);
    task->args=args;
    return task;
}
