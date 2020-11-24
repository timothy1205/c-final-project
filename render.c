#include "render.h"

/*
 * Render all balls
 * Intentionally not in header file
 */

void r_render_balls(sfRenderWindow* window) {
    node_t* node = p_balls->head;

    while(node) {
        ball_t* ball = (ball_t*) node->val;
        sfRenderWindow_drawCircleShape(window, ball->circleShape, NULL);
        node = node->next;
    }
}

/*
 * Render all blocks
 * Intentionally not in header file
 */

void r_render_blocks(sfRenderWindow* window) {
    node_t* node = p_blocks->head;

    while(node) {
        block_t* block = (block_t*) node->val;
        sfRenderWindow_drawRectangleShape(window, block->rectangleShape, NULL);
        node = node->next;
    }
}

/*
 * Render all objects
 */

void r_render(sfRenderWindow* window) {
    r_render_blocks(window);
    r_render_balls(window);
}