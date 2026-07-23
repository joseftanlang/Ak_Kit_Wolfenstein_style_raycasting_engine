#include "scr_game.h"
#include "../room_engine/room_engine.h"
#include "../room_engine/Player.h"
#include "../room_engine/Camera.h"
#include "../room_engine/Map.h"
#include "../room_engine/Raycaster.h"
#include "../room_engine/Renderer.h"
#include "timer.h"
#include "buzzer.h"
#include "app_dbg.h"

// Game state
static bool game_initialized = false;
static uint32_t frame_counter = 0;
static uint32_t button_counter = 0;

// Forward declaration
static void view_scr_game();

// View dynamic structure
view_dynamic_t dyn_view_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game
};

// Screen structure
view_screen_t scr_game = {
    &dyn_view_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

// View render function - draws the game
static void view_scr_game() {
    view_render.clear();
    
    if (!game_initialized) {
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(20, 20);
        view_render.print("Loading...");
        return;
    }
    
    // Get framebuffer
    uint8_t* framebuffer = scr_game_get_framebuffer();
    
    // Draw framebuffer (packed 8 pixels per byte)
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = (y * SCREEN_WIDTH + x) / 8;
            int bit = 7 - (x % 8);
            uint8_t pixel = (framebuffer[index] >> bit) & 1;
            
            if (pixel) {
                view_render.drawPixel(x, y, WHITE);
            }
        }
    }
    
    // Show FPS and position
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(2, 2);
    view_render.print("F:");
    view_render.print(frame_counter);
    
    view_render.setCursor(2, 10);
    view_render.print("B:");
    view_render.print(button_counter);
    
    view_render.setCursor(2, 56);
    view_render.print("X:");
    view_render.print((int)g_player.getX());
    view_render.print(" Y:");
    view_render.print((int)g_player.getY());
}

// Game screen handler
void scr_game_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case SCREEN_ENTRY: {
            APP_DBG_SIG("SCREEN_ENTRY - Game Screen\n");
            
            if (!game_initialized) {
                // Initialize all game components
                g_camera.init(60.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
                g_map.init();
                g_player.init(7.5f, 7.5f, 0.0f);
                g_raycaster.init(&g_map, &g_player, &g_camera);
                g_renderer.init(&g_raycaster, &g_camera);
                
                game_initialized = true;
                APP_DBG("Game initialized!\n");
            }
            
            // Start timers
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_GAME_UPDATE_TICK, 
                      33, TIMER_PERIODIC);  // 30 FPS
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_GAME_RENDER_TICK,
                      33, TIMER_PERIODIC);
            
            frame_counter = 0;
            button_counter = 0;
        } break;
        
        case AC_DISPLAY_GAME_UPDATE_TICK: {
            frame_counter++;
            
            // ============= PLAYER UPDATE =============
            if(g_move_forward) {
                g_player.moveForward(g_player.getMoveSpeed());
            }
            if(g_move_backward) {
                g_player.moveBackward(g_player.getMoveSpeed());
            }
            if(g_rotate_left) {
                g_player.rotateLeft(g_player.getRotSpeed());
            }
            if(g_rotate_right) {
                g_player.rotateRight(g_player.getRotSpeed());
            }
            
            // Simple collision
            int mapX = (int)g_player.getX();
            int mapY = (int)g_player.getY();
            if(g_map.isWall(mapX, mapY)) {
                g_player.setX(g_player.getX() - cosf(g_player.getAngle()) * 0.1f);
                g_player.setY(g_player.getY() - sinf(g_player.getAngle()) * 0.1f);
            }
            
            // ============= RAYCASTING =============
            // Update camera
            g_camera.update(g_player.getX(), g_player.getY(), g_player.getAngle());
            
            // Cast rays
            g_raycaster.castRays(g_zbuffer);
            
            // ============= RENDER =============
            g_renderer.render(g_zbuffer, g_framebuffer);
            
        } break;
        
        case AC_DISPLAY_GAME_RENDER_TICK: {
            view_render.update();
        } break;
        
        case AC_DISPLAY_BUTON_MODE_PRESSED: {
            APP_DBG_SIG("MODE PRESSED\n");
        } break;
        
        case AC_DISPLAY_BUTON_UP_PRESSED: {
            APP_DBG_SIG("UP PRESSED - Move Forward\n");
            button_counter++;
            g_move_forward = true;
        } break;
        
        case AC_DISPLAY_BUTON_DOWN_PRESSED: {
            APP_DBG_SIG("DOWN PRESSED - Move Backward\n");
            button_counter++;
            g_move_backward = true;
        } break;
        
        case AC_DISPLAY_BUTON_MODE_UP_PRESSED: {
            APP_DBG_SIG("MODE+UP PRESSED - Rotate Left\n");
            button_counter++;
            g_rotate_left = true;
        } break;
        
        case AC_DISPLAY_BUTON_MODE_DOWN_PRESSED: {
            APP_DBG_SIG("MODE+DOWN PRESSED - Rotate Right\n");
            button_counter++;
            g_rotate_right = true;
        } break;
        
        case AC_DISPLAY_BUTON_RELEASE: {
            g_move_forward = false;
            g_move_backward = false;
            g_rotate_left = false;
            g_rotate_right = false;
        } break;
        
        case AC_DISPLAY_BUTON_MODE_LONG_PRESSED: {
            APP_DBG_SIG("MODE LONG PRESSED - Exit Game\n");
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_GAME_UPDATE_TICK);
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_GAME_RENDER_TICK);
            SCREEN_TRAN(scr_idle_handle, &scr_idle);
        } break;
        
        default:
            break;
    }
}