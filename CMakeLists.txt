# [======= SETUP =======]
cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE OFF)
project(tazuku-06A)
link_directories(${CMAKE_SOURCE_DIR}) # EQUIVALENT "LDFLAGS = -L ./"
set(CMAKE_C_FLAGS "-std=c99 -Wall -g ")
#set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage")   # options de compilation en mode Debug
#set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

# [====== TARGETS ======]
add_executable(game_text game_text.c) # FINAL EXEC
add_library(game game.c game_aux.c game_ext.c queue.c annex_funcs.c game_tools.c) # GAME LIB MAKER

# [====== LIBRARY LINK ======]
target_link_libraries(game_text game)

## find SDL2
include(SDL2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

include_directories(${SDL2_ALL_INC})

add_executable(game_sdl main.c model.c)
target_link_libraries(game_sdl game)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m)

# # copy useful data files in build dir
# foreach(file "background.jpg" "white.png" "black.png")
#   file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# endforeach(file)

# copy our sprites in build 
file (COPY sprites DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# [====== TESTS ======]
include(CTest)
enable_testing()

# TEST EXES OF EACH MEMBERS
add_executable(game_test_rdubernard game_test_rdubernard.c)
add_executable(game_test_gifrim game_test_gifrim.c)
add_executable(game_test_ibensaad game_test_ibensaad.c)
add_executable(game_solve game_solve.c)

target_link_libraries(game_solve game)
# -> dummy test (gifrim)
target_link_libraries(game_test_gifrim game)
add_test(test_gifrim_dummy ./game_test_gifrim dummy)
add_test(test_gifrim_game_check_move ./game_test_gifrim game_check_move)
add_test(test_gifrim_game_has_error ./game_test_gifrim game_has_error)
add_test(test_gifrim_game_is_immutable ./game_test_gifrim game_is_immutable)
add_test(test_gifrim_game_is_empty ./game_test_gifrim game_is_empty)
add_test(test_gifrim_game_get_number ./game_test_gifrim game_get_number)
add_test(test_gifrim_game_get_next_number ./game_test_gifrim game_get_next_number)
add_test(test_gifrim_game_get_next_square ./game_test_gifrim game_get_next_square)
add_test(test_gifrim_game_restart ./game_test_gifrim game_restart)
add_test(test_gifrim_game_is_wrapping ./game_test_gifrim game_is_wrapping)
add_test(test_gifrim_game_is_unique ./game_test_gifrim game_is_unique)
add_test(test_gifrim_game_has_error_unique ./game_test_gifrim game_has_error_unique)
add_test(test_gifrim_game_undo_redo_some ./game_test_gifrim undo_redo_some)
add_test(test_gifrim_game_save ./game_test_gifrim game_save)
add_test(test_gifrim_game_load ./game_test_gifrim game_load)



# -> dummy test (ibensaad)
target_link_libraries(game_test_ibensaad game)
add_test(test_ibensaad_dummy ./game_test_ibensaad dummy)
add_test(test_ibensaad_default_solution ./game_test_ibensaad default_solution)
add_test(test_ibensaad_default ./game_test_ibensaad default)
add_test(test_ibensaad_print ./game_test_ibensaad print)
add_test(test_ibensaad_play_move ./game_test_ibensaad play_move)
add_test(test_ibensaad_is_over ./game_test_ibensaad is_over)
add_test(test_ibensaad_game_undo ./game_test_ibensaad game_undo)
add_test(test_ibensaad_game_redo ./game_test_ibensaad game_redo)

add_test(test_ibensaad_game_solve ./game_test_ibensaad game_solve)
add_test(test_ibensaad_game_nb_solutions ./game_test_ibensaad game_nb_solutions)

# <====== GAME TESTS rdubernard ======>
target_link_libraries(game_test_rdubernard game)
add_test(test_rdubernard_dummy ./game_test_rdubernard dummy)
add_test(test_rdubernard_game_get_square ./game_test_rdubernard game_get_square)
add_test(test_rdubernard_game_set_square ./game_test_rdubernard game_set_square)
add_test(test_rdubernard_game_delete ./game_test_rdubernard game_delete)
add_test(test_rdubernard_game_equal ./game_test_rdubernard game_equal)
add_test(test_rdubernard_game_copy ./game_test_rdubernard game_copy)
add_test(test_rdubernard_game_new ./game_test_rdubernard game_new)
add_test(test_rdubernard_game_new_empty ./game_test_rdubernard game_new_empty)
add_test(test_rdubernard_game_nb_rows ./game_test_rdubernard game_nb_rows)
add_test(test_rdubernard_game_nb_cols ./game_test_rdubernard game_nb_cols)
add_test(test_rdubernard_game_new_ext ./game_test_rdubernard game_new_ext)
add_test(test_rdubernard_game_is_over_unique  ./game_test_rdubernard game_is_over_unique)
add_test(test_rdubernard_game_is_over_wrapping  ./game_test_rdubernard game_is_over_wrapping)
