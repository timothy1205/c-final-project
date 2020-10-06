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

typedef struct Ball ball_t;

extern list_t* p_balls;
extern list_t* p_blocks;

void p_initialize(void);

ball_t* p_ball_create(float radius, sfVector2f pos, sfVector2f vel, sfColor col);
void p_ball_destroy(ball_t* ball);

#endif //C_FINAL_PROJECT_PHYSICS_H
