#include "event_handler.h"
#include "ui.h"
#include "map.h"
#include "camera.h"
#include "transformations.h"
#include "save_load.h"
#include "my_world.h" 
#include <SFML/Graphics.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

extern e_tool g_current_tool;
extern int selected_x;
extern int selected_y;
extern int g_map_cols;
extern int g_map_rows;
extern float height_map[MAX_MAP_COLS][MAX_MAP_ROWS];

static int rotating = 0;
static int old_mouse_x = 0;
static int old_mouse_y = 0;

static float sign(sfVector2f p1, sfVector2f p2, sfVector2f p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static int point_in_triangle(sfVector2f pt, sfVector2f v1, sfVector2f v2, sfVector2f v3)
{
    float d1 = sign(pt, v1, v2);
    float d2 = sign(pt, v2, v3);
    float d3 = sign(pt, v3, v1);

    int hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

static void pick_tile_2D(int mx, int my, int *out_x, int *out_y)
{
    sfVector2f mousePt = { (float)mx, (float)my };

    for (int x = 0; x < g_map_cols - 1; x++) {
        for (int y = 0; y < g_map_rows - 1; y++) {
            float h1 = height_map[x][y];
            float h2 = height_map[x+1][y];
            float h3 = height_map[x][y+1];
            float h4 = height_map[x+1][y+1];

            sfVector2f p1 = map_to_screen_3drot(x, y, h1);
            sfVector2f p2 = map_to_screen_3drot(x+1, y, h2);
            sfVector2f p3 = map_to_screen_3drot(x, y+1, h3);
            sfVector2f p4 = map_to_screen_3drot(x+1, y+1, h4);

            if (point_in_triangle(mousePt, p1, p2, p3)) {
                *out_x = x;
                *out_y = y;
                return;
            }
            if (point_in_triangle(mousePt, p2, p4, p3)) {
                *out_x = x;
                *out_y = y;
                return;
            }
        }
    }
    *out_x = -1;
    *out_y = -1;
}

static void pick_tile_by_triangles(int mx, int my)
{
    int tx, ty;
    pick_tile_2D(mx, my, &tx, &ty);
    if (tx >= 0 && ty >= 0) {
        selected_x = tx;
        selected_y = ty;
        printf("Selected tile: %d, %d (height=%.1f)\n", tx, ty, height_map[tx][ty]);
    } else {
        selected_x = -1;
        selected_y = -1;
    }
}

void handle_events(sfRenderWindow *window)
{
    sfEvent event;
    while (sfRenderWindow_pollEvent(window, &event))
    {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(window);

        if (event.type == sfEvtMouseWheelScrolled)
            camera_handle_mouse_wheel(event.mouseWheelScroll.delta);

        if (event.type == sfEvtMouseButtonPressed) {
            sfVector2i pixelPos = { event.mouseButton.x, event.mouseButton.y };

            sfVector2f uiCoords = sfRenderWindow_mapPixelToCoords(window, pixelPos, g_ui_view);
            sfVector2i uiMouse = { (int)uiCoords.x, (int)uiCoords.y };
            e_tool clicked_tool = ui_handle_click(uiMouse);
            if (clicked_tool != TOOL_NONE) {
                g_current_tool = clicked_tool;
            } else {
                sfView* worldView = sfRenderWindow_getDefaultView(window);
                sfVector2f worldCoords = sfRenderWindow_mapPixelToCoords(window, pixelPos, worldView);
                sfVector2i worldMouse = { (int)worldCoords.x, (int)worldCoords.y };
                pick_tile_by_triangles(worldMouse.x, worldMouse.y);
                if (event.mouseButton.button == sfMouseLeft) {
                    rotating = 1;
                    old_mouse_x = worldMouse.x;
                    old_mouse_y = worldMouse.y;
                }
            }
        }
        if (event.type == sfEvtMouseButtonReleased) {
            sfVector2i pixelPos = { event.mouseButton.x, event.mouseButton.y };
            ui_handle_release(pixelPos);
            if (event.mouseButton.button == sfMouseLeft)
                rotating = 0;
        }
        if (event.type == sfEvtMouseMoved) {
            sfVector2i pixelPos = { event.mouseMove.x, event.mouseMove.y };

            sfVector2f uiCoords = sfRenderWindow_mapPixelToCoords(window, pixelPos, g_ui_view);
            sfVector2i uiMouse = { (int)uiCoords.x, (int)uiCoords.y };
            ui_check_hover(uiMouse);

            if (rotating) {
                sfView* worldView = sfRenderWindow_getDefaultView(window);
                sfVector2f worldCoords = sfRenderWindow_mapPixelToCoords(window, pixelPos, worldView);
                sfVector2i worldMouse = { (int)worldCoords.x, (int)worldCoords.y };
                int dx = worldMouse.x - old_mouse_x;
                int dy = worldMouse.y - old_mouse_y;
                g_rotY += dx * 0.01f;
                g_rotX += dy * 0.01f;
                old_mouse_x = worldMouse.x;
                old_mouse_y = worldMouse.y;
            }
        }
        if (event.type == sfEvtKeyPressed) {
            camera_handle_key(event.key.code);
            if (event.key.code == sfKeyEscape)
                sfRenderWindow_close(window);
        }
        if (event.type == sfEvtTextEntered) {
            extern textbox_t tb_cols;
            extern textbox_t tb_rows;
            ui_handle_textbox_input(&tb_cols, event);
            ui_handle_textbox_input(&tb_rows, event);
        }
    }

    switch (g_current_tool)
    {
        case TOOL_RAISE:
            if (selected_x >= 0 && selected_y >= 0)
                modify_tile_height(selected_x, selected_y, 10.f);
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_LOWER:
            if (selected_x >= 0 && selected_y >= 0)
                modify_tile_height(selected_x, selected_y, -10.f);
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_LEVEL:
            if (selected_x >= 0 && selected_y >= 0)
                height_map[selected_x][selected_y] = 0.f;
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_RESET:
            reset_all_tiles();
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_GENERATE:
        {
            srand((unsigned)time(NULL));
            for (int x = 0; x < g_map_cols; x++) {
                for (int y = 0; y < g_map_rows; y++) {
                    height_map[x][y] = (float)(rand() % 80);
                }
            }
            g_current_tool = TOOL_NONE;
        } break;
        case TOOL_SAVE:
            save_map("my_map.legend");
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_LOAD:
            load_map("my_map.legend");
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_SMALL:
            g_map_cols = 16;
            g_map_rows = 16;
            reset_all_tiles();
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_MEDIUM:
            g_map_cols = 32;
            g_map_rows = 32;
            reset_all_tiles();
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_LARGE:
            g_map_cols = 64;
            g_map_rows = 64;
            reset_all_tiles();
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_RESIZE:
        {
            extern textbox_t tb_cols;
            extern textbox_t tb_rows;
            int new_cols = atoi(tb_cols.text);
            int new_rows = atoi(tb_rows.text);
            if (new_cols > 0 && new_cols <= MAX_MAP_COLS &&
                new_rows > 0 && new_rows <= MAX_MAP_ROWS)
            {
                g_map_cols = new_cols;
                g_map_rows = new_rows;
                reset_all_tiles();
                printf("Map resized to %d x %d\n", g_map_cols, g_map_rows);
            } else {
                printf("Invalid map size.\n");
            }
            g_current_tool = TOOL_NONE;
        } break;
        case TOOL_PAINT_GRASS:
            if (selected_x >= 0 && selected_y >= 0 &&
                selected_x < g_map_cols && selected_y < g_map_rows)
            {
                color_override[selected_x][selected_y] = 1;
                printf("Tile (%d,%d) painted grass!\n", selected_x, selected_y);
            }
            g_current_tool = TOOL_NONE;
            break;
        case TOOL_SELECTION:
            if (g_selection_mode == 1) {
                g_selection_mode = 4;
                printf("Selection mode set to 4 (2x2 block).\n");
            } else {
                g_selection_mode = 1;
                printf("Selection mode set to 1 (single tile).\n");
            }
            g_current_tool = TOOL_NONE;
            break;
        default:
            break;
    }
}
