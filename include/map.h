#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.h>

#define MAX_MAP_COLS 64
#define MAX_MAP_ROWS 64

extern float height_map[MAX_MAP_COLS][MAX_MAP_ROWS];
extern int g_map_cols;
extern int g_map_rows;

extern float water_level;

// Seçili tile
extern int selected_x;
extern int selected_y;

// Kullanıcının seçebileceği tile sayısı (1 veya 4)
extern int g_selection_mode;

// EK: color_override global değişken bildirimi
extern int color_override[MAX_MAP_COLS][MAX_MAP_ROWS];

// Dokuları yükle/boşalt
void load_textures(void);
void unload_textures(void);

// Haritayı çiz (dokulu + grid)
void draw_map(sfRenderWindow *window);

// Yükseklik değiştirme ve sıfırlama
void modify_tile_height(int x, int y, float amount);
void reset_all_tiles(void);

#endif
