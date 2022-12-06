#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_aux.h"

struct game_s {
  square* ArrayOfSquare;
};

// We have to implement the functions now:

game game_new(square* squares) {  // robs
  if (squares == NULL) {
    fprintf(stderr,
            "ERROR -> game_new(square * squares) : invalid  squares parameter; "
            "pointing on nothing...");
    exit(EXIT_FAILURE);
  }

  game new_game = malloc(sizeof(game));
  square* arrayClone = malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(square));

  for (int i = 0; i < (DEFAULT_SIZE * DEFAULT_SIZE); i++) {
    arrayClone[i] = squares[i];
  }

  new_game->ArrayOfSquare = arrayClone;  // bug
  return new_game;
}

game game_new_empty(void) {  // robs
  uint nb_cases = DEFAULT_SIZE * DEFAULT_SIZE;

  // automatically generate an empty square array with calloc (0 <=> S_EMPTY)
  square* squares_empty = calloc(nb_cases, sizeof(square));

  // using game_new with our empty squares ==> creation of empty game
  game new_empty_game = game_new(squares_empty);
  free(squares_empty);
  return new_empty_game;
}

game game_copy(cgame g) {  // robs
  if (g == NULL) {
    fprintf(stderr,
            "ERROR -> game_copy(cgame g) : invalid g parameter; pointing on "
            "nothing...");
    exit(EXIT_FAILURE);
  }

  // sending back a new game with the actual state of squares of g
  game g2 = game_new_empty();

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int k = 0; k < DEFAULT_SIZE; k++) {
      game_set_square(g2, i, k, game_get_square(g, i, k));
    }
  }
  return g2;
}

bool game_equal(cgame g1, cgame g2) {  // robs
  if (g1 == NULL || g2 == NULL) {
    fprintf(stderr,
            "ERROR -> game_equal(cgame g1, cgame g2 : one of the two games are "
            "pointing on nothing...");
    exit(EXIT_FAILURE);
  }

  // traverse and compare every square of both games
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      square g1_actual_square = game_get_square(g1, x, y);
      square g2_actual_square = game_get_square(g2, x, y);
      if (g1_actual_square != g2_actual_square) {
        // unequal squares on same pos
        return false;
      }
    }
  }
  // out of the loop -> identical games
  return true;
}

void game_delete(game g) {  // robs
  if (g->ArrayOfSquare != NULL) free(g->ArrayOfSquare);
  free(g);
}

void game_set_square(game g, uint i, uint j, square s) {  // robs
  if (g == NULL) {
    fprintf(stderr,
            "ERROR on game_set_square(game g, uint i, uint j, square s) : "
            "invalid parameters; g pointing on nothing...");
    exit(EXIT_FAILURE);
  } else if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    fprintf(stderr,
            "ERROR on game_set_square(game g, uint i, uint j, square s) : "
            "invalid parameters; i or j aren't under DEFAULT_SIZE (%d) ...",
            DEFAULT_SIZE);
    exit(EXIT_FAILURE);
  } else if (s < S_EMPTY || s > S_IMMUTABLE_ONE) {  // s in [0;4]
    fprintf(stderr,
            "ERROR on game_set_square(game g, uint i, uint j, square s) : "
            "invalid parameters; the square s (%u) isn't a square ...",
            s);
    exit(EXIT_FAILURE);
  }

  // CIBLE VERROUILÉE ...
  uint index_to_set = (i * DEFAULT_SIZE) + j;

  // FIRE (LET THAT F** SQUARE IN)
  g->ArrayOfSquare[index_to_set] = s;
}

square game_get_square(cgame g, uint i, uint j) {  // robs
  if (g == NULL) {
    fprintf(stderr,
            "ERROR on game_get_square(game g, uint i, uint j : invalid "
            "parameters; g pointing on nothing...\n");
    exit(EXIT_FAILURE);
  } else if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE) {
    fprintf(stderr,
            "ERROR on game_get_square(game g, uint i, uint j, : invalid "
            "parameters; i (%u) or j (%u) over DEFAULT_SIZE (%d) ...\n",
            i, j, DEFAULT_SIZE);
    exit(EXIT_FAILURE);
  }

  // CIBLE VERROUILLÉE ...
  uint index_to_get = (i * DEFAULT_SIZE) + j;

  // GET THAT SQUARE AND GIVE IT BACK
  return g->ArrayOfSquare[index_to_get];
}

int game_get_number(cgame g, uint i, uint j) {  // gab

  if (i < 0 || i >= DEFAULT_SIZE || j < 0 || j >= DEFAULT_SIZE || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_ONE)
    return 1;

  if (game_get_square(g, i, j) == S_IMMUTABLE_ZERO ||
      game_get_square(g, i, j) == S_ZERO)
    return 0;

  else
    return -1;
}

int game_get_next_square(cgame g, uint i, uint j, direction dir,
                         uint dist) {  // gab

  // CHECKING GAME AND DIST
  if (g == NULL || dist > 2) {
    fprintf(stderr, "g is null, or  dist too far :/\n");
    return -1;
  }

  // CHECKING IF INITIAL COORDINATES ARE OVER GRID
  if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE) return -1;

  // REAJUST THE POSITION WITH THE DISTANCE PARAMETER
  if (dir == UP) i -= dist;
  if (dir == DOWN) i += dist;
  if (dir == LEFT) j -= dist;
  if (dir == RIGHT) j += dist;

  // CHECKING IF NEW COORDINATES ARE STILL INSIDE GRID
  if (i < DEFAULT_SIZE && j < DEFAULT_SIZE)
    return game_get_square(g, i, j);
  else
    return -1;

  //
  ;
}

int game_get_next_number(cgame g, uint i, uint j, direction dir,
                         uint dist) {  // gab

  if (g == NULL || i >= DEFAULT_SIZE || j >= DEFAULT_SIZE || dist > 2) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    return -1;
  }

  if (game_get_next_square(g, i, j, dir, dist) == S_IMMUTABLE_ONE ||
      game_get_next_square(g, i, j, dir, dist) == S_ONE)
    return 1;

  if (game_get_next_square(g, i, j, dir, dist) == S_IMMUTABLE_ZERO ||
      game_get_next_square(g, i, j, dir, dist) == S_ZERO)
    return 0;

  else
    return -1;
}

bool game_is_empty(cgame g, uint i, uint j) {  // gab

  if (i < 0 || i > DEFAULT_SIZE || j < 0 || j > DEFAULT_SIZE || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  return game_get_square(g, i, j) == S_EMPTY;
}

bool game_is_immutable(cgame g, uint i, uint j) {  // gab

  if (i < 0 || i >= DEFAULT_SIZE || j < 0 || j >= DEFAULT_SIZE || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE)
    return true;

  else if (game_get_square(g, i, j) == S_IMMUTABLE_ZERO)
    return true;

  else
    return false;
}

int game_has_error(cgame g, uint i, uint j) {  // gab

  if (i < 0 || i > DEFAULT_SIZE || j < 0 || j > DEFAULT_SIZE || g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  // THE FUNCTION SHOULD ONLY RETURN AN ERROR AND DO NOT PRINT ANYTHING AT ALL,
  // THE PRINT DOES THIS IN THE GAME_TEXT DO NOT HAVE 3 CONSECUTIVE CAD three in
  // a row => WWW BBB, W BBB

  int primaryCase = game_get_number(g, i, j);

  if (primaryCase != -1) {  // mean that we ignore empty cases

    if (game_get_next_number(g, i, j, DOWN, 1) == primaryCase &&
        game_get_next_number(g, i, j, DOWN, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, RIGHT, 1) == primaryCase &&
        game_get_next_number(g, i, j, RIGHT, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, UP, 1) == primaryCase &&
        game_get_next_number(g, i, j, UP, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, LEFT, 1) == primaryCase &&
        game_get_next_number(g, i, j, LEFT, 2) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, LEFT, 1) == primaryCase &&
        game_get_next_number(g, i, j, RIGHT, 1) == primaryCase)
      return 1;

    if (game_get_next_number(g, i, j, DOWN, 1) == primaryCase &&
        game_get_next_number(g, i, j, UP, 1) == primaryCase)
      return 1;

    int whiteCol = 0;
    int whiteLine = 0;
    int blackCol = 0;
    int blackLine = 0;

    for (int h = 0; h < DEFAULT_SIZE; h++) {
      if (game_get_square(g, h, j) == S_ZERO ||
          game_get_square(g, h, j) == S_IMMUTABLE_ZERO)
        whiteLine++;

      if (game_get_square(g, h, j) == S_ONE ||
          game_get_square(g, h, j) == S_IMMUTABLE_ONE)
        blackLine++;
    }

    for (int w = 0; w < DEFAULT_SIZE; w++) {
      if (game_get_square(g, i, w) == S_ZERO ||
          game_get_square(g, i, w) == S_IMMUTABLE_ZERO)
        whiteCol++;

      if (game_get_square(g, i, w) == S_ONE ||
          game_get_square(g, i, w) == S_IMMUTABLE_ONE)
        blackCol++;
    }

    if (whiteLine > 3 || whiteCol > 3 || blackCol > 3 ||
        blackLine > 3)  // we check the parity of the squares if >3 mean that
                        // parity isnt respected
      return 1;
  }

  return 0;  // 0 mean that there is no error
}

bool game_check_move(cgame g, uint i, uint j, square s) {  // gab

  if (g == NULL) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE) return false;

  square cas = game_get_square(g, i, j);

  if (cas == S_IMMUTABLE_ONE || cas == S_IMMUTABLE_ZERO ||
      s == S_IMMUTABLE_ONE || s == S_IMMUTABLE_ZERO)
    return false;

  return true;
}

void game_play_move(game g, uint i, uint j, square s) {  // ilisa

  if (i < 0 || i >= DEFAULT_SIZE || j < 0 || j >= DEFAULT_SIZE || g == NULL ||
      game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO || s == S_IMMUTABLE_ONE ||
      s == S_IMMUTABLE_ZERO) {
    fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
    exit(EXIT_FAILURE);
  }

  game_set_square(g, i, j, s);
}

bool game_is_over(cgame g) {  // ilisa

  // We test if our pointer g is NULL, if it is the case we exit the programm.

  if (g == NULL) {
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == S_EMPTY) return false;
      if (game_has_error(g, i, j) != 0) {
        return false;
      }
    }
  }

  // In case we haven't returned false, all the rules are satisied and we return
  // true ---> the game is won.
  return true;
}

void game_restart(game g) {  // ilisa

  if (g == NULL) {
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == S_ONE) game_set_square(g, i, j, S_EMPTY);

      if (game_get_square(g, i, j) == S_ZERO) game_set_square(g, i, j, S_EMPTY);
    }
  }
}
