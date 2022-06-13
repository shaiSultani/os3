#include "list.h"

List createList(int limit){
    List list = malloc(sizeof (*list));
    if(!list)
        return NULL;
    list->size = 0;
    list->limit = limit;
    list->head = malloc(sizeof(list_node));
    list->head->prev = NULL;
    list->head->next = NULL;
    list->head->task = NULL;
    list->tail = NULL;
    return list;
}

list_size addToList(List list, Task task) {
    list_node *new_node = malloc(sizeof(list_node));
    new_node->task = task;
    if(list->size == 0){
        new_node->prev = list->head;
        list->head->next = new_node;
    }
    else{
        new_node->prev = list->tail;
        list->tail->next = new_node;
    }
    new_node->next = NULL;
    list->tail = new_node;
    list->size++;
    return LIST_OK;
}

Task removeHead(List list) {
    Task target = list->head->next->task;
    if (list->size == 1){
        list->tail = NULL;
        list->head->next = NULL;
    }
    else{
        list->head->next->next->prev = list->head;
        list->head->next = list->head->next->next;
    }
    list->size--;
    return target;
}

void removeRand(List list) {
    for (int i = 0; ceil(list->size / 10 * 3); i++) {
        if (list->size == 0) return;
        int j = 1;
        int rand_task = rand() % list->size;
        if (rand_task == 0 || rand_task == 1) {
            Close(*list->head->next->task->args);
            removeHead(list);
        } else {
            list_node *current = list->head;
            while (j < rand_task) {
                current = current->next;
                j++;
            }
            Close(*current->task->args);
            if (current->next){
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            else{
                current->prev->next = NULL;
            }
            list->size--;
        }
    }
}

list_size listSize(List list) {
    if (list->size == 0) return LIST_EMPTY;
    if (list->size == list->limit) return LIST_FULL;
    else return LIST_OK;
}

int getCurrSize(List list){
    return list->size;
}
