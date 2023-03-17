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
  int s = fscanf(fgame, "%d %d %d %d", &nb_rows, &nb_cols, &wrapping, &unique);

  if (s == EOF) return NULL;

  square* tab = malloc(nb_rows * nb_cols * sizeof(square));

  // We insert in this array, the values of the file.
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      unsigned char c;
      int s = fscanf(fgame, " %c", &c);
      if (s == EOF) return NULL;
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

bool game_solve(game g) {
  check(g != NULL, "game_solve(game g) : g is pointing on nothing");
  game game_solved = game_copy(g);
  bool solved = true;
  uint count = 0;
  int nb_rows = game_nb_rows(g);
  int nb_cols = game_nb_cols(g);
  generate_first_solution(game_solved, g, 0, nb_cols * nb_rows, &count, solved,
                          nb_rows, nb_cols);

  if (count > 0) {
    for (int i = 0; i < nb_cols; i++) {
      for (int j = 0; j < nb_rows; j++) {
        game_set_square(g, i, j, game_get_square(game_solved, i, j));
      }
    }
    game_delete(game_solved);
    return true;
  } else {
    game_delete(game_solved);
    return false;
  }
}

uint game_nb_solutions(cgame g) {
  check(g != NULL, "game_nb_solutions(cgame g) : g is pointing on nothing");
  ;
  game g_copy = game_copy(g);
  uint count = generate_all_solutions(0, game_nb_cols(g) * game_nb_rows(g),
                                      g_copy, game_nb_cols(g));
  game_delete(g_copy);
  return count;
}
