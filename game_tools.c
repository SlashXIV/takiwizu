#include "game_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annex_funcs.h"
#include "game.h"
#include "game_ext.h"
#include "game_struct.h"

char square_to_str[255] = {[S_EMPTY] = 'e',
                           [S_ZERO] = 'w',
                           [S_ONE] = 'b',
                           [S_IMMUTABLE_ZERO] = 'W',
                           [S_IMMUTABLE_ONE] = 'B'};

square str_to_square[255] = {['e'] = S_EMPTY,
                             ['w'] = S_ZERO,
                             ['b'] = S_ONE,
                             ['W'] = S_IMMUTABLE_ZERO,
                             ['B'] = S_IMMUTABLE_ONE};

game game_load(char* file_path) {
  // Oppening of the saving file:
  FILE* fgame = fopen(file_path, "r");
  if (fgame == NULL) {
    return NULL;
  }
  // We save the size of the grid (nb_rows nb_cols wrapping unique)

  int nb_rows, nb_cols, wrapping, unique;
  fscanf(fgame, "%d %d %d %d", &nb_rows, &nb_cols, &wrapping, &unique);

  square* tab = malloc(nb_rows * nb_cols * sizeof(square));

  // We insert in this array, the values of the file.
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      unsigned char c;
      fscanf(fgame, " %c", &c);
      tab[i * nb_cols + j] = str_to_square[c];
    }
  }
  game g_output = game_new_ext(nb_rows, nb_cols, tab, wrapping, unique);
  free(tab);
  fclose(fgame);
  return g_output;
}


void game_save(cgame g, char* filename) {
  // Creation of a saving file
  FILE* fgame = fopen(filename, "w");

  // We save the size of the grid (nb_rows nb_cols wrapping unique\n)
  fprintf(fgame, "%d %d %d %d\n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g), game_is_unique(g));

  // Saving the game grid line by line
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      fprintf(fgame, "%c", square_to_str[game_get_square(g, i, j)]);
    }
    fprintf(fgame, "\n");
  }
  fclose(fgame);
}

/*
void square_fill_black(game g, uint row, uint col) {
  game_set_square(g, row, col, S_ONE);
}

void square_fill_white(game g, uint row, uint col) {
  game_set_square(g, row, col, S_ZERO);
}

void square_clear(game g, uint row, uint col) {
  game_set_square(g, row, col, S_EMPTY);
}



bool game_solve(game g) {
  check(g != NULL, "game_solve(game g) : g is pointing on nothing");

  uint row, col;
  for (row = 0; row < game_nb_rows(g); row++) {
    for (col = 0; col < game_nb_cols(g); col++) {
      if (empty_square(game_get_square(g, row, col))){
        // Try to fill the current square with a black square
        square_fill_black(g, row, col);
        if (game_has_error(g, row, col) == GAME_HAS_NO_ERROR) {
          if (game_solve(g)) {
            return true;
          }
        }
        // Backtrack and try to fill the current square with a white square
        square_fill_white(g, row, col);
        if (game_has_error(g, row, col) == GAME_HAS_NO_ERROR) {
          if (game_solve(g)) {
            return true;
          }
        }
        // Backtrack again and return false if the current square cannot be
filled with either a white or a black square square_clear(g, row, col); return
false;
      }
    }
  }
  return true;
}


bool game_solve(game ga) {

  FILE* fgame = fopen("solved_game.txt", "w");
  game g = game_copy(ga);
  check(g != NULL, "game_solve(game g) : g is pointing on nothing");

  uint row, col;
  int black_count_col, white_count_col;
  int black_count_row, white_count_row;

  for (row = 0; row < game_nb_rows(g); row++) {
    black_count_row = white_count_row = 0;
    for (col = 0; col < game_nb_cols(g); col++) {
      if (empty_square(game_get_square(g, row, col))) {
        // Try to fill the current square with a black square
        square_fill_black(g, row, col);
        black_count_row++;
        if (game_has_error(g, row, col) == GAME_HAS_NO_ERROR) {
          if (game_solve(g)) {
            return true;
          }
        }
        // Backtrack and try to fill the current square with a white square
        square_fill_white(g, row, col);
        white_count_row++;
        if (game_has_error(g, row, col) == GAME_HAS_NO_ERROR) {
          if (game_solve(g)) {
            return true;
          }
        }
        // Backtrack again and return false if the current square cannot be
filled with either a white or a black square square_clear(g, row, col); return
false; } else if (game_get_square(g, row, col) == S_ONE) { black_count_row++; }
else if (game_get_square(g, row, col) == S_ZERO) { white_count_row++;
      }
    }
    if (black_count_row > game_nb_cols(g) / 2 || white_count_row >
game_nb_cols(g) / 2) { return false;
    }
  }

  for (col = 0; col < game_nb_cols(g); col++) {
    black_count_col = white_count_col = 0;
    for (row = 0; row < game_nb_rows(g); row++) {
      if (game_get_square(g, row, col) == S_ONE) {
        black_count_col++;
      } else if (game_get_square(g, row, col) == S_ZERO) {
        white_count_col++;
      }
    }
    if (black_count_col > game_nb_rows(g) / 2 || white_count_col >
game_nb_rows(g) / 2) { return false;
    }
  }
  printf("Je cherche solution\n");

  //put all in a file with this format :
010101|011010|100101|101001|010101|100110

  for (row = 0; row < game_nb_rows(g); row++) {
    for (col = 0; col < game_nb_cols(g); col++) {
      fprintf(fgame, "%c", square_to_str[game_get_square(g, row, col)]);
    }
    fprintf(fgame, "|");
  }
  fclose(fgame);
  game_delete(g);


  return true;
}


*/

void generate_first_solution(int pos, int len, uint* count, bool solved,
                             game solution, game g) {
  // If the game is over, we've found a solution
  if (game_is_over(g)) {
    // Increment the count of solutions found
    (*count)++;

    // If this is the first solution we've found and we want to solve the game,
    // copy the current game state into the solution game board
    if (*count == 1 && solved) {
      for (int i = 0; i < game_nb_cols(g); i++) {
        for (int j = 0; j < game_nb_rows(g); j++) {
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
  int i = pos / game_nb_cols(g);
  int j = pos % game_nb_cols(g);

  // If the current square is immutable, skip to the next position
  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO) {
    generate_first_solution(pos + 1, len, count, solved, solution, g);
  } else {
    // Try setting the current square to 1
    game_set_square(g, i, j, S_ONE);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      generate_first_solution(pos + 1, len, count, solved, solution, g);
    }

    // Try setting the current square to 0
    game_set_square(g, i, j, S_ZERO);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      generate_first_solution(pos + 1, len, count, solved, solution, g);
    }

    // Reset the square back to empty so we can try the next possibility
    game_set_square(g, i, j, S_EMPTY);
  }
}

bool game_solve(game g) {
  check(g != NULL, "game_solve(game g) : g is pointing on nothing");
  FILE* fgame = fopen("solved_game.txt", "w");
  game game_solved = game_copy(g);
  bool solved = true;
  uint count = 0;
  generate_first_solution(0, game_nb_cols(g) * game_nb_rows(g), &count, solved,
                          game_solved, g);

  // boucle for pour récupérer les valeurs des cases dans game_solved et les
  // mettre dans un fichier

  if (count > 0) {
    for (int i = 0; i < game_nb_cols(g); i++) {
      for (int j = 0; j < game_nb_rows(g); j++) {
        fprintf(fgame, "%d", game_get_number(game_solved, i, j));
      }
      fprintf(fgame, "|");
    }
    fclose(fgame);
    game_delete(game_solved);
    return true;
  } else {
    fclose(fgame);
    game_delete(game_solved);
    return false;
  }
}

uint generate_all_solutions(int pos, int len, game g) {
  // If the game is over, we've found a solution
  if (game_is_over(g)) {
    return 1;
  }

  // If we've reached the end of the word, return from the function
  if (pos == len) {
    return 0;
  }

  // Get the row and column of the current position in the game board
  int i = pos / game_nb_cols(g);
  int j = pos % game_nb_cols(g);

  uint count = 0;

  // If the current square is immutable, skip to the next position
  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO) {
    count += generate_all_solutions(pos + 1, len, g);
  } else {
    // Try setting the current square to 1
    game_set_square(g, i, j, S_ONE);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      count += generate_all_solutions(pos + 1, len, g);
    }

    // Try setting the current square to 0
    game_set_square(g, i, j, S_ZERO);
    if (game_has_error(g, i, j) == GAME_HAS_NO_ERROR) {
      count += generate_all_solutions(pos + 1, len, g);
    }

    // Reset the square back to empty so we can try the next possibility
    game_set_square(g, i, j, S_EMPTY);
  }

  return count;
}

uint game_nb_solutions(cgame g) {
  check(g != NULL, "game_nb_solutions(cgame g) : g is pointing on nothing");
  ;
  game g_copy = game_copy(g);
  uint count =
      generate_all_solutions(0, game_nb_cols(g) * game_nb_rows(g), g_copy);
  game_delete(g_copy);
  return count;
}
