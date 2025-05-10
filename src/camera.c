#include "camera.h"
#include <SFML/Window/Keyboard.h>
#include <math.h>


float g_camera_x = 400.f;
float g_camera_y = 300.f;
float g_camera_zoom = 1.f;

float g_rotX = 0.f;
float g_rotY = 0.f;


static const float MOVE_STEP = 20.f;
static const float ZOOM_STEP = 0.1f;
static const float MIN_ZOOM  = 0.3f;
static const float MAX_ZOOM  = 3.0f;

void camera_handle_key(sfKeyCode key)
{
  
    if (key == sfKeyLeft)
        g_camera_x += MOVE_STEP;
    else if (key == sfKeyRight)
        g_camera_x -= MOVE_STEP;
    else if (key == sfKeyUp)
        g_camera_y += MOVE_STEP;
    else if (key == sfKeyDown)
        g_camera_y -= MOVE_STEP;
}

void camera_handle_mouse_wheel(float delta)
{
    
    g_camera_zoom += delta * ZOOM_STEP;
    if (g_camera_zoom < MIN_ZOOM)
        g_camera_zoom = MIN_ZOOM;
    if (g_camera_zoom > MAX_ZOOM)
        g_camera_zoom = MAX_ZOOM;
}
