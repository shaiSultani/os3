//
// Created by student on 6/4/22.
//

#ifndef WEBSERVER_FILES_TASK_H
#define WEBSERVER_FILES_TASK_H
#include "request.h"

typedef void (*Handle)(int);
typedef int* HandleArgs;
typedef struct struct_task{
    Handle handler;
    HandleArgs args;
}*Task;

Task TaskInit(Handle handler, HandleArgs args);

#endif //WEBSERVER_FILES_TASK_H
