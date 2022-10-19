# [======= SETUP =======]
cmake_minimum_required(VERSION 2.6) 
set(CMAKE_VERBOSE_MAKEFILE ON) 
project(tazuku-06A) 
link_directories(${CMAKE_SOURCE_DIR}) # EQUIVALENT "LDFLAGS = -L ./" 
set(CMAKE_C_FLAGS "-std=c99 -Wall") 

# [====== TARGETS ======]
add_executable(game_text game_text.c) # FINAL EXEC 
# add_library(libgame.a game.c game_aux.c) # GAME LIB MAKER (useless)

# [====== LIBRARY LINK ======]
target_link_libraries(game_text game)

# [====== TESTS ======] 
include(CTest) 
enable_testing() 

# test rdubernard
add_executable(game_test_rdubernard game_test_rdubernard.c)
add_test(test_rdubernard_dummy ./game_test_rdubernard dummy)

# test gifrim
add_executable(game_test_gifrim game_test_gifrim.c)
add_test(test_gifrim_dummy ./game_test_gifrim dummy)


# test ibensaad

add_executable(game_test_ibensaad game_test_ibensaad.c)
add_test(test_ibensaad_dummy ./game_test_ibensaad dummy)


# test game.h
 
