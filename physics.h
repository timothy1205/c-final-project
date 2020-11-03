#ifndef C_FINAL_PROJECT_PHYSICS_H
#define C_FINAL_PROJECT_PHYSICS_H

#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.h>
#include "utils.h"

static sfVector2f gravity = {.0f, 9.8f};
static float p_time_multiplier = 10.f;

struct Ball {
  sfCircleShape* circleShape;
  sfVector2f vel;
  float energyConserved; // 0 - 1
};

struct Block {
    sfRectangleShape* rectangleShape;
};
typedef struct Ball ball_t;

typedef struct Block block_t;
// Make available in other files
extern list_t* p_balls;
extern list_t* p_blocks;

void p_initialize(void);
void p_update(float* delta);

ball_t* p_ball_create(float radius, sfVector2f pos, sfVector2f vel, float energyConserved, sfColor col);
void p_ball_destroy(ball_t* ball);
void p_ball_bounce(ball_t* ball, sfVector2f direction);
float p_ball_dist_squared(ball_t* ball, ball_t* ball2);

block_t* p_block_create(float angle, sfVector2f size, sfVector2f pos, sfColor col);
void p_block_destroy(block_t* block);

float p_distance_squared_ball_block(ball_t* ball, block_t* block);

#endif //C_FINAL_PROJECT_PHYSICS_H
