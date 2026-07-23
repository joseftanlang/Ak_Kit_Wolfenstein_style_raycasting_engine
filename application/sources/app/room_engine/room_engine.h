#ifndef ROOM_ENGINE_H
#define ROOM_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

// Forward declare ak_msg_t instead of including ak.h
// This avoids circular dependencies
struct ak_msg_t;

// Akkit display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Packed framebuffer: 8 pixels per byte
#define FRAMEBUFFER_SIZE ((SCREEN_WIDTH * SCREEN_HEIGHT) / 8)

// Global engine instances - forward declared
class Camera;
class Player;
class Map;
class Raycaster;
class Renderer;

extern Camera g_camera;
extern Player g_player;
extern Map g_map;
extern Raycaster g_raycaster;
extern Renderer g_renderer;

// Framebuffers
extern uint8_t g_framebuffer[FRAMEBUFFER_SIZE];
extern uint16_t g_zbuffer[SCREEN_WIDTH];

// Input state
extern volatile bool g_move_forward;
extern volatile bool g_move_backward;
extern volatile bool g_rotate_left;
extern volatile bool g_rotate_right;
extern volatile uint8_t g_key_pressed;

// Task declarations - using forward declared ak_msg_t
void task_room_engine_camera(ak_msg_t* msg);
void task_room_engine_player(ak_msg_t* msg);
void task_room_engine_raycaster(ak_msg_t* msg);
void task_room_engine_render(ak_msg_t* msg);
void task_room_engine_draw(ak_msg_t* msg);
void task_room_engine_map(ak_msg_t* msg);

// Screen functions
void scr_game_init(void);
uint8_t* scr_game_get_framebuffer(void);

#endif