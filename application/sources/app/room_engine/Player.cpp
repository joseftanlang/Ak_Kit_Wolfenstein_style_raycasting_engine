#include "Player.h"
#include "room_engine.h"
#include "Adafruit_oled_drv.h"
#include <math.h>
#include "app_dbg.h"
#include "task_list.h"
#include "ak.h"  

Player::Player() : x(0.0f), y(0.0f), angle(0.0f), moveSpeed(0.05f), rotSpeed(0.03f) {
}

void Player::init(float startX, float startY, float startAngle) {
    this->x = startX;
    this->y = startY;
    this->angle = startAngle;
}

void Player::moveForward(float speed) {
    x += cosf(angle) * speed;
    y += sinf(angle) * speed;
}

void Player::moveBackward(float speed) {
    x -= cosf(angle) * speed;
    y -= sinf(angle) * speed;
}

void Player::rotateLeft(float angle) {
    this->angle -= angle;
}

void Player::rotateRight(float angle) {
    this->angle += angle;
}

void Player::strafeLeft(float speed) {
    x += cosf(angle + 1.5708f) * speed;
    y += sinf(angle + 1.5708f) * speed;
}

void Player::strafeRight(float speed) {
    x -= cosf(angle + 1.5708f) * speed;
    y -= sinf(angle + 1.5708f) * speed;
}


void task_room_engine_player(ak_msg_t* msg) {
    (void)msg;
    APP_DBG_SIG("*** PLAYER TASK STARTED ***\n");  // CRITICAL DEBUG
    g_player.init(7.5f, 7.5f, 0.0f);
    APP_DBG_SIG("Player at X:%.2f Y:%.2f Angle:%.2f\n", 
            g_player.getX(), g_player.getY(), g_player.getAngle());
    
    while(1) {
        if(g_move_forward) {
            APP_DBG_SIG("FWD\n");
            g_player.moveForward(g_player.getMoveSpeed());
        }
        if(g_move_backward) {
            APP_DBG_SIG("BWD\n");
            g_player.moveBackward(g_player.getMoveSpeed());
        }
        if(g_rotate_left) {
            APP_DBG_SIG("LFT\n");
            g_player.rotateLeft(g_player.getRotSpeed());
        }
        if(g_rotate_right) {
            APP_DBG_SIG("RGT\n");
            g_player.rotateRight(g_player.getRotSpeed());
        }
        
        int mapX = (int)g_player.getX();
        int mapY = (int)g_player.getY();
        if(g_map.isWall(mapX, mapY)) {
            g_player.setX(g_player.getX() - cosf(g_player.getAngle()) * 0.1f);
            g_player.setY(g_player.getY() - sinf(g_player.getAngle()) * 0.1f);
        }
        
        delay(10);
    }
}