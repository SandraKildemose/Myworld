#include "ui.h"
#include "audio.h"  
#include "map.h"    
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <string.h>
#include <stdio.h>

static int total_buttons = 0;

typedef struct s_button {
    sfFloatRect bounds;
    e_tool action;
    const char* label;
    int is_hovered;
    int is_pressed;
} button_t;

static button_t buttons[20];  
e_tool g_current_tool = TOOL_NONE;


textbox_t tb_cols;
textbox_t tb_rows;

static void init_textbox(textbox_t* tb, float x, float y, float w, float h)
{
    tb->bounds = (sfFloatRect){x, y, w, h};
    tb->text[0] = '\0';
    tb->is_active = 0;
}

void ui_handle_textbox_input(textbox_t *tb, sfEvent event)
{
    if (!tb->is_active) return;

    if (event.type == sfEvtTextEntered) {
        unsigned int unicode = event.text.unicode;
        if (unicode >= '0' && unicode <= '9') {
            size_t len = strlen(tb->text);
            if (len < sizeof(tb->text) - 1) {
                tb->text[len] = (char)unicode;
                tb->text[len + 1] = '\0';
            }
        }
        else if (unicode == 8) { 
            size_t len = strlen(tb->text);
            if (len > 0)
                tb->text[len - 1] = '\0';
        }
    }
}

void ui_init(void)
{
  
    buttons[0].bounds = (sfFloatRect){10, 0, 80, 40};
    buttons[0].action = TOOL_RAISE;
    buttons[0].label = "Raise";

    buttons[1].bounds = (sfFloatRect){100, 0, 80, 40};
    buttons[1].action = TOOL_LOWER;
    buttons[1].label = "Lower";

    buttons[2].bounds = (sfFloatRect){190, 0, 80, 40};
    buttons[2].action = TOOL_LEVEL;
    buttons[2].label = "Level";

    buttons[3].bounds = (sfFloatRect){280, 0, 80, 40};
    buttons[3].action = TOOL_RESET;
    buttons[3].label = "Reset";

    buttons[4].bounds = (sfFloatRect){370, 0, 80, 40};
    buttons[4].action = TOOL_GENERATE;
    buttons[4].label = "Gen";

    buttons[5].bounds = (sfFloatRect){460, 0, 80, 40};
    buttons[5].action = TOOL_SAVE;
    buttons[5].label = "Save";

    buttons[6].bounds = (sfFloatRect){550, 0, 80, 40};
    buttons[6].action = TOOL_LOAD;
    buttons[6].label = "Load";

   
    buttons[7].bounds = (sfFloatRect){10, 50, 80, 40};
    buttons[7].action = TOOL_SMALL;
    buttons[7].label = "Small";

    buttons[8].bounds = (sfFloatRect){100, 50, 80, 40};
    buttons[8].action = TOOL_MEDIUM;
    buttons[8].label = "Medium";

    buttons[9].bounds = (sfFloatRect){190, 50, 80, 40};
    buttons[9].action = TOOL_LARGE;
    buttons[9].label = "Large";

    
    buttons[10].bounds = (sfFloatRect){280, 50, 80, 40};
    buttons[10].action = TOOL_PAINT_GRASS;
    buttons[10].label = "Grass";

    buttons[11].bounds = (sfFloatRect){370, 50, 80, 40};
    buttons[11].action = TOOL_RESIZE;
    buttons[11].label = "Resize";

    
    buttons[12].bounds = (sfFloatRect){460, 50, 80, 40};
    buttons[12].action = TOOL_SELECTION;
    buttons[12].label = "Sel:1"; 

    total_buttons = 13;

    for (int i = 0; i < total_buttons; i++) {
        buttons[i].is_hovered = 0;
        buttons[i].is_pressed = 0;
    }

    
    init_textbox(&tb_cols, 550, 55, 50, 30);
    init_textbox(&tb_rows, 610, 55, 50, 30);
}

void ui_draw(sfRenderWindow *window)
{
    
    for (int i = 0; i < total_buttons; i++) {
        if (buttons[i].action == TOOL_SELECTION) {
            if (g_selection_mode == 1)
                buttons[i].label = "Sel:1";
            else
                buttons[i].label = "Sel:4";
        }
    }


    for (int i = 0; i < total_buttons; i++)
    {
        button_t *b = &buttons[i];
        sfRectangleShape* rect = sfRectangleShape_create();
        sfRectangleShape_setPosition(rect, (sfVector2f){b->bounds.left, b->bounds.top});
        sfRectangleShape_setSize(rect, (sfVector2f){b->bounds.width, b->bounds.height});

        if (b->is_pressed)
            sfRectangleShape_setFillColor(rect, sfColor_fromRGB(200,50,50));
        else if (b->is_hovered)
            sfRectangleShape_setFillColor(rect, sfColor_fromRGB(180,180,255));
        else
            sfRectangleShape_setFillColor(rect, sfColor_fromRGB(200,200,200));

        sfRenderWindow_drawRectangleShape(window, rect, NULL);
        sfRectangleShape_destroy(rect);

        sfFont* font = sfFont_createFromFile("assets/arial.ttf");
        if (!font) continue;
        sfText* txt = sfText_create();
        sfText_setString(txt, b->label);
        sfText_setCharacterSize(txt, 16);
        sfText_setFont(txt, font);
        sfText_setPosition(txt, (sfVector2f){b->bounds.left+5, b->bounds.top+5});
        sfRenderWindow_drawText(window, txt, NULL);
        sfText_destroy(txt);
        sfFont_destroy(font);
    }

    
    {
        sfRectangleShape* r = sfRectangleShape_create();
        sfRectangleShape_setPosition(r, (sfVector2f){tb_cols.bounds.left, tb_cols.bounds.top});
        sfRectangleShape_setSize(r, (sfVector2f){tb_cols.bounds.width, tb_cols.bounds.height});
        sfRectangleShape_setFillColor(r, tb_cols.is_active ? sfColor_fromRGB(255,255,180) : sfColor_fromRGB(200,200,200));
        sfRenderWindow_drawRectangleShape(window, r, NULL);
        sfRectangleShape_destroy(r);

        sfFont* font = sfFont_createFromFile("assets/arial.ttf");
        sfText* txt = sfText_create();
        sfText_setString(txt, tb_cols.text);
        sfText_setFont(txt, font);
        sfText_setCharacterSize(txt, 16);
        sfText_setPosition(txt, (sfVector2f){tb_cols.bounds.left+5, tb_cols.bounds.top+5});
        sfRenderWindow_drawText(window, txt, NULL);
        sfText_destroy(txt);
        sfFont_destroy(font);
    }

    {
        sfRectangleShape* r = sfRectangleShape_create();
        sfRectangleShape_setPosition(r, (sfVector2f){tb_rows.bounds.left, tb_rows.bounds.top});
        sfRectangleShape_setSize(r, (sfVector2f){tb_rows.bounds.width, tb_rows.bounds.height});
        sfRectangleShape_setFillColor(r, tb_rows.is_active ? sfColor_fromRGB(255,255,180) : sfColor_fromRGB(200,200,200));
        sfRenderWindow_drawRectangleShape(window, r, NULL);
        sfRectangleShape_destroy(r);

        sfFont* font = sfFont_createFromFile("assets/arial.ttf");
        sfText* txt = sfText_create();
        sfText_setString(txt, tb_rows.text);
        sfText_setFont(txt, font);
        sfText_setCharacterSize(txt, 16);
        sfText_setPosition(txt, (sfVector2f){tb_rows.bounds.left+5, tb_rows.bounds.top+5});
        sfRenderWindow_drawText(window, txt, NULL);
        sfText_destroy(txt);
        sfFont_destroy(font);
    }
}

void ui_check_hover(sfVector2i mouse_pos)
{
    for (int i = 0; i < total_buttons; i++) {
        button_t *b = &buttons[i];
        if (mouse_pos.x >= b->bounds.left && 
            mouse_pos.x <= b->bounds.left + b->bounds.width &&
            mouse_pos.y >= b->bounds.top && 
            mouse_pos.y <= b->bounds.top + b->bounds.height)
        {
            b->is_hovered = 1;
        } else {
            b->is_hovered = 0;
        }
    }
}

e_tool ui_handle_click(sfVector2i mouse_pos)
{
   
    if (mouse_pos.x >= tb_cols.bounds.left && mouse_pos.x <= tb_cols.bounds.left+tb_cols.bounds.width &&
        mouse_pos.y >= tb_cols.bounds.top  && mouse_pos.y <= tb_cols.bounds.top +tb_cols.bounds.height)
    {
        tb_cols.is_active = 1;
        tb_rows.is_active = 0;
    }
    else if (mouse_pos.x >= tb_rows.bounds.left && mouse_pos.x <= tb_rows.bounds.left+tb_rows.bounds.width &&
             mouse_pos.y >= tb_rows.bounds.top  && mouse_pos.y <= tb_rows.bounds.top +tb_rows.bounds.height)
    {
        tb_rows.is_active = 1;
        tb_cols.is_active = 0;
    }
    else {
        tb_cols.is_active = 0;
        tb_rows.is_active = 0;
    }

    for (int i = 0; i < total_buttons; i++) {
        button_t *b = &buttons[i];
        if (mouse_pos.x >= b->bounds.left &&
            mouse_pos.x <= b->bounds.left + b->bounds.width &&
            mouse_pos.y >= b->bounds.top &&
            mouse_pos.y <= b->bounds.top + b->bounds.height)
        {
            if (g_clickSound) {
                sfSound_play(g_clickSound);
            }
            b->is_pressed = 1;
            return b->action;
        }
    }
    return TOOL_NONE;
}

void ui_handle_release(sfVector2i mouse_pos)
{
    for (int i = 0; i < total_buttons; i++)
        buttons[i].is_pressed = 0;
}
