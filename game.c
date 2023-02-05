#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annex_funcs.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

// [===== GAME FONCTIONS =====]
game game_new(square* squares) {
  // checking existance of squares
  check(squares != NULL,
         "game_new(square * squares) : squares is pointing on nothing");

  // game mallocation & properties assignment
  game g = malloc(sizeof(struct game_s));
  g->width = DEFAULT_SIZE;
  g->height = DEFAULT_SIZE;
  g->unique = false;
  g->wrapping = false;
  g->last_moves = queue_new();
  g->cancelled_moves = queue_new();

  // assign the grid to the game (by a copy)
  uint nb_cases = game_nb_cols(g) * game_nb_rows(g);
  g->grid = grid_copy(squares, nb_cases);

  // output
  return g;
}

game game_new_empty(void) {
  // generating an S_EMPTY grid on DEFAULT SIZE
  uint nb_cases = DEFAULT_SIZE * DEFAULT_SIZE;
  square* squares_empty = calloc(nb_cases, sizeof(square));  // 0 <=> S_EMPTY

  // create the game with our recently generated empty grid
  game game_empty = game_new(squares_empty);
  free(squares_empty);  //  no longer needed (copied by game_new)

  // output
  return game_empty;
}

game game_copy(cgame g) {
  // checking existance of game
  check(g != NULL, "game_copy(cgame g) : g pointing on nothing");

  // cloning the game
  game game_clone = game_new_ext(game_nb_rows(g), game_nb_cols(g), g->grid,
                                 game_is_wrapping(g), game_is_unique(g));

  // output
  return game_clone;
}

bool game_equal(cgame g1, cgame g2) {
  check(g1 != NULL, "game_equal(cgame g1, cgame g2) : g1 pointing on nothing");
  check(g2 != NULL, "game_equal(cgame g1, cgame g2) : g2 pointing on nothing");

  // verifying if the dimensions of both games are equals
  if (!identical_game_dimension(g1, g2) || !identical_game_dimension(g1, g2)) return false;

  // traverse and compare every square of both games
  for (int x = 0; x < g1->width; x++) {
    for (int y = 0; y < g1->height; y++) {
      square g1_current_square = game_get_square(g1, x, y);
      square g2_current_square = game_get_square(g2, x, y);
      if (!identical_squares(g1_current_square, g2_current_square)) {
        return false;
      }
    }

  }
  // out of the loop -> identical games
  return true;
}

void game_delete(game g) {
  if (g->grid != NULL) free(g->grid);
  if (g->last_moves != NULL) queue_free_full(g->last_moves, free);
  if (g->cancelled_moves != NULL) queue_free_full(g->cancelled_moves, free);
  free(g);
}

void game_set_square(game g, uint i, uint j, square s) {
  check(g != NULL, "game_set_square(game g) : g pointing on nothing");
  check(i < game_nb_rows(g), "game_set_square(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_set_square(uint j) : j over grid");
  check(s >= S_EMPTY && s <= S_IMMUTABLE_ONE,
         "game_set_square(square s) : s not a square");

  uint index_to_set = (i * g->width) + j;
  g->grid[index_to_set] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  check(g != NULL, "game_get_square(game g) : g pointing on nothing");
  check(i < game_nb_rows(g), "game_get_square(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_get_square(uint j) : j over grid");

  uint index_to_get = (i * g->width) + j;
  return g->grid[index_to_get];
}

int game_get_number(cgame g, uint i, uint j) {
  check(g != NULL, "game_get_number(game g) : g pointing on nothing");
  check(i < game_nb_rows(g), "game_get_number(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_get_number(uint j) : j over grid");

  square s = game_get_square(g, i, j);

  if (one_square(s))
    return 1;
  else if (zero_square(s))
    return 0;
  else
    return -1;
}

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist) {
  check(g != NULL, "game_get_next_square(cgame g) : g is pointing on nothing");
  check(dist <= 2, "game_get_next_square(uint dist) : dist too far");
  check(i < game_nb_rows(g), "game_get_next_square(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_get_next_square(uint j) : j over grid");

  if (!game_is_wrapping(g)) {
    // REAJUST THE POSITION WITH THE DISTANCE PARAMETER
    if (dir == UP) i -= dist;
    if (dir == DOWN) i += dist;
    if (dir == LEFT) j -= dist;
    if (dir == RIGHT) j += dist;

    // CHECKING IF NEW COORDINATES ARE STILL INSIDE GRID

    if (i >= g->height || j >= g->width) return -1;
    return game_get_square(g, i, j);
  }

  int ii = i;
  int jj = j;

  // REAJUST THE POSITION WITH THE DISTANCE PARAMETER
  if (dir == UP) ii -= dist;
  if (dir == DOWN) ii += dist;
  if (dir == LEFT) jj -= dist;
  if (dir == RIGHT) jj += dist;

  // CHECKING IF NEW COORDINATES ARE STILL INSIDE GRID

  if (ii >= (int)g->height) {
    ii = ii - g->height;
  }

  if (jj >= (int)g->width) {
    jj = jj - g->width;
  }

  if (ii < 0) {
    ii = ii + g->height;
  }

  if (jj < 0) {
    jj = jj + g->width;
  }
  i = ii;
  j = jj;

  return game_get_square(g, i, j);
}

int game_get_next_number(cgame g, uint i, uint j, direction dir,
                         uint dist) {  // gab

  check(g, "game_get_next_number(cgame g) : g pointing on nothing");

  // if (i >= g->height || j >= g->width || dist > 2) return -1;

  square s = game_get_next_square(g, i, j, dir, dist);

  if (one_square(s))
    return 1;
  else if (zero_square(s))
    return 0;
  else
    return -1;
}

bool game_is_empty(cgame g, uint i, uint j) {
  check(g != NULL, "game_empty_square(cgame g) : g is pointing on nothing");
  check(i < game_nb_rows(g), "game_empty_square(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_empty_square(uint j) : j over grid");

  return empty_square(game_get_square(g, i, j));
}

bool game_is_immutable(cgame g, uint i, uint j) {
  check(g != NULL,
         "game_immutable_square(cgame g) : g is pointing on nothing");
  check(i < game_nb_rows(g), "game_immutable_square(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_immutable_square(uint j) : j over grid");

  return immutable_square(game_get_square(g, i, j));
}

int game_has_error(cgame g, uint i, uint j) {
  check(g != NULL, "game_has_error(cgame g) : g is pointing on nothing");
  check(i < game_nb_rows(g), "game_has_error(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_has_error(uint j) : j over grid");

  if (three_identical_consecutives_squares_on_pos(g, i, j)) {
    return GAME_HAS_ERROR;
  }

  if (!parity_lines(g, i, j)) {
    return GAME_HAS_ERROR;
  }

  if (game_is_unique(g) && unicity_disrespected(g, i, j)) {
    return GAME_HAS_ERROR;
  }

  return GAME_HAS_NO_ERROR;
}

bool game_check_move(cgame g, uint i, uint j, square s) {
  check(g != NULL, "game_check_move(game g) : g is NULL");

  if (i >= game_nb_rows(g) || j >= game_nb_cols(g)) return false;

  square case_actuelle = game_get_square(g, i, j);

  if (case_actuelle == S_IMMUTABLE_ONE || case_actuelle == S_IMMUTABLE_ZERO ||
      s == S_IMMUTABLE_ONE || s == S_IMMUTABLE_ZERO)
    return false;

  return true;
}

void game_play_move(game g, uint i, uint j, square s) {
  check(g != NULL, "game_play_move(cgame g) : g is pointing on nothing");
  check(i < game_nb_rows(g), "game_play_move(uint i) : i over grid");
  check(j < game_nb_cols(g), "game_play_move(uint j) : j over grid");
  check(!game_is_immutable(g, i, j),
         "game_play_move(uint i, uint j) : square is immutable");
  check(!immutable_square(s), "game_play_move(square s) : s is immutable");

  // We create an array for store the current move
  int* move = malloc(sizeof(int) * MOVE_SIZE);
  move[MOVE_SQUARE_INDEX] = game_get_square(g, i, j);  // store square
  move[MOVE_I_INDEX] = i;                              // store i
  move[MOVE_J_INDEX] = j;                              // store j

  game_set_square(g, i, j, s);

  queue_push_head(g->last_moves, move);
  queue_clear_full(g->cancelled_moves, free);
}

bool game_is_over(cgame g) {
  // We test if our pointer g is NULL, if it is the case we exit the programm.

  check(g != NULL, "game_is_over(game g) : g is NULL!");

  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == S_EMPTY) return false;
      if (game_has_error(g, i, j) != 0) {
        return false;
      }
    }
  }

  // In case we haven't returned false, all the rules are satisied and we
  // return true ---> the game is won.
  return true;
}

void game_clear_history(game g) {
  // clear all history
  queue_free_full(g->last_moves, free);
  queue_free_full(g->cancelled_moves, free);

  g->last_moves = queue_new();
  g->cancelled_moves = queue_new();
}

void game_reset_default(game g) {
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == S_ONE) game_set_square(g, i, j, S_EMPTY);

      if (game_get_square(g, i, j) == S_ZERO) game_set_square(g, i, j, S_EMPTY);
    }
  }
}

void game_restart(game g) {
  check(g != NULL, "game_restart(game g) : g is NULL!");

  game_reset_default(g);
  game_clear_history(g);
}
