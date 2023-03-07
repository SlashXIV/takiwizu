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