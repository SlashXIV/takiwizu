# SETUP 
cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(tazuku-06A)
link_directories(${CMAKE_SOURCE_DIR}) # EQUIVALENT "LDFLAGS = -L ./" 

# COMPILATOR VAR
set(CMAKE_C_FLAGS "-std=c99 -Wall")

# ONE EXEC -> game_text
add_executable(game_text game_text.c)

# GAME LIBRARY MAKER (useless here)
add_library(libgame.a game.c game_aux.c)

# LIBRARY LINK (libgame.a)
target_link_libraries(game_text game)

