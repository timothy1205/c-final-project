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
            float distSquared = p_distance_squared_ball(ball, ball2);

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
 * Check for collisions between balls and blocks
 * Intentionally not in header file
 */

void p_check_block_collisions(ball_t* ball) {
    sfVector2f closestPos;

    node_t* node = p_blocks->head;

    while(node) {
        block_t* block = (block_t*) node->val;
        sfVector2f blockPos = sfRectangleShape_getPosition(block->rectangleShape);
        sfVector2f blockSize = sfRectangleShape_getSize(block->rectangleShape);
        float blockAngle =  sfRectangleShape_getRotation(block->rectangleShape);


        sfVector2f circlePos = sfCircleShape_getPosition(ball->circleShape);
        float circleRadius = sfCircleShape_getRadius(ball->circleShape);

        // http://www.migapro.com/circle-and-rotated-rectangle-collision-detection/
        // Circle rotated around block's center by block's angle
        sfVector2f unrotatedCirclePos = u_rotate_around_point(circlePos, blockPos, u_degrees_to_rad(-blockAngle));

        // Calculate point on block closest to ball
        if (unrotatedCirclePos.x < blockPos.x - blockSize.x / 2)
            closestPos.x = blockPos.x - blockSize.x / 2;
        else if (unrotatedCirclePos.x > blockPos.x + blockSize.x / 2)
            closestPos.x = blockPos.x + blockSize.x / 2;
        else
            closestPos.x = unrotatedCirclePos.x;

        if (unrotatedCirclePos.y < blockPos.y - blockSize.y / 2)
            closestPos.y = blockPos.y - blockSize.y / 2;
        else if (unrotatedCirclePos.y > blockPos.y + blockSize.y / 2)
            closestPos.y = blockPos.y + blockSize.y / 2;
        else
            closestPos.y = unrotatedCirclePos.y;

        float distSquared = u_distance_squared(unrotatedCirclePos, closestPos);
        if (distSquared < circleRadius * circleRadius) {
            // Collision
            // Rotate our closestPos to match block's angle
            sfVector2f rotatedClosestPos = u_rotate_around_point(closestPos, blockPos, u_degrees_to_rad(blockAngle));
            float angle = atan2f(rotatedClosestPos.y - circlePos.y, rotatedClosestPos.x - circlePos.x);
            float iptr;
            float angle_flipped = modff(angle + (float) M_PI, &iptr) + iptr;

            float moveDist = (circleRadius - sqrtf(distSquared));
            sfCircleShape_move(ball->circleShape, (sfVector2f) {moveDist * cosf(angle_flipped), moveDist * sinf(angle_flipped)});
            p_ball_bounce_magnitude(ball, (sfVector2f) {cosf(angle_flipped), sinf(angle_flipped)});
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
        p_check_block_collisions(ball);

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
 * Delete saved physics objects
 */

void p_free_resources(void) {
    node_t* node = p_balls->head;

    while(node) {
        p_ball_destroy((ball_t*) node->val);

        node = node->next;
    }
    u_list_destroy(p_balls);

    node = p_blocks->head;

    while(node) {
        p_block_destroy((block_t*) node->val);

        node = node->next;
    }
    u_list_destroy(p_blocks);
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
    sfCircleShape_destroy(ball->circleShape);

    node_t* node = u_list_find(p_balls, ball);
    if (node) u_list_remove(p_balls, node);

}

/*
 * Return pointer to object at position or NULL
 * Parameter type is set to object type
 */

void* p_get_object_at_pos(sfVector2i pos, pObject* type) {
    sfVector2f posf = u_vector2i_to_f(pos);

    // Check blocks
    node_t* node = p_blocks->head;
    while(node) {
        block_t *block = (block_t *) node->val;
        float blockAngle = sfRectangleShape_getRotation(block->rectangleShape);
        sfRectangleShape_setRotation(block->rectangleShape, 0);

        sfVector2f blockPos = sfRectangleShape_getPosition(block->rectangleShape);
        sfVector2f blockSize = sfRectangleShape_getSize(block->rectangleShape);
        sfVector2f unrotatedPos = u_rotate_around_point(posf, blockPos, u_degrees_to_rad(-blockAngle));

        if (unrotatedPos.x > blockPos.x - blockSize.x / 2 && unrotatedPos.x < blockPos.x + blockSize.x / 2
            && unrotatedPos.y > blockPos.y - blockSize.y / 2 && unrotatedPos.y < blockPos.y + blockSize.y / 2) {
            sfRectangleShape_setRotation(block->rectangleShape, blockAngle);
            *type = OBJECT_BLOCK;
            return node->val;
        }

        sfRectangleShape_setRotation(block->rectangleShape, blockAngle);
        node = node->next;;
    }

    // Check balls
    node = p_balls->head;
    while(node) {
        ball_t *ball = (ball_t *) node->val;
        sfVector2f ballPos = sfCircleShape_getPosition(ball->circleShape);
        float radius = sfCircleShape_getRadius(ball->circleShape);
        float distSquared = u_distance_squared(posf, ballPos);

        if (distSquared < radius * radius) {
            *type = OBJECT_BALL;
            return node->val;
        }

        node = node->next;
    }

    *type = OBJECT_NONE;
    return NULL;
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
 * Incorporate overall velocity magnitude (combined components) into final
 * bounce velocity
 */

void p_ball_bounce_magnitude(ball_t* ball, sfVector2f direction) {
    float magnitude = sqrtf(ball->vel.x * ball->vel.x + ball->vel.y * ball->vel.y);

    ball->vel.x = magnitude * direction.x * ball->energyConserved;
    ball->vel.y = magnitude * direction.y * ball->energyConserved;
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
    sfRectangleShape_destroy(block->rectangleShape);

    node_t* node = u_list_find(p_blocks, block);
    if (node) u_list_remove(p_blocks, node);

}

/*
 * Wrapper function to return distance between two balls
 */

float p_distance_squared_ball(ball_t* ball, ball_t* ball2) {
    return u_distance_squared(sfCircleShape_getPosition(ball->circleShape),
                              sfCircleShape_getPosition(ball2->circleShape));;
}