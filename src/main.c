#include <SFML/Graphics.h>
#include <stdio.h>
#include "my_world.h"  
#include "update.h"

sfView* g_ui_view = NULL;


int main(void)
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "My World", sfResize | sfClose, NULL);
    if (!window) {
        printf("Failed to create window\n");
        return 1;
    }

   
    audio_init();

  
    g_ui_view = sfView_create();
    sfView_setSize(g_ui_view, (sfVector2f){800, 600});
    sfView_setCenter(g_ui_view, (sfVector2f){400, 300});

    load_textures();
    ui_init();
    reset_all_tiles();

    sfClock* clock = sfClock_create();

    while (sfRenderWindow_isOpen(window))
    {
        sfTime dt_time = sfClock_restart(clock);
        float dt = sfTime_asSeconds(dt_time);

        handle_events(window);
        update_game(dt);

        sfRenderWindow_clear(window, sfBlack);

        sfRenderWindow_setView(window, sfRenderWindow_getDefaultView(window));
        draw_map(window);

        sfRenderWindow_setView(window, g_ui_view);
        ui_draw(window);

        sfRenderWindow_display(window);
    }

   
    audio_cleanup();

    sfClock_destroy(clock);
    unload_textures();
    sfRenderWindow_destroy(window);
    return 0;
}
