#include "utils.h"

void u_allocate_failure(void) {
    fprintf(stderr, "Failed to allocate space\n");
    u_free_resources();
    exit(1);
}

void u_free_resources(void) {
    // TODO: Free everything, probably split into sub-functions in other files
}

/*
 * Create an empty list
 */

list_t* u_list_create(void) {
    list_t* list = malloc(sizeof(list_t));
    if (!list) {
        u_allocate_failure();
        return NULL;
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

/*
 * Recursively destroy nodes in a list
 * Function is intentionally not in header
 */

void list_destroy_r(node_t* node) {
    if (!node) return;

    list_destroy_r(node->next);
    u_node_destroy(node);
}

/*
 * Destroy a list
 */

void u_list_destroy(list_t* list) {
    list_destroy_r(list->head);
    free(list);
}

/*
 * Create and add node with given value to a list
 */

void u_list_add(list_t* list, void* val) {
    node_t* node = u_node_create(val);

    if (list->size) {
        // Existing node(s)
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    } else {
        // Empty
        list->head = node;
        list->tail = node;
    }

    list->size++;
}

/*
 *  Remove a node from a list
 *  I opted to skip sanity check to see if node is actually in list
 */

void u_list_remove(list_t* list, node_t* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    u_node_destroy(node);
    list->size--;
}

node_t* u_list_find(list_t* list, void* val) {
    node_t* node = list->head;
    while (node) {
        if (node->val == val) return node;
        node = node->next;
    }

    return NULL;
}


node_t* u_node_create(void* val) {
    node_t* node = malloc(sizeof(node_t));
    if (!node) {
        u_allocate_failure();
        return NULL;
    }

    node->val = val;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void u_node_destroy(node_t* node) {
    free(node->val);
    free(node);
}

/*
 * Add two vectors, updating and returning the first
 */

sfVector2f u_vector2f_add(sfVector2f vector1, sfVector2f vector2) {
    vector1.x += vector2.x;
    vector1.y += vector2.y;

    return vector1;
}

/*
 * Apply a multiplier to a vector, updating vector and returning
 */

sfVector2f u_vector2f_float_mult(sfVector2f vector, float multiplier) {
//    printf("%.1lf\n", multiplier);
    vector.x *= multiplier;
    vector.y *= multiplier;

    return vector;
}