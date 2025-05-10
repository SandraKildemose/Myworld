#ifndef UI_H
#define UI_H

#include <SFML/Graphics.h>

// Araçlar
typedef enum e_tool {
    TOOL_NONE = 0,
    TOOL_RAISE,
    TOOL_LOWER,
    TOOL_LEVEL,
    TOOL_RESET,
    TOOL_GENERATE,
    TOOL_SAVE,
    TOOL_LOAD,
    TOOL_SMALL,
    TOOL_MEDIUM,
    TOOL_LARGE,
    TOOL_RESIZE,
    TOOL_PAINT_GRASS,
    TOOL_SELECTION   // Yeni: Seçim modunu değiştirmek için (1 veya 4)
} e_tool;

// Basit textbox
typedef struct s_textbox {
    sfFloatRect bounds;
    char text[8];
    int is_active;
} textbox_t;

extern e_tool g_current_tool;

// 2 textbox: cols, rows
extern textbox_t tb_cols;
extern textbox_t tb_rows;

void ui_init(void);
void ui_draw(sfRenderWindow *window);
void ui_check_hover(sfVector2i mouse_pos);
e_tool ui_handle_click(sfVector2i mouse_pos);
void ui_handle_release(sfVector2i mouse_pos);
void ui_handle_textbox_input(textbox_t *tb, sfEvent event);

#endif
