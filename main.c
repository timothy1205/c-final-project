#include <stdio.h>
#include <SFML/Graphics.h>
#include "physics.h"
#include "render.h"

#define WIDTH 1920.0f
#define HEIGHT 1080.0f

int main() {
    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode){(unsigned int) WIDTH, (unsigned int) HEIGHT}, "c-final-project",  sfDefaultStyle, NULL);
    const sfView* view = sfRenderWindow_getDefaultView(window);

    p_initialize();
    p_ball_create(10.f, (sfVector2f) {0.f, 0.f}, (sfVector2f) {50.f, 0.f}, sfRed);

    sfClock* clock = sfClock_create();
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

        sfRenderWindow_clear(window, sfBlack);
        p_update(&deltaSeconds);
        r_render(window);
        sfRenderWindow_display(window);
    }
    return 0;
}
