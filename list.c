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

list_size addToList(List list, Task* task) {
    if (list->size == list->limit){
        return LIST_FULL;
    }
    list_node *new_node = malloc(sizeof(list_node));
    new_node->task = task;
    if(!list->size){
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

Task* removeTail(List list){
    Task* target = list->tail->task;
    if (list->size == 1){
        list->tail = NULL;
        list->head->next = NULL;
    }
    else{
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    list->size--;
    return target;
}

Task* removeHead(List list) {
    Task* target = list->head->next->task;
    if (list->size == 1){
        list->tail == NULL;
        list->head->next = NULL;
    }
    else{
        list->head->next = list->head->next->next;
        list->head->next->prev = list->head;
    }
    list->size--;
    return target;
}

list_size removeRand(List list ){
    int target_size;
    for (int i=0 ; ceil(list->size/10*3) ; i++){
        int j = 1;
        int rand_task = rand() % list->size;
        if (rand_task == 0 || rand_task == 1){
            removeHead(list);
        }
        else if (rand_task == list->size -1){
            removeTail(list);
        }
        else{
            list_node* current = list->head;
            while (j<rand_task){
                current = current->next;
                j++;
            }
            current->prev->next = current->next;
            current->next->prev = current->next;
        }
    }
}

list_size listSize(List list) {
    if (list->size == 0) return LIST_EMPTY;
    if (list->size == list->limit) return LIST_FULL;
    else return LIST_OK;
}
