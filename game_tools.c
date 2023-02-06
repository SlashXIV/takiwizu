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
    
  //Oppening of the saving file:
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
  game g_output =  game_new_ext(nb_rows, nb_cols, tab, wrapping, unique);
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


