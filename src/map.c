#include "map.h"
#include "transformations.h"
#include <SFML/Graphics.h>
#include <stdio.h>
#include <math.h>


float height_map[MAX_MAP_COLS][MAX_MAP_ROWS] = {0};
int g_map_cols = 16;
int g_map_rows = 16;
float water_level = 0.f;
int selected_x = -1;
int selected_y = -1;


int g_selection_mode = 1;


int color_override[MAX_MAP_COLS][MAX_MAP_ROWS] = {0};


static sfTexture* tex_water = NULL;
static sfTexture* tex_grass = NULL;
static sfTexture* tex_sand  = NULL;
static sfTexture* tex_rock  = NULL;

void load_textures(void)
{
    tex_water = sfTexture_createFromFile("assets/water.png", NULL);
    if (!tex_water)  printf("Failed to load water.png\n");

    tex_grass = sfTexture_createFromFile("assets/grass.png", NULL);
    if (!tex_grass)  printf("Failed to load grass.png\n");

    tex_sand  = sfTexture_createFromFile("assets/sand.png", NULL);
    if (!tex_sand)   printf("Failed to load sand.png\n");

    tex_rock  = sfTexture_createFromFile("assets/rock.png", NULL);
    if (!tex_rock)   printf("Failed to load rock.png\n");
}

void unload_textures(void)
{
    if (tex_water) { sfTexture_destroy(tex_water); tex_water = NULL; }
    if (tex_grass) { sfTexture_destroy(tex_grass); tex_grass = NULL; }
    if (tex_sand)  { sfTexture_destroy(tex_sand);  tex_sand  = NULL; }
    if (tex_rock)  { sfTexture_destroy(tex_rock);  tex_rock  = NULL; }
}

static sfTexture* get_texture_for_height(float h)
{
    if (h < 1.f)
        return tex_water;
    else if (h < 30.f)
        return tex_grass;
    else if (h < 60.f)
        return tex_sand;
    else
        return tex_rock;
}

static sfTexture* get_texture_override(int override_val)
{
    if (override_val == 1) {
        return tex_grass;
    }
    return NULL;
}

void draw_map(sfRenderWindow *window)
{
    sfVertexArray* wireVA = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(wireVA, sfLines);

    for (int x = 0; x < g_map_cols - 1; x++) {
        for (int y = 0; y < g_map_rows - 1; y++) {
            float h1 = height_map[x][y];
            float h2 = height_map[x+1][y];
            float h3 = height_map[x][y+1];
            float h4 = height_map[x+1][y+1];
            float avg = (h1 + h2 + h3 + h4) * 0.25f;

            sfTexture* tex = NULL;
            if (color_override[x][y] != 0) {
                tex = get_texture_override(color_override[x][y]);
            } else {
                tex = get_texture_for_height(avg);
            }

            sfRenderStates states = { sfBlendAlpha, sfTransform_Identity, tex, NULL };
            sfColor tileColor = sfWhite;

            
            if (g_selection_mode == 1) {
                if (x == selected_x && y == selected_y) {
                    states.texture = NULL;
                    tileColor = sfColor_fromRGB(255, 0, 0);
                }
            }
            else if (g_selection_mode == 4) {
               
                if (selected_x >= 0 && selected_y >= 0 &&
                    x >= selected_x && x <= selected_x + 1 &&
                    y >= selected_y && y <= selected_y + 1)
                {
                    states.texture = NULL;
                    tileColor = sfColor_fromRGB(255, 0, 0);
                }
            }

            sfVector2f p1 = map_to_screen_3drot(x,   y,   h1);
            sfVector2f p2 = map_to_screen_3drot(x+1, y,   h2);
            sfVector2f p3 = map_to_screen_3drot(x,   y+1, h3);
            sfVector2f p4 = map_to_screen_3drot(x+1, y+1, h4);

            sfVertexArray* tileVA = sfVertexArray_create();
            sfVertexArray_setPrimitiveType(tileVA, sfTriangles);

            sfVertex v1 = { p1, tileColor, {0.f, 0.f} };
            sfVertex v2 = { p2, tileColor, {64.f, 0.f} };
            sfVertex v3 = { p3, tileColor, {0.f, 32.f} };
            sfVertex v4 = { p4, tileColor, {64.f, 32.f} };

            sfVertexArray_append(tileVA, v1);
            sfVertexArray_append(tileVA, v2);
            sfVertexArray_append(tileVA, v3);
            sfVertexArray_append(tileVA, v2);
            sfVertexArray_append(tileVA, v4);
            sfVertexArray_append(tileVA, v3);

            sfRenderWindow_drawVertexArray(window, tileVA, &states);
            sfVertexArray_destroy(tileVA);

            sfColor lineC = sfColor_fromRGB(0,0,0);
            sfVertex w1 = { p1, lineC, {0,0} };
            sfVertex w2 = { p2, lineC, {0,0} };
            sfVertex w3 = { p3, lineC, {0,0} };
            sfVertex w4 = { p4, lineC, {0,0} };

            sfVertexArray_append(wireVA, w1);
            sfVertexArray_append(wireVA, w2);
            sfVertexArray_append(wireVA, w2);
            sfVertexArray_append(wireVA, w4);
            sfVertexArray_append(wireVA, w4);
            sfVertexArray_append(wireVA, w3);
            sfVertexArray_append(wireVA, w3);
            sfVertexArray_append(wireVA, w1);
        }
    }
    sfRenderWindow_drawVertexArray(window, wireVA, NULL);
    sfVertexArray_destroy(wireVA);
}

void modify_tile_height(int x, int y, float amount)
{
    if (x >= 0 && x < g_map_cols && y >= 0 && y < g_map_rows) {
        height_map[x][y] += amount;
        if (height_map[x][y] < 0)
            height_map[x][y] = 0.f;
    }
}

void reset_all_tiles(void)
{
    for (int i = 0; i < g_map_cols; i++) {
        for (int j = 0; j < g_map_rows; j++) {
            height_map[i][j] = 0.f;
            color_override[i][j] = 0;
        }
    }
}
