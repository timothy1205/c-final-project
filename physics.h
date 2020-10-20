#ifndef C_FINAL_PROJECT_PHYSICS_H
#define C_FINAL_PROJECT_PHYSICS_H

#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.h>
#include "utils.h"

struct Ball {
  sfCircleShape* circleShape;
  sfVector2f vel;
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

ball_t* p_ball_create(float radius, sfVector2f pos, sfVector2f vel, sfColor col);
void p_ball_destroy(ball_t* ball);

block_t* p_block_create(float angle, sfVector2f size, sfVector2f pos, sfColor col);
void p_block_destroy(block_t* block);

#endif //C_FINAL_PROJECT_PHYSICS_H
