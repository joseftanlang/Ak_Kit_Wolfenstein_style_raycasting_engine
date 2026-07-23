#include "Camera.h"
#include "room_engine.h"
#include "task_list.h"        // ADD THIS
#include "Adafruit_oled_drv.h"
#include <math.h>

Camera::Camera() : fov(60.0f), screenWidth(128), screenHeight(64), 
                   planeX(0.0f), planeY(0.0f), dirX(0.0f), dirY(0.0f) {
}

void Camera::init(float fov, uint16_t screenWidth, uint16_t screenHeight) {
    this->fov = fov;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

void Camera::update(float playerX, float playerY, float playerAngle) {
    dirX = cosf(playerAngle);
    dirY = sinf(playerAngle);
    
    float fovRad = fov * 3.14159f / 180.0f;
    float planeLength = tanf(fovRad / 2.0f);
    
    planeX = -dirY * planeLength;
    planeY = dirX * planeLength;
}

// Task function - using ak_msg_t* instead of void*
void task_room_engine_camera(ak_msg_t* msg) {
    (void)msg; // Suppress unused parameter warning
    
    while(1) {
        g_camera.update(g_player.getX(), g_player.getY(), g_player.getAngle());
        delay(10);
    }
}