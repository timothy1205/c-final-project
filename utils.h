#ifndef C_FINAL_PROJECT_UTILS_H
#define C_FINAL_PROJECT_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.h>

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
void u_resource_failure(void);
void u_free_resources(void);

list_t* u_list_create(void);
void u_list_destroy(list_t* list);
void u_list_add(list_t* list, void* val);
void u_list_remove(list_t* list, node_t* node);
node_t* u_list_find(list_t* list, void* val);

node_t* u_node_create(void* val);
void u_node_destroy(node_t* node);

sfVector2f u_vector2f_add(sfVector2f vector1, sfVector2f vector2);
sfVector2f u_vector2f_float_mult(sfVector2f vector, float multiplier);

float u_degrees_to_rad(float deg);
float u_rad_to_degrees(float rad);
float u_real_angle(float deg);
float u_flip_angle(float rad);
sfVector2f u_rotate_around_point(sfVector2f point, sfVector2f origin, float angle);

float u_distance_squared(sfVector2f from, sfVector2f to);

sfVector2f u_vector2i_to_f(sfVector2i vector);

float u_rand_range(float min, float max);

sfColor u_rand_color(void);

#endif //C_FINAL_PROJECT_UTILS_H
