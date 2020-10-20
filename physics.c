#include "physics.h"

// Declare linked list pointers
list_t* p_balls = NULL;
list_t* p_blocks = NULL;

/*
 * Initialize physics portion of program
 */

void p_initialize(void) {
    p_balls = u_list_create();
    p_blocks = u_list_create();
}

/*
 * Create and return a ball
 */

ball_t* p_ball_create(float radius, sfVector2f pos, sfVector2f vel, sfColor col) {
    // Allocate space for ball
    ball_t* ball = (ball_t*) malloc(sizeof(ball_t));
    if (!ball) {
        u_allocate_failure();
        return NULL;
    }

    ball->vel = vel;
    // Create circle shape from sfml
    ball->circleShape = sfCircleShape_create();
    sfCircleShape_setRadius(ball->circleShape, radius);
    sfCircleShape_setPosition(ball->circleShape, pos);
    sfCircleShape_setFillColor(ball->circleShape, col);

    u_list_add(p_balls, ball);

    return ball;
}

/*
 * Destroy a ball
 */

void p_ball_destroy(ball_t* ball) {
    node_t* node = u_list_find(p_balls, ball);
    if (node) u_list_remove(p_balls, node);
}

/*
 *  Create and return block
 */

block_t* p_block_create(float angle, sfVector2f size,  sfVector2f pos, sfColor col) {
    block_t* block = (block_t*) malloc(sizeof(block_t));
    if (!block) {
        u_allocate_failure();
        return NULL;
    }

    block->rectangleShape = sfRectangleShape_create();
    sfRectangleShape_setOrigin(block->rectangleShape, (sfVector2f){size.x / 2, size.y / 2});
    sfRectangleShape_setRotation(block->rectangleShape, angle);
    sfRectangleShape_setSize(block->rectangleShape, size);
    sfRectangleShape_setPosition(block->rectangleShape, pos);
    sfRectangleShape_setFillColor(block->rectangleShape, col);

    u_list_add(p_blocks, block);

    return block;
}

/*
 * Destroy a block
 */

void p_block_destroy(block_t* block) {
    node_t* node = u_list_find(p_blocks, block);
    if (node) u_list_remove(p_balls, node);
}