#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <SFML/System/Vector2.h>

// (x, y, height) -> 3D döndürme + isometrik projeksiyon + kamera
sfVector2f map_to_screen_3drot(float x, float y, float height);

#endif
