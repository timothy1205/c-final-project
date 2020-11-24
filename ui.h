#ifndef C_FINAL_PROJECT_UI_H
#define C_FINAL_PROJECT_UI_H

#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include "physics.h"

typedef enum uistate_enum {STATE_BALL_FLING, STATE_BLOCK_ROTATE, STATE_BLOCK_MOVE, STATE_WAITING} UIState;

static struct UIData_struct {
    sfRenderWindow* window;
    UIState state;
    sfVector2i oldMousePos;
    pObject spawnType;
    void* object;
    sfVector2f oldPos;
    float oldAngle;
} s_uiData;

void ui_initialize(sfRenderWindow* window);
void ui_update(void);
void ui_switch_state(UIState state, void* object);
UIState ui_get_state(void);
void ui_set_spawn_type(pObject spawnType);
pObject ui_get_spawn_type(void);
sfVector2f ui_get_relative_mouse_pos(void);
float ui_get_relative_mouse_angle(void);

#endif //C_FINAL_PROJECT_UI_H
