#include "utils.h"
#include "math.h"
#include "physics.h"

void u_allocate_failure(void) {
    fprintf(stderr, "Failed to allocate space, quitting...\n");
    u_free_resources();
    exit(1);
}

/*
 * Failed to load resources
 */

void u_resource_failure(void) {
    fprintf(stderr, "Failed to load resources, quitting...\n");
    u_free_resources();
    exit(1);
}

void u_free_resources(void) {
    p_free_resources();
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
    node->val = NULL;
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

/*
 * Convert degrees to radians
 */

float u_degrees_to_rad(float deg) {
    return deg * ((float) M_PI / 180.f);
}

/*
 * Convert radians to degrees
 */

float u_rad_to_degrees(float rad) {
    return rad * 180.f / (float) M_PI;
}

/*
 * Get the "real" angle (in radians) from SFML degrees
 * SFML defines it's rotation as clockwise degrees
 */

float u_real_angle(float deg) {
    return u_degrees_to_rad(deg);
}

/*
 * Flip an angle (in radians) by adding PI (180 deg)
 */

float u_flip_angle(float rad) {
    float iptr;
    return modff(rad + (float) M_PI, &iptr) + iptr;
}

/*
 * Rotate from point by angle around origin
 */

sfVector2f u_rotate_around_point(sfVector2f point, sfVector2f origin, float angle) {
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

//    // Translate to origin
//    point.x -= origin.x;
//    point.y -= origin.y;
//
//    // Rotate
//    float xRotated = point.x * cosAngle + point.y * sinAngle;
//    float yRotated = point.x * sinAngle + point.y * cosAngle;
//
//    // Translate back
//    point.x = xRotated + origin.x;
//    point.y = yRotated + origin.y;
//
//    return point;

    float xRotated = cosAngle * (point.x - origin.x) -
            sinAngle * (point.y - origin.y) + origin.x;
    float yRotated = sinAngle * (point.x - origin.x) +
            cosAngle * (point.y - origin.y) + origin.y;

    return (sfVector2f) {xRotated, yRotated};
}

/*
 * Return distance between two points
 */

float u_distance_squared(sfVector2f from, sfVector2f to) {
    float x = fabsf(from.x - to.x);
    float y = fabsf(from.y - to.y);

    return (x * x) + (y * y);
}

/*
 * Convert sfVector2i to sfVector2f
 */

sfVector2f u_vector2i_to_f(sfVector2i vector) {
    sfVector2f vectorf = {(float) vector.x, (float) vector.y};

    return vectorf;
}

float u_rand_range(float min, float max) {
    return (float) rand() / (float) RAND_MAX * (max - min) + min;
}

sfColor u_rand_color(void) {
    return sfColor_fromRGB(rand() % 255, rand() % 255, rand() % 255);
}