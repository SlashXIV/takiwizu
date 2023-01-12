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
  new_game->height = nb_rows;
  new_game->width = nb_cols;
  new_game->unique = unique;
  new_game->wrapping = wrapping;
  new_game->undo = queue_new();
  new_game->redo = queue_new();

  square* arrayClone =
      malloc((new_game->width * new_game->height) * sizeof(square));

  for (int i = 0; i < new_game->width * new_game->height; i++) {
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
  new_empty_game->height = nb_rows;
  new_empty_game->width = nb_cols;
  new_empty_game->unique = unique;
  new_empty_game->wrapping = wrapping;
  new_empty_game->undo = queue_new();
  new_empty_game->redo = queue_new();

  return new_empty_game;
}

uint game_nb_rows(cgame g) { return g->height; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrapping; }

bool game_is_unique(cgame g) { return g->unique; }

void game_undo(game g) {
  if (g == NULL) {
    fprintf(stderr, "game undefined");
    exit(EXIT_FAILURE);
  }

  if (queue_is_empty(g->undo)) {
    return;
  }

  // GET THE LAST MOVE
  int* last_move = queue_peek_head(g->undo);

  // CANCEL MOVE
  game_set_square(g, last_move[MOVE_I_INDEX], last_move[MOVE_J_INDEX], S_EMPTY);

  // NOW GOES INTO REDO
  queue_pop_head(g->undo);
  queue_push_head(g->redo, last_move);
}

void game_redo(game g) {
  if (g == NULL) {
    fprintf(stderr, "game undefined");
    exit(EXIT_FAILURE);
  }

  if (queue_is_empty(g->redo)) {
    return;
  }

  // We get the last move canceled from undo:
  int* get_last_move = queue_peek_head(g->redo);

  // We re set the canceled move in the game:
  game_set_square(g, get_last_move[MOVE_I_INDEX], get_last_move[MOVE_J_INDEX],
                  get_last_move[MOVE_SQUARE_INDEX]);

  queue_push_head(g->undo, get_last_move);
  queue_pop_head(g->redo);
}
