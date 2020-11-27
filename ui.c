#include <math.h>
#include "ui.h"

/*
 * Initialize some defaults for our UI data
 */

void ui_initialize(sfRenderWindow* window) {
    s_uiData.window = window;
    s_uiData.state = STATE_WAITING;
    s_uiData.spawnType = OBJECT_BALL; // Default spawn type
}

/*
 * Update the world/ui data (or not) depending on
 * our current state
 */

void ui_update(void) {
    float angle;
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(s_uiData.window);
    sfVector2f relativeMousePos;

    switch (s_uiData.state) {
        case STATE_BALL_FLING:
            break;
        case STATE_BLOCK_ROTATE:
            angle = ui_get_relative_mouse_angle();
            // Not a perfect method, but it works
            sfRectangleShape_setRotation(((block_t*) (s_uiData.object))->rectangleShape, u_rad_to_degrees(angle) + s_uiData.oldAngle );
            break;
        case STATE_BLOCK_MOVE:
            relativeMousePos = ui_get_relative_mouse_pos();
            sfRectangleShape_setPosition(((block_t*) (s_uiData.object))->rectangleShape,
                                         u_vector2f_add(relativeMousePos, s_uiData.oldPos));
            break;
        case STATE_WAITING:
            break;
    }
}

/*
 * Switch to a new state, setup the data needed for said state,
 * and do anything that is necessary to transition from a specific state
 */

void ui_switch_state(UIState state, void* object) {
    // Handle old state first
    if (s_uiData.state == STATE_BALL_FLING) {
        ball_t* ball = ((ball_t*) s_uiData.object);
        // Halve our velocity so a small drag isn't so powerful
        ball->vel = u_vector2f_float_mult(ui_get_relative_mouse_pos(), 0.5f);
    }

    s_uiData.state = state;
    s_uiData.object = object;

    switch (state) {

        case STATE_BALL_FLING:
            break;
        case STATE_BLOCK_ROTATE:
            s_uiData.oldAngle = sfRectangleShape_getRotation(((block_t*) object)->rectangleShape);
            break;
        case STATE_BLOCK_MOVE:
            s_uiData.oldPos = sfRectangleShape_getPosition(((block_t*) object)->rectangleShape);

            break;
        case STATE_WAITING:
            break;
    }

    if (state != STATE_WAITING) {
        s_uiData.oldMousePos = sfMouse_getPositionRenderWindow(s_uiData.window);
    }
}

/*
 * Return UI state
 */

UIState ui_get_state(void) {
    return s_uiData.state;
}

/*
 * Set object spawn type
 */

void ui_set_spawn_type(pObject spawnType) {
    s_uiData.spawnType = spawnType;
}

/*
 * Return current object spawn type
 */

pObject ui_get_spawn_type(void) {
    return s_uiData.spawnType;
}

/*
 * Return the mouse position relative to the stored oldMousePos
 */

sfVector2f ui_get_relative_mouse_pos(void) {
    sfVector2f mousePos = u_vector2i_to_f(sfMouse_getPositionRenderWindow(s_uiData.window));
    mousePos.x -= (float) s_uiData.oldMousePos.x;
    mousePos.y -= (float) s_uiData.oldMousePos.y;

    return mousePos;
}

/*
 * Return the angle between the mouse position and the stored oldMousePos
 */

float ui_get_relative_mouse_angle(void) {
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(s_uiData.window);

    return atan2f((float) (mousePos.y - s_uiData.oldMousePos.y), (float) (mousePos.x - s_uiData.oldMousePos.x));
}