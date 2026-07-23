#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include "Adafruit_oled_drv.h"
#include "Player.h"

class Camera {
public:
    Camera();
    void init(float fov, uint16_t screenWidth, uint16_t screenHeight);
    void update(float playerX, float playerY, float playerAngle);
    
    float getFOV() const { return fov; }
    uint16_t getScreenWidth() const { return screenWidth; }
    uint16_t getScreenHeight() const { return screenHeight; }
    float getPlaneX() const { return planeX; }
    float getPlaneY() const { return planeY; }
    float getDirX() const { return dirX; }
    float getDirY() const { return dirY; }
    
private:
    float fov;
    uint16_t screenWidth;
    uint16_t screenHeight;
    float planeX;
    float planeY;
    float dirX;
    float dirY;
};

#endif