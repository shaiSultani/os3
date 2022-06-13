//
// Created by student on 6/4/22.
//

#ifndef WEBSERVER_FILES_LIST_H
#define WEBSERVER_FILES_LIST_H

#include "task.h"
#include "segel.h"
typedef enum {LIST_EMPTY, LIST_FULL, LIST_OK} list_size;

typedef struct list_node_t {
    Task task;
    struct list_node_t *next;
    struct list_node_t *prev;
} list_node;

typedef struct list_t {
    int size;
    int limit;
    list_node* head;
    list_node* tail;
}* List;

List createList(int limit);
list_size addToList(List, Task);
list_size listSize(List);
Task removeHead(List list);
void removeRand(List);
int getCurrSize(List list);

#endif //WEBSERVER_FILES_LIST_H
