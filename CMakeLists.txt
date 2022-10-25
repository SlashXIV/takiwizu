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
add_executable(game_test_gifrim game_test_gifrim.c)
add_executable(game_test_ibensaad game_test_ibensaad.c)

# -> dummy test (gifrim)
target_link_libraries(game_test_gifrim game)
add_test(test_gifrim_dummy ./game_test_gifrim dummy)
add_test(test_gifrim_game_check_move ./game_test_gifrim game_check_move)
add_test(test_gifrim_game_has_error ./game_test_gifrim game_has_error)
add_test(test_gifrim_game_is_immutable ./game_test_gifrim game_is_immutable)
add_test(test_gifrim_game_is_empty ./game_test_gifrim game_is_empty)
add_test(test_gifrim_game_get_number ./game_test_gifrim game_get_number)
add_test(test_game_get_next_number ./game_test_gifrim game_get_next_number)
add_test(test_game_get_next_square ./game_test_gifrim game_get_next_square)

# -> dummy test (ibensaad)
target_link_libraries(game_test_ibensaad game)
add_test(test_ibensaad_dummy ./game_test_ibensaad dummy)
add_test(test_ibensaad_default_solution ./game_test_ibensaad default_solution)
add_test(test_ibensaad_default ./game_test_ibensaad default)

# <====== GAME TESTS rdubernard ======>
target_link_libraries(game_test_rdubernard game)
add_test(test_rdubernard_dummy ./game_test_rdubernard dummy)
add_test(test_rdubernard_get_square ./game_test_rdubernard get_square)
add_test(test_rdubernard_set_square ./game_test_rdubernard set_square)
add_test(test_rdubernard_game_delete ./game_test_rdubernard game_delete)
add_test(test_rdubernard_game_equal ./game_test_rdubernard game_equal)
add_test(test_rdubernard_game_copy ./game_test_rdubernard game_copy)