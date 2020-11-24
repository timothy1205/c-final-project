#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include "constants.h"
#include "physics.h"
#include "render.h"
#include "ui.h"

void HandleInput(sfRenderWindow* window, sfEvent event);

int main() {
    srand(time(NULL));

    // Global font that will be used for all text
    sfFont* font = sfFont_createFromFile("./resources/fonts/RobotoMono-Regular.ttf");
    if (!font) {
        u_resource_failure();
    }

    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode){(unsigned int) WIDTH, (unsigned int) HEIGHT}, "c-final-project",  sfDefaultStyle, NULL);

    p_initialize();

    sfClock* clock = sfClock_create();
    sfText* fps = sfText_create();
    sfText_setFont(fps, font);
    sfText_setCharacterSize(fps, 20);
    char fps_str[50];
    while(sfRenderWindow_isOpen(window)) {
        sfTime delta = sfClock_restart(clock);
        float deltaSeconds = sfTime_asSeconds(delta) * p_time_multiplier;

        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event.type == sfEvtResized) {
                // TODO: Is this even needed?
            }

            HandleInput(window, event);
        }

        sprintf(fps_str, "FPS: %.0lf", 1/deltaSeconds);
        sfText_setString(fps, fps_str);

        ui_update();

        sfRenderWindow_clear(window, sfBlack);
        p_update(&deltaSeconds);
        r_render(window);

        sfRenderWindow_drawText(window, fps, NULL);
        sfRenderWindow_display(window);
    }
    return 0;
}

void HandleInput(sfRenderWindow* window, sfEvent event) {
    if (event.type == sfEvtMouseButtonPressed) {
        if (event.mouseButton.button == sfMouseLeft) {
            // Check if clicking on an object or the void
            pObject type;
            void *object = p_get_object_at_pos(sfMouse_getPositionRenderWindow(window), &type);
            switch (type) {
                case OBJECT_BALL:
                    // Fling ball
                    printf("Flinging ball...\n");
                    ui_switch_state(STATE_BALL_FLING, object);
                    break;
                case OBJECT_BLOCK:
                    // Move block
                    printf("Moving block...\n");
                    ui_switch_state(STATE_BLOCK_MOVE, object);
                    break;
                case OBJECT_NONE:
                    // Create object
                    printf("Creating object...\n");
                    if (ui_get_spawn_type() == OBJECT_BALL) {
                        p_ball_create(u_rand_range(10, 30),
                                      u_vector2i_to_f(sfMouse_getPositionRenderWindow(window)),
                                      (sfVector2f) {0, 0},
                                      u_rand_range(0, 1),
                                      u_rand_color());
                    } else {
                        p_block_create(0.f,
                                       (sfVector2f) {u_rand_range(30, 100), u_rand_range(30, 100)},
                                       u_vector2i_to_f(sfMouse_getPositionRenderWindow(window)),
                                       u_rand_color());
                    }
                    break;
                default:
                    break;
            }
        } else if (event.mouseButton.button == sfMouseRight) {
            pObject type;
            void *object = p_get_object_at_pos(sfMouse_getPositionRenderWindow(window), &type);
            if (type == OBJECT_BLOCK) {
                printf("Rotating block...\n");
                ui_switch_state(STATE_BLOCK_ROTATE, object);
            }
        }
    } else if (event.type == sfEvtMouseButtonReleased) {
        if (event.mouseButton.button == sfMouseLeft) {
            switch (ui_get_state()) {
                case STATE_BALL_FLING:
                    printf("Stop flinging ball...\n");
                    ui_switch_state(STATE_WAITING, NULL);
                    break;
                case STATE_BLOCK_MOVE:
                    printf("Stop moving block...\n");
                    ui_switch_state(STATE_WAITING, NULL);
                    break;
                case STATE_BLOCK_ROTATE:
                    break;
                case STATE_WAITING:
                    break;
            }
        } else if (event.mouseButton.button == sfMouseRight) {
            if (ui_get_state() == STATE_BLOCK_ROTATE) {
                printf("Stop rotating block...\n");
                ui_switch_state(STATE_WAITING, NULL);
            }
        }
    } else if (event.type == sfEvtKeyPressed) {
        if (event.key.code == sfKeyB) {
            // Set spawner
            if (event.key.shift) {
                // Set spawner to blocks
                ui_set_spawn_type(OBJECT_BLOCK);
            } else {
                // Set Spawner to balls
                ui_set_spawn_type(OBJECT_BALL);
            }
        }
    }
}