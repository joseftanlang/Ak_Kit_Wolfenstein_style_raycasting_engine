#ifndef __SCR_GAME_H__
#define __SCR_GAME_H__

#include "screens.h"
#include "../room_engine/room_engine.h"
#include "../room_engine/Player.h"      // ADD THIS - for Player class definition
#include "../room_engine/Camera.h"      // ADD THIS - for Camera class definition
#include "../room_engine/Map.h"         // ADD THIS - for Map class definition
#include "../room_engine/Raycaster.h"   // ADD THIS - for Raycaster class definition
#include "../room_engine/Renderer.h"    // ADD THIS - for Renderer class definition
#include "../room_engine/room_engine.h"

// Game screen external declarations
extern view_screen_t scr_game;

// Game screen handler function
void scr_game_handle(ak_msg_t *msg);

#endif //__SCR_GAME_H__