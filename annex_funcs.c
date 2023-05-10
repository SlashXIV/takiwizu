#include "game.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"
#include "stdio.h"
#include "stdlib.h"

void check(bool expr, const char* error_code) {
  if (!expr) {
    fputs("ERROR : ", stderr);
    fputs(error_code, stderr);
    fputs("\U0001F917 \n", stderr);
    exit(EXIT_FAILURE);
  }
}

square* grid_copy(square* squares_primal, uint nb_squares) {
  square* squares_clone = malloc(nb_squares * sizeof(square));

  for (int i = 0; i < nb_squares; i++) {
    squares_clone[i] = squares_primal[i];
  }

  return squares_clone;
}

bool identical_squares(square S1, square S2) { return (S1 == S2); }

bool identical_game_dimension(cgame g1, cgame g2) {
  return ((game_nb_cols(g1) == game_nb_cols(g2)) &&
          (game_nb_rows(g1) == game_nb_rows(g2)));
}

bool identical_options(cgame g1, cgame g2) {
  return (game_is_unique(g1) == game_is_unique(g2) &&
          game_is_wrapping(g1) == game_is_wrapping(g2));
}

bool one_square(square s) { return (s == S_IMMUTABLE_ONE || s == S_ONE); }

bool zero_square(square s) { return (s == S_IMMUTABLE_ZERO || s == S_ZERO); }

bool empty_square(square s) { return (s == S_EMPTY); }

bool immutable_square(square s) {
  return (s == S_IMMUTABLE_ONE || s == S_IMMUTABLE_ZERO);
}

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
    }
  }

  for (int w = 0; w < game_nb_cols(g); w++) {
    switch (game_get_number(g, i, w)) {
      case 0:
        cpt_zero_row++;
        break;

      case 1:
        cpt_one_row++;
        break;
    }
  }

  if (cpt_zero_col == cpt_one_col && cpt_zero_row == cpt_one_row)
    return true;

  else if (cpt_zero_col > (game_nb_cols(g) / 2))
    return false;

  else if (cpt_one_col > (game_nb_cols(g) / 2))
    return false;

  else if (cpt_zero_row > (game_nb_rows(g) / 2))
    return false;

  else if (cpt_one_row > (game_nb_rows(g) / 2))
    return false;

  else
    return true;
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

    // WE SAVE THE LINE (on which i is located) TO COMPARE AMONG THE
    // OTHER LINES
    int compare_line[game_nb_cols(g)];
    for (uint case_index = 0; case_index < game_nb_cols(g); case_index++) {
      compare_line[case_index] = game_get_number(g, i, case_index);
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
        if (compare_line[x] != game_get_number(g, y, x)) break;

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

    // WE SAVE THE LINE (on which i is located) TO COMPARE AMONG THE
    // OTHER LINES
    int compare_line[game_nb_rows(g)];
    for (uint case_index = 0; case_index < game_nb_rows(g); case_index++) {
      compare_line[case_index] = game_get_number(g, case_index, j);
    }

    // COMPARAISON DE LA LIGNE SAUVEGARDÉ AVEC TOUTES LES AUTRES LIGNES :
    for (int x = 0; x < game_nb_cols(g); x++) {
      // re-initialized cpt to 0 for each loop
      cpt_equal_case = 0;

      // LES COMPARAISONS
      for (int y = 0; y < game_nb_rows(g); y++) {
        // IGNORE ITSELF
        if (x == j) break;

        if (compare_line[y] != game_get_number(g, y, x)) break;

        cpt_equal_case++;
      }

      if (cpt_equal_case == game_nb_rows(g)) return true;
    }
  }
  return false;
}

bool is_even(uint n) { return (n % 2 == 0); }

uint generate_all_solutions(int pos, int len, game g, uint nb_cols) {
  // If the game is over, we've found a solution and we return 1 that is added
  // to the counter
  if (game_is_over(g)) {
    return 1;
  }

  // If we've reached the end of the word, return from the function
  if (pos == len) {
    return 0;
  }

  // Get the row and column of the current position in the game board
  int i = pos / nb_cols;
  int j = pos % nb_cols;
  uint count = 0;

  // If the current square is immutable, skip to the next position
  if (game_is_immutable(g, i, j)) {
    count += generate_all_solutions(pos + 1, len, g, nb_cols);
  } else {
    // Try setting the current square to 1
    game_set_square(g, i, j, S_ONE);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      count += generate_all_solutions(pos + 1, len, g, nb_cols);
    }

    // Try setting the current square to 0
    game_set_square(g, i, j, S_ZERO);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      count += generate_all_solutions(pos + 1, len, g, nb_cols);
    }

    // Reset the square back to empty so we can try the next possibility
    game_set_square(g, i, j, S_EMPTY);
  }

  return count;
}

void generate_first_solution(game solution, game g, int pos, int len,
                             uint* count, bool solved, int nb_rows,
                             int nb_cols) {
  // If the game is over, we've found a solution
  if (game_is_over(g)) {
    // Increment the count of solutions found
    (*count)++;

    // If this is the first solution we've found and we want to solve the game,
    // copy the current game state into the solution game board
    if (*count == 1 && solved) {
      for (int i = 0; i < nb_cols; i++) {
        for (int j = 0; j < nb_rows; j++) {
          game_set_square(solution, i, j, game_get_square(g, i, j));
        }
      }
    }

    // Return from the function
    return;
  }

  // If we've reached the end of the word, return from the function
  if (pos == len) {
    return;
  }

  // Get the row and column of the current position in the game board
  int i = pos / nb_cols;
  int j = pos % nb_cols;

  // If the current square is immutable, skip to the next position
  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO) {
    generate_first_solution(solution, g, pos + 1, len, count, solved, nb_rows,
                            nb_cols);
  } else {
    // Try setting the current square to 1
    game_set_square(g, i, j, S_ONE);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      generate_first_solution(solution, g, pos + 1, len, count, solved, nb_rows,
                              nb_cols);
    }

    // Try setting the current square to 0
    game_set_square(g, i, j, S_ZERO);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      generate_first_solution(solution, g, pos + 1, len, count, solved, nb_rows,
                              nb_cols);
    }

    // Reset the square back to empty so we can try the next possibility
    game_set_square(g, i, j, S_EMPTY);
  }
}

// Replace all the squares by the squares of an other game

void game_replace(game g, game g2) {
  // verification if the games have the same size
  if (game_nb_cols(g) != game_nb_cols(g2) ||
      game_nb_rows(g) != game_nb_rows(g2)) {
    printf("The games have not the same size");
    return;
  }
  for (int i = 0; i < game_nb_cols(g); i++) {
    for (int j = 0; j < game_nb_rows(g); j++) {
      game_set_square(g, i, j, game_get_square(g2, i, j));
    }
  }
}

// Clear all game to empty

void game_clear(game g) {
  for (int i = 0; i < game_nb_cols(g); i++) {
    for (int j = 0; j < game_nb_rows(g); j++) {
      square s = game_get_square(g, i, j);
      if(!immutable_square(s)){
        game_set_square(g, i, j, S_EMPTY);
      }
    }
  }
}

game game_custom1(void) {
  square* grid = malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  check(grid != NULL, "game_custom1() : could not create grid");

  grid[0] = S_EMPTY;
  grid[1] = S_EMPTY;
  grid[2] = S_EMPTY;
  grid[3] = S_IMMUTABLE_ONE;
  grid[4] = S_EMPTY;
  grid[5] = S_EMPTY;
  grid[6] = S_EMPTY;
  grid[7] = S_EMPTY;
  grid[8] = S_EMPTY;
  grid[9] = S_IMMUTABLE_ONE;
  grid[10] = S_EMPTY;
  grid[11] = S_EMPTY;
  grid[12] = S_EMPTY;
  grid[13] = S_EMPTY;
  grid[14] = S_IMMUTABLE_ZERO;
  grid[15] = S_EMPTY;
  grid[16] = S_EMPTY;
  grid[17] = S_EMPTY;
  grid[18] = S_EMPTY;
  grid[19] = S_IMMUTABLE_ONE;
  grid[20] = S_EMPTY;
  grid[21] = S_EMPTY;
  grid[22] = S_EMPTY;
  grid[23] = S_EMPTY;
  grid[24] = S_EMPTY;
  grid[25] = S_EMPTY;
  grid[26] = S_IMMUTABLE_ZERO;
  grid[27] = S_EMPTY;
  grid[28] = S_EMPTY;
  grid[29] = S_EMPTY;
  grid[30] = S_EMPTY;
  grid[31] = S_EMPTY;
  grid[32] = S_EMPTY;
  grid[33] = S_IMMUTABLE_ZERO;
  grid[34] = S_EMPTY;
  grid[35] = S_IMMUTABLE_ONE;

  game g_output = game_new(grid);
  free(grid);

  return g_output;
}
