#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

class Map {
public:
    Map();
    void init();
    uint8_t getCell(int x, int y) const;  // Changed to uint8_t
    bool isWall(int x, int y) const;
    int getMapWidth() const { return MAP_WIDTH; }
    int getMapHeight() const { return MAP_HEIGHT; }
    const uint8_t* getMapData() const { return &mapData[0][0]; }  // Changed to uint8_t
    
private:
    uint8_t mapData[MAP_WIDTH][MAP_HEIGHT];  // Changed from int to uint8_t
};

#endif