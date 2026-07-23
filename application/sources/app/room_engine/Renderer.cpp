#include "Renderer.h"
#include "room_engine.h"
#include "task_list.h"
#include "Adafruit_oled_drv.h"
#include <math.h>

Renderer::Renderer() : raycaster(nullptr), camera(nullptr), screenWidth(128), screenHeight(64) {
}

void Renderer::init(Raycaster* raycaster, Camera* camera) {
    this->raycaster = raycaster;
    this->camera = camera;
    this->screenWidth = camera->getScreenWidth();
    this->screenHeight = camera->getScreenHeight();
}

void setPixel(uint8_t* framebuffer, int x, int y, uint8_t color) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    
    int index = (y * 128 + x) / 8;
    int bit = 7 - (x % 8);
    if (color) {
        framebuffer[index] |= (1 << bit);
    } else {
        framebuffer[index] &= ~(1 << bit);
    }
}

void Renderer::clearFramebuffer(uint8_t* framebuffer, uint8_t color) {
    if (framebuffer == nullptr) return;
    
    uint8_t fill = color ? 0xFF : 0x00;
    for(int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        framebuffer[i] = fill;
    }
}

void Renderer::render(const uint16_t* zbuffer, uint8_t* framebuffer) {
    // Safety checks
    if (zbuffer == nullptr || framebuffer == nullptr || raycaster == nullptr || camera == nullptr) {
        return;
    }
    
    clearFramebuffer(framebuffer, 0);
    
    // If camera not initialized, draw a test pattern
    if (camera->getDirX() == 0 && camera->getDirY() == 0) {
        // Draw a simple pattern to show something
        for(int y = 0; y < 64; y++) {
            for(int x = 0; x < 128; x++) {
                if ((x + y) % 8 < 4) {
                    setPixel(framebuffer, x, y, 1);
                }
            }
        }
        return;
    }
    
    // Render 3D view
    for(int x = 0; x < screenWidth; x++) {
        float cameraX = 2.0f * x / screenWidth - 1.0f;
        float rayDirX = camera->getDirX() + camera->getPlaneX() * cameraX;
        float rayDirY = camera->getDirY() + camera->getPlaneY() * cameraX;
        
        RayResult result = raycaster->castSingleRay(rayDirX, rayDirY);
        
        float distance = zbuffer[x] / 100.0f;
        if (distance < 0.1f) distance = 0.1f;
        
        float correctDist = distance * cosf(atanf(cameraX));
        if (correctDist < 0.1f) correctDist = 0.1f;
        
        int lineHeight = (int)(screenHeight / correctDist);
        if (lineHeight > screenHeight) lineHeight = screenHeight;
        if (lineHeight < 1) lineHeight = 1;
        
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight) drawEnd = screenHeight - 1;
        
        // Draw wall
        for(int y = drawStart; y <= drawEnd && y < screenHeight; y++) {
            setPixel(framebuffer, x, y, 1);  // White walls
        }
        
        // Draw floor/ceiling with pattern
        for(int y = 0; y < drawStart && y < screenHeight; y++) {
            // Ceiling - black
            setPixel(framebuffer, x, y, 0);
        }
        for(int y = drawEnd; y < screenHeight; y++) {
            // Floor - simple checker pattern
            if ((x + y) % 4 < 2) {
                setPixel(framebuffer, x, y, 1);
            }
        }
    }
}

// At the bottom of Renderer.cpp - NO class, NO namespace, just the function
void task_room_engine_render(ak_msg_t* msg) {
    (void)msg;
    g_renderer.init(&g_raycaster, &g_camera);
    while(1) {
        g_renderer.render(g_zbuffer, g_framebuffer);
        delay(16);
    }
}

void task_room_engine_draw(ak_msg_t* msg) {
    (void)msg;
    while(1) {
        delay(16);
    }
}