#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__
#include "game.h"

// Structure of the game:

struct game_s {
  square* ArrayOfSquare;
  uint width;
  uint height;
  bool wrapping;
  bool unique;
};

#endif
