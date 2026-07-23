CFLAGS		+= -I./sources/app/game_amongus
CPPFLAGS	+= -I./sources/app/game_amongus

VPATH += sources/app/game_amongus

# CPP source files
SOURCES_CPP += sources/app/game_amongus/Enemy.cpp
SOURCES_CPP += sources/app/game_amongus/Game.cpp
SOURCES_CPP += sources/app/game_amongus/Health.cpp
SOURCES_CPP += sources/app/game_amongus/Level.cpp

