#include "Raycaster.h"
#include "room_engine.h"
#include "task_list.h"
#include "Adafruit_oled_drv.h"
#include <math.h>
#include <stdlib.h>

Raycaster::Raycaster() : map(nullptr), player(nullptr), camera(nullptr), 
                         screenWidth(128), screenHeight(64) {
}

void Raycaster::init(Map* map, Player* player, Camera* camera) {
    this->map = map;
    this->player = player;
    this->camera = camera;
    this->screenWidth = camera->getScreenWidth();
    this->screenHeight = camera->getScreenHeight();
}

RayResult Raycaster::castSingleRay(float rayDirX, float rayDirY) {
    RayResult result;
    result.distance = 20.0f;  // Default far distance
    result.wallX = 0;
    result.wallY = 0;
    result.side = 0;
    result.wallXHit = 0.0f;
    
    // Safety check
    if (player == nullptr || map == nullptr) {
        return result;
    }
    
    float posX = player->getX();
    float posY = player->getY();
    
    // Safety: prevent NaN
    if (rayDirX == 0.0f) rayDirX = 0.001f;
    if (rayDirY == 0.0f) rayDirY = 0.001f;
    
    int stepX = (rayDirX < 0) ? -1 : 1;
    int stepY = (rayDirY < 0) ? -1 : 1;
    
    float deltaDistX = fabsf(1.0f / rayDirX);
    float deltaDistY = fabsf(1.0f / rayDirY);
    
    int mapX = (int)posX;
    int mapY = (int)posY;
    
    // Clamp to map bounds
    if (mapX < 0) mapX = 0;
    if (mapX >= MAP_WIDTH) mapX = MAP_WIDTH - 1;
    if (mapY < 0) mapY = 0;
    if (mapY >= MAP_HEIGHT) mapY = MAP_HEIGHT - 1;
    
    float sideDistX, sideDistY;
    
    if(rayDirX < 0) {
        sideDistX = (posX - mapX) * deltaDistX;
    } else {
        sideDistX = (mapX + 1.0f - posX) * deltaDistX;
    }
    
    if(rayDirY < 0) {
        sideDistY = (posY - mapY) * deltaDistY;
    } else {
        sideDistY = (mapY + 1.0f - posY) * deltaDistY;
    }
    
    bool hit = false;
    int maxLoops = 50;  // Safety counter
    
    while(!hit && maxLoops > 0) {
        maxLoops--;
        
        if(sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            result.side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            result.side = 1;
        }
        
        // Check bounds
        if(mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            result.distance = 20.0f;
            return result;
        }
        
        if(map->isWall(mapX, mapY)) {
            hit = true;
            result.wallX = mapX;
            result.wallY = mapY;
        }
    }
    
    if(!hit) {
        result.distance = 20.0f;
        return result;
    }
    
    if(result.side == 0) {
        result.distance = sideDistX - deltaDistX;
    } else {
        result.distance = sideDistY - deltaDistY;
    }
    
    // Safety: prevent negative or zero distance
    if(result.distance < 0.1f) result.distance = 0.1f;
    if(result.distance > 20.0f) result.distance = 20.0f;
    
    // Calculate wall hit position
    if(result.side == 0) {
        result.wallXHit = posY + result.distance * rayDirY;
    } else {
        result.wallXHit = posX + result.distance * rayDirX;
    }
    result.wallXHit -= floorf(result.wallXHit);
    
    return result;
}

void Raycaster::castRays(uint16_t* zbuffer) {
    // Safety check
    if (zbuffer == nullptr || camera == nullptr) {
        return;
    }
    
    for(int x = 0; x < (int)screenWidth; x++) {
        float cameraX = 2.0f * x / screenWidth - 1.0f;
        float rayDirX = camera->getDirX() + camera->getPlaneX() * cameraX;
        float rayDirY = camera->getDirY() + camera->getPlaneY() * cameraX;
        
        RayResult result = castSingleRay(rayDirX, rayDirY);
        
        // Scale distance to uint16_t
        float dist = result.distance;
        if(dist > 20.0f) dist = 20.0f;
        zbuffer[x] = (uint16_t)(dist * 100.0f);
    }
}

void task_room_engine_raycaster(ak_msg_t* msg) {
    (void)msg;
    
    // Wait for everything to be initialized
    delay(100);
    
    g_raycaster.init(&g_map, &g_player, &g_camera);
    
    while(1) {
        g_raycaster.castRays(g_zbuffer);
        delay(10);  // Increased from 5 to give more time
    }
}