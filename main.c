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

    while(sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event.type == sfEvtResized) {
                sfView_setSize((sfView*) view, (sfVector2f){(float) event.size.width, (float) event.size.height});
                sfRenderWindow_setView(window, view);
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        r_render(window);
        sfRenderWindow_display(window);
    }
    return 0;
}
