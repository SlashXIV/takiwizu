#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annex_funcs.h"
#include "game_struct.h"

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping,
                  bool unique) {
  check(squares != NULL,
         "game_new_ext(square* squares) : squares is pointing on nothing");
  check(is_even(nb_cols), "game_new_ext(uint nb_cols) : nb_cols is not even");
  check(is_even(nb_rows), "game_new_ext(uint nb_rows) : nb_rows is not even");

  game new_game = malloc(sizeof(struct game_s));
  new_game->height = nb_rows;
  new_game->width = nb_cols;
  new_game->unique = unique;
  new_game->wrapping = wrapping;
  new_game->last_moves = queue_new();
  new_game->cancelled_moves = queue_new();
  new_game->grid = grid_copy(squares, nb_cols * nb_rows);

  return new_game;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool unique) {
  square* empty_grid = calloc(nb_rows * nb_cols, sizeof(square));

  game g_empty = game_new_ext(nb_rows, nb_cols, empty_grid, wrapping, unique);

  free(empty_grid);  // did a copy in game_new_ext, no longer needed
  return g_empty;
}

uint game_nb_rows(cgame g) { return g->height; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrapping; }

bool game_is_unique(cgame g) { return g->unique; }

void game_undo(game g) {
  check(g != NULL, "game_undo(game g) : g is pointing on nothing");

  if (queue_is_empty(g->last_moves)) {
    printf("\nimpossible : nothing to undo\n");
    return;
  }

  int* m = queue_pop_head(g->last_moves);
  square saved = game_get_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX]);

  game_set_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX], m[MOVE_SQUARE_INDEX]);

  m[MOVE_SQUARE_INDEX] = saved;
  queue_push_head(g->cancelled_moves, m);
}

void game_redo(game g) {
  check(g != NULL, "game_redo(game g) : g is pointing on nothing");
  if (queue_is_empty(g->cancelled_moves)) {
    printf("\nimpossible : nothing to redo\n");
    return;
  }

  int* m = queue_pop_head(g->cancelled_moves);
  square saved = game_get_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX]);

  game_set_square(g, m[MOVE_I_INDEX], m[MOVE_J_INDEX], m[MOVE_SQUARE_INDEX]);

  m[MOVE_SQUARE_INDEX] = saved;
  queue_push_head(g->last_moves, m);
}
