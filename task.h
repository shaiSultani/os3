#ifndef WEBSERVER_FILES_TASK_H
#define WEBSERVER_FILES_TASK_H
#include "request.h"
#include "segel.h"

typedef void (*Handle)(int);
typedef int* HandleArgs;
typedef struct struct_task{
    Handle handler;
    HandleArgs args;
    Headers headers;
}*Task;

Task TaskInit(Handle handler, HandleArgs args);

#endif //WEBSERVER_FILES_TASK_H
