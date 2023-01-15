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
void assert(bool expr, char * error_code){
  if (!expr) {
    fprintf(stderr, "ERROR : \n\t");
    fprintf(stderr, error_code);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
  } 
}

// returns a dynamic-allocated square array identical to the one in the first parameter
square * grid_copy(square * squares_primal, uint nb_squares) {
  square* squares_clone = malloc(nb_squares * sizeof(square));

  for (int i = 0; i < nb_squares; i++) {
    squares_clone[i] = squares_primal[i];
  }

  return squares_clone;
}

// returns true if both squares are equals, false otherwise
bool identical_squares(square S1, square S2){
  return (S1 == S2);
}

// returns true if both game dimensions are equals, false otherwise
bool identical_game_dimension(cgame g1, cgame g2){
  return ((game_nb_cols(g1) == game_nb_cols(g2)) && (game_nb_rows(g1) == game_nb_rows(g2)));
}


// [===== GAME FONCTIONS =====]
game game_new(square* squares) {
  // checking existance of squares
  assert(squares != NULL, "game_new(square * squares) : squares is pointing on nothing");

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
  g->grid = grid_copy(squares, nb_cases);;

  // output
  return g;
}

game game_new_empty(void) {
  // generating an S_EMPTY grid on DEFAULT SIZE
  uint nb_cases = DEFAULT_SIZE * DEFAULT_SIZE;
  square* squares_empty = calloc(nb_cases, sizeof(square)); // 0 <=> S_EMPTY

  // create the game with our recently generated empty grid
  game game_empty = game_new(squares_empty);
  free(squares_empty); //  no longer needed (copied by game_new)

  // output
  return game_empty;
}

game game_copy(cgame g) {
  // checking existance of game
  assert(g != NULL, "game_copy(cgame g) : g pointing on nothing");

  // cloning the game
  game game_clone = game_new_ext(
    game_nb_rows(g),
    game_nb_cols(g),
    grid_copy(g->grid, game_nb_rows(g)*game_nb_cols(g)),
    game_is_wrapping(g),
    game_is_unique(g)
  );

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
  assert(g != NULL, "game_set_square(game g) : g pointing on nothing\n");
  assert(i < game_nb_rows(g), "game_set_square(uint i) : i over grid");
  assert(j < game_nb_cols(g), "game_set_square(uint j) : j over grid");
  assert(s >= S_EMPTY && s <= S_IMMUTABLE_ONE, "game_set_square(square s) : s not a square");

  uint index_to_set = (i * g->width) + j;
  g->grid[index_to_set] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  if (g == NULL) {
    fprintf(stderr,
            "ERROR on game_get_square(game g, uint i, uint j : invalid "
            "parameters; g pointing on nothing...\n");
    exit(EXIT_FAILURE);
  } else if (i >= g->height || j >= g->width) {
    fprintf(stderr,
            "ERROR on game_get_square(game g, uint i, uint j, : invalid "
            "parameters; i (%u) or j (%u)\n",
            i, j);
    exit(EXIT_FAILURE);
  }

  // CIBLE VERROUILLÉE ...
  uint index_to_get = (i * g->width) + j;

  // GET THAT SQUARE AND GIVE IT BACK
  return g->grid[index_to_get];
}

int game_get_number(cgame g, uint i, uint j) {
  if (i >= g->height || j >= g->width || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_ONE)
    return 1;

  if (game_get_square(g, i, j) == S_IMMUTABLE_ZERO ||
      game_get_square(g, i, j) == S_ZERO)
    return 0;

  else
    return -1;
}

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist) {
  // CHECKING GAME AND DIST
  if (g == NULL || dist > 2) {
    fprintf(stderr, "g is null, or  dist too far :/\n");
    return -1;
  }
  // CHECKING IF INITIAL COORDINATES ARE OVER GRID
  if (i >= g->height || j >= g->width) return -1;

  int ii = i;
  int jj = j;

  if (g->wrapping) {
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

  } else {
    // REAJUST THE POSITION WITH THE DISTANCE PARAMETER
    if (dir == UP) i -= dist;
    if (dir == DOWN) i += dist;
    if (dir == LEFT) j -= dist;
    if (dir == RIGHT) j += dist;

    // CHECKING IF NEW COORDINATES ARE STILL INSIDE GRID

    if (i >= g->height || j >= g->width) return -1;
    return game_get_square(g, i, j);
  }
}

int game_get_next_number(cgame g, uint i, uint j, direction dir,
                         uint dist) {  // gab

  if (g == NULL || i >= g->height || j >= g->width || dist > 2) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    return -1;
  }

  if (game_get_next_square(g, i, j, dir, dist) == S_IMMUTABLE_ONE ||
      game_get_next_square(g, i, j, dir, dist) == S_ONE)
    return 1;

  if (game_get_next_square(g, i, j, dir, dist) == S_IMMUTABLE_ZERO ||
      game_get_next_square(g, i, j, dir, dist) == S_ZERO)
    return 0;

  else
    return -1;
}

bool game_is_empty(cgame g, uint i, uint j) {
  if (i > g->height || j > g->width || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  return game_get_square(g, i, j) == S_EMPTY;
}

bool game_is_immutable(cgame g, uint i, uint j) {
  if (i >= g->height || j >= g->width || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE)
    return true;

  else if (game_get_square(g, i, j) == S_IMMUTABLE_ZERO)
    return true;

  else
    return false;
}

int game_has_error(cgame g, uint i, uint j) {
  if (i > g->height || j > g->width || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  // THE FUNCTION SHOULD ONLY RETURN AN ERROR AND DO NOT PRINT ANYTHING AT ALL,
  // THE PRINT DOES THIS IN THE GAME_TEXT DO NOT HAVE 3 CONSECUTIVE CAD three in
  // a row => WWW BBB, W BBB

  int primaryCase = game_get_number(g, i, j);

  if (primaryCase != -1) {  // mean that we ignore empty cases

    if (game_get_next_number(g, i, j, DOWN, 1) == primaryCase &&
        game_get_next_number(g, i, j, DOWN, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, RIGHT, 1) == primaryCase &&
        game_get_next_number(g, i, j, RIGHT, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, UP, 1) == primaryCase &&
        game_get_next_number(g, i, j, UP, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, LEFT, 1) == primaryCase &&
        game_get_next_number(g, i, j, LEFT, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, LEFT, 1) == primaryCase &&
        game_get_next_number(g, i, j, RIGHT, 1) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, DOWN, 1) == primaryCase &&
        game_get_next_number(g, i, j, UP, 1) == primaryCase)
      return 1;

    int whiteCol = 0;
    int whiteLine = 0;
    int blackCol = 0;
    int blackLine = 0;

    for (int h = 0; h < g->height; h++) {
      if (game_get_square(g, h, j) == S_ZERO ||
          game_get_square(g, h, j) == S_IMMUTABLE_ZERO)
        whiteLine++;

      if (game_get_square(g, h, j) == S_ONE ||
          game_get_square(g, h, j) == S_IMMUTABLE_ONE)
        blackLine++;
    }

    for (int w = 0; w < g->width; w++) {
      if (game_get_square(g, i, w) == S_ZERO ||
          game_get_square(g, i, w) == S_IMMUTABLE_ZERO)
        whiteCol++;

      if (game_get_square(g, i, w) == S_ONE ||
          game_get_square(g, i, w) == S_IMMUTABLE_ONE)
        blackCol++;
    }

    if (whiteLine > 3 || whiteCol > 3 || blackCol > 3 ||
        blackLine > 3)  // we check the parity of the squares if >3 mean that
                        // parity isnt respected
      return 1;
  }

  if (g->unique) {
    // printf("=> TEST UNIQUE\n");

    bool test_cols = true;
    bool test_rows = true;

    for (int x = 0; x < g->width; x++) {
      if (game_get_square(g, i, x) == S_EMPTY) test_rows = false;
    }

    for (int y = 0; y < g->height; y++) {
      if (game_get_square(g, y, j) == S_EMPTY) test_cols = false;
    }

    // printf("=> DOINE : \n\ttest_cols = %d\n\ttest_rows = %d\n",
    // test_cols,test_rows);

    // lets check the collumns first

    if (test_rows) {
      uint cpt_equal_case;

      // ON SAUVEGARDE LA LIGNE (sur laquelle i se trouve) À COMPARER PARMI LES
      // AUTRES LIGNES
      square compare_line[g->width];
      for (uint case_index = 0; case_index < g->width; case_index++) {
        compare_line[case_index] = game_get_square(g, i, case_index);
      }

      // COMPARAISON DE LA LIGNE SAUVEGARDÉ AVEC TOUTES LES AUTRES LIGNES :
      for (int y = 0; y < g->height; y++) {
        // re-initialized cpt to 0 for each loop
        cpt_equal_case = 0;

        // LES COMPARAISONS
        for (int x = 0; x < g->width; x++) {
          // IGNORE ITSELF
          if (y == i) break;

          // ONE DIFFERENCE IS ENOUGH TO COMPARE THE NEXT LINE
          if (compare_line[x] != game_get_square(g, y, x)) break;

          cpt_equal_case++;
        }

        // TWO LINES ARE IDENTICALS
        if (cpt_equal_case == g->width) {
          return -1;
        }
      }
    }

    if (test_cols) {
      uint cpt_equal_case;

      // ON SAUVEGARDE LA LIGNE (sur laquelle i se trouve) À COMPARER PARMI LES
      // AUTRES LIGNES
      square compare_line[g->height];
      for (uint case_index = 0; case_index < g->height; case_index++) {
        compare_line[case_index] = game_get_square(g, case_index, j);
      }

      // COMPARAISON DE LA LIGNE SAUVEGARDÉ AVEC TOUTES LES AUTRES LIGNES :
      for (int x = 0; x < g->width; x++) {
        // re-initialized cpt to 0 for each loop
        cpt_equal_case = 0;

        // LES COMPARAISONS
        for (int y = 0; y < g->height; y++) {
          // IGNORE ITSELF
          if (x == j) break;

          if (compare_line[y] != game_get_square(g, y, x)) break;

          cpt_equal_case++;
        }

        if (cpt_equal_case == g->height) {
          return -1;
        }
      }
    }
  }

  // now the rows

  return 0;  // 0 mean that there is no error
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

  game_set_square(g, i, j, s);

  // We create an array for store the current move
  int* move = malloc(sizeof(int) * MOVE_SIZE);
  move[MOVE_SQUARE_INDEX] = s;  // store square
  move[MOVE_I_INDEX] = i;       // store i
  move[MOVE_J_INDEX] = j;       // store j

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

  // In case we haven't returned false, all the rules are satisied and we return
  // true ---> the game is won.
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
