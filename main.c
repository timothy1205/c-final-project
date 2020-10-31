#include <stdio.h>
#include <SFML/Graphics.h>
#include "constants.h"
#include "physics.h"
#include "render.h"


int main() {
    // Global font that will be used for all text
    sfFont* font = sfFont_createFromFile("./resources/fonts/RobotoMono-Regular.ttf");
    if (!font) {
        u_resource_failure();
    }

    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode){(unsigned int) WIDTH, (unsigned int) HEIGHT}, "c-final-project",  sfDefaultStyle, NULL);

    p_initialize();
    p_ball_create(10.f, (sfVector2f) {50.f, 500.f}, (sfVector2f) {50.f, -50.f}, 0.75f, sfRed);
    p_ball_create(10.f, (sfVector2f) {10.f, 500.f}, (sfVector2f) {50.f, -50.f}, 1.f, sfBlue);

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
        }

        sprintf(fps_str, "FPS: %.0lf", 1/deltaSeconds);
        sfText_setString(fps, fps_str);

        sfRenderWindow_clear(window, sfBlack);
        p_update(&deltaSeconds);
        r_render(window);
        sfRenderWindow_drawText(window, fps, NULL);
        sfRenderWindow_display(window);
    }
    return 0;
}
