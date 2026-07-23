#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "Adafruit_oled_drv.h"
#include "Map.h"
#include "Camera.h"
#include "room_engine.h"  

class Player {
public:
    Player();
    void init(float startX, float startY, float startAngle);
    void moveForward(float speed);
    void moveBackward(float speed);
    void rotateLeft(float angle);
    void rotateRight(float angle);
    void strafeLeft(float speed);
    void strafeRight(float speed);
    
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getAngle() const { return angle; }
    float getMoveSpeed() const { return moveSpeed; }  // Add this
    float getRotSpeed() const { return rotSpeed; }    // Add this
    
    // Setters
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setAngle(float newAngle) { angle = newAngle; }
    
private:
    float x;
    float y;
    float angle;
    float moveSpeed;
    float rotSpeed;
};

#endif