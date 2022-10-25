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

# TEST EXES OF EACH MEMBERS
add_executable(game_test_rdubernard game_test_rdubernard.c)
target_link_libraries(game_test_rdubernard game)


add_executable(game_test_gifrim game_test_gifrim.c)
add_executable(game_test_ibensaad game_test_ibensaad.c)

# -> dummy test (gifrim)
target_link_libraries(game_test_gifrim game)
add_test(test_gifrim_dummy ./game_test_gifrim dummy)
add_test(test_gifrim_game_check_move ./game_test_gifrim game_check_move)

# -> dummy test (ibensaad)
add_test(test_ibensaad_dummy ./game_test_ibensaad dummy)
add_test(test_ibensaad_default_solution ./game_test_ibensaad default_solution)
add_test(test_ibensaad_default ./game_test_ibensaad default)
target_link_libraries(game_test_ibensaad game)

# [====== GAME TESTS ROBIN ======]
add_test(test_rdubernard_dummy ./game_test_rdubernard dummy)
add_test(test_rdubernard_get_square ./game_test_rdubernard get_square)
