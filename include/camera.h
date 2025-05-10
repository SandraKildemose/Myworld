#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.h>

// Kamera ofseti
extern float g_camera_x;
extern float g_camera_y;

// Zoom faktörü
extern float g_camera_zoom;

// 3D döndürme açıları (radyan)
extern float g_rotX; // X ekseni (yukarı-aşağı eğim)
extern float g_rotY; // Y ekseni (sağa-sola döndürme)

// Fonksiyonlar
void camera_handle_key(sfKeyCode key);
void camera_handle_mouse_wheel(float delta);

#endif
