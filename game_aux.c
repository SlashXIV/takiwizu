#include "game_aux.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"
#include "annex_funcs.h"

void game_print(cgame g) {
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

game game_default(void) {
  square * grid = malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  assert(grid != NULL, "game_default() : could not create grid");

  grid[0] = S_EMPTY;
  grid[1] = S_IMMUTABLE_ONE;
  grid[2] = S_IMMUTABLE_ZERO;
  grid[3] = S_EMPTY;
  grid[4] = S_EMPTY;
  grid[5] = S_EMPTY;
  grid[6] = S_EMPTY;
  grid[7] = S_EMPTY;
  grid[8] = S_EMPTY;
  grid[9] = S_EMPTY;
  grid[10] = S_EMPTY;
  grid[11] = S_EMPTY;
  grid[12] = S_EMPTY;
  grid[13] = S_IMMUTABLE_ZERO;
  grid[14] = S_EMPTY;
  grid[15] = S_EMPTY;
  grid[16] = S_IMMUTABLE_ZERO;
  grid[17] = S_EMPTY;
  grid[18] = S_EMPTY;
  grid[19] = S_IMMUTABLE_ZERO;
  grid[20] = S_IMMUTABLE_ONE;
  grid[21] = S_EMPTY;
  grid[22] = S_EMPTY;
  grid[23] = S_EMPTY;
  grid[24] = S_EMPTY;
  grid[25] = S_EMPTY;
  grid[26] = S_IMMUTABLE_ONE;
  grid[27] = S_EMPTY;
  grid[28] = S_EMPTY;
  grid[29] = S_IMMUTABLE_ZERO;
  grid[30] = S_EMPTY;
  grid[31] = S_EMPTY;
  grid[32] = S_EMPTY;
  grid[33] = S_EMPTY;
  grid[34] = S_EMPTY;
  grid[35] = S_IMMUTABLE_ZERO;

  game g_output = game_new(grid);
  free(grid);

  return g_output;
}

game game_default_solution(void) {
  square *grid = malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  assert(grid != NULL, "game_default_solution() : could not create grid");

  grid[0] = S_ZERO;
  grid[1] = S_IMMUTABLE_ONE;
  grid[2] = S_IMMUTABLE_ZERO;
  grid[3] = S_ONE;
  grid[4] = S_ZERO;
  grid[5] = S_ONE;

  grid[6] = S_ZERO;
  grid[7] = S_ONE;
  grid[8] = S_ONE;
  grid[9] = S_ZERO;
  grid[10] = S_ONE;
  grid[11] = S_ZERO;

  grid[12] = S_ONE;
  grid[13] = S_IMMUTABLE_ZERO;
  grid[14] = S_ZERO;
  grid[15] = S_ONE;
  grid[16] = S_IMMUTABLE_ZERO;
  grid[17] = S_ONE;

  grid[18] = S_ONE;
  grid[19] = S_IMMUTABLE_ZERO;
  grid[20] = S_IMMUTABLE_ONE;
  grid[21] = S_ZERO;
  grid[22] = S_ZERO;
  grid[23] = S_ONE;

  grid[24] = S_ZERO;
  grid[25] = S_ONE;
  grid[26] = S_IMMUTABLE_ONE;
  grid[27] = S_ZERO;
  grid[28] = S_ONE;
  grid[29] = S_IMMUTABLE_ZERO;

  grid[30] = S_ONE;
  grid[31] = S_ZERO;
  grid[32] = S_ZERO;
  grid[33] = S_ONE;
  grid[34] = S_ONE;
  grid[35] = S_IMMUTABLE_ZERO;

  game g_output = game_new(grid);
  free(grid);
  return g_output;
}