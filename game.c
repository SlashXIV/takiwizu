#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

// [===== AUXILIARY FONCTIONS =====]

// verifies if the expression is true, else it abort on error_code
void assert(bool expr, const char* error_code) {
  if (!expr) {
    fputs("ERROR : ", stderr);
    fputs(error_code, stderr);
    fputs(" ;-;\n", stderr);
    exit(EXIT_FAILURE);
  }
}

// returns a dynamic-allocated square array identical to the first parameter
square* grid_copy(square* squares_primal, uint nb_squares) {
  square* squares_clone = malloc(nb_squares * sizeof(square));

  for (int i = 0; i < nb_squares; i++) {
    squares_clone[i] = squares_primal[i];
  }

  return squares_clone;
}

// returns true if both squares are equals, false otherwise
bool identical_squares(square S1, square S2) { return (S1 == S2); }

// returns true if both game dimensions are equals, false otherwise
bool identical_game_dimension(cgame g1, cgame g2) {
  return ((game_nb_cols(g1) == game_nb_cols(g2)) &&
          (game_nb_rows(g1) == game_nb_rows(g2)));
}

// returns true if the square is a one (immutable or not), false otherwise
bool one_square(square s) { return (s == S_IMMUTABLE_ONE || s == S_ONE); }

// returns true if the square is a zero (immutable or not), false otherwise
bool zero_square(square s) { return (s == S_IMMUTABLE_ZERO || s == S_ZERO); }

// returns true if empty square, false otherwise
bool empty_square(square s) { return (s == S_EMPTY); }

// returns true if IMMUTABLE square, false otherwise
bool immutable_square(square s) {
  return (s == S_IMMUTABLE_ONE || s == S_IMMUTABLE_ZERO);
}

// returns true if there is 3 consecutives equal squares in the game, false
// otherwise
bool three_identical_consecutives_squares_on_pos(cgame g, uint i, uint j) {
  int n = game_get_number(g, i, j);

  if (n == -1) return false;  // mean that we ignore empty cases

  int n_l = game_get_next_number(g, i, j, LEFT, 1);
  int n_u = game_get_next_number(g, i, j, UP, 1);
  int n_r = game_get_next_number(g, i, j, RIGHT, 1);
  int n_d = game_get_next_number(g, i, j, DOWN, 1);

  if (n_l == n && n_r == n) return true;
  if (n_u == n && n_d == n) return true;

  int n_ll = game_get_next_number(g, i, j, LEFT, 2);
  int n_uu = game_get_next_number(g, i, j, UP, 2);
  int n_rr = game_get_next_number(g, i, j, RIGHT, 2);
  int n_dd = game_get_next_number(g, i, j, DOWN, 2);

  int left_neighbours[2] = {n_l, n_ll};
  int up_neighbours[2] = {n_u, n_uu};
  int right_neighbours[2] = {n_r, n_rr};
  int down_neighbours[2] = {n_d, n_dd};

  int* neighbours_directions[4] = {left_neighbours, up_neighbours,
                                   right_neighbours, down_neighbours};
  uint dist = 0;
  for (uint dir = 0; dir < NEIGHBOURS_DIRS; dir++) {
    if (neighbours_directions[dir][dist] == -1 ||
        neighbours_directions[dir][dist + 1] == -1)
      continue;

    if (n != neighbours_directions[dir][dist]) continue;

    if (neighbours_directions[dir][dist] ==
        neighbours_directions[dir][dist + 1])
      return true;
  }

  return false;
}

bool parity_lines(cgame g, uint i, uint j) {
  uint cpt_zero_col = 0;
  uint cpt_zero_row = 0;
  uint cpt_one_col = 0;
  uint cpt_one_row = 0;

  for (int h = 0; h < game_nb_rows(g); h++) {
    switch (game_get_number(g, h, j)) {
      case 0:
        cpt_zero_col++;
        break;

      case 1:
        cpt_one_col++;
        break;

      default:
        return true;
    }
  }

  for (int w = 0; w < game_nb_rows(g); w++) {
    switch (game_get_number(g, i, w)) {
      case 0:
        cpt_zero_row++;
        break;

      case 1:
        cpt_one_row++;
        break;

      default:  // -1, empty
        return true;
    }
  }

  if (cpt_zero_col == cpt_one_col && cpt_zero_row == cpt_one_row) return true;

  return false;
}

bool unicity_disrespected(cgame g, uint i, uint j) {
  bool test_cols = true;
  bool test_rows = true;

  for (int x = 0; x < game_nb_cols(g); x++) {
    if (game_get_square(g, i, x) == S_EMPTY) test_rows = false;
  }

  for (int y = 0; y < game_nb_rows(g); y++) {
    if (game_get_square(g, y, j) == S_EMPTY) test_cols = false;
  }

  if (test_rows) {
    uint cpt_equal_case;

    // ON SAUVEGARDE LA LIGNE (sur laquelle i se trouve) À COMPARER PARMI LES
    // AUTRES LIGNES
    square compare_line[game_nb_cols(g)];
    for (uint case_index = 0; case_index < game_nb_cols(g); case_index++) {
      compare_line[case_index] = game_get_square(g, i, case_index);
    }

    // COMPARAISON DE LA LIGNE SAUVEGARDÉ AVEC TOUTES LES AUTRES LIGNES :
    for (int y = 0; y < game_nb_rows(g); y++) {
      // re-initialized cpt to 0 for each loop
      cpt_equal_case = 0;

      // LES COMPARAISONS
      for (int x = 0; x < game_nb_cols(g); x++) {
        // IGNORE ITSELF
        if (y == i) break;

        // ONE DIFFERENCE IS ENOUGH TO COMPARE THE NEXT LINE
        if (compare_line[x] != game_get_square(g, y, x)) break;

        cpt_equal_case++;
      }

      // TWO LINES ARE IDENTICALS
      if (cpt_equal_case == game_nb_cols(g)) {
        return true;
      }
    }
  }

  if (test_cols) {
    uint cpt_equal_case;

    // ON SAUVEGARDE LA LIGNE (sur laquelle i se trouve) À COMPARER PARMI LES
    // AUTRES LIGNES
    square compare_line[game_nb_rows(g)];
    for (uint case_index = 0; case_index < game_nb_rows(g); case_index++) {
      compare_line[case_index] = game_get_square(g, case_index, j);
    }

    // COMPARAISON DE LA LIGNE SAUVEGARDÉ AVEC TOUTES LES AUTRES LIGNES :
    for (int x = 0; x < game_nb_cols(g); x++) {
      // re-initialized cpt to 0 for each loop
      cpt_equal_case = 0;

      // LES COMPARAISONS
      for (int y = 0; y < game_nb_rows(g); y++) {
        // IGNORE ITSELF
        if (x == j) break;

        if (compare_line[y] != game_get_square(g, y, x)) break;

        cpt_equal_case++;
      }

      if (cpt_equal_case == game_nb_rows(g)) return true;
    }
  }
  return false;
}

// [===== GAME FONCTIONS =====]
game game_new(square* squares) {
  // checking existance of squares
  assert(squares != NULL,
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
  assert(g != NULL, "game_copy(cgame g) : g pointing on nothing");

  // cloning the game
  game game_clone = game_new_ext(game_nb_rows(g), game_nb_cols(g), g->grid,
                                 game_is_wrapping(g), game_is_unique(g));

  // output
  return game_clone;
}

bool game_equal(cgame g1, cgame g2) {
  assert(g1 != NULL, "game_equal(cgame g1, cgame g2) : g1 pointing on nothing");
  assert(g2 != NULL, "game_equal(cgame g1, cgame g2) : g2 pointing on nothing");

  // verifying if the dimensions of both games are equals
  if (!identical_game_dimension(g1, g2)) return false;

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
  assert(g != NULL, "game_set_square(game g) : g pointing on nothing");
  assert(i < game_nb_rows(g), "game_set_square(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_set_square(uint j) : j over grid");
  assert(s >= S_EMPTY && s <= S_IMMUTABLE_ONE,
         "game_set_square(square s) : s not a square");

  uint index_to_set = (i * g->width) + j;
  g->grid[index_to_set] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  assert(g != NULL, "game_get_square(game g) : g pointing on nothing");
  assert(i < game_nb_rows(g), "game_get_square(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_get_square(uint j) : j over grid");

  uint index_to_get = (i * g->width) + j;
  return g->grid[index_to_get];
}

int game_get_number(cgame g, uint i, uint j) {
  assert(g != NULL, "game_get_number(game g) : g pointing on nothing");
  assert(i < game_nb_rows(g), "game_get_number(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_get_number(uint j) : j over grid");

  square s = game_get_square(g, i, j);

  if (one_square(s))
    return 1;
  else if (zero_square(s))
    return 0;
  else
    return -1;
}

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist) {
  assert(g != NULL, "game_get_next_square(cgame g) : g is pointing on nothing");
  assert(dist <= 2, "game_get_next_square(uint dist) : dist too far");
  assert(i < game_nb_rows(g), "game_get_next_square(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_get_next_square(uint j) : j over grid");

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

  assert(g, "game_get_next_number(cgame g) : g pointing on nothing");

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
  assert(g != NULL, "game_empty_square(cgame g) : g is pointing on nothing");
  assert(i < game_nb_rows(g), "game_empty_square(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_empty_square(uint j) : j over grid");

  return empty_square(game_get_square(g, i, j));
}

bool game_is_immutable(cgame g, uint i, uint j) {
  assert(g != NULL,
         "game_immutable_square(cgame g) : g is pointing on nothing");
  assert(i < game_nb_rows(g), "game_immutable_square(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_immutable_square(uint j) : j over grid");

  return immutable_square(game_get_square(g, i, j));
}

int game_has_error(cgame g, uint i, uint j) {
  assert(g != NULL, "game_has_error(cgame g) : g is pointing on nothing");
  assert(i < game_nb_rows(g), "game_has_error(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_has_error(uint j) : j over grid");

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
  if (g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  if (i >= g->height || j >= g->width) return false;

  square cas = game_get_square(g, i, j);

  if (cas == S_IMMUTABLE_ONE || cas == S_IMMUTABLE_ZERO ||
      s == S_IMMUTABLE_ONE || s == S_IMMUTABLE_ZERO)
    return false;

  return true;
}

void game_play_move(game g, uint i, uint j, square s) {
  if (i >= g->height || j >= g->width || g == NULL ||
      game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO || s == S_IMMUTABLE_ONE ||
      s == S_IMMUTABLE_ZERO) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  // We create an array for store the current move
  int* move = malloc(sizeof(int) * MOVE_SIZE);
  move[MOVE_SQUARE_INDEX] = game_get_square(g, i, j);  // store square
  move[MOVE_I_INDEX] = i;       // store i
  move[MOVE_J_INDEX] = j;       // store j

  game_set_square(g, i, j, s);

  queue_push_head(g->last_moves, move);
  queue_clear_full(g->cancelled_moves, free);
}

bool game_is_over(cgame g) {
  // We test if our pointer g is NULL, if it is the case we exit the programm.

  if (g == NULL) {
    exit(EXIT_FAILURE);
  }

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

void game_restart(game g) {
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == S_ONE) game_set_square(g, i, j, S_EMPTY);

      if (game_get_square(g, i, j) == S_ZERO) game_set_square(g, i, j, S_EMPTY);
    }
  }

  // clear all history
  queue_free_full(g->last_moves, free);
  queue_free_full(g->cancelled_moves, free);

  g->last_moves = queue_new();
  g->cancelled_moves = queue_new();
}
