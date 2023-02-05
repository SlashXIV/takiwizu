#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annex_funcs.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

void usage(char *cmd) {
  fprintf(stderr, "Usage %s <testname> [<...>]\n", cmd);
  exit(EXIT_FAILURE);
}

bool test_dummy(void) { return true; }

bool test_game_get_square(void) {
  // game init simulation
  game g = game_default();
  game_set_square(g, 0, 0, S_ONE);
  game_set_square(g, 0, 5, S_ZERO);

  // game_default() each total square on this basic configuration
  unsigned int total_empty = 25;
  unsigned int total_immutable_white = 6;
  unsigned int total_immutable_black = 3;
  unsigned int total_white = 1;
  unsigned int total_black = 1;

  // cpt of each square type
  unsigned int cpt_empty = 0;
  unsigned int cpt_immutable_white = 0;
  unsigned int cpt_immutable_black = 0;
  unsigned int cpt_black = 0;
  unsigned int cpt_white = 0;

  // counting each square type on default config
  for (uint x = 0; x < DEFAULT_SIZE; x++) {
    for (uint y = 0; y < DEFAULT_SIZE; y++) {
      square actual_square = game_get_square(g, x, y);
      switch (actual_square) {
        // actual square type => empty
        case S_EMPTY:
          cpt_empty++;
          break;

        // actual square type => immutable black
        case S_IMMUTABLE_ONE:
          cpt_immutable_black++;
          break;

        // actual square type => immutable white
        case S_IMMUTABLE_ZERO:
          cpt_immutable_white++;
          break;

        // actual square type => black
        case S_ONE:
          // printf("there is a black square at (%d, %d)\n",x, y);
          cpt_black = cpt_black + 1;
          break;

        // actual square type => white
        case S_ZERO:
          cpt_white++;
          break;

        default:
          // printf("zero ?\n");
          break;
      }
    }
  }

  // verifying that the values match
  bool values_matches = (cpt_white == total_white && cpt_empty == total_empty &&
                         cpt_black == total_black &&
                         cpt_immutable_black == total_immutable_black &&
                         cpt_immutable_white == total_immutable_white);

  game_delete(g);

  // découle de l'utilisation itérée de game_get_square()
  return values_matches;
}

// ISSUE #6 -> game_set_square()
bool test_game_set_square(void) {
  // GENERATE EMPTY GAME
  game g = game_new_empty();
  check(g, "g is null");

  // FILLING 'b' then 'w' MID
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE / 2; y++) {
      game_set_square(g, x, y, S_ONE);
    }
    for (int y = DEFAULT_SIZE / 2; y < DEFAULT_SIZE; y++) {
      game_set_square(g, x, y, S_ZERO);
    }
  }

  game_print(g);

  // VERIF
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE / 2; y++) {
      printf("%d", game_get_square(g, x, y) == S_ONE);
      if (game_get_square(g, x, y) != S_ONE) {
        game_delete(g);
        return false;
      }
    }
    for (int y = DEFAULT_SIZE / 2; y < DEFAULT_SIZE; y++) {
      printf("%d", game_get_square(g, x, y) == S_ZERO);
      if (game_get_square(g, x, y) != S_ZERO) {
        game_delete(g);
        return false;
      }
    }
    printf("\n");
  }

  // CLEAR
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_square(g, x, y, S_EMPTY);
    }
  }

  game_print(g);

  // FILLING EACH SQUARE AT RANDOM POS
  game_set_square(g, 2, 4, S_IMMUTABLE_ONE);
  game_set_square(g, 0, 1, S_IMMUTABLE_ZERO);
  game_set_square(g, 0, 2, S_ONE);
  game_set_square(g, 0, 3, S_ZERO);
  game_set_square(g, 0, 4, S_EMPTY);
  game_set_square(g, 3, 2, S_ONE);
  game_set_square(g, 5, 1, S_ZERO);
  game_set_square(g, 3, 4, S_IMMUTABLE_ONE);
  game_set_square(g, 0, 3, S_EMPTY);

  game_print(g);

  // AND CHECKING IF IT WORKED
  bool values_matches =
      game_get_square(g, 3, 2) == S_ONE && game_get_square(g, 5, 1) == S_ZERO &&
      game_get_square(g, 3, 4) == S_IMMUTABLE_ONE &&
      game_get_next_square(g, 2, 3, RIGHT, 1) == S_IMMUTABLE_ONE &&
      game_get_square(g, 0, 1) == S_IMMUTABLE_ZERO &&
      game_get_square(g, 0, 2) == S_ONE &&
      game_get_square(g, 0, 3) == S_EMPTY &&
      game_get_square(g, 0, 4) == S_EMPTY;

  game_set_square(g, 4, 2, S_ONE);
  if (game_get_square(g, 4, 2) != S_ONE) {
    game_delete(g);
    return false;
  }

  game_set_square(g, 4, 2, S_EMPTY);
  if (game_get_square(g, 4, 2) != S_EMPTY) {
    game_delete(g);
    return false;
  }

  game_set_square(g, 1, 5, S_ZERO);
  if (game_get_square(g, 1, 5) != S_ZERO) {
    game_delete(g);
    return false;
  }

  game_set_square(g, 2, 1, S_IMMUTABLE_ONE);
  if (game_get_square(g, 2, 4) != S_IMMUTABLE_ONE) {
    game_delete(g);
    return false;
  }

  game_set_square(g, 3, 4, S_IMMUTABLE_ZERO);
  if (game_get_square(g, 3, 4) != S_IMMUTABLE_ZERO) {
    game_delete(g);
    return false;
  }

  game_delete(g);

  if (!values_matches) {
    return false;
  }

  return true;
}

// ISSUE #5 -> game_delete()
bool test_game_delete() {
  game g = game_new_empty();
  game_delete(g);
  return true;
}

// ISSUE #4 -> game_equal()
bool test_game_equal() {
  /*
  game g_empty = game_new_empty();
  game g_default = game_default();
  game g_end = game_default_solution();




  if (!game_equal(g_end, g_end)){
      return false;
  }

  if (game_equal(g_empty, g_default)){
      return false;
  }

  if (!game_equal(g_empty, g_empty)){
      return false;
  }

  if (game_equal(g_end, g_default)){
      return false;
  }*/

  game g1 = game_new_empty();
  game g2 = game_new_empty();

  game_set_square(g1, 0, 0, S_EMPTY);
  game_set_square(g1, 5, 5, S_ONE);
  game_set_square(g1, 2, 1, S_IMMUTABLE_ZERO);
  game_set_square(g1, 3, 2, S_IMMUTABLE_ONE);
  game_set_square(g1, 4, 0, S_ZERO);

  game g3 = game_copy(g1);
  game g4 = game_copy(g1);

  game_set_square(g4, 4, 0, S_IMMUTABLE_ZERO);
  game_set_square(g4, 5, 5, S_IMMUTABLE_ONE);

  if (game_equal(g1, g2))  // we modified g1 so its supposed to return false
    return false;

  if (!game_equal(
          g1, g3))  // g3 is a simple copy of g1, so its supposed to return true
    return false;

  if (game_equal(g1, g4))  // g4 is a copy of g1 but we change all the simple
                           // statements by his immutable value
    return false;

  /*game_delete(g_empty);
  game_delete(g_default);
  game_delete(g_end);*/
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);

  return true;
}

// ISSUE 3 -> game_copy()
bool test_game_copy() {
  // GAME DEFAULT V1
  game g_default_primal = game_default();
  game g_default_clone = game_copy(g_default_primal);

  game_print(g_default_clone);
  game_print(g_default_primal);

  // game_default() each total square on this basic configuration
  unsigned int total_empty = 27;
  unsigned int total_immutable_white = 6;
  unsigned int total_immutable_black = 3;
  unsigned int total_white = 0;
  unsigned int total_black = 0;

  // cpt of each square type
  unsigned int cpt_empty = 0;
  unsigned int cpt_immutable_white = 0;
  unsigned int cpt_immutable_black = 0;
  unsigned int cpt_black = 0;
  unsigned int cpt_white = 0;

  // counting each square type on default config
  for (int x = 0; x < game_nb_cols(g_default_primal); x++) {
    for (int y = 0; y < game_nb_rows(g_default_primal); y++) {
      square actual_square = game_get_square(g_default_clone, x, y);

      switch (actual_square) {
        // actual square type => empty
        case S_EMPTY:
          cpt_empty++;
          break;

        // actual square type => immutable black
        case S_IMMUTABLE_ONE:
          cpt_immutable_black++;
          break;

        // actual square type => immutable white
        case S_IMMUTABLE_ZERO:
          cpt_immutable_white++;
          break;

        // actual square type => black
        case S_ONE:
          // printf("there is a black square at (%d, %d)\n",x, y);
          cpt_black++;
          break;

        // actual square type => white
        case S_ZERO:
          cpt_white++;
          break;

        default:
          // printf("zero ?\n");
          break;
      }
    }
  }

  // verifying that the values match
  printf("white : total = %d / cpt = %d\n", total_white, cpt_white);
  printf("empty : total = %d / cpt = %d\n", total_empty, cpt_empty);
  printf("black : total = %d / cpt = %d\n", total_black, cpt_black);
  printf("immutable_black : total = %d / cpt = %d\n", total_immutable_black,
         cpt_immutable_black);
  printf("immutable_white : total = %d / cpt = %d\n", total_immutable_white,
         cpt_immutable_white);
  bool values_matches = (cpt_white == total_white && cpt_empty == total_empty &&
                         cpt_black == total_black &&
                         cpt_immutable_black == total_immutable_black &&
                         cpt_immutable_white == total_immutable_white);

  if (!game_equal(g_default_clone, g_default_primal) || !values_matches) {
    return false;
  }

  // UNIQUE
  if (game_is_unique(g_default_clone) != game_is_unique(g_default_primal))
    return false;

  // WRAPPING
  if (game_is_wrapping(g_default_clone) != game_is_wrapping(g_default_primal))
    return false;

  // COLS
  if (game_nb_cols(g_default_clone) != game_nb_cols(g_default_primal))
    return false;

  // ROWS
  if (game_nb_rows(g_default_clone) != game_nb_rows(g_default_primal))
    return false;

  game_delete(g_default_clone);
  game_delete(g_default_primal);

  game g_ext_primal = game_new_empty_ext(8, 4, true, false);
  game g_ext_clone = game_copy(g_ext_primal);

  // UNIQUE
  if (game_is_unique(g_ext_clone) != game_is_unique(g_ext_primal)) return false;

  // WRAPPING
  if (game_is_wrapping(g_ext_clone) != game_is_wrapping(g_ext_primal))
    return false;

  // COLS
  if (game_nb_cols(g_ext_clone) != game_nb_cols(g_ext_primal)) return false;

  // ROWS
  if (game_nb_rows(g_ext_clone) != game_nb_rows(g_ext_primal)) return false;

  game_delete(g_ext_clone);
  game_delete(g_ext_primal);

  return true;
}

// ISSUE 2 -> game_new_empty()
bool test_game_new_empty() {
  game g = game_new_empty();

  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_get_square(g, x, y) != S_EMPTY) {
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

// ISSUE 1 -> game_new()
bool test_game_new() {
  uint nb_cases = DEFAULT_SIZE * DEFAULT_SIZE;

  square *square_array = malloc(nb_cases * sizeof(square));

  for (int i = 0; i < nb_cases; i++) {
    square_array[i] = S_EMPTY;
  }

  game g = game_new(square_array);
  free(square_array);

  game_print(g);
  game_delete(g);
  return true;
}

bool test_game_nb_rows() {
  // GAMEs CREATION
  game g_default = game_default();
  game g_ext = game_new_empty_ext(4, 6, true, true);

  // Tester la fonction game_nb_rows(g)
  if (game_nb_rows(g_default) != 6) return false;

  if (game_nb_rows(g_ext) != 4) return false;

  // GAMEs DELETION
  game_delete(g_default);
  game_delete(g_ext);

  return true;
}
bool test_game_nb_cols() {
  // GAMEs CREATION
  game g_default = game_default();
  game g_ext = game_new_empty_ext(16, 24, true, true);

  // Tester la fonction game_nb_cols(g)
  if (game_nb_cols(g_default) != 6) return false;

  if (game_nb_cols(g_ext) != 24) return false;

  // GAMEs DELETION
  game_delete(g_default);
  game_delete(g_ext);

  return true;
}

bool test_game_new_ext() {  // inversion nb rows et nb cols
  // square creation
  square *grid = calloc((6 * 4), sizeof(square));

  // GAME CREATION
  game g = game_new_ext(6, 4, grid, true, true);

  // vérification inversion
  if (game_nb_rows(g) == 4) return false;
  if (game_nb_cols(g) == 6) return false;

  game_delete(g);
  free(grid);
  return true;
}

bool test_game_is_over_unique() {
  game g = game_new_empty_ext(4, 4, true, true);

  // create finished game when unique is disabled (BUT here it's enabled)
  game_set_square(g, 0, 0, S_ONE);
  game_set_square(g, 0, 1, S_ZERO);
  game_set_square(g, 0, 2, S_ONE);
  game_set_square(g, 0, 3, S_ZERO);

  game_set_square(g, 1, 0, S_ZERO);
  game_set_square(g, 1, 1, S_ONE);
  game_set_square(g, 1, 2, S_ZERO);
  game_set_square(g, 1, 3, S_ONE);

  game_set_square(g, 2, 0, S_ONE);
  game_set_square(g, 2, 1, S_ZERO);
  game_set_square(g, 2, 2, S_ONE);
  game_set_square(g, 2, 3, S_ZERO);

  game_set_square(g, 3, 0, S_ZERO);
  game_set_square(g, 3, 1, S_ONE);
  game_set_square(g, 3, 2, S_ZERO);
  game_set_square(g, 3, 3, S_ONE);

  // game should not be over because of unique;

  bool game_over = game_is_over(g);
  game_print(g);
  game_delete(g);

  if (game_over) return false;

  return true;
}

bool test_game_is_over_wrapping() {
  game g = game_new_empty_ext(4, 4, true, true);

  // create finished game when unique is disabled (BUT here it's enabled)
  game_set_square(g, 0, 0, S_ZERO);
  game_set_square(g, 0, 1, S_ZERO);
  game_set_square(g, 0, 2, S_ONE);
  game_set_square(g, 0, 3, S_ONE);

  game_set_square(g, 1, 0, S_ZERO);
  game_set_square(g, 1, 1, S_ONE);
  game_set_square(g, 1, 2, S_ZERO);
  game_set_square(g, 1, 3, S_ONE);

  game_set_square(g, 2, 0, S_ONE);
  game_set_square(g, 2, 1, S_ZERO);
  game_set_square(g, 2, 2, S_ONE);
  game_set_square(g, 2, 3, S_ZERO);

  game_set_square(g, 3, 0, S_ZERO);
  game_set_square(g, 3, 1, S_ONE);
  game_set_square(g, 3, 2, S_ZERO);
  game_set_square(g, 3, 3, S_ONE);

  // game should not be over because of wrapping;
  bool game_over = game_is_over(g);
  game_print(g);
  game_delete(g);

  if (game_over) return false;

  return true;
}

/* [====== MAIN ROUTINE ======] */
// MAIN ROUTINE
int main(int argc, char *argv[]) {
  // CHECKING FOR 1 ARG ONLY
  if (argc == 1 || argc > 2) {
    usage(argv[0]);
  }

  // TEST START
  fprintf(stderr, "=> Start test : \"%s\"\n", argv[1]);
  bool ok = false;
  /* [===== STR COMPARE OF ARG 1 =====] */

  // -> dummy
  if (!strcmp("dummy", argv[1])) {
    ok = test_dummy();
  }

  // -> get_square
  else if (!strcmp("game_get_square", argv[1])) {
    ok = test_game_get_square();
  }

  // -> set_square
  else if (!strcmp("game_set_square", argv[1])) {
    ok = test_game_set_square();
  }

  // -> game_delete
  else if (!strcmp("game_delete", argv[1])) {
    ok = test_game_delete();
  }

  // -> game_equal
  else if (!strcmp("game_equal", argv[1])) {
    ok = test_game_equal();
  }

  // -> game_copy
  else if (!strcmp("game_copy", argv[1])) {
    ok = test_game_copy();
  }

  // -> game_new
  else if (!strcmp("game_new", argv[1])) {
    ok = test_game_new();
  }

  else if (!strcmp("game_new_empty", argv[1])) {
    ok = test_game_new_empty();
  }

  else if (!strcmp("game_nb_rows", argv[1])) {
    ok = test_game_nb_rows();
  }

  else if (!strcmp("game_nb_cols", argv[1])) {
    ok = test_game_nb_cols();
  }

  else if (!strcmp("game_new_ext", argv[1])) {
    ok = test_game_new_ext();
  }

  else if (!strcmp("game_is_over_unique", argv[1])) {
    ok = test_game_is_over_unique();
  }

  else if (!strcmp("game_is_over_wrapping", argv[1])) {
    ok = test_game_is_over_wrapping();
  }

  else {
    // INVALID : "?"
    fprintf(stderr, "=> ERROR : test \"%s\" not found !\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // TEST RESULTS
  if (ok) {
    fprintf(stderr, "=> Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "=> Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
