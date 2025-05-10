#ifndef MY_WORLD_H
#define MY_WORLD_H

#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include "event_handler.h"
#include "map.h"
#include "transformations.h"
#include "ui.h"
#include "save_load.h"
#include "camera.h"
#include "update.h"
#include "audio.h"

// Sabit UI view: UI elemanları sabit koordinatlarda (örneğin 800x600) kalacak.
extern sfView* g_ui_view;

#endif
