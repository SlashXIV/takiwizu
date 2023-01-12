#include "game_aux.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"

void game_print(cgame g) {  // ilisa
  // printing j coords

  printf("   ");
  for (int x = 0; x < game_nb_cols(g); x++) {
    printf("%d", x);
  }
  printf("\n");

  printf("   ");

  for (int x = 0; x < game_nb_cols(g); x++) {
    printf("%c", '_');
  }
  printf("\n");

  // printing grid and i coords
  for (uint i = 0; i < game_nb_rows(g); i++) {
    printf("%u |", i);
    for (uint j = 0; j < game_nb_cols(g); j++) {
      square actual_square = game_get_square(g, i, j);
      switch (actual_square) {
        case (S_IMMUTABLE_ONE):
          printf("B");
          break;

        case (S_ONE):
          printf("b");
          break;

        case (S_IMMUTABLE_ZERO):
          printf("W");
          break;

        case (S_ZERO):
          printf("w");
          break;

        default:  // EMPTY
          printf(" ");
          break;
      }
    }
    printf("|\n");
  }
  printf("   ");

  for (int x = 0; x < game_nb_cols(g); x++) {
    printf("%c", '-');
  }
  printf("\n");
}

game game_default(void) {  // ilisa

  square *tab = malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));

  if (tab == NULL) exit(EXIT_FAILURE);

  tab[0] = S_EMPTY;
  tab[1] = S_IMMUTABLE_ONE;
  tab[2] = S_IMMUTABLE_ZERO;
  tab[3] = S_EMPTY;
  tab[4] = S_EMPTY;
  tab[5] = S_EMPTY;
  tab[6] = S_EMPTY;
  tab[7] = S_EMPTY;
  tab[8] = S_EMPTY;
  tab[9] = S_EMPTY;
  tab[10] = S_EMPTY;
  tab[11] = S_EMPTY;
  tab[12] = S_EMPTY;
  tab[13] = S_IMMUTABLE_ZERO;
  tab[14] = S_EMPTY;
  tab[15] = S_EMPTY;
  tab[16] = S_IMMUTABLE_ZERO;
  tab[17] = S_EMPTY;
  tab[18] = S_EMPTY;
  tab[19] = S_IMMUTABLE_ZERO;
  tab[20] = S_IMMUTABLE_ONE;
  tab[21] = S_EMPTY;
  tab[22] = S_EMPTY;
  tab[23] = S_EMPTY;
  tab[24] = S_EMPTY;
  tab[25] = S_EMPTY;
  tab[26] = S_IMMUTABLE_ONE;
  tab[27] = S_EMPTY;
  tab[28] = S_EMPTY;
  tab[29] = S_IMMUTABLE_ZERO;
  tab[30] = S_EMPTY;
  tab[31] = S_EMPTY;
  tab[32] = S_EMPTY;
  tab[33] = S_EMPTY;
  tab[34] = S_EMPTY;
  tab[35] = S_IMMUTABLE_ZERO;

  game g_output = game_new(tab);  // copie du tableau en interne
  free(tab);                      // désormais inutile
  g_output->wrapping = false;
  g_output->unique = false;
  g_output->width = DEFAULT_SIZE;
  g_output->height = DEFAULT_SIZE;
  g_output->undo = queue_new();
  g_output->redo = queue_new();
  return g_output;
}

game game_default_solution(void) {  // ilisa

  square *tab = malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));

  if (tab == NULL) exit(EXIT_FAILURE);

  tab[0] = S_ZERO;
  tab[1] = S_IMMUTABLE_ONE;
  tab[2] = S_IMMUTABLE_ZERO;
  tab[3] = S_ONE;
  tab[4] = S_ZERO;
  tab[5] = S_ONE;

  tab[6] = S_ZERO;
  tab[7] = S_ONE;
  tab[8] = S_ONE;
  tab[9] = S_ZERO;
  tab[10] = S_ONE;
  tab[11] = S_ZERO;

  tab[12] = S_ONE;
  tab[13] = S_IMMUTABLE_ZERO;
  tab[14] = S_ZERO;
  tab[15] = S_ONE;
  tab[16] = S_IMMUTABLE_ZERO;
  tab[17] = S_ONE;

  tab[18] = S_ONE;
  tab[19] = S_IMMUTABLE_ZERO;
  tab[20] = S_IMMUTABLE_ONE;
  tab[21] = S_ZERO;
  tab[22] = S_ZERO;
  tab[23] = S_ONE;

  tab[24] = S_ZERO;
  tab[25] = S_ONE;
  tab[26] = S_IMMUTABLE_ONE;
  tab[27] = S_ZERO;
  tab[28] = S_ONE;
  tab[29] = S_IMMUTABLE_ZERO;

  tab[30] = S_ONE;
  tab[31] = S_ZERO;
  tab[32] = S_ZERO;
  tab[33] = S_ONE;
  tab[34] = S_ONE;
  tab[35] = S_IMMUTABLE_ZERO;

  game g_output = game_new(tab);  // copie du tableau en interne
  free(tab);                      // désormais inutile
  return g_output;
}