#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "Raycaster.h"
#include "Camera.h"

class Renderer {
public:
    Renderer();
    void init(Raycaster* raycaster, Camera* camera);
    void render(const uint16_t* zbuffer, uint8_t* framebuffer);  // Changed to uint16_t
    void clearFramebuffer(uint8_t* framebuffer, uint8_t color);
    
private:
    void drawWall(int x, int drawStart, int drawEnd, const RayResult& result, uint8_t* framebuffer);
    void drawFloorAndCeiling(int x, int drawStart, int drawEnd, uint8_t* framebuffer);
    uint8_t getWallColor(const RayResult& result, float distance);
    
    Raycaster* raycaster;
    Camera* camera;
    uint16_t screenWidth;
    uint16_t screenHeight;
};

#endif