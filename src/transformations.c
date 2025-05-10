#include "transformations.h"
#include "camera.h"
#include <math.h>

#define TILE_SIZE 50


sfVector2f map_to_screen_3drot(float x, float y, float height)
{

    float px = x * TILE_SIZE;
    float py = height;
    float pz = y * TILE_SIZE;

  
    float cosX = cosf(g_rotX);
    float sinX = sinf(g_rotX);
    float ry = py * cosX - pz * sinX;
    float rz = py * sinX + pz * cosX;
    float rx = px;

   
    float cosY = cosf(g_rotY);
    float sinY = sinf(g_rotY);
    float rxx =  rx * cosY + rz * sinY;
    float ryy =  ry;
    float rzz = -rx * sinY + rz * cosY;


    float iso_x = (rxx - rzz);
    float iso_y = (rxx + rzz)/2.f - ryy;


    iso_x *= g_camera_zoom;
    iso_y *= g_camera_zoom;
    iso_x += g_camera_x;
    iso_y += g_camera_y;

    return (sfVector2f){ iso_x, iso_y };
}
