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
  new_game->last_moves = queue_new();
  new_game->cancelled_moves = queue_new();

  square* arrayClone =
      malloc((new_game->width * new_game->height) * sizeof(square));

  for (int i = 0; i < new_game->width * new_game->height; i++) {
    arrayClone[i] = squares[i];
  }

  new_game->grid = arrayClone;
  return new_game;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool unique) {
  uint nb_cases = nb_rows * nb_cols;

  // automatically generate an empty square array with calloc (0 <=> S_EMPTY)
  square* squares_empty = calloc(nb_cases, sizeof(square));

  // using game_new with our empty squares ==> creation of empty game
  game new_empty_game =
      game_new_ext(nb_rows, nb_cols, squares_empty, wrapping, unique);
  free(squares_empty);

  return new_empty_game;
}

uint game_nb_rows(cgame g) { return g->height; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrapping; }

bool game_is_unique(cgame g) { return g->unique; }

void game_undo(game g) {
  // - game_undo(g) récupère le dernier move réalisé (qui est stocké dans la
  // pile undo)
  // - game_undo(g) récupère le dernier move réalisé (qui est stocké dans la
  // pile undo)
  // - place un empty là ou le move a été joué
  // - renvoie ce move dans la pile redo en s'assurant qu'il a été retiré de la
  // pile undo

  if (g == NULL) {
    fprintf(stderr, "game undefined");
    exit(EXIT_FAILURE);
  }

  if (queue_is_empty(g->last_moves)) return;

  int* m = queue_pop_head(g->last_moves);

  square save = game_get_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX]);

  for (int i = 0; i < 3; i++) {
    printf("%d ", m[i]);
  }
  printf("\n");

  game_set_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX], m[MOVE_SQUARE_INDEX]);

  m[MOVE_SQUARE_INDEX] = save;

  queue_push_head(g->cancelled_moves, m);
}

void game_redo(game g) {
  if (g == NULL) {
    fprintf(stderr, "game undefined");
    exit(EXIT_FAILURE);
  }

  // z -> y -> z (le undo après un redo bug)

  if (queue_is_empty(g->cancelled_moves)) {
    return;
  }

  // We get the last move canceled from cancelled_moves:
  int* m = queue_pop_head(g->cancelled_moves);
  square saved = game_get_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX]);

  // We re set the canceled move in the game:
  game_set_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX],
                      m[MOVE_SQUARE_INDEX]);

  m[MOVE_SQUARE_INDEX] = saved;

  queue_push_head(g->last_moves, m);
}
