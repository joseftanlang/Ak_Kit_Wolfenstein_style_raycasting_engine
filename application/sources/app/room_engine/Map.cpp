#include "Map.h"
#include "room_engine.h"
#include "task_list.h"        // ADD THIS - for ak_msg_t and task declarations
#include "Adafruit_oled_drv.h" // For delay()

Map::Map() {
    init();
}

void Map::init() {
    // 1 = wall, 0 = empty
    uint8_t level[MAP_WIDTH][MAP_HEIGHT] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            mapData[x][y] = level[x][y];
        }
    }
}

uint8_t Map::getCell(int x, int y) const {
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 1;
    return mapData[x][y];
}

bool Map::isWall(int x, int y) const {
    return getCell(x, y) == 1;
}

// Task function
void task_room_engine_map(ak_msg_t* msg) {
    (void)msg;
    while(1) {
        delay(100);
    }
}