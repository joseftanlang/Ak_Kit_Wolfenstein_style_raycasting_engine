#include "room_engine.h"
#include "Camera.h"
#include "Player.h"
#include "Map.h"
#include "Raycaster.h"
#include "Renderer.h"

// Global engine instances
Camera g_camera;
Player g_player;
Map g_map;
Raycaster g_raycaster;
Renderer g_renderer;

// Framebuffers
uint8_t g_framebuffer[FRAMEBUFFER_SIZE];
uint16_t g_zbuffer[SCREEN_WIDTH];  // Keep as uint16_t

// Input state
volatile bool g_move_forward = false;
volatile bool g_move_backward = false;
volatile bool g_rotate_left = false;
volatile bool g_rotate_right = false;
volatile uint8_t g_key_pressed = 0;

void scr_game_init(void) {
    g_camera.init(60.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    g_map.init();
}

uint8_t* scr_game_get_framebuffer(void) {
    return g_framebuffer;
}