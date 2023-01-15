#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__
#include "game.h"
#include "queue.h"

// Structure of the game:

struct game_s {
  square* grid;
  uint width;
  uint height;
  bool wrapping;
  bool unique;
  queue* last_moves;
  queue* cancelled_moves;
};

#endif
