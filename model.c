#include "model.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "annex_funcs.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

#define FONT "arial.ttf"
#define FONTSIZE 20

#define BACKGROUND "sprites/background.jpg"
#define WHITE_TILE "sprites/white.png"
#define BLACK_TILE "sprites/black.png"
#define EMPTY_TILE "sprites/empty_frame.jpg"
#define TILE_SIZE 34
#define GRID "sprites/grid.png"

/* Idée pour l'interface graphique:

-Fond sombre (noir/ bleu nuit très foncé)
-Ecriture en rouge.
-creation d'un menu avec l'option "jouer" et l'option "rules" qui afficherait
les régles du jeu. -Un temps de start (avec un compteur 3,2,1... avant que le
jeu commence après avoir choisi l'option jouer). -une grille au centre de
l'ecran. -Nom du groupe en petit en bas à droite.

-Optionnel : ajout d'une petite takuzu "cagnotte" avec ajout de points à chaque
coup joué correctement, si erreur la cagnotte se remet à 0.

*/

/* **************************************************************** */

struct Env_t {
  /* PUT YOUR VARIABLES HERE */
  SDL_Texture *background;

  SDL_Texture *white_tile;
  SDL_Texture *black_tile;
  SDL_Texture *empty_tile;
  game g;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc,
          char *argv[]) {  // main equivalent

  Env *env = malloc(sizeof(struct Env_t));
  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->white_tile = IMG_LoadTexture(ren, WHITE_TILE);
  if (!env->white_tile) ERROR("IMG_LoadTexture: %s\n", WHITE_TILE);

  env->black_tile = IMG_LoadTexture(ren, BLACK_TILE);
  if (!env->black_tile) ERROR("IMG_LoadTexture: %s\n", BLACK_TILE);

  env->empty_tile = IMG_LoadTexture(ren, EMPTY_TILE);
  if (!env->empty_tile) ERROR("IMG_LoadTexture: %s\n", EMPTY_TILE);

  env->g = game_default();

  return env;
}

/* **************************************************************** */

void set_tile_pos(SDL_Rect *rect, int x, int y) {
  rect->x = x;
  rect->y = y;
}

void move_to(SDL_Rect *rect, int x, int y) {
  rect->x = x;
  rect->y = y;
}

void mv_left(SDL_Rect *rect) { rect->x -= TILE_SIZE; }
void mv_right(SDL_Rect *rect) { rect->x += TILE_SIZE; }
void mv_up(SDL_Rect *rect) { rect->y -= TILE_SIZE; }
void mv_down(SDL_Rect *rect) { rect->y += TILE_SIZE; }

void SDL_set_tile(Env *env, SDL_Renderer *ren, SDL_Rect rect,
                  SDL_Texture *tile) {
  SDL_QueryTexture(tile, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(ren, tile, NULL, &rect);
}

void render(SDL_Window *win, SDL_Renderer *ren,
            Env *env) { /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* étirable */

  SDL_Rect rect;

  move_to(&rect, 10, 10);
  SDL_QueryTexture(env->white_tile, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(ren, env->white_tile, NULL, &rect);

  mv_right(&rect);
  SDL_QueryTexture(env->black_tile, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(ren, env->black_tile, NULL, &rect);

  mv_right(&rect);
  SDL_QueryTexture(env->empty_tile, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(ren, env->empty_tile, NULL, &rect);

  mv_down(&rect);
  mv_down(&rect);
  mv_down(&rect);
  mv_down(&rect);

  // display game
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      square current_square = game_get_square(env->g, i, j);

      if (one_square(current_square))
        SDL_set_tile(env, ren, rect, env->white_tile);
      else if (zero_square(current_square))
        SDL_set_tile(env, ren, rect, env->black_tile);
      else if (empty_square(current_square))
        SDL_set_tile(env, ren, rect, env->empty_tile);

      mv_right(&rect);
    }

    mv_down(&rect);
    for (int k = 0; k < game_nb_cols(env->g); k++) mv_left(&rect);
  }
  printf("\n");
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  int w, h;

  SDL_GetWindowSize(win, &w, &h);

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  SDL_DestroyTexture(env->background);

  SDL_DestroyTexture(env->white_tile);

  SDL_DestroyTexture(env->black_tile);

  free(env);
}