#ifndef C_FINAL_PROJECT_UTILS_H
#define C_FINAL_PROJECT_UTILS_H

#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    struct node* prev;
    struct node* next;
    void* val;
} node_t;

typedef struct list {
    node_t* head;
    node_t* tail;
    unsigned int size;
} list_t;


void u_allocate_failure(void);
void u_free_resources(void);

list_t* u_list_create(void);
void u_list_destroy(list_t* list);
void u_list_add(list_t* list, void* val);
void u_list_remove(list_t* list, node_t* node);
node_t* u_list_find(list_t* list, void* val);

node_t* u_node_create(void* val);
void u_node_destroy(node_t* node);

#endif //C_FINAL_PROJECT_UTILS_H
