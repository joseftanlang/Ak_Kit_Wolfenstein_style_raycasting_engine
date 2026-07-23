#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <stdint.h>
#include "Map.h"
#include "Player.h"
#include "Camera.h"

#define MAX_SCREEN_WIDTH 128
#define MAX_SCREEN_HEIGHT 64

struct RayResult {
    float distance;
    int wallX;
    int wallY;
    int side; // 0 = NS wall, 1 = EW wall
    float wallXHit; // Exact wall hit position (0-1)
};

class Raycaster {
public:
    Raycaster();
    void init(Map* map, Player* player, Camera* camera);
    void castRays(uint16_t* zbuffer); 
    RayResult castSingleRay(float rayDirX, float rayDirY);
    
private:
    Map* map;
    Player* player;
    Camera* camera;
    float screenWidth;
    float screenHeight;
};

#endif