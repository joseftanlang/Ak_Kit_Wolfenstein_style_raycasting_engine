CFLAGS		+= -I./sources/app/room_engine
CPPFLAGS	+= -I./sources/app/room_engine

VPATH += sources/app/room_engine

# CPP source files
SOURCES_CPP += sources/app/room_engine/Map.cpp
SOURCES_CPP += sources/app/room_engine/Player.cpp
SOURCES_CPP += sources/app/room_engine/Raycaster.cpp
SOURCES_CPP += sources/app/room_engine/Renderer.cpp
SOURCES_CPP += sources/app/room_engine/Camera.cpp
SOURCES_CPP += sources/app/room_engine/room_engine_globals.cpp
