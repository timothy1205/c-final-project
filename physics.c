#include "physics.h"
#include "constants.h"
#include "math.h"

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
 * Check for screen border collisions
 * Intentionally not in header file
 */

void p_check_borders(ball_t* ball) {
    sfVector2f pos = sfCircleShape_getPosition(ball->circleShape);
    float radius = sfCircleShape_getRadius(ball->circleShape);

    if (pos.x - radius < 0) {
        // Left
        p_ball_bounce(ball, (sfVector2f) {-1, 0});
        pos.x = radius;
        sfCircleShape_setPosition(ball->circleShape, pos);
    } else if (pos.x + radius > WIDTH) {
        // Right
        p_ball_bounce(ball, (sfVector2f) {-1, 0});
        pos.x = WIDTH - radius;
        sfCircleShape_setPosition(ball->circleShape, pos);
    }

    if (pos.y - radius < 0) {
        // Top
        p_ball_bounce(ball, (sfVector2f) {0, -1});
        pos.y = radius;
        sfCircleShape_setPosition(ball->circleShape, pos);
    } else if (pos.y + radius > HEIGHT) {
        // Bottom
        if (fabsf(ball->vel.y) < MIN_BOUNCE_VEL) {
            // Only bounce if velocity >= MIN_BOUNCE_VEL
            // otherwise
            ball->vel.y = 0.f;
        } else {
            p_ball_bounce(ball, (sfVector2f) {0, -1});
        }
        pos.y = HEIGHT - radius;
        sfCircleShape_setPosition(ball->circleShape, pos);
    }
}

/*
 * Check for collisions between balls
 * Intentionally not in header file
 */

void p_check_ball_collisions(ball_t* ball) {
    node_t* node = p_balls->head;

    while(node) {
        ball_t* ball2 = (ball_t*) node->val;

        if (ball != ball2) {
            // Different balls
            float radius = sfCircleShape_getRadius(ball->circleShape);
            float radius2 = sfCircleShape_getRadius(ball2->circleShape);
            float distSquared = p_ball_dist_squared(ball, ball2);

            if (distSquared < (radius + radius2) * (radius + radius2)) {
                sfVector2f pos = sfCircleShape_getPosition(ball->circleShape);
                sfVector2f pos2 = sfCircleShape_getPosition(ball2->circleShape);
                // Collision

                // Angle between circles
                float angle = atan2f(pos2.y - pos.y, pos2.x - pos.x);
                float angle_flipped = fmodf(angle + (float) M_PI, (float) M_PI);
                float moveDist = (radius + radius2 - sqrtf(distSquared)) / 2;

                sfCircleShape_move(ball->circleShape, (sfVector2f) {moveDist * cosf(angle_flipped), moveDist * sinf(angle_flipped)});
                sfCircleShape_move(ball2->circleShape, (sfVector2f) {moveDist * cosf(angle), moveDist * sinf(angle)});
                p_ball_bounce(ball, (sfVector2f) {cosf(angle), sinf(angle)});
                p_ball_bounce(ball2, (sfVector2f) {cosf(angle), sinf(angle)});
            }
        }
        node = node->next;
    }

}

/*
 * Update all balls
 * Intentionally not in header file
 */

void p_update_balls(const float* delta) {
    node_t* node = p_balls->head;

    while(node) {
        ball_t* ball = (ball_t*) node->val;

        p_check_borders(ball);
        p_check_ball_collisions(ball);

        sfCircleShape_move(ball->circleShape, u_vector2f_float_mult(ball->vel, *delta));
        ball->vel = u_vector2f_add(ball->vel, u_vector2f_float_mult(gravity, *delta));

        node = node->next;
    }
}

/*
 * Update world
 */

void p_update(float* delta) {
    p_update_balls(delta);
}

/*
 * Create and return a ball
 */

ball_t* p_ball_create(float radius, sfVector2f pos, sfVector2f vel, float energyConserved, sfColor col) {
    // Allocate space for ball
    ball_t* ball = (ball_t*) malloc(sizeof(ball_t));
    if (!ball) {
        u_allocate_failure();
        return NULL;
    }
    ball->vel = vel;
    ball->energyConserved = energyConserved;

    // Create circle shape from sfml
    ball->circleShape = sfCircleShape_create();
    sfCircleShape_setOrigin(ball->circleShape, (sfVector2f) {radius, radius});
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
 * Adjust balls velocity to "bounce" in given direction
 */

void p_ball_bounce(ball_t* ball, sfVector2f direction) {
    if (direction.x != 0)
        ball->vel.x *= direction.x * ball->energyConserved;
    if (direction.y != 0)
        ball->vel.y *= direction.y * ball->energyConserved;
}

/*
 * Return the distance squared between two balls
 */

float p_ball_dist_squared(ball_t* ball, ball_t* ball2) {
    sfVector2f pos = sfCircleShape_getPosition(ball->circleShape);
    sfVector2f pos2 = sfCircleShape_getPosition(ball2->circleShape);

    return (pos2.x - pos.x) * (pos2.x - pos.x) + (pos2.y - pos.y) * (pos2.y - pos.y);
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