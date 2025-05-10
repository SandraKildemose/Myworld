#include "save_load.h"
#include "map.h"
#include <stdio.h>

void save_map(const char* filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Save failed!\n");
        return;
    }
    fprintf(fp, "%d %d\n", g_map_cols, g_map_rows);
    for (int y = 0; y < g_map_rows; y++) {
        for (int x = 0; x < g_map_cols; x++) {
            fprintf(fp, "%.2f ", height_map[x][y]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Map saved to %s\n", filename);
}

void load_map(const char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Load failed!\n");
        return;
    }
    int new_cols, new_rows;
    fscanf(fp, "%d %d", &new_cols, &new_rows);
    g_map_cols = new_cols;
    g_map_rows = new_rows;
    for (int y = 0; y < g_map_rows; y++) {
        for (int x = 0; x < g_map_cols; x++) {
            float val;
            fscanf(fp, "%f", &val);
            height_map[x][y] = val;
        }
    }
    fclose(fp);
    printf("Map loaded from %s\n", filename);
}
