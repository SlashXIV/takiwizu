#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_struct.h"

/*@brief Creates a new game with extended options and initializes it.
 * @details See description of game extensions on @ref index.
 * @param nb_rows number of rows in game
 * @param nb_cols number of columns in game
 * @param squares an array describing the initial state of each square using
 * row-major storage
 * @param wrapping wrapping option
 * @param unique unique option
 * @return the created game */

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping,
                  bool unique) {
  if (squares == NULL) {
    fprintf(
        stderr,
        "ERROR -> game_new_ext(square * squares) : invalid  squares parameter; "
        "pointing on nothing...");
    exit(EXIT_FAILURE);
  }

  if (nb_cols % 2 != 0 || nb_rows % 2 != 0) {
    fprintf(stderr,
            "ERROR -> game_new_ext(uint nb_rows, uint nb_cols) : invalid "
            "parameters, nb_rows and nb_cols should be both even");
    exit(EXIT_FAILURE);
  }

  game new_game = malloc(sizeof(struct game_s));
  new_game->width = nb_rows;
  new_game->heigh = nb_cols;
  new_game->unique = unique;
  new_game->wrapping = wrapping;

  square* arrayClone =
      malloc((new_game->width * new_game->heigh) * sizeof(square));

  for (int i = 0; i < new_game->width * new_game->heigh; i++) {
    arrayClone[i] = squares[i];
  }

  new_game->ArrayOfSquare = arrayClone;
  return new_game;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool unique) {
  uint nb_cases = nb_rows * nb_cols;

  // automatically generate an empty square array with calloc (0 <=> S_EMPTY)
  square* squares_empty = calloc(nb_cases, sizeof(square));

  // using game_new with our empty squares ==> creation of empty game
  game new_empty_game = game_new(squares_empty);
  free(squares_empty);
  new_empty_game->heigh = nb_cols;
  new_empty_game->width = nb_rows;
  new_empty_game->unique = unique;
  new_empty_game->wrapping = wrapping;
  return new_empty_game;
}

uint game_nb_rows(cgame g) { return g->heigh; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrapping; }

bool game_is_unique(cgame g) { return g->unique; }

void game_undo(game g) {}

void game_redo(game g) {}
